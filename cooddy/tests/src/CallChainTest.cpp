/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <CallChainConsumer.h>
#include <CompilerOptionsList.h>
#include <Parser.h>
#include <ProblemsHolder.h>

#include "CheckerTest.h"
#include "TestBaseClass.h"

// The fixture for testing class CallChainTest
class CallChainTest : public TestBaseClass, public testing::WithParamInterface<std::string> {
    class CallChainTestErrorConsumer : public CallChainConsumer {
        TestErrorCheckConsumer* myTestErrorCheckConsumer;

    public:
        explicit CallChainTestErrorConsumer(TestErrorCheckConsumer* testErrorCheckConsume)
            : CallChainConsumer("main", 4), myTestErrorCheckConsumer(testErrorCheckConsume)
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
        std::filesystem::path checkerSuit(TEST_SUITE_PATH("CallChain"));
        ::HCXX::Test::FileSystem::ForEachFileInDirectory(checkerSuit, ".json", [&](const std::filesystem::path& entry) {
            result.emplace_back(entry.generic_u8string());
        });
        return result;
    }

protected:
    CallChainTestErrorConsumer myCallChainTestErrorConsumer;

    CallChainTest() : myCallChainTestErrorConsumer(&myTestErrorCheckConsumer) {}

    void TestCallChainAnalyze(Parser& parser, Workspace& workspace, ProblemsHolder& holder)
    {
        auto analyzer = HCXX::Analyzer::Create(parser, workspace);
        analyzer->Analyze(workspace.GetCompilerOptions(), holder, myCallChainTestErrorConsumer);
    }
};

TEST_P(CallChainTest, AnalyzeAsExpected)
{
    std::filesystem::path jsonFile = GetParam().c_str();
    std::string resultJson;
    std::ifstream resultStream(jsonFile.string());
    std::getline(resultStream, resultJson);

    std::cout << jsonFile.string() << std::endl;

    HCXX::Workspace workspace(jsonFile.replace_extension(".cpp").string());
    HCXX::ProblemsHolder problemsList(workspace);
    TestCallChainAnalyze(TestBaseClass::GetParser(), workspace, problemsList);

    std::stringstream outJson;
    myCallChainTestErrorConsumer.OutReport(outJson);

    ASSERT_EQ(outJson.str(), resultJson);
}

INSTANTIATE_TEST_SUITE_P(GeneralAndSpecial, CallChainTest, testing::ValuesIn(CallChainTest::GetTestCases()));
