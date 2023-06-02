/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/CastExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/TaintedChecker.h>

using namespace HCXX;

class IntegerOverflowChecker : public TaintedChecker {
    Annotation::Kind myOverflowSinkKind;
    Annotation::Kind myCheckOverflowKind;

public:
    IntegerOverflowChecker()
    {
        myOverflowSinkKind =
            Annotation::RegisterKind("OverflowSink", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
        myCheckOverflowKind = Annotation::RegisterKind("CheckOverflow", Annotation::PROPAGATED_BY_DECL);
    }

    bool ShouldAnnotateOperand(const Type& opType, bool explicitCheck, bool isDecrement)
    {
        if (!opType.IsTrivial()) {
            return false;
        }
        if (explicitCheck) {
            return true;
        }
        return isDecrement && opType.IsUnsigned() || !isDecrement && opType.GetSizeInBits() < 32;
    }

    void AnnotateBinaryExpr(DfaState& state, bool explicitCheck)
    {
        auto binExpr = state.GetNodeAs<BinaryExpression>();
        if (binExpr == nullptr || (!binExpr->IsArithmeticOp() && !binExpr->IsArithmeticAssignOp())) {
            return;
        }
        auto parentCast = state.GetParentAs<CastExpression>();
        Type type = parentCast != nullptr ? parentCast->GetType() : binExpr->GetType();
        if (!ShouldAnnotateOperand(type, explicitCheck,
                                   binExpr->GetOperation() == BinaryExpression::Operation::SUB_ASSIGN)) {
            return;
        }
        Condition condition{Condition::Operation::INT_OVERFLOW};
        condition.intTypeInfo = IntTypeInfo{type.IsUnsigned(), uint8_t(type.GetSizeInBits())};
        state.Annotate(myOverflowSinkKind, condition);
    }

    void AnnotateUnaryExpr(DfaState& state, bool explicitCheck)
    {
        auto unExpr = state.GetParentAs<UnaryExpression>();
        if (unExpr == nullptr || unExpr->GetOperand() == nullptr || (!unExpr->IsPostfix() && !unExpr->IsPrefix())) {
            return;
        }
        auto op = unExpr->GetOperation();
        Type type = unExpr->GetOperand()->GetType();
        bool isDecrement = op == UnaryExpression::Operation::POST_DEC || op == UnaryExpression::Operation::PRE_DEC;
        if (!ShouldAnnotateOperand(type, explicitCheck, isDecrement)) {
            return;
        }
        Condition condition;
        condition.intTypeInfo = IntTypeInfo{type.IsUnsigned(), uint8_t(type.GetSizeInBits())};
        condition.operation = isDecrement ? Condition::Operation::INT_MIN_VAL : Condition::Operation::INT_MAX_VAL;
        state.Annotate(myOverflowSinkKind, condition);
    }

    void InitState(DfaState& state) override
    {
        if (state.HasAnnotation(myOverflowSinkKind) || state.GetNode()->InMacro()) {
            return;
        }
        bool explicitCheck = state.HasAnnotation(myCheckOverflowKind);
        if (explicitCheck || state.GetFuncState().GetCfgBlock().HasLoopIterators()) {
            AnnotateBinaryExpr(state, explicitCheck);
            AnnotateUnaryExpr(state, explicitCheck);
        }
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation(myUntrustedSourceKind) && state.HasAnnotation(myOverflowSinkKind)) {
            auto sinkCondition = state.GetAnnotationSources(myOverflowSinkKind).begin()->first.GetUserData<Condition>();
            state.AddSuspiciousPath({*this, myOverflowSinkKind, myUntrustedSourceKind, "", sinkCondition,
                                     CheckPathParams::USE_INT_TYPE_LIMITS});
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateIntegerOverflowChecker()
{
    return std::make_unique<IntegerOverflowChecker>();
}
