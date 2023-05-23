/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include <ProblemsList.h>

#include "ProjectPaths.h"
#include "TestBaseClass.h"

// The fixture for testing class ProfileTest
class ProfileTest : public TestBaseClass {};

TEST_F(ProfileTest, ProfileIsCopied)
{
    auto path = ProjectPaths::GetBinaryFolder() / ".cooddy";
    ASSERT_TRUE(std::filesystem::exists(path)) << " No path found: '" << path << "'";
    ASSERT_TRUE(std::filesystem::is_directory(path)) << " Found path '" << path << "' is not a directory";

    path.append("default.profile.json");
    ASSERT_TRUE(std::filesystem::exists(path)) << " No path found: '" << path << "'";
    ASSERT_TRUE(std::filesystem::is_regular_file(path)) << " Found path '" << path << "' is not a file";
}

TEST_F(ProfileTest, UnfoundrofileTest)
{
    auto file = TEST_SUITE_PATH("ProfileTest").append("main.cpp").string();
    HCXX::Workspace workspace(std::move(file), {}, {}, "error");
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);
    ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError("No profile found: \"error\""));
    myTestErrorCheckConsumer.ClearToolError();
}