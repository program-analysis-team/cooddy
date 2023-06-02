/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <Parser.h>
#include <ProblemsList.h>
#include <utils/Log.h>

#include <string>
#include <string_view>

#include "ProjectPaths.h"
#include "TestBaseClass.h"
#include "dfa/TaintSettings.h"

using namespace HCXX;

struct TaintInputOutputTest : public TestBaseClass {};

template <typename F>
bool Contains(const std::vector<Problem>& problems, F&& unaryPredicate)
{
    return std::find_if(problems.begin(), problems.end(), unaryPredicate) != problems.end();
}

TEST_F(TaintInputOutputTest, TaintInputOutputTest)
{
    Workspace workspace(TEST_SUITE_PATH("TaintInputOutput/foo.cpp").string());
    workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(HCXX::TaintSettings{"su"}));

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);

    const std::vector<Problem>& problems = problemsList.GetProblems();

    // Defined function, input arguments
    ASSERT_TRUE(Contains(problems, [](const Problem& problem) { return problem.line == 16; }));
    ASSERT_FALSE(Contains(problems, [](const Problem& problem) { return problem.line == 17; }));
    ASSERT_TRUE(Contains(problems, [](const Problem& problem) { return problem.line == 18; }));

    // Undefined function, return and output arguments
    ASSERT_TRUE(Contains(problems, [](const Problem& problem) { return problem.line == 25; }));
    ASSERT_FALSE(Contains(problems, [](const Problem& problem) { return problem.line == 26; }));
    ASSERT_TRUE(Contains(problems, [](const Problem& problem) { return problem.line == 30; }));

    // Legacy undefined function, return and output arguments
    ASSERT_TRUE(Contains(problems, [](const Problem& problem) { return problem.line == 32; }));
    ASSERT_TRUE(Contains(problems, [](const Problem& problem) { return problem.line == 36; }));
}
