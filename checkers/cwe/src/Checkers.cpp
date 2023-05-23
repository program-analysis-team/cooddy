/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#define TEST_SUITE_ROOT "sdk/checkers/cwe/tests/suite"

#include <CheckersFactory.h>

DECLARE_CHECKER_BEGIN(CWE)
DECLARE_CHECKER(DoNotReturnAddrOfLocalVarChecker)
DECLARE_CHECKER(UninitializedMemoryUsageChecker)
DECLARE_CHECKER(DivisionByZeroChecker)
DECLARE_CHECKER(UseAfterFreeChecker)
DECLARE_CHECKER(NullPtrDereferenceChecker)
DECLARE_CHECKER(SensitiveDataExposureChecker)
DECLARE_CHECKER(BufferMaxSizeChecker)
DECLARE_CHECKER(MismatchedMemManagementChecker)
DECLARE_CHECKER(OutOfBoundsChecker)
DECLARE_CHECKER(MemoryAndResourceLeakChecker)
DECLARE_CHECKER(MemoryAllocationSizeChecker)
DECLARE_CHECKER(StringSizeChecker)
DECLARE_CHECKER(LoopLimitTaintChecker)
DECLARE_CHECKER(TypeSizeMismatchChecker)
DECLARE_CHECKER(FmtSpecMismatchChecker)
DECLARE_CHECKER(IntegerOverflowChecker)
DECLARE_CHECKER(WrongStringIncrementChecker)
DECLARE_CHECKER(IncompatiblePointerTypeChecker)
DECLARE_CHECKER_END()
