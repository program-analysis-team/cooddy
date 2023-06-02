/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_STRINGTOEXPRCONVERTER_H
#define COODDY_STRINGTOEXPRCONVERTER_H

#include <ast/VirtualOffset.h>
#include <z3++.h>

#include <cstdint>
#include <functional>
#include <optional>

#include "utils/ConditionParser.h"
#include "utils/Log.h"

class StringToExprConverter {
public:
    using CreateExprCallback = std::function<z3::expr(bool, bool, uint32_t, HCXX::VirtualOffset*)>;

    explicit StringToExprConverter(const std::string& cond, bool isResult, uint32_t argPos, HCXX::VirtualOffset offset,
                                   const CreateExprCallback& callback)
        : myCondition(cond), myIsResult(isResult), myArgPos(argPos), myCallback(callback), myOffset(offset)
    {}

    std::optional<z3::expr> Convert()
    {
        if (myIsResult && myArgPos > 0) {
            myCondition = "$" + std::to_string(myArgPos) + "==" + myCondition;
        }

        auto tokens = ConditionParser::Lexer::Tokenize(myCondition);
        if (tokens.empty()) {
            return std::nullopt;
        }

        if (ConditionParser::Token::IsOpKind(tokens.front().GetKind())) {
            myCondition = "$" + std::to_string(myArgPos) + myCondition;
            tokens = ConditionParser::Lexer::Tokenize(myCondition);
        }

        auto parser = ConditionParser::Parser(tokens);
        auto root = parser.Parse();

        return root != nullptr ? Convert(root) : std::nullopt;
    }

private:
    const CreateExprCallback& myCallback;
    HCXX::VirtualOffset myOffset;
    std::string myCondition;
    uint32_t myArgPos;
    bool myIsResult;

    std::optional<z3::expr> Convert(const ConditionParser::Node* node)
    {
        if (node == nullptr) {
            // LCOV_EXCL_START
            return std::nullopt;
            // LCOV_EXCL_STOP
        }
        if (auto val = ConditionParser::Node::Cast<ConditionParser::Value>(node); val != nullptr) {
            if (val->IsConst()) {
                return myCallback(false, true, val->GetValue(), nullptr);
            } else if (val->IsFreeSymbol()) {
                return myCallback(true, false, 0, &myOffset);
            } else {
                return myCallback(false, false, val->GetValue(), &myOffset);
            }
        } else if (auto call = ConditionParser::Node::Cast<ConditionParser::CallExpr>(node); call != nullptr) {
            if (call->GetName() == "ite") {
                auto& args = call->GetArgs();
                if (args.size() != 3) {
                    // LCOV_EXCL_START
                    return std::nullopt;
                    // LCOV_EXCL_STOP
                }
                auto arg1 = Convert(args[0]);
                auto arg2 = Convert(args[1]);
                auto arg3 = Convert(args[2]);
                if (arg1 && arg2 && arg3) {
                    return z3::ite(*arg1, *arg2, *arg3);
                }
            }
        } else if (auto binary = ConditionParser::Node::Cast<ConditionParser::BinaryExpr>(node); binary != nullptr) {
            auto lhs = Convert(binary->GetLhs());
            auto rhs = Convert(binary->GetRhs());
            if (lhs && rhs) {
                return ConvertBinaryExpr(*binary, *lhs, *rhs);
            }
        }
        // LCOV_EXCL_START
        return std::nullopt;
        // LCOV_EXCL_STOP
    }

    static std::optional<z3::expr> ConvertBinaryExpr(const ConditionParser::BinaryExpr& node, const z3::expr& lhs,
                                                     const z3::expr& rhs)
    {
        switch (node.GetOperation()) {
            case ConditionParser::BinaryExpr::Operation::MUL:
                return lhs * rhs;
            case ConditionParser::BinaryExpr::Operation::DIV:
                return lhs / rhs;
            case ConditionParser::BinaryExpr::Operation::REM:
                return lhs % rhs;
            case ConditionParser::BinaryExpr::Operation::ADD:
                return lhs + rhs;
            case ConditionParser::BinaryExpr::Operation::SUB:
                return lhs - rhs;
            case ConditionParser::BinaryExpr::Operation::GT:
                return lhs > rhs;
            case ConditionParser::BinaryExpr::Operation::LT:
                return lhs < rhs;
            case ConditionParser::BinaryExpr::Operation::GE:
                return lhs >= rhs;
            case ConditionParser::BinaryExpr::Operation::LE:
                return lhs <= rhs;
            case ConditionParser::BinaryExpr::Operation::NE:
                return lhs != rhs;
            case ConditionParser::BinaryExpr::Operation::EQ:
                return lhs == rhs;
            case ConditionParser::BinaryExpr::Operation::AND:
                return lhs && rhs;
            case ConditionParser::BinaryExpr::Operation::OR:
                return lhs || rhs;
                // LCOV_EXCL_START
            default:
                return std::nullopt;
                // LCOV_EXCL_STOP
        }
    }
};

#endif  // COODDY_STRINGTOEXPRCONVERTER_H
