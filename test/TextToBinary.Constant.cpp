// Copyright (c) 2015-2016 The Khronos Group Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and/or associated documentation files (the
// "Materials"), to deal in the Materials without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Materials, and to
// permit persons to whom the Materials are furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Materials.
//
// MODIFICATIONS TO THIS FILE MAY MEAN IT NO LONGER ACCURATELY REFLECTS
// KHRONOS STANDARDS. THE UNMODIFIED, NORMATIVE VERSIONS OF KHRONOS
// SPECIFICATIONS AND HEADER INFORMATION ARE LOCATED AT
//    https://www.khronos.org/registry/
//
// THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

// Assembler tests for instructions in the "Group Instrucions" section of the
// SPIR-V spec.

#include "UnitSPIRV.h"

#include <cstdint>
#include <limits>

#include "TestFixture.h"
#include "gmock/gmock.h"

namespace {

using spvtest::EnumCase;
using spvtest::MakeInstruction;
using spvtest::Concatenate;
using ::testing::Eq;

// Test Sampler Addressing Mode enum values

using SamplerAddressingModeTest = spvtest::TextToBinaryTestBase<
    ::testing::TestWithParam<EnumCase<SpvSamplerAddressingMode>>>;

TEST_P(SamplerAddressingModeTest, AnySamplerAddressingMode) {
  const std::string input =
      "%result = OpConstantSampler %type " + GetParam().name() + " 0 Nearest";
  EXPECT_THAT(CompiledInstructions(input),
              Eq(MakeInstruction(SpvOpConstantSampler,
                                 {1, 2, GetParam().value(), 0, 0})));
}

// clang-format off
#define CASE(NAME) { SpvSamplerAddressingMode##NAME, #NAME }
INSTANTIATE_TEST_CASE_P(
    TextToBinarySamplerAddressingMode, SamplerAddressingModeTest,
    ::testing::ValuesIn(std::vector<EnumCase<SpvSamplerAddressingMode>>{
        CASE(None),
        CASE(ClampToEdge),
        CASE(Clamp),
        CASE(Repeat),
        CASE(RepeatMirrored),
    }));
#undef CASE
// clang-format on

TEST_F(SamplerAddressingModeTest, WrongMode) {
  EXPECT_THAT(CompileFailure("%r = OpConstantSampler %t xxyyzz 0 Nearest"),
              Eq("Invalid sampler addressing mode 'xxyyzz'."));
}

// Test Sampler Filter Mode enum values

using SamplerFilterModeTest = spvtest::TextToBinaryTestBase<
    ::testing::TestWithParam<EnumCase<SpvSamplerFilterMode>>>;

TEST_P(SamplerFilterModeTest, AnySamplerFilterMode) {
  const std::string input =
      "%result = OpConstantSampler %type Clamp 0 " + GetParam().name();
  EXPECT_THAT(CompiledInstructions(input),
              Eq(MakeInstruction(SpvOpConstantSampler,
                                 {1, 2, 2, 0, GetParam().value()})));
}

// clang-format off
#define CASE(NAME) { SpvSamplerFilterMode##NAME, #NAME}
INSTANTIATE_TEST_CASE_P(
    TextToBinarySamplerFilterMode, SamplerFilterModeTest,
    ::testing::ValuesIn(std::vector<EnumCase<SpvSamplerFilterMode>>{
        CASE(Nearest),
        CASE(Linear),
    }));
#undef CASE
// clang-format on

TEST_F(SamplerFilterModeTest, WrongMode) {
  EXPECT_THAT(CompileFailure("%r = OpConstantSampler %t Clamp 0 xxyyzz"),
              Eq("Invalid sampler filter mode 'xxyyzz'."));
}

struct ConstantTestCase {
  std::string constant_type;
  std::string constant_value;
  std::vector<uint32_t> expected_instructions;
};

using OpConstantValidTest =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<ConstantTestCase>>;

TEST_P(OpConstantValidTest, ValidTypes) {
  const std::string input = "%1 = " + GetParam().constant_type +
                            "\n"
                            "%2 = OpConstant %1 " +
                            GetParam().constant_value + "\n";
  std::vector<uint32_t> instructions;
  EXPECT_THAT(CompiledInstructions(input),
              Eq(GetParam().expected_instructions));
}

// clang-format off
INSTANTIATE_TEST_CASE_P(
    TextToBinaryOpConstantValid, OpConstantValidTest,
    ::testing::ValuesIn(std::vector<ConstantTestCase>{
      // Check 16 bits
      {"OpTypeInt 16 0", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 0}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x1234})})},
      {"OpTypeInt 16 0", "0x8000",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 0}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x8000})})},
      {"OpTypeInt 16 1", "0x8000", // Test sign extension.
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 1}),
         MakeInstruction(SpvOpConstant, {1, 2, 0xffff8000})})},
      {"OpTypeInt 16 1", "-32",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 1}),
         MakeInstruction(SpvOpConstant, {1, 2, uint32_t(-32)})})},
      // Check 32 bits
      {"OpTypeInt 32 0", "42",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 32, 0}),
         MakeInstruction(SpvOpConstant, {1, 2, 42})})},
      {"OpTypeInt 32 1", "-32",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 32, 1}),
         MakeInstruction(SpvOpConstant, {1, 2, uint32_t(-32)})})},
      {"OpTypeFloat 32", "1.0",
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x3f800000})})},
      {"OpTypeFloat 32", "10.0",
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x41200000})})},
      {"OpTypeFloat 32", "-0x1p+128", // -infinity
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpConstant, {1, 2, 0xFF800000})})},
      {"OpTypeFloat 32", "0x1p+128", // +infinity
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x7F800000})})},
      {"OpTypeFloat 32", "-0x1.8p+128", // A -NaN
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpConstant, {1, 2, 0xFFC00000})})},
      {"OpTypeFloat 32", "-0x1.0002p+128", // A +NaN
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpConstant, {1, 2, 0xFF800100})})},
      // Check 48 bits
      {"OpTypeInt 48 0", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 0}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x1234, 0})})},
      {"OpTypeInt 48 0", "0x800000000001",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 0}),
         MakeInstruction(SpvOpConstant, {1, 2, 1, 0x00008000})})},
      {"OpTypeInt 48 1", "0x800000000000", // Test sign extension.
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 1}),
         MakeInstruction(SpvOpConstant, {1, 2, 0, 0xffff8000})})},
      {"OpTypeInt 48 1", "-32",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 1}),
         MakeInstruction(SpvOpConstant, {1, 2, uint32_t(-32), uint32_t(-1)})})},
      // Check 64 bits
      {"OpTypeInt 64 0", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 64, 0}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x1234, 0})})},
      {"OpTypeInt 64 1", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 64, 1}),
         MakeInstruction(SpvOpConstant, {1, 2, 0x1234, 0})})},
      {"OpTypeInt 64 1", "-42",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 64, 1}),
         MakeInstruction(SpvOpConstant, {1, 2, uint32_t(-42), uint32_t(-1)})})},
    }));
// clang-format on

using OpConstantInvalidTypeTest =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<std::string>>;

TEST_P(OpConstantInvalidTypeTest, InvalidTypes) {
  const std::string input = "%1 = " + GetParam() +
                            "\n"
                            "%2 = OpConstant %1 0\n";
  EXPECT_THAT(
      CompileFailure(input),
      Eq("Type for Constant must be a scalar floating point or integer type"));
}

// clang-format off
INSTANTIATE_TEST_CASE_P(
    TextToBinaryOpConstantInvalidValidType, OpConstantInvalidTypeTest,
    ::testing::ValuesIn(std::vector<std::string>{
      {"OpTypeVoid",
       "OpTypeBool",
       "OpTypeVector %a 32",
       "OpTypeMatrix %a 32",
       "OpTypeImage %a 1D 0 0 0 0 Unknown",
       "OpTypeSampler",
       "OpTypeSampledImage %a",
       "OpTypeArray %a %b",
       "OpTypeRuntimeArray %a",
       "OpTypeStruct %a",
       "OpTypeOpaque \"Foo\"",
       "OpTypePointer UniformConstant %a",
       "OpTypeFunction %a %b",
       "OpTypeEvent",
       "OpTypeDeviceEvent",
       "OpTypeReserveId",
       "OpTypeQueue",
       "OpTypePipe ReadOnly",
       "OpTypeForwardPointer %a UniformConstant",
        // At least one thing that isn't a type at all
       "OpNot %a %b"
      },
    }));
// clang-format on

using OpSpecConstantValidTest =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<ConstantTestCase>>;

TEST_P(OpSpecConstantValidTest, ValidTypes) {
  const std::string input = "%1 = " + GetParam().constant_type +
                            "\n"
                            "%2 = OpSpecConstant %1 " +
                            GetParam().constant_value + "\n";
  std::vector<uint32_t> instructions;
  EXPECT_THAT(CompiledInstructions(input),
              Eq(GetParam().expected_instructions));
}

// clang-format off
INSTANTIATE_TEST_CASE_P(
    TextToBinaryOpSpecConstantValid, OpSpecConstantValidTest,
    ::testing::ValuesIn(std::vector<ConstantTestCase>{
      // Check 16 bits
      {"OpTypeInt 16 0", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 0}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0x1234})})},
      {"OpTypeInt 16 0", "0x8000",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 0}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0x8000})})},
      {"OpTypeInt 16 1", "0x8000", // Test sign extension.
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 1}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0xffff8000})})},
      {"OpTypeInt 16 1", "-32",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 16, 1}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, uint32_t(-32)})})},
      // Check 32 bits
      {"OpTypeInt 32 0", "42",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 32, 0}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 42})})},
      {"OpTypeInt 32 1", "-32",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 32, 1}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, uint32_t(-32)})})},
      {"OpTypeFloat 32", "1.0",
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0x3f800000})})},
      {"OpTypeFloat 32", "10.0",
        Concatenate({MakeInstruction(SpvOpTypeFloat, {1, 32}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0x41200000})})},
      // Check 48 bits
      {"OpTypeInt 48 0", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 0}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0x1234, 0})})},
      {"OpTypeInt 48 0", "0x800000000001",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 0}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 1, 0x00008000})})},
      {"OpTypeInt 48 1", "0x800000000000", // Test sign extension.
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 1}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0, 0xffff8000})})},
      {"OpTypeInt 48 1", "-32",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 48, 1}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, uint32_t(-32), uint32_t(-1)})})},
      // Check 64 bits
      {"OpTypeInt 64 0", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 64, 0}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0x1234, 0})})},
      {"OpTypeInt 64 1", "0x1234",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 64, 1}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, 0x1234, 0})})},
      {"OpTypeInt 64 1", "-42",
        Concatenate({MakeInstruction(SpvOpTypeInt, {1, 64, 1}),
         MakeInstruction(SpvOpSpecConstant, {1, 2, uint32_t(-42), uint32_t(-1)})})},
    }));
// clang-format on

using OpSpecConstantInvalidTypeTest =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<std::string>>;

TEST_P(OpSpecConstantInvalidTypeTest, InvalidTypes) {
  const std::string input = "%1 = " + GetParam() +
                            "\n"
                            "%2 = OpSpecConstant %1 0\n";
  EXPECT_THAT(CompileFailure(input),
              Eq("Type for SpecConstant must be a scalar floating point or "
                 "integer type"));
}

// clang-format off
INSTANTIATE_TEST_CASE_P(
    TextToBinaryOpSpecConstantInvalidValidType, OpSpecConstantInvalidTypeTest,
    ::testing::ValuesIn(std::vector<std::string>{
      {"OpTypeVoid",
       "OpTypeBool",
       "OpTypeVector %a 32",
       "OpTypeMatrix %a 32",
       "OpTypeImage %a 1D 0 0 0 0 Unknown",
       "OpTypeSampler",
       "OpTypeSampledImage %a",
       "OpTypeArray %a %b",
       "OpTypeRuntimeArray %a",
       "OpTypeStruct %a",
       "OpTypeOpaque \"Foo\"",
       "OpTypePointer UniformConstant %a",
       "OpTypeFunction %a %b",
       "OpTypeEvent",
       "OpTypeDeviceEvent",
       "OpTypeReserveId",
       "OpTypeQueue",
       "OpTypePipe ReadOnly",
       "OpTypeForwardPointer %a UniformConstant",
        // At least one thing that isn't a type at all
       "OpNot %a %b"
      },
    }));
// clang-format on

using RoundTripTest =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<std::string>>;

const int64_t kMaxUnsigned48Bit = (int64_t(1) << 48) - 1;
const int64_t kMaxSigned48Bit = (int64_t(1) << 47) - 1;
const int64_t kMinSigned48Bit = -kMaxSigned48Bit - 1;

TEST_P(RoundTripTest, Sample) {
  EXPECT_THAT(EncodeAndDecodeSuccessfully(GetParam()), Eq(GetParam()))
      << GetParam();
}

INSTANTIATE_TEST_CASE_P(
    OpConstantRoundTrip, RoundTripTest,
    ::testing::ValuesIn(std::vector<std::string>{
        // 16 bit
        "%1 = OpTypeInt 16 0\n%2 = OpConstant %1 0\n",
        "%1 = OpTypeInt 16 0\n%2 = OpConstant %1 65535\n",
        "%1 = OpTypeInt 16 1\n%2 = OpConstant %1 -32768\n",
        "%1 = OpTypeInt 16 1\n%2 = OpConstant %1 32767\n",
        "%1 = OpTypeInt 32 0\n%2 = OpConstant %1 0\n",
        // 32 bit
        std::string("%1 = OpTypeInt 32 0\n%2 = OpConstant %1 0\n"),
        std::string("%1 = OpTypeInt 32 0\n%2 = OpConstant %1 ") +
            std::to_string(std::numeric_limits<uint32_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 32 1\n%2 = OpConstant %1 ") +
            std::to_string(std::numeric_limits<int32_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 32 1\n%2 = OpConstant %1 ") +
            std::to_string(std::numeric_limits<int32_t>::min()) + "\n",
        // 48 bit
        std::string("%1 = OpTypeInt 48 0\n%2 = OpConstant %1 0\n"),
        std::string("%1 = OpTypeInt 48 0\n%2 = OpConstant %1 ") +
            std::to_string(kMaxUnsigned48Bit) + "\n",
        std::string("%1 = OpTypeInt 48 1\n%2 = OpConstant %1 ") +
            std::to_string(kMaxSigned48Bit) + "\n",
        std::string("%1 = OpTypeInt 48 1\n%2 = OpConstant %1 ") +
            std::to_string(kMinSigned48Bit) + "\n",
        // 64 bit
        std::string("%1 = OpTypeInt 64 0\n%2 = OpConstant %1 0\n"),
        std::string("%1 = OpTypeInt 64 0\n%2 = OpConstant %1 ") +
            std::to_string(std::numeric_limits<uint64_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 64 1\n%2 = OpConstant %1 ") +
            std::to_string(std::numeric_limits<int64_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 64 1\n%2 = OpConstant %1 ") +
            std::to_string(std::numeric_limits<int64_t>::min()) + "\n",
        // 32-bit float
        "%1 = OpTypeFloat 32\n%2 = OpConstant %1 0\n",
        "%1 = OpTypeFloat 32\n%2 = OpConstant %1 13.5\n",
        "%1 = OpTypeFloat 32\n%2 = OpConstant %1 -12.5\n",
        // 64-bit float
        "%1 = OpTypeFloat 64\n%2 = OpConstant %1 0\n",
        "%1 = OpTypeFloat 64\n%2 = OpConstant %1 1.79769e+308\n",
        "%1 = OpTypeFloat 64\n%2 = OpConstant %1 -1.79769e+308\n",
    }));

// clang-format off
// (Clang-format really wants to break up these strings across lines.
INSTANTIATE_TEST_CASE_P(
    OpConstantRoundTripNonFinite, RoundTripTest,
    ::testing::ValuesIn(std::vector<std::string>{
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 -0x1p+128\n",         // -inf
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 0x1p+128\n",          // inf
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 -0x1.8p+128\n",       // -nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 -0x1.0002p+128\n",    // -nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 -0x1.0018p+128\n",    // -nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 -0x1.01ep+128\n",     // -nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 -0x1.fffffep+128\n",  // -nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 0x1.8p+128\n",        // +nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 0x1.0002p+128\n",     // +nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 0x1.0018p+128\n",     // +nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 0x1.01ep+128\n",      // +nan
  "%1 = OpTypeFloat 32\n%2 = OpConstant %1 0x1.fffffep+128\n",   // +nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 -0x1p+1024\n",                //-inf
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 0x1p+1024\n",                 //+inf
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 -0x1.8p+1024\n",              // -nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 -0x1.0fp+1024\n",             // -nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 -0x1.0000000000001p+1024\n",  // -nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 -0x1.00003p+1024\n",          // -nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 -0x1.fffffffffffffp+1024\n",  // -nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 0x1.8p+1024\n",               // +nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 0x1.0fp+1024\n",              // +nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 0x1.0000000000001p+1024\n",   // -nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 0x1.00003p+1024\n",           // -nan
  "%1 = OpTypeFloat 64\n%2 = OpConstant %1 0x1.fffffffffffffp+1024\n",   // -nan
    }));
// clang-format on

INSTANTIATE_TEST_CASE_P(
    OpSpecConstantRoundTrip, RoundTripTest,
    ::testing::ValuesIn(std::vector<std::string>{
        // 16 bit
        "%1 = OpTypeInt 16 0\n%2 = OpSpecConstant %1 0\n",
        "%1 = OpTypeInt 16 0\n%2 = OpSpecConstant %1 65535\n",
        "%1 = OpTypeInt 16 1\n%2 = OpSpecConstant %1 -32768\n",
        "%1 = OpTypeInt 16 1\n%2 = OpSpecConstant %1 32767\n",
        "%1 = OpTypeInt 32 0\n%2 = OpSpecConstant %1 0\n",
        // 32 bit
        std::string("%1 = OpTypeInt 32 0\n%2 = OpSpecConstant %1 0\n"),
        std::string("%1 = OpTypeInt 32 0\n%2 = OpSpecConstant %1 ") +
            std::to_string(std::numeric_limits<uint32_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 32 1\n%2 = OpSpecConstant %1 ") +
            std::to_string(std::numeric_limits<int32_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 32 1\n%2 = OpSpecConstant %1 ") +
            std::to_string(std::numeric_limits<int32_t>::min()) + "\n",
        // 48 bit
        std::string("%1 = OpTypeInt 48 0\n%2 = OpSpecConstant %1 0\n"),
        std::string("%1 = OpTypeInt 48 0\n%2 = OpSpecConstant %1 ") +
            std::to_string(kMaxUnsigned48Bit) + "\n",
        std::string("%1 = OpTypeInt 48 1\n%2 = OpSpecConstant %1 ") +
            std::to_string(kMaxSigned48Bit) + "\n",
        std::string("%1 = OpTypeInt 48 1\n%2 = OpSpecConstant %1 ") +
            std::to_string(kMinSigned48Bit) + "\n",
        // 64 bit
        std::string("%1 = OpTypeInt 64 0\n%2 = OpSpecConstant %1 0\n"),
        std::string("%1 = OpTypeInt 64 0\n%2 = OpSpecConstant %1 ") +
            std::to_string(std::numeric_limits<uint64_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 64 1\n%2 = OpSpecConstant %1 ") +
            std::to_string(std::numeric_limits<int64_t>::max()) + "\n",
        std::string("%1 = OpTypeInt 64 1\n%2 = OpSpecConstant %1 ") +
            std::to_string(std::numeric_limits<int64_t>::min()) + "\n",
        // 32-bit float
        "%1 = OpTypeFloat 32\n%2 = OpSpecConstant %1 0\n",
        "%1 = OpTypeFloat 32\n%2 = OpSpecConstant %1 13.5\n",
        "%1 = OpTypeFloat 32\n%2 = OpSpecConstant %1 -12.5\n",
        // 64-bit float
        "%1 = OpTypeFloat 64\n%2 = OpSpecConstant %1 0\n",
        "%1 = OpTypeFloat 64\n%2 = OpSpecConstant %1 1.79769e+308\n",
        "%1 = OpTypeFloat 64\n%2 = OpSpecConstant %1 -1.79769e+308\n",
    }));

// Test OpSpecConstantOp

using OpSpecConstantOpTestWithIds =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<EnumCase<SpvOp>>>;

// The operands to the OpSpecConstantOp opcode are all Ids.
TEST_P(OpSpecConstantOpTestWithIds, Assembly) {
  std::stringstream input;
  input << "%2 = OpSpecConstantOp %1 " << GetParam().name();
  for (auto id : GetParam().operands()) input << " %" << id;
  input << "\n";

  EXPECT_THAT(CompiledInstructions(input.str()),
              Eq(MakeInstruction(SpvOpSpecConstantOp,
                                 {1, 2, uint32_t(GetParam().value())},
                                 GetParam().operands())));

  // Check the disassembler as well.
  EXPECT_THAT(EncodeAndDecodeSuccessfully(input.str()), input.str());
}

// clang-format off
#define CASE1(NAME) { SpvOp##NAME, #NAME, {3} }
#define CASE2(NAME) { SpvOp##NAME, #NAME, {3, 4} }
#define CASE3(NAME) { SpvOp##NAME, #NAME, {3, 4, 5} }
#define CASE4(NAME) { SpvOp##NAME, #NAME, {3, 4, 5, 6} }
#define CASE5(NAME) { SpvOp##NAME, #NAME, {3, 4, 5, 6, 7} }
#define CASE6(NAME) { SpvOp##NAME, #NAME, {3, 4, 5, 6, 7, 8} }
INSTANTIATE_TEST_CASE_P(
    TextToBinaryOpSpecConstantOp, OpSpecConstantOpTestWithIds,
    ::testing::ValuesIn(std::vector<EnumCase<SpvOp>>{
        // Conversion
        CASE1(SConvert),
        CASE1(FConvert),
        CASE1(ConvertFToS),
        CASE1(ConvertSToF),
        CASE1(ConvertFToU),
        CASE1(ConvertUToF),
        CASE1(UConvert),
        CASE1(ConvertPtrToU),
        CASE1(ConvertUToPtr),
        CASE1(GenericCastToPtr),
        CASE1(PtrCastToGeneric),
        CASE1(Bitcast),
        CASE1(QuantizeToF16),
        // Arithmetic
        CASE1(SNegate),
        CASE1(Not),
        CASE2(IAdd),
        CASE2(ISub),
        CASE2(IMul),
        CASE2(UDiv),
        CASE2(SDiv),
        CASE2(UMod),
        CASE2(SRem),
        CASE2(SMod),
        CASE2(ShiftRightLogical),
        CASE2(ShiftRightArithmetic),
        CASE2(ShiftLeftLogical),
        CASE2(BitwiseOr),
        CASE2(BitwiseAnd),
        CASE2(BitwiseXor),
        CASE1(FNegate),
        CASE2(FAdd),
        CASE2(FSub),
        CASE2(FMul),
        CASE2(FDiv),
        CASE2(FRem),
        CASE2(FMod),
        // Composite operations use literal numbers. So they're in another test.
        // Logical
        CASE2(LogicalOr),
        CASE2(LogicalAnd),
        CASE1(LogicalNot),
        CASE2(LogicalEqual),
        CASE2(LogicalNotEqual),
        CASE3(Select),
        // Comparison
        CASE2(IEqual),
        CASE2(ULessThan),
        CASE2(SLessThan),
        CASE2(UGreaterThan),
        CASE2(SGreaterThan),
        CASE2(ULessThanEqual),
        CASE2(SLessThanEqual),
        CASE2(UGreaterThanEqual),
        CASE2(SGreaterThanEqual),
        // Memory
        // For AccessChain, there is a base Id, then a sequence of index Ids.
        // Having no index Ids is a corner case.
        CASE1(AccessChain),
        CASE2(AccessChain),
        CASE6(AccessChain),
        CASE1(InBoundsAccessChain),
        CASE2(InBoundsAccessChain),
        CASE6(InBoundsAccessChain),
        // PtrAccessChain also has an element Id.
        CASE2(PtrAccessChain),
        CASE3(PtrAccessChain),
        CASE6(PtrAccessChain),
        CASE2(InBoundsPtrAccessChain),
        CASE3(InBoundsPtrAccessChain),
        CASE6(InBoundsPtrAccessChain),
    }));
#undef CASE1
#undef CASE2
#undef CASE3
#undef CASE4
#undef CASE5
#undef CASE6
// clang-format on

using OpSpecConstantOpTestWithTwoIdsThenLiteralNumbers =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<EnumCase<SpvOp>>>;

// The operands to the OpSpecConstantOp opcode are two Ids followed by a
// sequence of literal numbers.
TEST_P(OpSpecConstantOpTestWithTwoIdsThenLiteralNumbers, Assembly) {
  std::stringstream input;
  input << "%2 = OpSpecConstantOp %1 " << GetParam().name() << " %3 %4";
  for (auto number : GetParam().operands()) input << " " << number;
  input << "\n";

  EXPECT_THAT(CompiledInstructions(input.str()),
              Eq(MakeInstruction(SpvOpSpecConstantOp,
                                 {1, 2, uint32_t(GetParam().value()), 3, 4},
                                 GetParam().operands())));

  // Check the disassembler as well.
  EXPECT_THAT(EncodeAndDecodeSuccessfully(input.str()), input.str());
}

#define CASE(NAME) SpvOp##NAME, #NAME
INSTANTIATE_TEST_CASE_P(
    TextToBinaryOpSpecConstantOp,
    OpSpecConstantOpTestWithTwoIdsThenLiteralNumbers,
    ::testing::ValuesIn(std::vector<EnumCase<SpvOp>>{
        // For VectorShuffle, there are two vector operands, and at least
        // two selector Ids.  OpenCL can have up to 16-element vectors.
        {CASE(VectorShuffle), {0, 0}},
        {CASE(VectorShuffle), {4, 3, 2, 1}},
        {CASE(VectorShuffle), {0, 2, 4, 6, 1, 3, 5, 7}},
        {CASE(VectorShuffle),
         {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}},
        // For CompositeInsert, there is an object to insert, the target
        // composite, and then literal indices.
        {CASE(CompositeInsert), {0}},
        {CASE(CompositeInsert), {4, 3, 99, 1}},
    }));

using OpSpecConstantOpTestWithOneIdThenLiteralNumbers =
    spvtest::TextToBinaryTestBase<::testing::TestWithParam<EnumCase<SpvOp>>>;

// The operands to the OpSpecConstantOp opcode are one Id followed by a
// sequence of literal numbers.
TEST_P(OpSpecConstantOpTestWithOneIdThenLiteralNumbers, Assembly) {
  std::stringstream input;
  input << "%2 = OpSpecConstantOp %1 " << GetParam().name() << " %3";
  for (auto number : GetParam().operands()) input << " " << number;
  input << "\n";

  EXPECT_THAT(CompiledInstructions(input.str()),
              Eq(MakeInstruction(SpvOpSpecConstantOp,
                                 {1, 2, uint32_t(GetParam().value()), 3},
                                 GetParam().operands())));

  // Check the disassembler as well.
  EXPECT_THAT(EncodeAndDecodeSuccessfully(input.str()), input.str());
}

#define CASE(NAME) SpvOp##NAME, #NAME
INSTANTIATE_TEST_CASE_P(
    TextToBinaryOpSpecConstantOp,
    OpSpecConstantOpTestWithOneIdThenLiteralNumbers,
    ::testing::ValuesIn(std::vector<EnumCase<SpvOp>>{
        // For CompositeExtract, the universal limit permits up to 255 literal
        // indices.  Let's only test a few.
        {CASE(CompositeExtract), {0}},
        {CASE(CompositeExtract), {0, 99, 42, 16, 17, 12, 19}},
    }));

// TODO(dneto): OpConstantTrue
// TODO(dneto): OpConstantFalse
// TODO(dneto): OpConstantComposite
// TODO(dneto): OpConstantSampler: other variations Param is 0 or 1
// TODO(dneto): OpConstantNull
// TODO(dneto): OpSpecConstantTrue
// TODO(dneto): OpSpecConstantFalse
// TODO(dneto): OpSpecConstantComposite
// TODO(dneto): Negative tests for OpSpecConstantOp

}  // anonymous namespace
