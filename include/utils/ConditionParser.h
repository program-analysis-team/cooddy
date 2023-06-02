/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CONDITIONPARSER_H
#define COODDY_CONDITIONPARSER_H

// Formal grammar for the condition parser in BNF form (https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form)
// <factor> 	 ::= [0-9]+ | "$" [0-9]+ | "$$" | "(" <expression> ")" | <call>
// <expression3> ::= <factor> (("*" | "/" | "%") <factor>)*
// <expression2> ::= <expression3> (("+" | "-") <expression3>)*
// <expression1> ::= <expression2> ((">" | ">=" | "<" | "<=" | "!=" | "==") <expression2>)*
// <expression>  ::= <expression1> (("&&" | "||") <expression1>)*
// <call> 	 ::= [a-z]+ "(" (<expression> ("," <expression>)* )* ")"

#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "Log.h"

namespace ConditionParser {

class Token;
class Node;

using TokenList = std::vector<Token>;
using NodeList = std::vector<const Node*>;

class Token {
public:
    std::string ToString() const
    {
        return std::to_string(static_cast<uint32_t>(myKind)) + ": " + std::string(myValue);
    }

    enum class Kind {
        UNKNOWN,
        MUL,
        DIV,
        REM,
        ADD,
        SUB,
        GT,
        LT,
        GE,
        LE,
        NE,
        EQ,
        AND,
        OR,
        PAREN_OPEN,
        IDENT,
        CONST,
        ARG_POS,
        FREE_SYMBOL,
        COMMA,
        PAREN_CLOSE
    };

    explicit Token(Kind kind, std::string_view value = "") : myKind(kind), myValue(value) {}

    static bool IsOpKind(Kind kind)
    {
        return kind >= Kind::MUL && kind <= Kind::OR;
    }

    Kind GetKind() const
    {
        return myKind;
    }

    const std::string& GetValue() const
    {
        return myValue;
    }

private:
    Kind myKind = Kind::UNKNOWN;
    std::string myValue;
};

class Lexer {
    static std::string ReadToken(std::string_view input, uint32_t i, const std::function<bool(char)>& f)
    {
        std::string str;
        do {
            str += input[i];
            i++;
        } while (f(input[i]));
        return str;
    }

public:
    // COODDY_SUPPRESS FunctionLengthChecker
    static TokenList Tokenize(std::string_view input)
    {
        TokenList res;
        for (uint32_t i = 0; i < input.size(); i++) {
            if (input[i] == '(') {
                res.emplace_back(Token::Kind::PAREN_OPEN);
            } else if (input[i] == ')') {
                res.emplace_back(Token::Kind::PAREN_CLOSE);
            } else if (isdigit(input[i])) {
                auto str = ReadToken(input, i, isdigit);
                i += str.size() - 1;
                res.emplace_back(Token::Kind::CONST, str);
            } else if (isalpha(input[i])) {
                auto str = ReadToken(input, i, isalpha);
                i += str.size() - 1;
                res.emplace_back(Token::Kind::IDENT, str);
            } else if (input[i] == '$' && i + 1 < input.size() && isdigit(input[i + 1])) {
                auto str = ReadToken(input, i + 1, isdigit);
                i += str.size();
                res.emplace_back(Token::Kind::ARG_POS, str);
            } else if (input[i] == '$' && i + 1 < input.size() && input[i + 1] == '$') {
                i++;
                res.emplace_back(Token::Kind::FREE_SYMBOL);
            } else if (input[i] == ',') {
                res.emplace_back(Token::Kind::COMMA);
            } else if (input[i] == '*') {
                res.emplace_back(Token::Kind::MUL);
            } else if (input[i] == '/') {
                res.emplace_back(Token::Kind::DIV);
            } else if (input[i] == '%') {
                res.emplace_back(Token::Kind::REM);
            } else if (input[i] == '+') {
                res.emplace_back(Token::Kind::ADD);
            } else if (input[i] == '-') {
                res.emplace_back(Token::Kind::SUB);
            } else if (input[i] == '<') {
                if (i + 1 < input.size() && input[i + 1] == '=') {
                    i++;
                    res.emplace_back(Token::Kind::LE);
                } else {
                    res.emplace_back(Token::Kind::LT);
                }
            } else if (input[i] == '>') {
                if (i + 1 < input.size() && input[i + 1] == '=') {
                    i++;
                    res.emplace_back(Token::Kind::GE);
                } else {
                    res.emplace_back(Token::Kind::GT);
                }
            } else if (input[i] == '!' && i + 1 < input.size() && input[i + 1] == '=') {
                i++;
                res.emplace_back(Token::Kind::NE);
            } else if (input[i] == '=' && i + 1 < input.size() && input[i + 1] == '=') {
                i++;
                res.emplace_back(Token::Kind::EQ);
            } else if (input[i] == '&' && i + 1 < input.size() && input[i + 1] == '&') {
                i++;
                res.emplace_back(Token::Kind::AND);
            } else if (input[i] == '|' && i + 1 < input.size() && input[i + 1] == '|') {
                i++;
                res.emplace_back(Token::Kind::OR);
            } else if (input[i] != ' ') {
                HCXX::Log(HCXX::LogLevel::ERROR)
                    << "Found unknown token in string condition \'" << input << "', index: " << i << "\n";
                return {};
            }
        }
        return res;
    }
};

class Node {
public:
    enum class Kind { NODE, BINARY_EXPR, VALUE, CALL_EXPR };

    virtual ~Node() = default;

    template <class T>
    static const T* Cast(const Node* node)
    {
        return node != nullptr && node->IsKindOf(T::KIND) ? static_cast<const T*>(node) : nullptr;
    }

    // LCOV_EXCL_START
    virtual bool IsKindOf(Kind kind) const
    {
        return kind == Kind::NODE;
    }
    // LCOV_EXCL_STOP

    static const Kind KIND = Kind::NODE;
};

class BinaryExpr : public Node {
public:
    enum class Operation {
        UNKNOWN,
        MUL,
        DIV,
        REM,
        ADD,
        SUB,
        GT,
        LT,
        GE,
        LE,
        NE,
        EQ,
        AND,
        OR,
    };

    explicit BinaryExpr(const Node* lhs, const Node* rhs, Operation op) : myLhs(lhs), myRhs(rhs), myOp(op) {}

    static Operation ConvertTokenType(Token::Kind kind)
    {
        return static_cast<Operation>(static_cast<uint32_t>(kind));
    }

    const Node* GetLhs() const
    {
        return myLhs;
    }

    const Node* GetRhs() const
    {
        return myRhs;
    }

    Operation GetOperation() const
    {
        return myOp;
    }

    bool IsKindOf(Kind kind) const override
    {
        return kind == KIND;
    }

    static const Kind KIND = Kind::BINARY_EXPR;

private:
    const Node* myLhs;
    const Node* myRhs;
    Operation myOp;
};

class Value : public Node {
public:
    explicit Value(uint32_t value, bool isConst = false, bool isFreeSymbol = false)
        : myValue(value), myIsConst(isConst), myIsFreeSymbol(isFreeSymbol)
    {}

    uint32_t GetValue() const
    {
        return myValue;
    }

    bool IsConst() const
    {
        return myIsConst;
    }

    bool IsFreeSymbol() const
    {
        return myIsFreeSymbol;
    }

    bool IsKindOf(Kind kind) const override
    {
        return kind == KIND;
    }

    static const Kind KIND = Kind::VALUE;

private:
    uint32_t myValue;
    bool myIsConst;
    bool myIsFreeSymbol;
};

class CallExpr : public Node {
public:
    explicit CallExpr(std::string_view name, std::vector<const Node*>&& args) : myName(name), myArgs(std::move(args)) {}

    const std::string& GetName() const
    {
        return myName;
    }

    const std::vector<const Node*>& GetArgs() const
    {
        return myArgs;
    }

    bool IsKindOf(Kind kind) const override
    {
        return kind == KIND;
    }

    static const Kind KIND = Kind::CALL_EXPR;

private:
    std::string myName;
    std::vector<const Node*> myArgs;
};

class Parser {
public:
    explicit Parser(const TokenList& tokens) : myTokens(tokens), myCurrTokenIdx(0)
    {
        myCurrTokenKind = !myTokens.empty() ? myTokens[0].GetKind() : Token::Kind::UNKNOWN;
    }

    const Node* Parse()
    {
        auto root = Expression();
        if (myFoundUnexpectedToken || myCurrTokenIdx != myTokens.size()) {
            HCXX::Log(HCXX::LogLevel::ERROR) << "Found unexpected token\n";
            return nullptr;
        }
        return root;
    }

    ~Parser()
    {
        for (auto ptr : myNodes) {
            delete ptr;
        }
    }

private:
    uint32_t myCurrTokenIdx;
    Token::Kind myCurrTokenKind;
    TokenList myTokens;
    NodeList myNodes;
    bool myFoundUnexpectedToken = false;

    void Next()
    {
        if (++myCurrTokenIdx < myTokens.size()) {
            myCurrTokenKind = myTokens[myCurrTokenIdx].GetKind();
            return;
        }
        myCurrTokenKind = Token::Kind::UNKNOWN;
    }

    bool Accept(Token::Kind kind)
    {
        if (myCurrTokenKind == kind) {
            Next();
            return true;
        }
        return false;
    }

    bool Expect(Token::Kind kind)
    {
        if (Accept(kind)) {
            return true;
        }
        myFoundUnexpectedToken = true;
        return false;
    }

    const Node* Factor()
    {
        if (Accept(Token::Kind::CONST)) {
            myNodes.emplace_back(new Value(std::stoi(myTokens[myCurrTokenIdx - 1].GetValue()), true));
            return myNodes.back();
        } else if (Accept(Token::Kind::ARG_POS)) {
            myNodes.emplace_back(new Value(std::stoi(myTokens[myCurrTokenIdx - 1].GetValue())));
            return myNodes.back();
        } else if (Accept(Token::Kind::FREE_SYMBOL)) {
            myNodes.emplace_back(new Value(0, false, true));
            return myNodes.back();
        } else if (Accept(Token::Kind::PAREN_OPEN)) {
            auto node = Expression();
            Expect(Token::Kind::PAREN_CLOSE);
            return node;
        }
        return Call();
    }

    const Node* CreateBinExpr(const std::function<bool(Token::Kind)>& checkToken,
                              const std::function<const Node*()>& exprFunc)
    {
        auto lhs = exprFunc();
        if (lhs == nullptr) {
            myFoundUnexpectedToken = true;
            return nullptr;
        }
        while (checkToken(myCurrTokenKind)) {
            auto kind = myCurrTokenKind;
            Next();
            auto rhs = exprFunc();
            if (rhs == nullptr) {
                myFoundUnexpectedToken = true;
                return nullptr;
            }
            myNodes.emplace_back(new BinaryExpr(lhs, rhs, BinaryExpr::ConvertTokenType(kind)));
            lhs = myNodes.back();
        }
        return lhs;
    }

    const Node* Expression3()
    {
        return CreateBinExpr(
            [](Token::Kind kind) {
                return kind == Token::Kind::MUL || kind == Token::Kind::DIV || kind == Token::Kind::REM;
            },
            [this]() { return Factor(); });
    }

    const Node* Expression2()
    {
        return CreateBinExpr([](Token::Kind kind) { return kind == Token::Kind::ADD || kind == Token::Kind::SUB; },
                             [this]() { return Expression3(); });
    }

    const Node* Expression1()
    {
        return CreateBinExpr(
            [](Token::Kind kind) {
                return kind == Token::Kind::GT || kind == Token::Kind::GE || kind == Token::Kind::LT ||
                       kind == Token::Kind::LE || kind == Token::Kind::NE || kind == Token::Kind::EQ;
            },
            [this]() { return Expression2(); });
    }

    const Node* Expression()
    {
        return CreateBinExpr([](Token::Kind kind) { return kind == Token::Kind::AND || kind == Token::Kind::OR; },
                             [this]() { return Expression1(); });
    }

    const Node* Call()
    {
        if (!Accept(Token::Kind::IDENT)) {
            return nullptr;
        }
        std::vector<const Node*> args;
        std::string name = myTokens[myCurrTokenIdx - 1].GetValue();
        Expect(Token::Kind::PAREN_OPEN);
        if (myCurrTokenKind != Token::Kind::PAREN_CLOSE) {
            while (1) {
                args.emplace_back(Expression());
                if (myCurrTokenKind != Token::Kind::COMMA) {
                    break;
                }
                Next();
            }
        }
        Expect(Token::Kind::PAREN_CLOSE);
        myNodes.emplace_back(new CallExpr(name, std::move(args)));
        return myNodes.back();
    }
};

}  // namespace ConditionParser

#endif  // COODDY_CONDITIONPARSER_H
