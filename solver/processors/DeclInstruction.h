/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_DECLINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_DECLINSTRUCTION_H_

#include <ast/DeclStatement.h>

namespace Processor {

class DeclInstruction : public InstructionProcessor {
    void Compile(const Node& node, CompileContext& context) override
    {
        auto& decls = Node::Cast<DeclStatement>(&node)->GetDeclGroup();
        context.Add<uint8_t>(decls.size());
        for (auto& decl : decls) {
            context.Compile(decl);
        }
    }
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        uint32_t declsCount = context.Get<uint8_t>();
        for (uint32_t i = 0; i < declsCount; ++i) {
            context.Execute();
        }
        return context->CreateSymbolExpr(symbolId);
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_DECLINSTRUCTION_H_
