/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_REFINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_REFINSTRUCTION_H_

#include <ast/EnumConstantDecl.h>
#include <ast/LoopStatement.h>
#include <ast/RefExpression.h>

namespace Processor {

class RefInstruction : public InstructionProcessor {
public:
    struct RefInfo {
        uint8_t isConst : 1;
        uint8_t isGlobal : 1;
        uint8_t addSizeToContext : 1;
    };

    void Compile(const Node& node, CompileContext& context) override
    {
        auto declaration = Node::Cast<RefExpression>(&node)->GetDeclaration();
        auto typedNode = Node::Cast<TypedNode>(declaration);
        auto varDecl = Node::Cast<VarDecl>(declaration);
        auto varInstr = declaration != nullptr ? context.owner.GetInstruction(*declaration) : 0U;
        uint32_t memorySize = 0;

        RefInfo refInfo{varInstr == 0 && typedNode != nullptr && typedNode->IsConstExpr(),
                        varDecl != nullptr && !varDecl->IsLocalVariableDeclaration(),
                        typedNode != nullptr && typedNode->IsKindOf(Node::Kind::PARAM_VAR_DECL) &&
                            typedNode->IsMemoryBuffer(memorySize)};
        context.Add<RefInfo>(refInfo);

        if (refInfo.isConst) {
            context.Add<int64_t>(typedNode->GetSLimitedValue());
            return;
        }
        if (refInfo.addSizeToContext) {
            context.Add<uint32_t>(memorySize);
        }
        if (varInstr == 0 && declaration != nullptr) {
            varInstr = context.owner.GetInstruction(node);
            context.owner.AddInstruction(varInstr, *declaration, true);
        }
        context.Add<Instruction>(varInstr);
        if (refInfo.isGlobal) {
            context.Add<uint32_t>(varDecl != nullptr ? varDecl->GetUniqueId() : 0);
        }
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto refInfo = context.Get<RefInfo>();
        if (refInfo.isConst) {
            return context->CreateIntegerExpr(context.Get<int64_t>());
        }
        uint32_t refSize = refInfo.addSizeToContext ? context.Get<uint32_t>() : 0U;
        auto instr = context.Get<Instruction>();
        if (refInfo.isGlobal) {
            symbolId = context->CreateGlobalSymbolId(context.Get<uint32_t>(), instr);
        } else {
            symbolId.SetVarInstr(instr);
        }
        const z3::expr* result = &context->GetSymbol(symbolId);
        if (refSize != 0) {
            context->SetSymbolSize(*result, context->CreateIntegerExpr(refSize), true);
        }
        return *result;
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_REFINSTRUCTION_H_
