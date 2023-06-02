/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <utils/ConditionParser.h>

#include "CheckerTest.h"
#include "TestBaseClass.h"

class ConditionParserTest : public TestBaseClass {};

TEST_F(ConditionParserTest, LexerTest)
{
    using namespace ConditionParser;
    ASSERT_TRUE(Lexer::Tokenize("").empty());

    auto t1 = Lexer::Tokenize(" */%+-><>=<=!===&&||(ite42$43,)");
    ASSERT_EQ(t1.size(), 19);
    constexpr std::array<Token::Kind, 19> expectedKinds{
        Token::Kind::MUL,   Token::Kind::DIV,     Token::Kind::REM,   Token::Kind::ADD,        Token::Kind::SUB,
        Token::Kind::GT,    Token::Kind::LT,      Token::Kind::GE,    Token::Kind::LE,         Token::Kind::NE,
        Token::Kind::EQ,    Token::Kind::AND,     Token::Kind::OR,    Token::Kind::PAREN_OPEN, Token::Kind::IDENT,
        Token::Kind::CONST, Token::Kind::ARG_POS, Token::Kind::COMMA, Token::Kind::PAREN_CLOSE};
    for (auto i = 0; i < t1.size(); i++) {
        ASSERT_EQ(t1[i].GetKind(), expectedKinds[i]);
    }
    ASSERT_EQ(t1[14].GetValue(), "ite");
    ASSERT_EQ(t1[15].GetValue(), "42");
    ASSERT_EQ(t1[16].GetValue(), "43");
}

TEST_F(ConditionParserTest, LexerFailTest)
{
    std::array<std::string_view, 3> strings = {"$", "^", "!$"};
    for (auto& str : strings) {
        ASSERT_TRUE(ConditionParser::Lexer::Tokenize(str).empty());
        ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError("Found unknown token in string condition"));
        myTestErrorCheckConsumer.ClearToolError();
    }
}

TEST_F(ConditionParserTest, ParserSimpleTest)
{
    using namespace ConditionParser;
    auto parser = Parser(Lexer::Tokenize("$10 + 42"));
    auto root = parser.Parse();

    auto binary = Node::Cast<BinaryExpr>(root);
    ASSERT_NE(binary, nullptr);
    ASSERT_EQ(binary->GetOperation(), BinaryExpr::Operation::ADD);

    auto lhs = Node::Cast<Value>(binary->GetLhs());
    ASSERT_NE(lhs, nullptr);
    ASSERT_EQ(lhs->GetValue(), 10);
    ASSERT_FALSE(lhs->IsConst());

    auto rhs = Node::Cast<Value>(binary->GetRhs());
    ASSERT_NE(rhs, nullptr);
    ASSERT_EQ(rhs->GetValue(), 42);
    ASSERT_TRUE(rhs->IsConst());
}

TEST_F(ConditionParserTest, ParserPriorityTest)
{
    using namespace ConditionParser;
    auto parser = Parser(Lexer::Tokenize("1 - 2 * (3 + 4) || 4"));
    auto root = parser.Parse();

    auto binary = Node::Cast<BinaryExpr>(root);
    ASSERT_NE(binary, nullptr);
    ASSERT_EQ(binary->GetOperation(), BinaryExpr::Operation::OR);

    binary = Node::Cast<BinaryExpr>(binary->GetLhs());
    ASSERT_NE(binary, nullptr);
    ASSERT_EQ(binary->GetOperation(), BinaryExpr::Operation::SUB);

    binary = Node::Cast<BinaryExpr>(binary->GetRhs());
    ASSERT_NE(binary, nullptr);
    ASSERT_EQ(binary->GetOperation(), BinaryExpr::Operation::MUL);

    binary = Node::Cast<BinaryExpr>(binary->GetRhs());
    ASSERT_NE(binary, nullptr);
    ASSERT_EQ(binary->GetOperation(), BinaryExpr::Operation::ADD);
}

TEST_F(ConditionParserTest, ParserCallTest)
{
    using namespace ConditionParser;
    auto parser = Parser(Lexer::Tokenize("$1 != callA(callB(1 == $2, 2), callC() > callD(), 5)"));
    auto root = parser.Parse();

    auto binary = Node::Cast<BinaryExpr>(root);
    ASSERT_NE(binary, nullptr);
    ASSERT_EQ(binary->GetOperation(), BinaryExpr::Operation::NE);
    ASSERT_NE(Node::Cast<Value>(binary->GetLhs()), nullptr);

    auto callA = Node::Cast<CallExpr>(binary->GetRhs());
    ASSERT_NE(callA, nullptr);
    ASSERT_EQ(callA->GetName(), "callA");

    auto& argsA = callA->GetArgs();
    ASSERT_EQ(argsA.size(), 3);

    auto callB = Node::Cast<CallExpr>(argsA[0]);
    ASSERT_NE(callB, nullptr);
    ASSERT_EQ(callB->GetName(), "callB");
    ASSERT_EQ(callB->GetArgs().size(), 2);
    ASSERT_NE(Node::Cast<BinaryExpr>(callB->GetArgs()[0]), nullptr);
    ASSERT_NE(Node::Cast<Value>(callB->GetArgs()[1]), nullptr);

    auto binary2 = Node::Cast<BinaryExpr>(argsA[1]);
    ASSERT_NE(binary2, nullptr);
    ASSERT_EQ(binary2->GetOperation(), BinaryExpr::Operation::GT);
    ASSERT_NE(Node::Cast<CallExpr>(binary2->GetLhs()), nullptr);
    ASSERT_NE(Node::Cast<CallExpr>(binary2->GetRhs()), nullptr);

    ASSERT_NE(Node::Cast<Value>(argsA[2]), nullptr);
}

TEST_F(ConditionParserTest, ParserFailTest)
{
    using namespace ConditionParser;
    std::array<std::string_view, 8> strings = {"", "1 2", "1 (2)", "(1) 2", "> 2", "2 <", "call(1,2,3", "call 1,2,3)"};
    for (auto& str : strings) {
        auto parser = Parser(Lexer::Tokenize(str));
        ASSERT_EQ(parser.Parse(), nullptr);
        ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError("Found unexpected token"));
        myTestErrorCheckConsumer.ClearToolError();
    }
}
