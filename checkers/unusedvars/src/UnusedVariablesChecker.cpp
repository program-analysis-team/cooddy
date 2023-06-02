/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Unused variables checker: There is no any "read" operations with that variable.
//

#include <ast/LambdaExpression.h>
#include <ast/RefExpression.h>
#include <ast/VarDecl.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class UnusedVariablesChecker : public DfaChecker {
public:
    UnusedVariablesChecker()
    {
        myWriteKind = Annotation::GetKind("Write");
        myUsedVarKind = Annotation::RegisterKind("UsedVar", 0);
    }

    void InitState(DfaState& state) override
    {
        auto refExpression = state.GetNodeAs<RefExpression>();

        if (refExpression == nullptr) {
            return;
        }

        auto varDecl = Node::Cast<VarDecl>(refExpression->GetDeclaration());

        if (varDecl != nullptr && !state.HasAnnotation(myWriteKind)) {
            state.GetFuncState().GetState(varDecl).Annotate(myUsedVarKind);
        }
    }

    static bool HasLambdaParent(DfaState& state)
    {
        for (auto parentState = state.GetParentState(); parentState != nullptr;
             parentState = parentState->GetParentState()) {
            auto node = parentState->GetNode();
            if (node->IsKindOf(Node::Kind::LAMBDA_EXPRESSION)) {
                return true;
            }
        }
        return false;
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (HasLambdaParent(state)) {
            return;
        }
        auto varDecl = state.GetNodeAs<VarDecl>();
        if (varDecl != nullptr && !state.HasAnnotation(myUsedVarKind) && !varDecl->InMacro() &&
            varDecl->IsLocalVariableDeclaration() && !varDecl->GetType().IsDeclaration()) {
            holder.RegisterProblem(*this, *state.GetNode());
        }
    }

private:
    Annotation::Kind myWriteKind;
    Annotation::Kind myUsedVarKind;
};

std::unique_ptr<HCXX::Checker> CreateUnusedVariablesChecker()
{
    return std::make_unique<UnusedVariablesChecker>();
}
