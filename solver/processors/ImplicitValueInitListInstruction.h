/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_IMPLICITVALUEINITLISTITRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_IMPLICITVALUEINITLISTITRUCTION_H_

#include <ast/ImplicitValueInitExpression.h>
#include <ast/Type.h>

#include <iostream>

namespace Processor {

class ImplicitValueInitListInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        auto type = static_cast<const ImplicitValueInitExpression*>(&node)->GetType();
        bool shouldBeInitedWithNull = type.IsTrivial() || type.IsPointer() && !type.IsReference();
        context.Add<bool>(shouldBeInitedWithNull);
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto initWithNull = context.Get<bool>();
        if (initWithNull) {
            return context->CreateIntegerExpr(0);
        }
        return context->CreateSymbolExpr(symbolId);
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_IMPLICITVALUEINITLISTITRUCTION_H_
