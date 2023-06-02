/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CTORINITIALIZERINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CTORINITIALIZERINSTRUCTION_H_

#include <ast/CxxCtorInitializerDecl.h>

namespace Processor {

class CtorInitializerInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        auto initNode = static_cast<const CxxCtorInitializerDecl*>(&node);
        context.Compile(initNode->GetInitializer());

        auto fieldNode = Node::Cast<FieldDecl>(initNode->GetFieldDecl());
        context.Add<uint32_t>(fieldNode != nullptr ? fieldNode->GetIndex() + 1 : 0U);
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto symbolExpr = context.Execute();
        auto fieldIndex = context.Get<uint32_t>();
        if (fieldIndex == 0) {
            return context->CreateSymbolExpr(symbolId);  // LCOV_EXCL_LINE
        }
        symbolId.SetVarInstr(FunctionBehaviorImpl::GetThisInstruction());
        context->GetSubSymbol(symbolId, VirtualOffset(VirtualOffset::Kind::INDEX, fieldIndex - 1));
        return context->SetSymbol(symbolId, symbolExpr);
    }
};
}  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CTORINITIALIZERINSTRUCTION_H_
