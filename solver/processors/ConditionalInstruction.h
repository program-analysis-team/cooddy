/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CONDITIONALINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CONDITIONALINSTRUCTION_H_

#include <ast/ConditionalExpression.h>

namespace Processor {

class ConditionalInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        const auto& expr = static_cast<const ConditionalExpression*>(&node);
        context.Compile(expr->GetCondition());
        context.Compile(expr->GetTrueExpression());
        context.Compile(expr->GetFalseExpression());
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto condition = context.Execute();
        auto trueExpr = context.Execute();
        auto falseExpr = context.Execute();
        return z3::ite(context->CastToBool(condition), context->CastToBV(trueExpr), context->CastToBV(falseExpr));
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CONDITIONALINSTRUCTION_H_
