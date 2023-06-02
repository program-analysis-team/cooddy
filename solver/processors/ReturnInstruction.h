/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_RETURNINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_RETURNINSTRUCTION_H_

#include <ast/ReturnStatement.h>

namespace Processor {

class ReturnInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        context.Compile(Node::Cast<ReturnStatement>(&node)->GetRetValue());
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        context->AddLoopExitCondition();
        auto returnExpr = context.Execute();
        symbolId.SetVarInstr(context->RET_INSTRUCTION);
        return context->SetSymbol(symbolId, returnExpr);
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_RETURNINSTRUCTION_H_
