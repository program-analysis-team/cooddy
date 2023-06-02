/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_LITERALINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_LITERALINSTRUCTION_H_

#include <ast/BoolLiteralExpression.h>
#include <ast/CharLiteralExpression.h>
#include <ast/IntLiteralExpression.h>
#include <ast/LiteralExpression.h>
#include <ast/StringLiteralExpression.h>

#include <cinttypes>

namespace Processor {

class LiteralInstruction : public InstructionProcessor {
public:
    static uint32_t GetStringConst(const std::string& str)
    {
        return (uint32_t(str.size()) << 16U) | (StrUtils::GetStringId(str) + 1U);
    }

private:
    void Compile(const Node& node, CompileContext& context) override
    {
        auto type = Node::Cast<LiteralExpression>(&node)->GetLiteralType();
        context.AddEnum(type);

        switch (type) {
            case LiteralExpression::LiteralType::INTEGER:
                context.Add<uint64_t>(Node::Cast<IntLiteralExpression>(&node)->GetValue());
                break;

            case LiteralExpression::LiteralType::STRING:
                context.Add<uint32_t>(GetStringConst(Node::Cast<StringLiteralExpression>(&node)->GetValue()));
                break;

            case LiteralExpression::LiteralType::BOOL:
                context.Add<bool>(Node::Cast<BoolLiteralExpression>(&node)->GetValue());
                break;

            case LiteralExpression::LiteralType::NULLPTR:
                context.Add<uint64_t>(0ULL);
                break;

            case LiteralExpression::LiteralType::CHAR:
                context.Add<uint32_t>(Node::Cast<CharLiteralExpression>(&node)->GetSLimitedValue());
                break;

            default:
                // TODO: implement other types of literal HCAT-483
                break;
        }
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto type = context.GetEnum<LiteralExpression::LiteralType>();
        if (type == LiteralExpression::LiteralType::INTEGER || type == LiteralExpression::LiteralType::NULLPTR) {
            return context->CreateUIntegerExpr(context.Get<uint64_t>());
        } else if (type == LiteralExpression::LiteralType::BOOL) {
            return context->CreateBoolExpr(context.Get<bool>());
        } else if (type == LiteralExpression::LiteralType::STRING || type == LiteralExpression::LiteralType::CHAR) {
            return context->CreateIntegerExpr(context.Get<uint32_t>());
        }
        // LCOV_EXCL_START
        return context->CreateSymbolExpr(symbolId);
        // LCOV_EXCL_STOP
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_LITERALINSTRUCTION_H_
