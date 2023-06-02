/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CheckerTest.h"
#include "workspace/Workspace.h"

DECLARE_CHECKER(GenericTaintChecker)

DECLARE_TEST(SimpleChecker, SimpleChecker)
DECLARE_TEST(UncoveredEmptyChecker, UncoveredEmptyChecker)

void InitTests()
{
#ifdef TEST_PROFILE
    HCXX::Workspace::Init(TEST_PROFILE);
#else
    HCXX::Workspace::Init("test");
#endif
    registerSimpleChecker();
    registerUncoveredEmptyChecker();
}
