/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_COMPOUNDSTATEMENTINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_COMPOUNDSTATEMENTINSTRUCTION_H_

#include <ast/CompoundStatement.h>
#include <ast/Node.h>

namespace Processor {

class CompoundStatementInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        auto children = node.GetChildren();
        context.Add<uint32_t>(children.size());
        for (auto child : children) {
            // Ignore nested compound statements due to too long execution of huge hierarchy of compound statements
            context.Compile(Node::Cast<CompoundStatement>(child) == nullptr ? child : nullptr);
        }
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto size = context.Get<uint32_t>();
        for (auto i = 0; i < size; i++) {
            context.Execute();
        }
        return context->CreateSymbolExpr(symbolId);
    }
};

}  // namespace Processor

#endif
