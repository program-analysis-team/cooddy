/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//  The file contains tests for your HAST language

#include <HastMatchingEngine.h>
#include <Parser.h>

#include "CheckerTest.h"
#include "ProjectPaths.h"
#include "TestBaseClass.h"

struct HastTestParameters {
    const char* expression = nullptr;
    const char* fileTitle = nullptr;
    size_t amount = 0;

    friend std::ostream& operator<<(std::ostream& os, const HastTestParameters& params)
    {
        os << "(expression=" << params.expression << ", fileTitle=" << params.fileTitle << ", amount=" << params.amount
           << ')';
        return os;
    }
};

class HastTest : public TestBaseClass, public testing::WithParamInterface<HastTestParameters> {
public:
    static std::vector<HastTestParameters> EnumerateTestCases()
    {
        return std::vector<HastTestParameters>({
            //  Four tests below use different combination of function definition
            //  and search methods. Function declaration may be
            //  - a top level node
            //  - inside namespace
            //  The search may be applied for
            //  - root nodes only
            //  - all nodes
            {"/FunctionDecl", "function_as_root.cpp", 1},
            {"/FunctionDecl", "function_in_the_namespace.cpp", 0},
            {"//FunctionDecl", "function_as_root.cpp", 1},
            {"//FunctionDecl", "function_in_the_namespace.cpp", 1},
            //  Several following tests use different combinations of nested expressions
            //  and their positions in the tree
            {"//Namespace//Namespace", "nested_namespaces.cpp", 2},
            {"//Namespace/Namespace", "nested_namespaces.cpp", 2},
            {"/Namespace//Namespace", "nested_namespaces.cpp", 2},
            {"/Namespace/Namespace", "nested_namespaces.cpp", 1}
            //  Add new tests here
        });
    }
};

TEST_P(HastTest, TestCase)
{
    const HastTestParameters& params = GetParam();

    std::unique_ptr<HCXX::Parser> parser = HCXX::Parser::Create();

    auto path(TEST_SUITE_PATH("hast"));
    path.append(params.fileTitle);
    ASSERT_TRUE(std::filesystem::exists(path));
    ASSERT_TRUE(std::filesystem::is_regular_file(path));

    HCXX::CompilerOptions parseOptions;
    parseOptions.options.emplace_back(path.generic_string());
    HCXX::TranslationUnit tu(parseOptions);
    parser->ParseAST(tu);

    ::HCXX::HastMatchingEngine engine;

    std::string errorMsg;
    auto expressionPtr = engine.BuildExpression(params.expression, HCXX::SupportedLanguages(), &errorMsg);
    ASSERT_TRUE(expressionPtr.get());
    ASSERT_TRUE(errorMsg.empty());
    engine.MatchAll(tu);
    ASSERT_EQ(params.amount, expressionPtr->GetMatchedResult().size());
}

INSTANTIATE_TEST_SUITE_P(HastTestSuite, HastTest, testing::ValuesIn(HastTest::EnumerateTestCases()));
