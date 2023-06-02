/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_VARDECLINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_VARDECLINSTRUCTION_H_

#include <CrossTUContext.h>
#include <ast/VarDecl.h>

namespace Processor {

class VarDeclInstruction : public InstructionProcessor {
    struct VarInfo {
        uint8_t hasInitializer : 1;
        uint8_t addSizeToContext : 1;
        uint8_t addRecordId : 1;
        uint8_t isGlobal : 1;
        uint8_t isStringLiteral : 1;
        uint8_t isStatic : 1;
    };

    void Compile(const Node& node, CompileContext& context) override
    {
        auto varDecl = Node::Cast<VarDecl>(&node);
        auto initializer = varDecl->GetInitializer();
        auto type = node.GetType();
        auto recordDecl = Node::Cast<RecordDecl>(type.GetDeclaration());
        auto stringLiteral =
            initializer != nullptr ? Node::Cast<StringLiteralExpression>(initializer->GetInnerNode()) : nullptr;
        uint32_t memorySize = 0;

        VarInfo varInfo{initializer != nullptr && stringLiteral == nullptr,
                        varDecl->IsMemoryBuffer(memorySize),
                        recordDecl != nullptr && !type.IsArray(),
                        !varDecl->IsLocalVariableDeclaration(),
                        stringLiteral != nullptr,
                        varDecl->IsStaticLocal()};
        context.Add<VarInfo>(varInfo);
        if (varInfo.isGlobal) {
            context.Add<uint32_t>(varDecl != nullptr ? varDecl->GetUniqueId() : 0);
        }
        if (varInfo.addSizeToContext) {
            context.Add<uint32_t>(memorySize != 0 ? memorySize : UNDEFINED_VALUE);
        }
        if (varInfo.addRecordId) {
            context.Add<uint32_t>(recordDecl->GetUniqueId());
        }
        context.Compile(initializer);
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto varInfo = context.Get<VarInfo>();
        if (varInfo.isGlobal) {
            symbolId = context->CreateGlobalSymbolId(context.Get<uint32_t>(), context.instruction);
        } else {
            symbolId.SetVarInstr(context.instruction);
        }
        uint32_t varSize = varInfo.addSizeToContext ? context.Get<uint32_t>() : 0U;
        uint32_t recordId = varInfo.addRecordId ? context.Get<uint32_t>() : 0U;

        SymbolId initSymbolId(symbolId.GetStackPos());
        auto initExpr = context.Execute(&initSymbolId);

        const z3::expr* result = nullptr;
        if (varInfo.hasInitializer) {
            result = &context->SetSymbol(symbolId, initExpr, &initSymbolId);
            if (varInfo.isStatic) {
                auto sizeInfo = context->FindSizeExpr(*result);
                if (sizeInfo != nullptr && sizeInfo->isAlloc) {
                    context->AddNotNullCondition(*result);
                }
            }
        } else {
            result = &context->SetSymbol(symbolId, context->CreateSymbolExpr(symbolId));
        }
        if (varSize != 0) {
            context->SetSymbolSize(*result, context->CreateIntegerExpr(varSize), varInfo.isStringLiteral);
            if (varInfo.addRecordId) {
                auto sizeExpr = context->CreateSymbolExpr(symbolId + VirtualOffset(VirtualOffset::Kind::ADDR_OF));
                context->SetSymbolSize(sizeExpr, context->CreateIntegerExpr(varSize), varInfo.isStringLiteral);
            }
        }
        if (recordId != 0) {
            context->AddRecordId(recordId, symbolId);
        }
        return *result;
    }
};

}  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_VARDECLINSTRUCTION_H_
