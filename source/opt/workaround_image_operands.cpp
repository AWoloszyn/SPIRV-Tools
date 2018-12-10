// Copyright (c) 2018 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "workaround_image_operands.h"

#include <list>
#include <stack>

namespace spvtools {
namespace opt {

Pass::Status WorkaroundImageOperands::Process() {
  bool modified = false;
  modified = FixupOpTypeImage();
  return (modified ? Status::SuccessWithChange : Status::SuccessWithoutChange);
}

void WorkaroundImageOperands::WalkSampledImageUse(
    Instruction* op_type_sampled_image_usage, uint32_t idx,
    Instruction* img_type, analysis::SampledImage* sampled_image_type,
    analysis::DefUseManager* def_use_mgr) {
  auto op_sampled_image = op_type_sampled_image_usage;
  context()->get_def_use_mgr()->ForEachUse(
      op_sampled_image,
      [this, op_sampled_image, sampled_image_type, img_type](
          opt::Instruction* sampled_image_usage, uint32_t idx) {
        bool has_depth = false;
        if (sampled_image_usage->opcode() == SpvOpImageDrefGather ||
            sampled_image_usage->opcode() == SpvOpImageSampleDrefImplicitLod ||
            sampled_image_usage->opcode() == SpvOpImageSampleDrefExplicitLod ||
            sampled_image_usage->opcode() ==
                SpvOpImageSampleProjDrefImplicitLod ||
            sampled_image_usage->opcode() ==
                SpvOpImageSampleProjDrefExplicitLod ||
            sampled_image_usage->opcode() ==
                SpvOpImageSparseSampleDrefImplicitLod ||
            sampled_image_usage->opcode() ==
                SpvOpImageSparseSampleDrefExplicitLod ||
            sampled_image_usage->opcode() ==
                SpvOpImageSparseSampleProjDrefImplicitLod ||
            sampled_image_usage->opcode() ==
                SpvOpImageSparseSampleProjDrefExplicitLod ||
            sampled_image_usage->opcode() == SpvOpImageSparseDrefGather) {
          has_depth = true;
        }
        if (!has_depth) {
          return;
        }
        analysis::Image* image_type = context()
                                          ->get_type_mgr()
                                          ->GetType(img_type->result_id())
                                          ->AsImage();
        analysis::Type* t = context()->get_type_mgr()->GetType(
            context()->get_type_mgr()->GetId(image_type->sampled_type()));
        analysis::Image img(
            t, image_type->dim(), has_depth ? 1 : 0, image_type->is_arrayed(),
            image_type->is_multisampled(), image_type->sampled(),
            image_type->format(), image_type->access_qualifier());
        for (auto& dec : image_type->decorations()) {
          img.AddDecoration(std::vector<uint32_t>(dec));
        }
        uint32_t new_image =
            context()->get_type_mgr()->GetTypeInstruction(&img);

        analysis::SampledImage sampled_img(
            context()->get_type_mgr()->GetType(new_image));
        for (auto& dec : sampled_image_type->decorations()) {
          sampled_img.AddDecoration(std::vector<uint32_t>(dec));
        }

        uint32_t sampled_image_type =
            context()->get_type_mgr()->GetTypeInstruction(&sampled_img);

        auto new_sampled_image = op_sampled_image->Clone(context());
        new_sampled_image->SetResultType(sampled_image_type);

        new_sampled_image->InsertBefore(op_sampled_image);
        new_sampled_image->SetResultId(context()->TakeNextId());

        sampled_image_usage->SetOperand(idx, {new_sampled_image->result_id()});
        context()->AnalyzeUses(sampled_image_usage);
        context()->AnalyzeDefUse(new_sampled_image);
      });
}

void WorkaroundImageOperands::WalkImageUse(
    Instruction* inst, Instruction* img_type,
    analysis::DefUseManager* def_use_mgr) {
  analysis::SampledImage* sampled_image_type =
      context()->get_type_mgr()->GetType(inst->result_id())->AsSampledImage();

  context()->get_def_use_mgr()->ForEachUse(
      inst, [this, img_type, sampled_image_type, def_use_mgr](
                opt::Instruction* op_type_sampled_image_usage, uint32_t idx) {
        if (op_type_sampled_image_usage->opcode() == SpvOpSampledImage) {
          WalkSampledImageUse(op_type_sampled_image_usage, idx, img_type,
                              sampled_image_type, def_use_mgr);
        }
      });
}

bool WorkaroundImageOperands::FixupOpTypeImage() {
  bool modified = false;
  auto* module = get_module();
  auto* def_use_mgr = context()->get_def_use_mgr();
  std::vector<opt::Instruction*> unknown_depth_images;

  for (Module::inst_iterator type_it = context()->types_values_begin();
       type_it != context()->types_values_end(); ++type_it) {
    // 2 means no indication as to whether this is a depth or non-depth image
    if (type_it->opcode() == SpvOpTypeImage) {
      if (type_it->GetSingleWordOperand(3) == 2) {
        unknown_depth_images.push_back(&(*type_it));
        modified = true;
      }
    }
  }

  std::vector<opt::Instruction*> fixed_instructions;

  for (auto img_type : unknown_depth_images) {
    def_use_mgr->ForEachUse(
        img_type->result_id(),
        [this, img_type, def_use_mgr](opt::Instruction* inst, uint32_t) {
          if (inst->opcode() == SpvOpTypeSampledImage) {
            WalkImageUse(inst, img_type, def_use_mgr);
          }
        });
  }

  for (auto img_type : unknown_depth_images) {
    analysis::Image* image_type =
        context()->get_type_mgr()->GetType(img_type->result_id())->AsImage();
    analysis::Type* t = context()->get_type_mgr()->GetType(
        context()->get_type_mgr()->GetId(image_type->sampled_type()));
    analysis::Image img(t, image_type->dim(), 0, image_type->is_arrayed(),
                        image_type->is_multisampled(), image_type->sampled(),
                        image_type->format(), image_type->access_qualifier());
    for (auto& dec : image_type->decorations()) {
      img.AddDecoration(std::vector<uint32_t>(dec));
    }
    uint32_t new_image = context()->get_type_mgr()->GetTypeInstruction(&img);
    context()->ReplaceAllUsesWith(img_type->result_id(), new_image);
  }
  return modified;
}  // namespace opt

}  // namespace opt
}  // namespace spvtools