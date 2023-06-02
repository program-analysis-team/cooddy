/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <CompilerOptionsList.h>
#include <ProblemsList.h>

#include "ProjectPaths.h"
#include "TestBaseClass.h"

using namespace HCXX;

// The fixture for testing class CompileTargetTest
class CompileTargetTest : public TestBaseClass {};

TEST_F(CompileTargetTest, UnreachableNonTargetCodeTest)
{
    Workspace workspace(TEST_SUITE_PATH("CompileTargetTest/compile_commands.json").string(), {}, {}, "cwe");
    auto optionsList = workspace.GetCompilerOptions();
    ASSERT_TRUE(!optionsList.Get().empty());
    ASSERT_TRUE(optionsList.Get()[1].HasOption("-m32"));
    ASSERT_TRUE(optionsList.Get()[0].HasOption("-m64"));

    HCXX::ProblemsList myProblems(workspace);
    TestAnalyze(workspace, myProblems, &optionsList);

    ASSERT_TRUE(myProblems.GetProblems().empty());
}

TEST_F(CompileTargetTest, UsingCwcharTest)
{
    Workspace workspace(TEST_SUITE_PATH("CompileTargetTest/using_cwchar_cc.json").string());
    auto optionsList = workspace.GetCompilerOptions();
    ASSERT_TRUE(!optionsList.Get().empty());
    ASSERT_TRUE(optionsList.Get()[0].HasOption("-m64"));
    ASSERT_TRUE(optionsList.Get()[0].HasOption("-std=c++11"));

    HCXX::ProblemsList myProblems(workspace);
    TestAnalyze(workspace, myProblems, &optionsList);

    ASSERT_FALSE(myProblems.GetProblems().empty());
}
