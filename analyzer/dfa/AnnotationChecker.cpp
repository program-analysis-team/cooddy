/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/CastExpression.h>
#include <ast/ConditionalExpression.h>
#include <ast/CxxConstructExpression.h>
#include <ast/ParenExpression.h>
#include <ast/TemporaryExpression.h>
#include <ast/UnaryExpression.h>
#include <ast/VarDecl.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class AnnotationChecker : public BuildInDfaChecker {
private:
    void InitState(DfaState& state)
    {
        if (state.GetParentAs<CastExpression>() != nullptr || IsParentIsTemporaryExprWithoutDecl(state) ||
            IsParentIsParenExprWithoutDecl(state) ||
            // TODO HCAT-574
            state.GetParentAs<CxxConstructExpression>() != nullptr) {
            auto& parentState = *state.GetParentState();
            for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
                parentState.MoveAnnotation(static_cast<Annotation::Kind>(i), state, false);
            }
        }
        auto conditionalExpr = state.GetParentAs<ConditionalExpression>();
        if (conditionalExpr == nullptr || conditionalExpr->GetCondition() == state.GetNode()) {
            return;
        }
        auto& parentState = state.GetFuncState().GetState(conditionalExpr);
        for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
            parentState.CopyAnnotation(static_cast<Annotation::Kind>(i), state);
        }
    }

    bool IsParentIsParenExprWithoutDecl(DfaState& state)
    {
        auto parenExpr = state.GetParentAs<ParenExpression>();
        return parenExpr != nullptr && parenExpr->GetDeclaration() == nullptr;
    }

    bool IsParentIsTemporaryExprWithoutDecl(DfaState& state)
    {
        auto tempExpr = state.GetParentAs<TemporaryExpression>();
        return tempExpr != nullptr && tempExpr->GetDeclaration() == nullptr;
    }
};

std::unique_ptr<HCXX::Checker> CreateAnnotationChecker()
{
    return std::make_unique<AnnotationChecker>();
}
