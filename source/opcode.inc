// Instruction fields are:
//    name - skips the "Op" prefix
//    {0|1} - whether the instruction generates a result Id
//    {0|1} - whether the instruction encodes the type of the result Id
//    numLogicalOperands - does not include result id or type id
//    numCapabilities - we only handle 0 or 1 required capabilities
//    Capability(<capability-name>) - capability required to use this instruction. Might be None.
//       There can be Capability2(a,b) for dependence on two capabilities.
//    {0|1} - whether the instruction is variable number of words
//    EmptyList or List(...) - list of classes of logical operands
// Example use:
// #define EmptyList {}
// #define List(...) {__VA_ARGS__}
// #define Capability(C) Capability##C
// #define CapabilityNone -1
// #define Instruction(Name,HasResult,HasType,NumLogicalOperands,CapabiltyRequired,IsVariable,LogicalArgsList)
Instruction(Nop, 0, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(Undef, 1, 1, 0, 0, Capability(None), 0, EmptyList)
Instruction(SourceContinued, 0, 0, 1, 0, Capability(None), 1, List(OperandLiteralString))
Instruction(Source, 0, 0, 4, 0, Capability(None), 1, List(OperandSource, OperandLiteralNumber, OperandOptionalId, OperandOptionalLiteralString))
Instruction(SourceExtension, 0, 0, 1, 0, Capability(None), 1, List(OperandLiteralString))
Instruction(Name, 0, 0, 2, 0, Capability(None), 1, List(OperandId, OperandLiteralString))
Instruction(MemberName, 0, 0, 3, 0, Capability(None), 1, List(OperandId, OperandLiteralNumber, OperandLiteralString))
Instruction(String, 1, 0, 1, 0, Capability(None), 1, List(OperandLiteralString))
Instruction(Line, 0, 0, 3, 0, Capability(None), 0, List(OperandId, OperandLiteralNumber, OperandLiteralNumber))
Instruction(Extension, 0, 0, 1, 0, Capability(None), 1, List(OperandLiteralString))
Instruction(ExtInstImport, 1, 0, 1, 0, Capability(None), 1, List(OperandLiteralString))
Instruction(ExtInst, 1, 1, 3, 0, Capability(None), 1, List(OperandId, OperandLiteralNumber, OperandVariableIds))
Instruction(MemoryModel, 0, 0, 2, 0, Capability(None), 0, List(OperandAddressing, OperandMemory))
Instruction(EntryPoint, 0, 0, 4, 0, Capability(None), 1, List(OperandExecutionModel, OperandId, OperandLiteralString, OperandVariableIds))
Instruction(ExecutionMode, 0, 0, 3, 0, Capability(None), 1, List(OperandId, OperandExecutionMode, OperandOptionalLiteral))
Instruction(Capability, 0, 0, 1, 0, Capability(None), 0, List(OperandCapability))
Instruction(TypeVoid, 1, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(TypeBool, 1, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(TypeInt, 1, 0, 2, 0, Capability(None), 0, List(OperandLiteralNumber, OperandLiteralNumber))
Instruction(TypeFloat, 1, 0, 1, 0, Capability(None), 0, List(OperandLiteralNumber))
Instruction(TypeVector, 1, 0, 2, 0, Capability(None), 0, List(OperandId, OperandLiteralNumber))
Instruction(TypeMatrix, 1, 0, 2, 1, Capability(Matrix), 0, List(OperandId, OperandLiteralNumber))
Instruction(TypeImage, 1, 0, 8, 0, Capability(None), 1, List(OperandId, OperandDimensionality, OperandLiteralNumber, OperandLiteralNumber, OperandLiteralNumber, OperandLiteralNumber, OperandSamplerImageFormat, OperandOptionalLiteral))
Instruction(TypeSampler, 1, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(TypeSampledImage, 1, 0, 1, 0, Capability(None), 0, List(OperandId))
Instruction(TypeArray, 1, 0, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(TypeRuntimeArray, 1, 0, 1, 1, Capability(Shader), 0, List(OperandId))
Instruction(TypeStruct, 1, 0, 1, 0, Capability(None), 1, List(OperandVariableIds))
Instruction(TypeOpaque, 1, 0, 1, 1, Capability(Kernel), 1, List(OperandLiteralString))
Instruction(TypePointer, 1, 0, 2, 0, Capability(None), 0, List(OperandStorage, OperandId))
Instruction(TypeFunction, 1, 0, 2, 0, Capability(None), 1, List(OperandId, OperandVariableIds))
Instruction(TypeEvent, 1, 0, 0, 1, Capability(Kernel), 0, EmptyList)
Instruction(TypeDeviceEvent, 1, 0, 0, 1, Capability(DeviceEnqueue), 0, EmptyList)
Instruction(TypeReserveId, 1, 0, 0, 1, Capability(Pipes), 0, EmptyList)
Instruction(TypeQueue, 1, 0, 0, 1, Capability(DeviceEnqueue), 0, EmptyList)
Instruction(TypePipe, 1, 0, 1, 1, Capability(Pipes), 0, List(OperandAccessQualifier))
Instruction(TypeForwardPointer, 0, 0, 2, 1, Capability(Addresses), 0, List(OperandId, OperandStorage))
Instruction(ConstantTrue, 1, 1, 0, 0, Capability(None), 0, EmptyList)
Instruction(ConstantFalse, 1, 1, 0, 0, Capability(None), 0, EmptyList)
Instruction(Constant, 1, 1, 1, 0, Capability(None), 1, List(OperandVariableLiterals))
Instruction(ConstantComposite, 1, 1, 1, 0, Capability(None), 1, List(OperandVariableIds))
Instruction(ConstantSampler, 1, 1, 3, 1, Capability(LiteralSampler), 0, List(OperandSamplerAddressingMode, OperandLiteralNumber, OperandSamplerFilterMode))
Instruction(ConstantNull, 1, 1, 0, 0, Capability(None), 0, EmptyList)
Instruction(SpecConstantTrue, 1, 1, 0, 0, Capability(None), 0, EmptyList)
Instruction(SpecConstantFalse, 1, 1, 0, 0, Capability(None), 0, EmptyList)
Instruction(SpecConstant, 1, 1, 1, 0, Capability(None), 1, List(OperandVariableLiterals))
Instruction(SpecConstantComposite, 1, 1, 1, 0, Capability(None), 1, List(OperandVariableIds))
Instruction(SpecConstantOp, 1, 1, 2, 0, Capability(None), 1, List(OperandLiteralNumber, OperandVariableIds))
Instruction(Function, 1, 1, 2, 0, Capability(None), 0, List(OperandFunction, OperandId))
Instruction(FunctionParameter, 1, 1, 0, 0, Capability(None), 0, EmptyList)
Instruction(FunctionEnd, 0, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(FunctionCall, 1, 1, 2, 0, Capability(None), 1, List(OperandId, OperandVariableIds))
Instruction(Variable, 1, 1, 2, 0, Capability(None), 1, List(OperandStorage, OperandOptionalId))
Instruction(ImageTexelPointer, 1, 1, 3, 0, Capability(None), 0, List(OperandId, OperandId, OperandId))
Instruction(Load, 1, 1, 2, 0, Capability(None), 1, List(OperandId, OperandOptionalLiteral))
Instruction(Store, 0, 0, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandOptionalLiteral))
Instruction(CopyMemory, 0, 0, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandOptionalLiteral))
Instruction(CopyMemorySized, 0, 0, 4, 1, Capability(Addresses), 1, List(OperandId, OperandId, OperandId, OperandOptionalLiteral))
Instruction(AccessChain, 1, 1, 2, 0, Capability(None), 1, List(OperandId, OperandVariableIds))
Instruction(InBoundsAccessChain, 1, 1, 2, 0, Capability(None), 1, List(OperandId, OperandVariableIds))
Instruction(PtrAccessChain, 1, 1, 3, 1, Capability(Addresses), 1, List(OperandId, OperandId, OperandVariableIds))
Instruction(ArrayLength, 1, 1, 2, 1, Capability(Shader), 0, List(OperandId, OperandLiteralNumber))
Instruction(GenericPtrMemSemantics, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(InBoundsPtrAccessChain, 1, 1, 3, 1, Capability(Addresses), 1, List(OperandId, OperandId, OperandVariableIds))
Instruction(Decorate, 0, 0, 3, 0, Capability(None), 1, List(OperandId, OperandDecoration, OperandVariableLiterals))
Instruction(MemberDecorate, 0, 0, 4, 0, Capability(None), 1, List(OperandId, OperandLiteralNumber, OperandDecoration, OperandVariableLiterals))
Instruction(DecorationGroup, 1, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(GroupDecorate, 0, 0, 2, 0, Capability(None), 1, List(OperandId, OperandVariableIds))
Instruction(GroupMemberDecorate, 0, 0, 2, 0, Capability(None), 1, List(OperandId, OperandVariableIdLiteral))
Instruction(VectorExtractDynamic, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(VectorInsertDynamic, 1, 1, 3, 0, Capability(None), 0, List(OperandId, OperandId, OperandId))
Instruction(VectorShuffle, 1, 1, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandVariableLiterals))
Instruction(CompositeConstruct, 1, 1, 1, 0, Capability(None), 1, List(OperandVariableIds))
Instruction(CompositeExtract, 1, 1, 2, 0, Capability(None), 1, List(OperandId, OperandVariableLiterals))
Instruction(CompositeInsert, 1, 1, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandVariableLiterals))
Instruction(CopyObject, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(Transpose, 1, 1, 1, 1, Capability(Matrix), 0, List(OperandId))
Instruction(SampledImage, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ImageSampleImplicitLod, 1, 1, 3, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSampleExplicitLod, 1, 1, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSampleDrefImplicitLod, 1, 1, 4, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSampleDrefExplicitLod, 1, 1, 4, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSampleProjImplicitLod, 1, 1, 3, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSampleProjExplicitLod, 1, 1, 3, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSampleProjDrefImplicitLod, 1, 1, 4, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSampleProjDrefExplicitLod, 1, 1, 4, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageFetch, 1, 1, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageGather, 1, 1, 4, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageDrefGather, 1, 1, 4, 1, Capability(Shader), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageRead, 1, 1, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageWrite, 0, 0, 4, 0, Capability(None), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(Image, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(ImageQueryFormat, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(ImageQueryOrder, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(ImageQuerySizeLod, 1, 1, 2, 2, Capability2(Kernel,ImageQuery), 0, List(OperandId, OperandId))
Instruction(ImageQuerySize, 1, 1, 1, 2, Capability2(Kernel,ImageQuery), 0, List(OperandId))
Instruction(ImageQueryLod, 1, 1, 2, 1, Capability(ImageQuery), 0, List(OperandId, OperandId))
Instruction(ImageQueryLevels, 1, 1, 1, 2, Capability2(Kernel,ImageQuery), 0, List(OperandId))
Instruction(ImageQuerySamples, 1, 1, 1, 2, Capability2(Kernel,ImageQuery), 0, List(OperandId))
Instruction(ConvertFToU, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(ConvertFToS, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(ConvertSToF, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(ConvertUToF, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(UConvert, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(SConvert, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(FConvert, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(QuantizeToF16, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(ConvertPtrToU, 1, 1, 1, 1, Capability(Addresses), 0, List(OperandId))
Instruction(SatConvertSToU, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(SatConvertUToS, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(ConvertUToPtr, 1, 1, 1, 1, Capability(Addresses), 0, List(OperandId))
Instruction(PtrCastToGeneric, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(GenericCastToPtr, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(GenericCastToPtrExplicit, 1, 1, 2, 1, Capability(Kernel), 0, List(OperandId, OperandStorage))
Instruction(Bitcast, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(SNegate, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(FNegate, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(IAdd, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FAdd, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ISub, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FSub, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(IMul, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FMul, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(UDiv, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SDiv, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FDiv, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(UMod, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SRem, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SMod, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FRem, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FMod, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(VectorTimesScalar, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(MatrixTimesScalar, 1, 1, 2, 1, Capability(Matrix), 0, List(OperandId, OperandId))
Instruction(VectorTimesMatrix, 1, 1, 2, 1, Capability(Matrix), 0, List(OperandId, OperandId))
Instruction(MatrixTimesVector, 1, 1, 2, 1, Capability(Matrix), 0, List(OperandId, OperandId))
Instruction(MatrixTimesMatrix, 1, 1, 2, 1, Capability(Matrix), 0, List(OperandId, OperandId))
Instruction(OuterProduct, 1, 1, 2, 1, Capability(Matrix), 0, List(OperandId, OperandId))
Instruction(Dot, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(IAddCarry, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ISubBorrow, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(UMulExtended, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SMulExtended, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(Any, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(All, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(IsNan, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(IsInf, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(IsFinite, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(IsNormal, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(SignBitSet, 1, 1, 1, 1, Capability(Kernel), 0, List(OperandId))
Instruction(LessOrGreater, 1, 1, 2, 1, Capability(Kernel), 0, List(OperandId, OperandId))
Instruction(Ordered, 1, 1, 2, 1, Capability(Kernel), 0, List(OperandId, OperandId))
Instruction(Unordered, 1, 1, 2, 1, Capability(Kernel), 0, List(OperandId, OperandId))
Instruction(LogicalEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(LogicalNotEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(LogicalOr, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(LogicalAnd, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(LogicalNot, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(Select, 1, 1, 3, 0, Capability(None), 0, List(OperandId, OperandId, OperandId))
Instruction(IEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(INotEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(UGreaterThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SGreaterThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(UGreaterThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SGreaterThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ULessThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SLessThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ULessThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(SLessThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FOrdEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FUnordEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FOrdNotEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FUnordNotEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FOrdLessThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FUnordLessThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FOrdGreaterThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FUnordGreaterThan, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FOrdLessThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FUnordLessThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FOrdGreaterThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(FUnordGreaterThanEqual, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ShiftRightLogical, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ShiftRightArithmetic, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(ShiftLeftLogical, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(BitwiseOr, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(BitwiseXor, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(BitwiseAnd, 1, 1, 2, 0, Capability(None), 0, List(OperandId, OperandId))
Instruction(Not, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(BitFieldInsert, 1, 1, 4, 1, Capability(Shader), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(BitFieldSExtract, 1, 1, 3, 1, Capability(Shader), 0, List(OperandId, OperandId, OperandId))
Instruction(BitFieldUExtract, 1, 1, 3, 1, Capability(Shader), 0, List(OperandId, OperandId, OperandId))
Instruction(BitReverse, 1, 1, 1, 1, Capability(Shader), 0, List(OperandId))
Instruction(BitCount, 1, 1, 1, 0, Capability(None), 0, List(OperandId))
Instruction(DPdx, 1, 1, 1, 1, Capability(Shader), 0, List(OperandId))
Instruction(DPdy, 1, 1, 1, 1, Capability(Shader), 0, List(OperandId))
Instruction(Fwidth, 1, 1, 1, 1, Capability(Shader), 0, List(OperandId))
Instruction(DPdxFine, 1, 1, 1, 1, Capability(DerivativeControl), 0, List(OperandId))
Instruction(DPdyFine, 1, 1, 1, 1, Capability(DerivativeControl), 0, List(OperandId))
Instruction(FwidthFine, 1, 1, 1, 1, Capability(DerivativeControl), 0, List(OperandId))
Instruction(DPdxCoarse, 1, 1, 1, 1, Capability(DerivativeControl), 0, List(OperandId))
Instruction(DPdyCoarse, 1, 1, 1, 1, Capability(DerivativeControl), 0, List(OperandId))
Instruction(FwidthCoarse, 1, 1, 1, 1, Capability(DerivativeControl), 0, List(OperandId))
Instruction(EmitVertex, 0, 0, 0, 1, Capability(Geometry), 0, EmptyList)
Instruction(EndPrimitive, 0, 0, 0, 1, Capability(Geometry), 0, EmptyList)
Instruction(EmitStreamVertex, 0, 0, 1, 1, Capability(GeometryStreams), 0, List(OperandId))
Instruction(EndStreamPrimitive, 0, 0, 1, 1, Capability(GeometryStreams), 0, List(OperandId))
Instruction(ControlBarrier, 0, 0, 3, 0, Capability(None), 0, List(OperandScope, OperandScope, OperandMemorySemantics))
Instruction(MemoryBarrier, 0, 0, 2, 0, Capability(None), 0, List(OperandScope, OperandMemorySemantics))
Instruction(AtomicLoad, 1, 1, 3, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics))
Instruction(AtomicStore, 0, 0, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicExchange, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicCompareExchange, 1, 1, 6, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandMemorySemantics, OperandId, OperandId))
Instruction(AtomicCompareExchangeWeak, 1, 1, 6, 1, Capability(Kernel), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandMemorySemantics, OperandId, OperandId))
Instruction(AtomicIIncrement, 1, 1, 3, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics))
Instruction(AtomicIDecrement, 1, 1, 3, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics))
Instruction(AtomicIAdd, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicISub, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicSMin, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicUMin, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicSMax, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicUMax, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicAnd, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicOr, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(AtomicXor, 1, 1, 4, 0, Capability(None), 0, List(OperandId, OperandScope, OperandMemorySemantics, OperandId))
Instruction(Phi, 1, 1, 1, 0, Capability(None), 1, List(OperandVariableIds))
Instruction(LoopMerge, 0, 0, 3, 0, Capability(None), 0, List(OperandId, OperandId, OperandLoop))
Instruction(SelectionMerge, 0, 0, 2, 0, Capability(None), 0, List(OperandId, OperandSelect))
Instruction(Label, 1, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(Branch, 0, 0, 1, 0, Capability(None), 0, List(OperandId))
Instruction(BranchConditional, 0, 0, 4, 0, Capability(None), 1, List(OperandId, OperandId, OperandId, OperandVariableLiterals))
Instruction(Switch, 0, 0, 3, 0, Capability(None), 1, List(OperandId, OperandId, OperandVariableLiteralId))
Instruction(Kill, 0, 0, 0, 1, Capability(Shader), 0, EmptyList)
Instruction(Return, 0, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(ReturnValue, 0, 0, 1, 0, Capability(None), 0, List(OperandId))
Instruction(Unreachable, 0, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(LifetimeStart, 0, 0, 2, 1, Capability(Kernel), 0, List(OperandId, OperandLiteralNumber))
Instruction(LifetimeStop, 0, 0, 2, 1, Capability(Kernel), 0, List(OperandId, OperandLiteralNumber))
Instruction(GroupAsyncCopy, 1, 1, 6, 1, Capability(Kernel), 0, List(OperandScope, OperandId, OperandId, OperandId, OperandId, OperandId))
Instruction(GroupWaitEvents, 0, 0, 3, 1, Capability(Kernel), 0, List(OperandScope, OperandId, OperandId))
Instruction(GroupAll, 1, 1, 2, 1, Capability(Groups), 0, List(OperandScope, OperandId))
Instruction(GroupAny, 1, 1, 2, 1, Capability(Groups), 0, List(OperandScope, OperandId))
Instruction(GroupBroadcast, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandId, OperandId))
Instruction(GroupIAdd, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(GroupFAdd, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(GroupFMin, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(GroupUMin, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(GroupSMin, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(GroupFMax, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(GroupUMax, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(GroupSMax, 1, 1, 3, 1, Capability(Groups), 0, List(OperandScope, OperandGroupOperation, OperandId))
Instruction(ReadPipe, 1, 1, 4, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(WritePipe, 1, 1, 4, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(ReservedReadPipe, 1, 1, 6, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId, OperandId, OperandId))
Instruction(ReservedWritePipe, 1, 1, 6, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId, OperandId, OperandId))
Instruction(ReserveReadPipePackets, 1, 1, 4, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(ReserveWritePipePackets, 1, 1, 4, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(CommitReadPipe, 0, 0, 4, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(CommitWritePipe, 0, 0, 4, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(IsValidReserveId, 1, 1, 1, 1, Capability(Pipes), 0, List(OperandId))
Instruction(GetNumPipePackets, 1, 1, 3, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId))
Instruction(GetMaxPipePackets, 1, 1, 3, 1, Capability(Pipes), 0, List(OperandId, OperandId, OperandId))
Instruction(GroupReserveReadPipePackets, 1, 1, 5, 1, Capability(Pipes), 0, List(OperandScope, OperandId, OperandId, OperandId, OperandId))
Instruction(GroupReserveWritePipePackets, 1, 1, 5, 1, Capability(Pipes), 0, List(OperandScope, OperandId, OperandId, OperandId, OperandId))
Instruction(GroupCommitReadPipe, 0, 0, 5, 1, Capability(Pipes), 0, List(OperandScope, OperandId, OperandId, OperandId, OperandId))
Instruction(GroupCommitWritePipe, 0, 0, 5, 1, Capability(Pipes), 0, List(OperandScope, OperandId, OperandId, OperandId, OperandId))
Instruction(EnqueueMarker, 1, 1, 4, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(EnqueueKernel, 1, 1, 11, 1, Capability(DeviceEnqueue), 1, List(OperandId, OperandId, OperandId, OperandId, OperandId, OperandId, OperandId, OperandId, OperandId, OperandId, OperandVariableIds))
Instruction(GetKernelNDrangeSubGroupCount, 1, 1, 5, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId, OperandId, OperandId, OperandId))
Instruction(GetKernelNDrangeMaxSubGroupSize, 1, 1, 5, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId, OperandId, OperandId, OperandId))
Instruction(GetKernelWorkGroupSize, 1, 1, 4, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(GetKernelPreferredWorkGroupSizeMultiple, 1, 1, 4, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId, OperandId, OperandId))
Instruction(RetainEvent, 0, 0, 1, 1, Capability(DeviceEnqueue), 0, List(OperandId))
Instruction(ReleaseEvent, 0, 0, 1, 1, Capability(DeviceEnqueue), 0, List(OperandId))
Instruction(CreateUserEvent, 1, 1, 0, 1, Capability(DeviceEnqueue), 0, EmptyList)
Instruction(IsValidEvent, 1, 1, 1, 1, Capability(DeviceEnqueue), 0, List(OperandId))
Instruction(SetUserEventStatus, 0, 0, 2, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId))
Instruction(CaptureEventProfilingInfo, 0, 0, 3, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId, OperandId))
Instruction(GetDefaultQueue, 1, 1, 0, 1, Capability(DeviceEnqueue), 0, EmptyList)
Instruction(BuildNDRange, 1, 1, 3, 1, Capability(DeviceEnqueue), 0, List(OperandId, OperandId, OperandId))
Instruction(ImageSparseSampleImplicitLod, 1, 1, 3, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseSampleExplicitLod, 1, 1, 3, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseSampleDrefImplicitLod, 1, 1, 4, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseSampleDrefExplicitLod, 1, 1, 4, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseSampleProjImplicitLod, 1, 1, 3, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseSampleProjExplicitLod, 1, 1, 3, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseSampleProjDrefImplicitLod, 1, 1, 4, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseSampleProjDrefExplicitLod, 1, 1, 4, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseFetch, 1, 1, 3, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseGather, 1, 1, 4, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseDrefGather, 1, 1, 4, 1, Capability(SparseResidency), 1, List(OperandId, OperandId, OperandId, OperandOptionalImage))
Instruction(ImageSparseTexelsResident, 1, 1, 1, 1, Capability(SparseResidency), 0, List(OperandId))
Instruction(NoLine, 0, 0, 0, 0, Capability(None), 0, EmptyList)
Instruction(AtomicFlagTestAndSet, 1, 1, 3, 1, Capability(Kernel), 0, List(OperandId, OperandScope, OperandMemorySemantics))
Instruction(AtomicFlagClear, 0, 0, 3, 1, Capability(Kernel), 0, List(OperandId, OperandScope, OperandMemorySemantics))
