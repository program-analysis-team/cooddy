/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <memory>

#include "Analyzer.h"
#include "CompilerOptions.h"
#include "CompilerOptionsList.h"
#include "Parser.h"
#include "ProblemsList.h"
#include "ProjectPaths.h"
#include "TaintMacroCliDefinition.h"
#include "TestBaseClass.h"
#include "dfa/TaintSettings.h"

using namespace HCXX;

class TaintSettingsTest : public TestBaseClass {
protected:
    std::vector<Problem> Analyze(std::string_view path)
    {
        Workspace workspace(TEST_SUITE_PATH(path).string());

        ProblemsList problemsList(workspace);
        TestAnalyze(workspace, problemsList);
        return problemsList.GetProblems();
    }
};

template <typename F>
bool Contains(const std::vector<Problem>& problems, F&& unaryPredicate)
{
    return std::find_if(problems.begin(), problems.end(), unaryPredicate) != problems.end();
}

TEST_F(TaintSettingsTest, TaintSettingsGenerationTest)
{
    // Default values
    ASSERT_EQ(TaintSettings::FlagsDefault(), TaintSettings::FlagsFromCli(TaintSettings::DEFAULT_FLAGS));

    // Correct inputs
    ASSERT_EQ(TaintSettings::FlagsFromCli("all"), TaintSettings::FlagsFromCli("usp") | TaintSettings::DEFINED);
    ASSERT_EQ(TaintSettings::FlagsFromCli("none"), TaintSettings::NONE);
    ASSERT_EQ(TaintSettings::FlagsFromCli("usp"), TaintSettings::FlagsFromCli("ups"));
    ASSERT_EQ(TaintSettings::FlagsFromCli("usp"), TaintSettings::FlagsFromCli("sup"));
    ASSERT_EQ(TaintSettings::FlagsFromCli("usp"), TaintSettings::FlagsFromCli("psu"));

    // Incorrect inputs
    ASSERT_EQ(TaintSettings::FlagsFromCli(""), TaintSettings::FlagsDefault());
    ASSERT_EQ(TaintSettings::FlagsFromCli("\0"), TaintSettings::FlagsDefault());
    ASSERT_EQ(TaintSettings::FlagsFromCli("usa"), TaintSettings::FlagsDefault());
    ASSERT_EQ(TaintSettings::FlagsFromCli("nonex"), TaintSettings::FlagsDefault());
    ASSERT_EQ(TaintSettings::FlagsFromCli("allx"), TaintSettings::FlagsDefault());
    myTestErrorCheckConsumer.ClearToolError();
}
