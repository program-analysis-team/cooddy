/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "TestBaseClass.h"

// The fixture for testing class StrUtilsTests
class EnvironmentUtilsTest : public TestBaseClass {};

using namespace HCXX::EnvironmentUtils;

TEST_F(EnvironmentUtilsTest, GetSelfExecutable)
{
    ASSERT_NE(GetSelfExecutablePath(), "");                                                   // Returns a value
    ASSERT_EQ(&GetSelfExecutablePath(), &GetSelfExecutablePath());                            // Caches the value
    ASSERT_EQ(std::filesystem::canonical(GetSelfExecutablePath()), GetSelfExecutablePath());  // Is canonical

    ASSERT_NE(GetSelfExecutableDir(), "");
    ASSERT_EQ(&GetSelfExecutableDir(), &GetSelfExecutableDir());
    ASSERT_EQ(std::filesystem::canonical(GetSelfExecutableDir()), GetSelfExecutableDir());
}
