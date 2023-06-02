/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <CompilerOptionsList.h>
#include <ProblemsList.h>

#include <filesystem>

#include "ProjectPaths.h"
#include "TestBaseClass.h"

// The fixture for testing class SuppressTest
class SuppressTest : public TestBaseClass {};

TEST_F(SuppressTest, SuppressCommonTest)
{
    auto file = TEST_SUITE_PATH("SuppressTest").string();
    HCXX::Workspace workspace(std::move(file), {}, {}, "cwe");
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);
    ASSERT_EQ(problemsList.GetProblems().size(), 1);
}

TEST_F(SuppressTest, IgnoreSuppressesFlagTest)
{
    auto file = TEST_SUITE_PATH("SuppressTest").string();
    HCXX::Workspace workspace(std::move(file), {}, {true, HCXX::Problem::Severity::NOTIFY}, "cwe");
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);
    ASSERT_EQ(problemsList.GetProblems().size(), 1);
}
