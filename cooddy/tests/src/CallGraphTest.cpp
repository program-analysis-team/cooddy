/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <CallGraphConsumer.h>
#include <CompilerOptionsList.h>
#include <Parser.h>
#include <ProblemsHolder.h>

#include "CheckerTest.h"
#include "TestBaseClass.h"

// The fixture for testing class CallGraphTest
class CallGraphTest : public TestBaseClass, public testing::WithParamInterface<std::string> {
    class CallGraphTestErrorConsumer : public CallGraphConsumer {
        TestErrorCheckConsumer* myTestErrorCheckConsumer;

    public:
        explicit CallGraphTestErrorConsumer(TestErrorCheckConsumer* testErrorCheckConsume)
            : CallGraphConsumer(true), myTestErrorCheckConsumer(testErrorCheckConsume)
        {}

        void OnAnalysisBegin(HCXX::TranslationUnit& unit) override
        {
            TestErrorCheckConsumer::SetAsCurrent(myTestErrorCheckConsumer);
        }
    };

public:
    static std::vector<std::string> GetTestCases()
    {
        std::vector<std::string> result;
        std::filesystem::path checkerSuit(TEST_SUITE_PATH("CallGraph"));
        ::HCXX::Test::FileSystem::ForEachFileInDirectory(checkerSuit, ".json", [&](const std::filesystem::path& entry) {
            std::filesystem::path path = entry.generic_u8string();
            if (path.filename() != "compile_commands.json") {
                result.emplace_back(path.string());
            }
        });
        return result;
    }

protected:
    CallGraphTestErrorConsumer myCallGraphTestErrorConsumer;

    CallGraphTest() : myCallGraphTestErrorConsumer(&myTestErrorCheckConsumer) {}

    void TestCallGraphAnalyze(Parser& parser, Workspace& workspace, ProblemsHolder& holder)
    {
        auto analyzer = HCXX::Analyzer::Create(parser, workspace);
        analyzer->Analyze(workspace.GetCompilerOptions(), holder, myCallGraphTestErrorConsumer);
    }
};

TEST_P(CallGraphTest, AnalyzeAsExpected)
{
    std::filesystem::path jsonFile = GetParam().c_str();
    std::cout << jsonFile.string() << std::endl;
    std::string resultJson;
    std::ifstream resultStream(jsonFile.string());
    std::getline(resultStream, resultJson);
    std::filesystem::path root = absolute(jsonFile).string();
    // Bug on linux in absolute path
    root = std::regex_replace(root.string(), std::regex("/\\./"), "/");
    myCallGraphTestErrorConsumer.SetPath(root);
    jsonFile.replace_extension(".cpp");
    if (!exists(jsonFile)) {
        jsonFile.replace_extension("");
    }
    HCXX::Workspace workspace(jsonFile.string());
    HCXX::ProblemsHolder problemsList(workspace);
    TestCallGraphAnalyze(TestBaseClass::GetParser(), workspace, problemsList);

    std::stringstream outJson;
    myCallGraphTestErrorConsumer.OutReport(outJson);

    ASSERT_EQ(outJson.str(), resultJson);
}

TEST_P(CallGraphTest, AnalyzeAsExpectedDot)
{
    std::filesystem::path jsonFile = GetParam().c_str();
    std::filesystem::path dotFile = jsonFile.replace_extension(".dot");
    std::cout << dotFile.string() << std::endl;
    std::ifstream resultStream(dotFile.string());
    std::string resultDot = string((std::istreambuf_iterator<char>(resultStream)), std::istreambuf_iterator<char>());
    std::filesystem::path root = absolute(dotFile).string();
    // Bug on linux in absolute path
    root = std::regex_replace(root.string(), std::regex("/\\./"), "/");
    myCallGraphTestErrorConsumer.SetPath(root);
    jsonFile.replace_extension(".cpp");
    if (!exists(jsonFile)) {
        jsonFile.replace_extension("");
    }
    HCXX::Workspace workspace(jsonFile.string());
    HCXX::ProblemsHolder problemsList(workspace);
    TestCallGraphAnalyze(TestBaseClass::GetParser(), workspace, problemsList);

    std::stringstream outDot;
    myCallGraphTestErrorConsumer.OutReport(outDot, true);
    ASSERT_TRUE(outDot.str().find(resultDot) != std::string::npos);
}

INSTANTIATE_TEST_SUITE_P(GeneralAndSpecial, CallGraphTest, testing::ValuesIn(CallGraphTest::GetTestCases()));
