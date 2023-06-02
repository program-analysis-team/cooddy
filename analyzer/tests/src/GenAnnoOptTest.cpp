/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <utils/ScopeGuard.h>

#include <memory>

#include "Analyzer.h"
#include "Parser.h"
#include "ProblemsList.h"
#include "ProjectPaths.h"
#include "TestBaseClass.h"
#include "dfa/TaintSettings.h"
#include "utils/TempFile.h"
#include "workspace/GenAnnoSettings.h"

// The fixture for testing class GenAnnoOptTest
class GenAnnoOptTest : public TestBaseClass {
protected:
    void DoGenAnnoOptTest(const char* genAnnoOp, const char* funcNameIncluded, const char* funcNameExcluded)
    {
        using namespace HCXX;
        TempDir dir(TEST_SUITE_PATH("WorkspaceTest/genAnnoOps/.cooddy").string());

        WorkspaceOptions workspaceOptions;
        workspaceOptions.genAnnoSettings = GenAnnoSettings::FromCliArg(genAnnoOp);
        Workspace workspace(TEST_SUITE_PATH("WorkspaceTest/genAnnoOps/foo.cpp").string(), {}, workspaceOptions);

        workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(TaintSettings{"su"}));

        auto parser = HCXX::Parser::Create();
        ProblemsList problemsList(workspace);
        TestAnalyze(workspace, problemsList, nullptr, parser.get());
        auto filePath = TEST_SUITE_PATH("WorkspaceTest/genAnnoOps/.cooddy/.generated_annotations.json");
        ASSERT_TRUE(FileHasString(funcNameIncluded, filePath));

        ASSERT_FALSE(FileHasString(funcNameExcluded, filePath));
    }
};

TEST_F(GenAnnoOptTest, GenAnnoGenerationTest)
{
    using namespace HCXX;

    // Default values
    ASSERT_EQ(GenAnnoSettings::Default().genAnnoFlags,
              GenAnnoSettings::FromCliArg(GenAnnoSettings::DEFAULT_CLI_ARG).genAnnoFlags);

    // Correct inputs
    ASSERT_EQ(GenAnnoSettings::FromCliArg("all").genAnnoFlags, GenAnnoSettings::FromCliArg("uts").genAnnoFlags);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("none").genAnnoFlags, GenAnnoSettings::NONE);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("ut").genAnnoFlags, GenAnnoSettings::FromCliArg("ut").genAnnoFlags);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("sut").genAnnoFlags, GenAnnoSettings::FromCliArg("tus").genAnnoFlags);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("ts").genAnnoFlags, GenAnnoSettings::FromCliArg("st").genAnnoFlags);

    // Incorrect inputs
    ASSERT_EQ(GenAnnoSettings::FromCliArg("").genAnnoFlags, GenAnnoSettings::Default().genAnnoFlags);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("\0").genAnnoFlags, GenAnnoSettings::Default().genAnnoFlags);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("usa").genAnnoFlags, GenAnnoSettings::Default().genAnnoFlags);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("nonex").genAnnoFlags, GenAnnoSettings::Default().genAnnoFlags);
    ASSERT_EQ(GenAnnoSettings::FromCliArg("allx").genAnnoFlags, GenAnnoSettings::Default().genAnnoFlags);
    myTestErrorCheckConsumer.ClearToolError();
}

TEST_F(GenAnnoOptTest, AllOp)
{
    DoGenAnnoOptTest("all", "_Z3boov", "unknown");
}

TEST_F(GenAnnoOptTest, Undef)
{
    DoGenAnnoOptTest("u", "_Z3boov", "_Z3foov");
}

TEST_F(GenAnnoOptTest, Taint)
{
    DoGenAnnoOptTest("t", "_Z3boov", "_Z3foov");
}
