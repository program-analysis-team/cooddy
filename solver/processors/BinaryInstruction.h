/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_BINARYINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_BINARYINSTRUCTION_H_

#include <ast/BinaryExpression.h>

#include "RefInstruction.h"

namespace Processor {

class BinaryInstruction : public InstructionProcessor {
    struct OpFlags {
        uint8_t isPointerAssigment : 1;
        uint8_t isStaticAssigment : 1;
        uint8_t isAddrOperation : 1;
        uint8_t isUnsignedOperation : 1;
        uint8_t isConstant : 1;
    };

    void Compile(const Node& node, CompileContext& context) override
    {
        auto expr = static_cast<const BinaryExpression*>(&node);
        auto op = expr->GetOperation();
        context.AddEnum(op);

        OpFlags opFlags{0, 0};
        auto lhs = expr->GetLeftOperand();
        auto rhs = expr->GetRightOperand();
        if (IsAssignOperation(op)) {
            auto rhsType = rhs != nullptr ? rhs->GetInnerNode()->GetType() : Type();
            opFlags.isPointerAssigment = rhsType.IsPointer() && !rhsType.IsConstantArray();
            opFlags.isStaticAssigment = lhs != nullptr ? IsStaticAssigment(*lhs) : 0;
        }
        opFlags.isAddrOperation = IsAddOrSubOperation(op) && lhs != nullptr && lhs->GetType().IsPointer();
        opFlags.isUnsignedOperation = IsSignSensitiveOperation(op) && GetUnsignedFlag(op, lhs, rhs);

        auto typed = Node::Cast<TypedNode>(expr);
        opFlags.isConstant =
            (expr->IsArithmeticOp() || expr->IsBitwiseOp()) && typed != nullptr && typed->IsConstExpr();
        context.Add(opFlags);

        if (op == BinaryExpression::Operation::COMMA) {
            context.Compile(lhs);
            context.Compile(rhs);
        } else {
            context.Compile(rhs);
            context.Compile(lhs);
        }
        if (opFlags.isAddrOperation) {
            context.Add<uint32_t>(lhs->GetTranslationUnit()->GetSizeOfType(lhs->GetType().GetPointedType()) / 8);
        }
        if (opFlags.isConstant) {
            context.Add<int64_t>(typed->GetSLimitedValue());
        }
    }

    static bool IsStaticAssigment(const Node& node)
    {
        auto ref = Node::Cast<RefExpression>(node.GetInnerNode());
        auto var = Node::Cast<VarDecl>(ref != nullptr ? ref->GetDeclaration() : nullptr);
        return var != nullptr && (!var->IsLocalVariableDeclaration() || var->IsStaticLocal()) &&
               var->GetType().IsPointer();
    }

    bool GetUnsignedFlag(BinaryExpression::Operation op, const Node* lhs, const Node* rhs)
    {
        auto typedLhs = Node::Cast<TypedNode>(lhs);
        auto typedRhs = Node::Cast<TypedNode>(rhs);
        if (typedLhs == nullptr || typedRhs == nullptr) {
            return false;
        }
        auto typeLhs = typedLhs->GetInnerNode()->GetType();
        if (IsRem(op)) {
            return typeLhs.IsUnsigned();
        }
        return typeLhs.IsUnsigned() && typedRhs->IsConstExpr() && !typedLhs->IsConstExpr();
    }

    bool IsAddOrSubOperation(BinaryExpression::Operation op)
    {
        return op == BinaryExpression::Operation::ADD_ASSIGN || op == BinaryExpression::Operation::ADD ||
               op == BinaryExpression::Operation::SUB_ASSIGN || op == BinaryExpression::Operation::SUB;
    }

    bool IsAssignOperation(BinaryExpression::Operation op)
    {
        return op >= BinaryExpression::Operation::ASSIGN && op <= BinaryExpression::Operation::OR_ASSIGN;
    }

    bool IsSignSensitiveOperation(BinaryExpression::Operation op)
    {
        return op == BinaryExpression::Operation::LE || op == BinaryExpression::Operation::LT ||
               op == BinaryExpression::Operation::GE || op == BinaryExpression::Operation::GT || IsRem(op);
    }

    bool IsRem(BinaryExpression::Operation op) const
    {
        return op == BinaryExpression::Operation::REM || op == BinaryExpression::Operation::REM_ASSIGN;
    }

    z3::expr CreateShiftExpr(ExecutionContext& context, const z3::expr& lhe, const z3::expr& rhe, bool leftShift)
    {
        uint64_t rheVal = 0;
        if (!rhe.is_numeral_u64(rheVal)) {
            return lhe + rhe;
        }
        uint64_t lheVal = 0;
        if (lhe.is_numeral_u64(lheVal)) {
            return context->CreateIntegerExpr(leftShift ? lheVal << rheVal : lheVal >> rheVal);
        }
        auto powOfTwo = context->CreateUIntegerExpr(1ULL << rheVal);
        return leftShift ? lhe * powOfTwo : lhe / powOfTwo;
    }
    z3::expr CreateAndExpr(ExecutionContext& context, const z3::expr& lhe, const z3::expr& rhe)
    {
        int64_t rheVal = 0;
        int64_t lheVal = 0;
        bool isRheNumeral = rhe.is_numeral_i64(rheVal);
        bool isLheNumeral = lhe.is_numeral_i64(lheVal);
        if (isRheNumeral) {
            if (isLheNumeral) {
                return context->CreateIntegerExpr(lheVal & rheVal);
            } else if (rheVal + 1 != 0) {
                return lhe % context->CreateIntegerExpr(rheVal + 1);
            }
        } else if (isLheNumeral && lheVal + 1 != 0) {
            return rhe % context->CreateIntegerExpr(lheVal + 1);
        }
        return lhe + rhe;
    }
    template <int64_t (*T)(int64_t, int64_t)>
    z3::expr CreateExpr(ExecutionContext& context, const z3::expr& lhe, const z3::expr& rhe)
    {
        int64_t rheVal = 0;
        int64_t lheVal = 0;
        bool isRheNumeral = rhe.is_numeral_i64(rheVal);
        bool isLheNumeral = lhe.is_numeral_i64(lheVal);
        if (isRheNumeral) {
            return isLheNumeral ? context->CreateIntegerExpr(T(lheVal, rheVal)) : lhe;
        }
        return isLheNumeral ? rhe : lhe + rhe;
    }

    z3::expr CreateRemExpr(const z3::expr& lhe, const z3::expr& rhe)
    {
        return z3::ite(lhe < 0, -(lhe % rhe), lhe % rhe);
    }
    static int64_t Or(int64_t a, int64_t b)
    {
        return a | b;
    }
    static int64_t Xor(int64_t a, int64_t b)
    {
        return a ^ b;
    }
    z3::expr GetAssigningRhe(ExecutionContext& context, BinaryExpression::Operation op, const z3::expr& lhe,
                             z3::expr& rhe, SymbolId& symbolId)
    {
        switch (op) {
            case BinaryExpression::Operation::ASSIGN:
                return rhe;
            case BinaryExpression::Operation::MUL_ASSIGN:
                return lhe * rhe;
            case BinaryExpression::Operation::DIV_ASSIGN:
                return lhe / rhe;
            case BinaryExpression::Operation::REM_ASSIGN:
                return CreateRemExpr(lhe, rhe);
            case BinaryExpression::Operation::ADD_ASSIGN:
                return lhe + rhe;
            case BinaryExpression::Operation::SUB_ASSIGN:
                return lhe - rhe;
            case BinaryExpression::Operation::SHL_ASSIGN:
                return CreateShiftExpr(context, lhe, rhe, true);
            case BinaryExpression::Operation::SHR_ASSIGN:
                return CreateShiftExpr(context, lhe, rhe, false);
            case BinaryExpression::Operation::AND_ASSIGN:
                return lhe.is_bool() ? lhe & context->CastToBool(rhe) : CreateAndExpr(context, lhe, rhe);
            case BinaryExpression::Operation::XOR_ASSIGN:
                return lhe.is_bool() ? lhe ^ context->CastToBool(rhe) : CreateExpr<Xor>(context, lhe, rhe);
            case BinaryExpression::Operation::OR_ASSIGN:
                return lhe.is_bool() ? lhe | context->CastToBool(rhe) : CreateExpr<Or>(context, lhe, rhe);

                // LCOV_EXCL_START
            default:
                return context->CreateSymbolExpr(symbolId);
                // LCOV_EXCL_STOP
        }
    }

    // COODDY_SUPPRESS
    z3::expr ExecuteOp(ExecutionContext& context, BinaryExpression::Operation op, const z3::expr& lhe, z3::expr& rhe,
                       SymbolId& symbolId)
    {
        switch (op) {
            case BinaryExpression::Operation::EQ:
                return lhe == rhe;
            case BinaryExpression::Operation::NE:
                return lhe != rhe;
            case BinaryExpression::Operation::GE:
                return lhe >= rhe;
            case BinaryExpression::Operation::GT:
                return lhe > rhe;
            case BinaryExpression::Operation::LE:
                return lhe <= rhe;
            case BinaryExpression::Operation::LT:
                return lhe < rhe;
            case BinaryExpression::Operation::L_AND:
                return lhe && rhe;
            case BinaryExpression::Operation::L_OR:
                return lhe || rhe;
            case BinaryExpression::Operation::AND:
                return lhe.is_bool() ? lhe & context->CastToBool(rhe) : CreateAndExpr(context, lhe, rhe);
            case BinaryExpression::Operation::XOR:
                return lhe.is_bool() ? lhe ^ context->CastToBool(rhe) : CreateExpr<Xor>(context, lhe, rhe);
            case BinaryExpression::Operation::OR:
                return lhe.is_bool() ? lhe | context->CastToBool(rhe) : CreateExpr<Or>(context, lhe, rhe);
            case BinaryExpression::Operation::ADD:
                return lhe + rhe;
            case BinaryExpression::Operation::SUB:
                return lhe - rhe;
            case BinaryExpression::Operation::MUL:
                return lhe * rhe;
            case BinaryExpression::Operation::DIV:
                return lhe / rhe;
            case BinaryExpression::Operation::REM:
                return CreateRemExpr(lhe, rhe);
            case BinaryExpression::Operation::SHL:
                return CreateShiftExpr(context, lhe, rhe, true);
            case BinaryExpression::Operation::SHR:
                return CreateShiftExpr(context, lhe, rhe, false);
            case BinaryExpression::Operation::COMMA:
                return lhe;
                // LCOV_EXCL_START
            default:
                return context->CreateSymbolExpr(symbolId);
                // LCOV_EXCL_STOP
        }
    }

    void CastOperandsToAppropriateType(ExecutionContext& context, BinaryExpression::Operation op, z3::expr& lhe,
                                       z3::expr& rhe)
    {
        if (op == BinaryExpression::Operation::L_AND || op == BinaryExpression::Operation::L_OR) {
            context->CastToBool(lhe);
            context->CastToBool(rhe);
        } else if (!(op == BinaryExpression::Operation::ADD && lhe.is_seq() && rhe.is_seq())) {
            context->CastToBV(lhe);
            context->CastToBV(rhe);
        }
    }

    void CopyUntrustedSource(ExecutionContext& context, const z3::expr& lhe, const z3::expr& rhe)
    {
        auto lheSymbolId = SymbolId::CreateSymbolIdFromExpr(lhe);
        if (lheSymbolId) {
            auto dstExpr = context->CreateSymbolExpr(SymbolId(*lheSymbolId, VirtualOffset()));
            context->CopySubOffsetUntrustedSource(dstExpr, rhe);
        }
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto op = context.GetEnum<BinaryExpression::Operation>();
        auto opFlags = context.Get<OpFlags>();

        SymbolId rheSymbolId(symbolId.GetStackPos());
        z3::expr rhe = context.Execute(&rheSymbolId);

        SymbolId lheSymbolId(symbolId.GetStackPos());
        z3::expr lhe = context.Execute(&lheSymbolId);

        CastOperandsToAppropriateType(context, op, lhe, rhe);

        if (opFlags.isAddrOperation) {
            rhe = rhe * context->CreateIntegerExpr(context.Get<uint32_t>());
        }
        if (opFlags.isConstant) {
            return context->CreateIntegerExpr(context.Get<int64_t>());
        }
        if (!IsAssignOperation(op)) {
            if (opFlags.isUnsignedOperation) {
                context->AddUnsignedCondition(lhe);
            }
            return ExecuteOp(context, op, lhe, rhe, symbolId);
        }
        symbolId = lheSymbolId;
        rhe = GetAssigningRhe(context, op, lhe, rhe, symbolId);
        if (!symbolId.GetOffset().IsSourceObject()) {
            CopyUntrustedSource(context, lhe, rhe);
        }
        if (opFlags.isStaticAssigment && !rhe.is_numeral()) {
            context->AddNotNullCondition(rhe);
        }
        return context->SetSymbol(symbolId, rhe, opFlags.isPointerAssigment ? &rheSymbolId : nullptr);
    }
};
}  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_BINARYINSTRUCTION_H_
