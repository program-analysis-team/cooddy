/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CALLINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CALLINSTRUCTION_H_

#include <ast/CallExpression.h>
#include <ast/CxxConstructExpression.h>
#include <ast/CxxCtorDecl.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxNewExpression.h>
#include <ast/CxxOperatorCallExpression.h>
#include <ast/CxxRecordDecl.h>

#include "LiteralInstruction.h"

namespace Processor {

class CallInstruction : public InstructionProcessor {
protected:
    enum class CallType {
        NONE,
        INDIRECT,
        COPY_CTOR,
        BUILTIN_CONST,
        BUILTIN_EXPECT,
        EQUAL_OPERATOR,
        EQUAL_EQUAL_OPERATOR,
        EXCLAIM_EQUAL_OPERATOR,
        DEREF_OPERATOR,
        BOOL_CONVERSION
    };

    virtual CallType GetCallType(const CallExpression& call)
    {
        if (call.GetFunction() == nullptr) {
            return call.GetKind() == Node::Kind::CALL_EXPRESSION ? CallType::INDIRECT : CallType::NONE;
        }
        const auto& funcName = call.GetFunction()->GetQualifiedName();
        if (funcName == "__builtin_constant_p") {
            return CallType::BUILTIN_CONST;
        }
        if (funcName == "__builtin_expect") {
            return CallType::BUILTIN_EXPECT;
        }
        return CallType::NONE;
    }

    void Compile(const Node& node, CompileContext& context) override
    {
        auto callExpr = static_cast<const CallExpression*>(&node);
        context.AddEnum(GetCallType(*callExpr));
        context.Add(FunctionBehaviorImpl::MakeArgType(callExpr->GetType()));
        CompileCallee(*callExpr, context);

        auto& args = callExpr->GetArguments();
        uint16_t argsCount = args.size();
        context.Add<uint16_t>(argsCount);
        for (auto i = 0; i < argsCount; ++i) {
            context.Compile(args[i]);
            context.Add(FunctionBehaviorImpl::MakeArgType(callExpr->GetArgumentType(i), args[i]));
        }
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto callType = context.GetEnum<CallType>();

        z3::expr callExpr = context->CreateSymbolExpr(symbolId);
        context->EnterCall(callExpr, context.instruction, context.Get<ArgType>());
        if (callType == CallType::INDIRECT) {
            context->SetCallExpr(context.Execute(), false);
        } else {
            ExecuteCallee(context, callExpr, symbolId);
        }

        auto argsCount = context.Get<uint16_t>();

        if (callType != CallType::NONE && callType != CallType::INDIRECT) {
            auto result = ExecuteByType(context, callType, symbolId);
            context->SetCallResult(result, true);
            return result;
        }
        ExecuteArgs(context, argsCount);
        return callExpr;
    }

    z3::expr ExecuteByType(ExecutionContext& context, CallType type, SymbolId& symbolId)
    {
        switch (type) {
            case CallType::COPY_CTOR:
                return ExecuteArg(context, true);
            case CallType::EQUAL_OPERATOR: {
                z3::expr lhe = ExecuteArg(context, false, &symbolId);
                z3::expr rhe = ExecuteArg(context, false);
                return symbolId.IsValid() ? context->SetSymbol(symbolId, rhe) : rhe;
            }
            case CallType::EQUAL_EQUAL_OPERATOR: {
                z3::expr lhe = ExecuteArg(context, false);
                z3::expr rhe = ExecuteArg(context, false);
                return context->CastToBV(lhe) == context->CastToBV(rhe);
            }
            case CallType::EXCLAIM_EQUAL_OPERATOR: {
                z3::expr lhe = ExecuteArg(context, false);
                z3::expr rhe = ExecuteArg(context, false);
                return context->CastToBV(lhe) != context->CastToBV(rhe);
            }
            case CallType::DEREF_OPERATOR: {
                ExecuteArg(context, false, &symbolId);
                return context->GetSubSymbol(symbolId, VirtualOffset(VirtualOffset::Kind::DEREF));
            }
            case CallType::BOOL_CONVERSION: {
                auto& args = context->GetCurCallArgs();
                if (args.empty()) {
                    return context->CreateSymbolExpr(symbolId);  // LCOV_EXCL_LINE
                }
                auto callObj = args[0].expr;
                return context->CastToBV(callObj) != context->CreateIntegerExpr(0);
            }
            case CallType::BUILTIN_CONST: {
                z3::expr arg = ExecuteArg(context, false);
                return context->CreateBoolExpr(arg.is_const() && arg.is_numeral());
            }
            case CallType::BUILTIN_EXPECT: {
                z3::expr lhe = ExecuteArg(context, true);
                z3::expr rhe = ExecuteArg(context, true);
                return lhe;
            }
                // LCOV_EXCL_START
            default:
                return context->CreateSymbolExpr(symbolId);
                // LCOV_EXCL_STOP
        }
    }

    virtual void CompileCallee(const CallExpression& callExpr, CompileContext& context)
    {
        context.Compile(callExpr.GetCallee());
    }

    virtual void ExecuteCallee(ExecutionContext& context, z3::expr& callExpr, SymbolId& symbolId)
    {
        context.Execute();
    }

    void ExecuteArgs(ExecutionContext& context, uint32_t argsCount)
    {
        for (uint32_t i = 0; i < argsCount; ++i) {
            ExecuteArg(context, true);
        }
    }

    z3::expr ExecuteArg(ExecutionContext& context, bool addToCallArgs, SymbolId* symbolId = nullptr)
    {
        Instruction argInstr = context.curInstr + sizeof(InstructionHeader);
        SymbolId argSymbol(context->GetStackPos());
        auto argExpr = context.Execute(&argSymbol);
        auto argType = context.Get<ArgType>();
        if (addToCallArgs) {
            context->AddCallArg({argType, argInstr, argSymbol, argExpr});
        }
        if (symbolId != nullptr) {
            *symbolId = argSymbol;
        }
        return argExpr;
    }

    void PostExecute(ExecutionContext& context, z3::expr& result) override
    {
        result = context->LeaveCall();
    }
};

class CxxMemberCallInstruction : public CallInstruction {
    CallType GetCallType(const CallExpression& call) override
    {
        auto method = Node::Cast<CxxMethodDecl>(call.GetFunction());
        if (method != nullptr && method->GetFuncKind() == FunctionDecl::FunctionKind::CONVERSION) {
            auto retType = method->GetRetType();
            if (retType.IsTrivial() && retType.GetSizeInBits() == 8) {
                return CallType::BOOL_CONVERSION;
            }
        }
        return CallInstruction::GetCallType(call);
    }

    void CompileCallee(const CallExpression& callExpr, CompileContext& context) override
    {
        auto& memberCall = static_cast<const CxxMemberCallExpression&>(callExpr);
        context.Compile(memberCall.GetObject());
        context.Add(ArgType{1});
        context.Add<bool>(memberCall.IsVirtualCall());
    }

    void ExecuteCallee(ExecutionContext& context, z3::expr& callExpr, SymbolId& symbolId) override
    {
        auto objExpr = ExecuteArg(context, true);
        if (context.Get<bool>()) {
            context->SetCallExpr(objExpr, true);
        }
    }
};

class CxxOperatorInstruction : public CallInstruction {
    CallType GetCallType(const CallExpression& call) override
    {
        auto op = static_cast<const CxxOperatorCallExpression&>(call).GetOperator();
        if (op == CxxOperatorCallExpression::OperatorKind::EQUAL && call.GetArguments().size() == 2) {
            return CallType::EQUAL_OPERATOR;
        }
        if (op == CxxOperatorCallExpression::OperatorKind::EQUAL_EQUAL && call.GetArguments().size() == 2) {
            return CallType::EQUAL_EQUAL_OPERATOR;
        }
        if (op == CxxOperatorCallExpression::OperatorKind::EXCLAIM_EQUAL && call.GetArguments().size() == 2) {
            return CallType::EXCLAIM_EQUAL_OPERATOR;
        }
        if (op == CxxOperatorCallExpression::OperatorKind::STAR && call.GetArguments().size() == 1) {
            return CallType::DEREF_OPERATOR;
        }
        return CallInstruction::GetCallType(call);
    }
};

class CxxConstructInstruction : public CallInstruction {
    CallType GetCallType(const CallExpression& call) override
    {
        auto ctorDecl = Node::Cast<CxxCtorDecl>(call.GetFunction());
        if (ctorDecl != nullptr && call.GetArguments().size() == 1 &&
            (ctorDecl->IsCopy() || ctorDecl->IsMove() || IsArgumentNull(*call.GetArguments()[0]))) {
            return CallType::COPY_CTOR;
        }
        return CallInstruction::GetCallType(call);
    }

    bool IsArgumentNull(const Node& arg)
    {
        auto literalExpr = Node::Cast<LiteralExpression>(arg.GetInnerNode());
        return literalExpr != nullptr && literalExpr->GetLiteralType() == LiteralExpression::LiteralType::NULLPTR;
    }

    void CompileCallee(const CallExpression& callExpr, CompileContext& context) override {}

    void ExecuteCallee(ExecutionContext& context, z3::expr& callExpr, SymbolId& symbolId) override
    {
        context->AddCallArg({ArgType{1}, context.instruction, symbolId, callExpr});
    }

    void PostExecute(ExecutionContext& context, z3::expr& result) override
    {
        context->LeaveCall();
    }
};

class CxxThisInstruction : public InstructionProcessor {
    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        symbolId.SetVarInstr(FunctionBehaviorImpl::GetThisInstruction());
        return context->GetSymbol(symbolId);
    }
};

class NewInstruction : public CallInstruction {
protected:
    void Compile(const Node& node, CompileContext& context) override
    {
        CallInstruction::Compile(node, context);
        auto newExpr = Node::Cast<CxxNewExpression>(&node);
        context.Add<uint32_t>(newExpr->GetType().GetSizeInBits() / 8);
        context.Add<bool>(newExpr->IsArrayNew());
        if (newExpr->IsArrayNew()) {
            context.Compile(newExpr->GetArrayElemCount());
        }
        auto recordDecl = Node::Cast<RecordDecl>(node.GetType().GetPointedDeclaration());
        context.Add<uint32_t>(recordDecl ? recordDecl->GetUniqueId() : 0);
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        z3::expr base = CallInstruction::Execute(context, symbolId);
        uint32_t typeSize = context.Get<uint32_t>();
        bool isArrayNew = context.Get<bool>();
        z3::expr z3TypeSize = context->CreateIntegerExpr(typeSize);
        if (isArrayNew) {
            auto res = context.Execute();
            context->SetSymbolSize(base, z3TypeSize * res);
        } else {
            context->SetSymbolSize(base, z3TypeSize);
        }
        auto recordId = context.Get<uint32_t>();
        if (recordId != 0) {
            context->AddRecordId(recordId, symbolId);
        }
        return base;
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_CALLINSTRUCTION_H_
