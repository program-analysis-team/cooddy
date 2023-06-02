/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CheckersFactory.h"

#define TEST_SUITE_ROOT "sdk/checkers/unusedvars/tests/suite"

DECLARE_CHECKER_BEGIN(UnusedVars)
DECLARE_CHECKER(UnusedVariablesChecker)
DECLARE_CHECKER(UnusedParamChecker)
DECLARE_CHECKER(UnusedMemberExpressionChecker)
DECLARE_CHECKER_END()
