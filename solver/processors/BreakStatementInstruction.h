/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_BREAKSTATEMENTINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_BREAKSTATEMENTINSTRUCTION_H_

namespace Processor {

class BreakStatementInstruction : public InstructionProcessor {
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        context->AddLoopExitCondition();
        return context->CreateSymbolExpr(symbolId);
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_BREAKSTATEMENTINSTRUCTION_H_
