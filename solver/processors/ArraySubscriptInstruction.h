/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_ARRAYSUBSCRIPTINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_ARRAYSUBSCRIPTINSTRUCTION_H_

#include <ast/ArraySubscriptExpression.h>

#include "ast/UnaryExpression.h"

namespace Processor {

class ArraySubscriptInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        auto array = static_cast<const ArraySubscriptExpression*>(&node);
        auto unary = node.GetParentOfType<UnaryExpression>();
        auto base = array->GetBase();
        context.Compile(array->GetIdx());
        context.Compile(base);
        auto baseArray = Node::Cast<ArraySubscriptExpression>(base->GetInnerNode());
        if (baseArray != nullptr && baseArray->GetType().IsConstantArray() &&
            (unary == nullptr || unary->GetOperation() != UnaryExpression::Operation::ADDR_OF)) {
            auto baseBase = baseArray->GetBase()->GetInnerNode();
            if (baseBase != nullptr && baseBase->GetType().IsConstantArray()) {
                context.Add(baseArray->GetSizeOfType());
                return;
            }
        }
        context.Add(0);
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto indexExpr = context.Execute();
        auto arrExpr = context.Execute(&symbolId);
        auto size = context.Get<uint32_t>();
        if (size != 0) {
            context->SetSymbolSize(arrExpr, context->CreateIntegerExpr(size));
        }
        return context->GetArrayExpr(symbolId, arrExpr, indexExpr);
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_ARRAYSUBSCRIPTINSTRUCTION_H_
