/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <dfa/TaintedChecker.h>

using namespace HCXX;

class DivisionByZeroChecker : public TaintedChecker {
    Annotation::Kind myDividerKind;

public:
    DivisionByZeroChecker()
    {
        myDividerKind =
            Annotation::RegisterKind("Divider", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
    }

    bool IsDivider(DfaState& state)
    {
        auto binExpr = state.GetParentAs<BinaryExpression>();
        return (binExpr != nullptr && binExpr->GetRightOperand() == state.GetNode() &&
                (binExpr->GetOperation() == HCXX::BinaryExpression::Operation::DIV ||
                 binExpr->GetOperation() == HCXX::BinaryExpression::Operation::REM ||
                 binExpr->GetOperation() == HCXX::BinaryExpression::Operation::DIV_ASSIGN ||
                 binExpr->GetOperation() == HCXX::BinaryExpression::Operation::REM_ASSIGN) &&
                !binExpr->GetType().IsFloatingType());
    }

    void InitState(DfaState& state) override
    {
        if (IsDivider(state)) {
            state.Annotate(myDividerKind);
        }
    }

    void CheckConstMemberExpr(DfaState& state, ProblemsHolder& holder)
    {
        if (!IsDivider(state)) {
            return;
        }
        auto node = state.GetNodeAs<TypedNode>();
        if (node != nullptr && node->IsConstExpr() && node->GetSLimitedValue() == 0) {
            holder.RegisterProblem(*this, *state.GetNode());
        }
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (!state.HasAnnotation(myDividerKind)) {
            return;
        }
        const auto* node = state.GetNode();
        if (node != nullptr && state.HasAnnotation(myUntrustedSourceKind)) {
            state.AddSuspiciousPath({*this,
                                     myDividerKind,
                                     myUntrustedSourceKind,
                                     StrLocales::GetStringLocale("POSSIBLE_ZERO"),
                                     {Condition::Operation::EQ, 0LL}});
        }
        CheckConstMemberExpr(state, holder);
    }
};

std::unique_ptr<HCXX::Checker> CreateDivisionByZeroChecker()
{
    return std::make_unique<DivisionByZeroChecker>();
}
