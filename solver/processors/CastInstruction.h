/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CASTINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CASTINSTRUCTION_H_

#include <ast/CastExpression.h>

namespace Processor {

class CastInstruction : public InstructionProcessor {
    enum CastType { DEFAULT, NARROWING, CHANGE_SIGN };

    void Compile(const Node& node, CompileContext& context) override
    {
        auto expr = static_cast<const CastExpression*>(&node);

        auto operand = expr->GetCastedExpression();
        if (operand == nullptr || expr->GetCastKind() != CastExpression::Kind::INTEGRAL_CAST) {
            context.AddEnum(CastType::DEFAULT);
        } else {
            auto exprType = expr->GetType();
            uint8_t exprSize = exprType.GetSizeInBits();
            auto opType = operand->GetInnerNode()->GetType();

            if (exprSize < opType.GetSizeInBits() && opType.IsValid() && !expr->IsConstExpr()) {
                context.AddEnum(CastType::NARROWING);
                context.Add<IntTypeInfo>(IntTypeInfo{exprType.IsUnsigned(), exprSize});

            } else if (exprType.IsUnsigned() != opType.IsUnsigned()) {
                context.AddEnum(CastType::CHANGE_SIGN);
                context.Add<IntTypeInfo>(IntTypeInfo{exprType.IsUnsigned(), exprSize});
            } else {
                context.AddEnum(CastType::DEFAULT);
            }
        }
        context.Compile(operand);
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto castType = context.GetEnum<CastType>();
        auto castInfo = castType != DEFAULT ? context.Get<IntTypeInfo>() : IntTypeInfo();
        auto result = context.Execute(&symbolId);

        if (castType == CastType::NARROWING) {
            auto modVal = castInfo.GetMaxValue() + 1;
            auto expr = context->CastToBV(result);
            result = expr % context->CreateUIntegerExpr(modVal);
            if (!castInfo.isUnsigned) {
                result = z3::ite(expr < 0, -(expr % context->CreateUIntegerExpr(modVal + 1)), result);
            }
        } else if (castType == CastType::CHANGE_SIGN) {
            auto maxVal = castInfo.GetMaxValue();
            if (castInfo.isUnsigned) {
                int64_t value = 0;
                if (result.is_numeral_i64(value) && value < 0) {
                    result = context->CreateUIntegerExpr(maxVal) + result + 1;
                }
            } else {
                uint64_t value = 0;
                if (result.is_numeral_u64(value) && value > maxVal) {
                    result = context->CreateIntegerExpr(castInfo.GetMinValue() - 1) + result -
                             context->CreateUIntegerExpr(maxVal);
                }
            }
        }
        return result;
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CASTINSTRUCTION_H_
