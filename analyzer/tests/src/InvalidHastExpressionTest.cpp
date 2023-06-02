/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <HastMatchingEngine.h>

#include "TestBaseClass.h"

struct InvalidHastExpressionTestParameters {
    const char* expression = nullptr;
    const char* errorMsg = nullptr;

    friend std::ostream& operator<<(std::ostream& os, const InvalidHastExpressionTestParameters& params)
    {
        os << "(expression=" << params.expression << ", error=" << params.errorMsg << ')';
        return os;
    }
};

struct InvalidHastExpressionTest : public TestBaseClass,
                                   public testing::WithParamInterface<InvalidHastExpressionTestParameters> {
public:
    static std::vector<InvalidHastExpressionTestParameters> EnumerateTestCases()
    {
        return std::vector<InvalidHastExpressionTestParameters>({
            {"", "Expression is empty or contain only white spaces"},
            {" ", "Expression is empty or contain only white spaces"},
            {"\t", "Expression is empty or contain only white spaces"},
            {" \t ", "Expression is empty or contain only white spaces"},
            {"\t \t", "Expression is empty or contain only white spaces"},
            {"  ", "Expression is empty or contain only white spaces"},
            {"\t\t", "Expression is empty or contain only white spaces"},
            {"/FunctionDecl// ", "XPath step format is incorrect or not supported: ' '"},
            {"//FunctionDecl/\t", "XPath step format is incorrect or not supported: '\t'"},
            {"//FunctionDecl//\t \t", "XPath step format is incorrect or not supported: '\t \t'"},
            {"FunctionDecl", "Node description must start from '/'"},
            {"\t  \tFunctionDecl", "Node description must start from '/'"},
            {"//StrangeNodeType", "Node type 'StrangeNodeType' isn't supported"},
            {"/FunctionDecl[  \t]", "XPath step format is incorrect or not supported: 'FunctionDecl[  \t]'"},
            {"//CastExpression[CastTypeKind='DynamicCast']",
             "XPath step format is incorrect or not supported: 'CastExpression[CastTypeKind='DynamicCast']'"},
            {"//CastExpression[@CastTypeKind-'DynamicCast']",
             "XPath step format is incorrect or not supported: 'CastExpression[@CastTypeKind-'DynamicCast']'"},
            {"//CastExpression[@CastTypeKind=DynamicCast]",
             "XPath step format is incorrect or not supported: 'CastExpression[@CastTypeKind=DynamicCast]'"},
            {"//CastExpression[@CastTypeKind='DynamicCast]",
             "XPath step format is incorrect or not supported: 'CastExpression[@CastTypeKind='DynamicCast]'"},
            {"//CastExpression[@CastTypeKind='DynamicCas't]",
             "XPath step format is incorrect or not supported: 'CastExpression[@CastTypeKind='DynamicCas't]'"},
            {"//Namespace[/Namespace]", "XPath step format is incorrect or not supported: 'Namespace[/Namespace]'"},
            {"//Namespace[[]", "XPath step format is incorrect or not supported: 'Namespace[[]'"},
            {"//StrangeNodeType[@CastTypeKind='DynamicCast']", "Node type 'StrangeNodeType' isn't supported"},
            {"/  \t/Namespace", "XPath step format is incorrect or not supported: '  \t/Namespace'"},
            {"//[@CastKind='DerivedToBase' or @CastKind='DerivedToBase']",
             "XPath step format is incorrect or not supported: '[@CastKind='DerivedToBase' or "
             "@CastKind='DerivedToBase']'"},
            {"//[@CastKind='DerivedToBase'someabsracttext]",
             "XPath step format is incorrect or not supported: '[@CastKind='DerivedToBase'someabsracttext]'"},
            // The attribute condition starts from the 'AND':
            {"//[AND @Attr2='Value2']", "XPath step format is incorrect or not supported: '[AND @Attr2='Value2']'"}
            //  Add new tests here
        });
    }
};

TEST_P(InvalidHastExpressionTest, TestCase)
{
    const InvalidHastExpressionTestParameters& params = GetParam();

    std::string errorMsg;
    HCXX::HastMatchingEngine engine;
    auto expressionPtr = engine.BuildExpression(params.expression, HCXX::SupportedLanguages(), &errorMsg);
    ASSERT_FALSE(expressionPtr);
    ASSERT_EQ(params.errorMsg, errorMsg);
}

INSTANTIATE_TEST_SUITE_P(InvalidHastExpressionSuite, InvalidHastExpressionTest,
                         testing::ValuesIn(InvalidHastExpressionTest::EnumerateTestCases()));
