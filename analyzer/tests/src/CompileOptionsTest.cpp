/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <CompilerOptionsList.h>
#include <Parser.h>
#include <ProblemsList.h>
#include <utils/TempFile.h>
#include <workspace/CompileCommand.h>

#include <deps/HTTPRequest.hpp>
#include <fstream>

#include "CheckerTest.h"
#include "TestBaseClass.h"

using namespace HCXX;

// The fixture for testing class CompileOptionsTest
class CompileOptionsTest : public TestBaseClass {
protected:
    bool CommandCompilerHasExeExtension(const std::string& str)
    {
        size_t tokenEnd = str.find(' ');
        auto compiler = str.substr(0, tokenEnd);
        return compiler.find(".exe", compiler.size() - 4) != std::string::npos;
    }
};

TEST_F(CompileOptionsTest, SeparatedIncludeFileOptionTest)
{
    std::filesystem::path testCasePath = std::filesystem::path("CompileOptions") / "SeparatedIncludeDir";
    Workspace refWS(TEST_SUITE_PATH((testCasePath / "reference_cc.json").string().c_str()).string());
    Workspace chkWS(TEST_SUITE_PATH((testCasePath / "checked_cc.json").string().c_str()).string());
    testCasePath = refWS.GetWorkspacePath().parent_path() / ".";

    auto refOptList = refWS.GetCompilerOptions().Get();
    auto chkOptList = chkWS.GetCompilerOptions().Get();

    // Check there are options
    ASSERT_TRUE(!refOptList.empty());
    ASSERT_TRUE(!chkOptList.empty());

    // Check there are options
    ASSERT_EQ(refOptList.size(), chkOptList.size());

    for (auto refIt = refOptList.begin(), chkIt = chkOptList.begin();
         refIt != refOptList.end() && chkIt != chkOptList.end(); ++refIt, ++chkIt) {
        // Check compile node options are equal
        for (auto optRefIt = refIt->options.begin(), optCheIt = chkIt->options.begin();
             optRefIt != refIt->options.end() && optCheIt != chkIt->options.end(); ++optRefIt, ++optCheIt) {
            ASSERT_EQ(*optRefIt, *optCheIt);
        }

        // Check expected contents of options
        switch (chkIt - chkOptList.begin()) {
            case 0:
                ASSERT_EQ(chkIt->options[0], (testCasePath / "src1.cpp").string());
                ASSERT_EQ(chkIt->options[1], "-std=gnu++17");
                ASSERT_EQ(chkIt->options[2], std::string("-I") + (testCasePath / "./include1").string());
                break;
            case 1:
                ASSERT_EQ(chkIt->options[0], (testCasePath / "src2.cpp").string());
                ASSERT_EQ(chkIt->options[1], "-std=gnu++17");
                ASSERT_EQ(chkIt->options[2], std::string("-I") + (testCasePath / "./include1").string());
                ASSERT_EQ(chkIt->options[3], std::string("-I") + (testCasePath / "./include2").string());
                break;
            case 2:
                ASSERT_EQ(chkIt->options[0], (testCasePath / "src3.cpp").string());
                ASSERT_EQ(chkIt->options[1], "-std=gnu++17");
                ASSERT_EQ(chkIt->options[2], std::string("-I") + (testCasePath / "./include1").string());
                ASSERT_EQ(chkIt->options[3], std::string("-I") + (testCasePath / "./include2").string());
                ASSERT_EQ(chkIt->options[4], std::string("-I") + (testCasePath / "./include3").string());
                break;
            default:
                FAIL();
        }
    }
}

TEST_F(CompileOptionsTest, WhitespaceOptionsTest)
{
    auto workspacePath = TEST_SUITE_PATH("CompileOptions");
    workspacePath.append("WhitespaceOptions");
    TempDir tmpWorkspace(workspacePath / ".cooddy");
    Workspace workspace(workspacePath.string(), {"DivisionByZeroChecker"});
    auto compilerOptions = workspace.GetCompilerOptions();
    auto compilerOptionsList = compilerOptions.Get();
    // Check there are options
    ASSERT_TRUE(!compilerOptionsList.empty());
    // Check for each element there are 5 successfully parsed options
    ASSERT_EQ(compilerOptionsList[0].options.size(), 5);
    ASSERT_EQ(compilerOptionsList[1].options.size(), 6);

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);

    // should report one problem about division on zero
    ASSERT_EQ(problemsList.GetProblems().size(), 1);
}

TEST_F(CompileOptionsTest, NoExeCompilerCallsTest)
{
    std::filesystem::path rootPath(ProjectPaths::GetProjectRoot());
    auto testsPath = rootPath.append("tests");
    for (const auto& p : std::filesystem::recursive_directory_iterator(testsPath)) {
        if (std::filesystem::is_directory(p.path()) || p.path().extension() != ".json") {
            continue;
        }

        std::vector<CompileCommand> commands;
        CompilerOptionsList parser;
        try {
            std::ifstream scopeStream(p.path());
            jsoncpp::parse(commands, scopeStream);
            scopeStream.close();
        } catch (jsoncpp::ParserError& error) {
            continue;
        }

        for (auto& it : commands) {
            if (CommandCompilerHasExeExtension(it.command)) {
                std::cerr << p.path().string() << std::endl;
                FAIL();
            }
        }
    }
}
