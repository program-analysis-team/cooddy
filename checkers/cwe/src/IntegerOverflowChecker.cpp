/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/CastExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/TaintedChecker.h>
#include <solver/FunctionBehavior.h>

#include "ast/CxxOperatorCallExpression.h"

using namespace HCXX;

class IntegerOverflowChecker : public TaintedChecker {
    Annotation::Kind myOverflowSinkKind;
    Annotation::Kind myCheckOverflowKind;
    struct IntOverflowCondition {
        DECLARE_ENUM(OverflowType, OVERFLOW, TRUNCATION, LOOP_ITERATOR);
        OverflowType overflowType : 4;
        uint32_t isUnsigned : 1;
        Condition::Operation operation : 8;
        uint32_t sizeInBits : 8;
        uint32_t castSourceSizeInBits : 8;
    };

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
        IntOverflowCondition condition{explicitCheck ? IntOverflowCondition::OverflowType::OVERFLOW
                                                     : IntOverflowCondition::OverflowType::LOOP_ITERATOR,
                                       type.IsUnsigned(), Condition::Operation::INT_OVERFLOW,
                                       uint8_t(type.GetSizeInBits()), 0};
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
        IntOverflowCondition condition{
            explicitCheck ? IntOverflowCondition::OverflowType::OVERFLOW
                          : IntOverflowCondition::OverflowType::LOOP_ITERATOR,
            type.IsUnsigned(), isDecrement ? Condition::Operation::INT_MIN_VAL : Condition::Operation::INT_MAX_VAL,
            uint8_t(type.GetSizeInBits()), 0};
        state.Annotate(myOverflowSinkKind, condition);
    }

    bool SkipBinaryExpr(const BinaryExpression* binary, uint64_t size)
    {
        bool skip = false;
        if (binary != nullptr) {
            skip = !binary->IsArithmeticOp() || binary->GetOperation() == BinaryExpression::Operation::REM;
            binary->Traverse([&](const Node& node) {
                auto b = Node::Cast<BinaryExpression>(&node);
                if ((b != nullptr && !(b->IsArithmeticOp() && b->GetOperation() != BinaryExpression::Operation::REM)) ||
                    node.GetType().GetSizeInBits() <= size) {
                    skip = true;
                }
            });
        }
        return skip;
    }

    void AnnotateCastExpr(DfaState& state)
    {
        auto castNode = state.GetParentAs<CastExpression>();
        if (castNode != nullptr && !castNode->GetType().IsPointer() && castNode->IsImplicit() &&
            castNode->GetCastKind() == CastExpression::Kind::INTEGRAL_CAST) {
            auto castParentState = state.GetParentState();
            auto parent =
                castParentState == nullptr ? nullptr : castParentState->GetParentAs<CxxOperatorCallExpression>();
            auto parentCast = castParentState == nullptr ? nullptr : castParentState->GetParentAs<CastExpression>();
            if (auto typedInnerNode = Node::Cast<TypedNode>(castNode->GetCastedExpression());
                (parentCast == nullptr || parentCast->IsImplicit()) &&
                (parent == nullptr ||
                 parent->GetOperator() != HCXX::CxxOperatorCallExpression::OperatorKind::SUBSCRIPT) &&
                typedInnerNode != nullptr && !typedInnerNode->GetType().IsPointer()) {
                auto castExprType = castNode->GetType();
                auto castExprTypeSize = castExprType.GetSizeInBits();
                auto innerExprTypeSize = typedInnerNode->GetType().GetSizeInBits();
                auto declNode = typedInnerNode->GetDeclaration();
                auto binary = Node::Cast<BinaryExpression>(typedInnerNode->GetInnerNode());
                if (((binary != nullptr && !SkipBinaryExpr(binary, castExprTypeSize)) ||
                     (declNode != nullptr && declNode->GetType().IsTrivial() && !declNode->GetType().IsConstant() &&
                      declNode->GetKind() != Node::Kind::ENUM_CONSTANT_DECL)) &&
                    !typedInnerNode->IsConstExpr() && typedInnerNode->GetType().IsIntegralType() &&
                    castExprType.IsIntegralType() && castExprTypeSize < innerExprTypeSize && castExprTypeSize != 8) {
                    IntOverflowCondition condition{IntOverflowCondition::OverflowType::TRUNCATION,
                                                   castExprType.IsUnsigned(), Condition::Operation::INT_OVERFLOW,
                                                   uint8_t(castExprTypeSize), uint8_t(innerExprTypeSize)};
                    state.Annotate(myOverflowSinkKind, condition);
                }
            }
        }
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
        AnnotateCastExpr(state);
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation(myUntrustedSourceKind) && state.HasAnnotation(myOverflowSinkKind)) {
            auto intCond =
                state.GetAnnotationSources(myOverflowSinkKind).begin()->first.GetUserData<IntOverflowCondition>();
            Condition condition{intCond.operation};
            condition.intTypeInfo = IntTypeInfo{uint16_t(intCond.isUnsigned), uint16_t(intCond.sizeInBits)};
            state.AddSuspiciousPath({*this, myOverflowSinkKind, myUntrustedSourceKind, "", condition,
                                     CheckPathParams::USE_INT_TYPE_LIMITS});
        }
    }
    bool OnReportProblem(ProblemInfo& problemInfo) override
    {
        auto annot = problemInfo.trace.back().annotation;
        auto cond = annot.GetUserData<IntOverflowCondition>();
        if (cond.overflowType == IntOverflowCondition::OverflowType::TRUNCATION) {
            problemInfo.kind = "INT.TRUNC";
            problemInfo.replacements = {std::to_string(cond.castSourceSizeInBits), std::to_string(cond.sizeInBits)};
        }
        return true;
    }
};

std::unique_ptr<HCXX::Checker> CreateIntegerOverflowChecker()
{
    return std::make_unique<IntegerOverflowChecker>();
}
