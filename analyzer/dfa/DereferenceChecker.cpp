/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/ArraySubscriptExpression.h>
#include <ast/BinaryExpression.h>
#include <ast/CxxOperatorCallExpression.h>
#include <ast/MemberExpression.h>
#include <ast/ParenExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class DereferenceChecker : public BuildInDfaChecker {
public:
    DereferenceChecker()
    {
        myDerefKind = Annotation::RegisterKind("Deref", Annotation::PROPAGATED_BY_DECL);
    }

    void InitState(DfaState& state)
    {
        if (state.HasAnnotation(myDerefKind)) {
            const auto* memberExpr = state.GetNodeAs<MemberExpression>();
            if (memberExpr != nullptr && memberExpr->GetType().IsArray()) {
                state.MoveDeclAnnotation(myDerefKind, nullptr);
            }
        }
        auto* deref = FindDereference(state);
        if (!deref) {
            return;
        }

        if (IsPartOfCompileTimeExpression(state)) {
            return;
        }
        auto& derefState = state.GetFuncState().GetState(deref);
        derefState.Annotate(myDerefKind, 0, true);
    }

private:
    bool IsPartOfCompileTimeExpression(DfaState& state) const
    {
        for (auto* it = &state; it != nullptr; it = it->GetParentState()) {
            if (const auto* unaryExpression = it->GetNodeAs<UnaryExpression>(); unaryExpression) {
                if (unaryExpression->GetOperation() == HCXX::UnaryExpression::Operation::SIZEOF) {
                    return true;
                }
            }
        }

        return false;
    }

    // annotate pointers with deref annotation in expressions like *(arr + idx)
    void AnnotateBaseExpr(DfaState& state, const Node* node)
    {
        auto inner = node->GetInnerNode();
        if (auto typed = Node::Cast<TypedNode>(inner); typed == nullptr || typed->GetDeclaration() != nullptr) {
            return;
        }
        inner->Traverse([&](const Node& node) {
            if (node.IsDeclaration()) {
                return;
            }
            auto typed = Node::Cast<TypedNode>(&node);
            if (typed != nullptr && typed->GetType().IsPointer()) {
                auto& derefState = state.GetFuncState().GetState(&node);
                derefState.Annotate(myDerefKind, 0, true);
            }
        });
    }

    const Node* FindDereference(DfaState& state)
    {
        const auto* unaryExpression = state.GetNodeAs<UnaryExpression>();
        if (unaryExpression != nullptr && unaryExpression->GetOperation() == HCXX::UnaryExpression::Operation::DEREF) {
            AnnotateBaseExpr(state, unaryExpression->GetOperand());
            return unaryExpression->GetOperand();
        }
        const auto* arraySubscriptExpression = state.GetNodeAs<ArraySubscriptExpression>();
        if (arraySubscriptExpression != nullptr) {
            auto unaryParent = state.GetParentAs<UnaryExpression>();
            if (unaryParent && unaryParent->GetOperation() == UnaryExpression::Operation::ADDR_OF) {
                return nullptr;
            }
            auto base = arraySubscriptExpression->GetBase()->GetInnerNode();
            if (base != nullptr && base->GetType().IsArray()) {
                return nullptr;
            }
            return arraySubscriptExpression->GetBase();
        }
        const auto* memberExpr = state.GetNodeAs<MemberExpression>();
        if (memberExpr != nullptr && memberExpr->IsArrow()) {
            return memberExpr->GetBase();
        }
        const auto* operatorExpr = state.GetNodeAs<CxxOperatorCallExpression>();
        if (operatorExpr != nullptr) {
            auto& args = operatorExpr->GetArguments();
            if (args.size() > 0 && (operatorExpr->GetOperator() == CxxOperatorCallExpression::OperatorKind::STAR ||
                                    operatorExpr->GetOperator() == CxxOperatorCallExpression::OperatorKind::ARROW)) {
                return args[0];
            }
        }
        return nullptr;
    }

    Annotation::Kind myDerefKind;
};

std::unique_ptr<HCXX::Checker> CreateDereferenceChecker()
{
    return std::make_unique<DereferenceChecker>();
}
