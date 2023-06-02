/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "InstructionProcessor.h"

#include "FunctionBehaviorImpl.h"
#include "ast/ParenExpression.h"
#include "processors/ArraySubscriptInstruction.h"
#include "processors/AsmInstruction.h"
#include "processors/BinaryInstruction.h"
#include "processors/BreakStatementInstruction.h"
#include "processors/CallInstruction.h"
#include "processors/CastInstruction.h"
#include "processors/CompoundStatementInstruction.h"
#include "processors/ConditionalInstruction.h"
#include "processors/CtorInitializerInstruction.h"
#include "processors/DeclInstruction.h"
#include "processors/ImplicitValueInitListInstruction.h"
#include "processors/InitListInstruction.h"
#include "processors/LiteralInstruction.h"
#include "processors/MemberExprInstruction.h"
#include "processors/ProxyInstruction.h"
#include "processors/RefInstruction.h"
#include "processors/ReturnInstruction.h"
#include "processors/ThrowInstruction.h"
#include "processors/UnaryInstruction.h"
#include "processors/VarDeclInstruction.h"

void InstructionProcessor::CompileContext::Compile(const Node* node)
{
    InstructionHeader header{static_cast<uint32_t>(node != nullptr ? node->GetKind() : Node::Kind::UNDEFINED)};
    auto funcRange = owner.GetFunctionRange();
    auto nodeRange = node != nullptr ? node->GetRange() : funcRange;

    header.rangeOffset = funcRange.Contains(nodeRange) ? nodeRange.begin - funcRange.begin : UINT16_MAX;
    header.rangeSize = std::min(nodeRange.end - nodeRange.begin, uint32_t(UINT8_MAX));

    if (header.rangeOffset >= UINT16_MAX) {
        // param instruction should be related to function to avoid instruction size extension
        header.rangeOffset = (node == nullptr || !node->IsKindOf(Node::Kind::PARAM_VAR_DECL)) ? UINT16_MAX : 0;
    }
    if (node == nullptr) {
        Add<InstructionHeader>(header);
        return;
    }
    auto alreadyAddedInstr = owner.AddInstruction(instrSet.Count() + sizeof(header), *node);
    if (alreadyAddedInstr) {
        if (auto prevHeader = owner.GetInstructionHeader(alreadyAddedInstr); prevHeader != nullptr) {
            prevHeader->isCached = 1;
            header.isCached = 1;
            header.kind = 0;
            Add<InstructionHeader>(header);
            Add<Instruction>(alreadyAddedInstr);
            return;
        }
    }

    Add<InstructionHeader>(header);
    if (header.rangeOffset == UINT16_MAX) {
        Add<uint32_t>(nodeRange.begin);
    }
    ++curDepth;
    GetProcessor(node->GetKind()).Compile(*node, *this);
    --curDepth;
}

z3::expr InstructionProcessor::ExecutionContext::Execute(SymbolId* symbolId)
{
    auto header = Get<InstructionHeader>();
    if (header.isCached && header.kind == 0) {
        return context.GetExprFromCache(Get<Instruction>());
    }
    auto instrKind = static_cast<Node::Kind>(header.kind);
    auto& processor = GetProcessor(instrKind);

    Instruction instr = curInstr;
    instruction = instr;

    if (header.rangeOffset == UINT16_MAX) {
        Get<uint32_t>();
    }

    SymbolId resultSymbol(context.GetStackPos(), instr);
    z3::expr result = processor.Execute(*this, resultSymbol);

    if (static_cast<z3::ast>(result) == nullptr) {
        throw z3::exception("z3::expr is null");  // LCOV_EXCL_LINE
    }
    ExecutionResult callbackResult = ExecutionResult::OK;
    if (callback != nullptr) {
        do {
            context.SetCurExecInfo(instr, instrKind, result, resultSymbol);
            callbackResult = callback();
        } while (callbackResult == ExecutionResult::REPEAT);
    }
    processor.PostExecute(*this, result);
    context.ClearOperands();

    if (callbackResult != ExecutionResult::OK) {
        throw ExecutionException(callbackResult);
    }

    if (symbolId != nullptr) {
        *symbolId = resultSymbol;
    }
    if (header.isCached) {
        context.AddExprToCache(instr, result);
    }
    return result;
}

void InstructionProcessor::ExecutionContext::ExecutePostOperations()
{
    for (auto& it : postIncs) {
        auto expr = context.GetSymbol(it.first);
        expr = context.CastToBV(expr) + context.CreateIntegerExpr(it.second);
        context.SetSymbol(it.first, expr);
    }
    postIncs.clear();
}

z3::expr InstructionProcessor::Execute(ExecutionContext& context, SymbolId& symbolId)
{
    return context->CreateSymbolExpr(symbolId);
}

#define BIND(kind, processor)                   \
    case Node::Kind::kind: {                    \
        static Processor::processor _processor; \
        return _processor;                      \
    }

InstructionProcessor& InstructionProcessor::GetProcessor(Node::Kind kind)
{
    switch (kind) {
        BIND(VAR_DECL, VarDeclInstruction)
        BIND(REF_EXPRESSION, RefInstruction)
        BIND(INT_LITERAL_EXPRESSION, LiteralInstruction)
        BIND(BOOL_LITERAL_EXPRESSION, LiteralInstruction)
        BIND(CHAR_LITERAL_EXPRESSION, LiteralInstruction)
        BIND(LITERAL_EXPRESSION, LiteralInstruction)
        BIND(STRING_LITERAL_EXPRESSION, LiteralInstruction)
        BIND(UNARY_EXPRESSION, UnaryInstruction)
        BIND(BINARY_EXPRESSION, BinaryInstruction)
        BIND(DELETE_EXPRESSION, CallInstruction)
        BIND(CXX_NEW_EXPRESSION, NewInstruction)
        BIND(CALL_EXPRESSION, CallInstruction)
        BIND(CXX_CONSTRUCT_EXPRESSION, CxxConstructInstruction)
        BIND(CXX_DTOR_EXPRESSION, CxxMemberCallInstruction)
        BIND(CXX_MEMBER_CALL_EXPRESSION, CxxMemberCallInstruction)
        BIND(CXX_OPERATOR_CALL_EXPRESSION, CxxOperatorInstruction)
        BIND(ARRAY_SUBSCRIPT_EXPRESSION, ArraySubscriptInstruction)
        BIND(CAST_EXPRESSION, CastInstruction)
        BIND(TEMPORARY_EXPRESSION, ProxyInstruction)
        BIND(PAREN_EXPRESSION, ProxyInstruction)
        BIND(RETURN_STATEMENT, ReturnInstruction)
        BIND(DECL_STATEMENT, DeclInstruction)
        BIND(MEMBER_EXPRESSION, MemberExprInstruction)
        BIND(THIS_EXPRESSION, CxxThisInstruction)
        BIND(CONDITIONAL_EXPRESSION, ConditionalInstruction)
        BIND(INIT_LIST_EXPRESSION, InitListInstruction)
        BIND(CXX_CTOR_INITIALIZER, CtorInitializerInstruction)
        BIND(THROW_EXPRESSION, ThrowInstruction)
        BIND(IMPLICIT_VALUE_INIT_LIST_EXPRESSION, ImplicitValueInitListInstruction)
        BIND(BREAK_STATEMENT, BreakStatementInstruction)
        BIND(COMPOUND_STATEMENT, CompoundStatementInstruction)
        BIND(ASM_STATEMENT, AsmInstruction)
        default:
            break;
    }
    static InstructionProcessor defProcessor;
    return defProcessor;
}
