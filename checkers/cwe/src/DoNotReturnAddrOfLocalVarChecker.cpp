/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of MEM.STACK.RET(G.VAR.06) Do not return the address of a local variable to outside of its scope
//

#include <ast/CastExpression.h>
#include <ast/MemberExpression.h>
#include <ast/ReturnStatement.h>
#include <ast/UnaryExpression.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class DoNotReturnAddrOfLocalVarChecker : public DfaChecker {
    Annotation::Kind myLocalVarKind;
    Annotation::Kind myWriteKind;

public:
    DoNotReturnAddrOfLocalVarChecker()
    {
        myLocalVarKind = Annotation::RegisterKind("AddrOfLocalVar", 0);
        myWriteKind = Annotation::GetKind("Write");
    }

    void InitState(DfaState& state) override
    {
        if (!IsAddrOfLocalVar(state) || IsDereferenceOfPointer(state)) {
            return;
        }

        auto parent = GetNonTemporaryParent(state);
        if (parent == nullptr ||
            (!parent->IsKindOf(Node::Kind::BINARY_EXPRESSION) && !parent->IsKindOf(Node::Kind::RETURN_STATEMENT))) {
            return;
        }

        state.Annotate(myLocalVarKind);
        state.AddSuspiciousPath({*this,
                                 myWriteKind,
                                 myLocalVarKind,
                                 StrLocales::GetStringLocale("LOCAL_ADDRESS_RETURN"),
                                 {},
                                 CheckPathParams::USE_EXIT_SYMBOLS_AS_SINKS});
    }

private:
    const Node* GetNonTemporaryParent(DfaState& state) const
    {
        for (auto parentState = state.GetParentState(); parentState != nullptr;
             parentState = parentState->GetParentState()) {
            if (parentState->GetNode()->IsKindOf(Node::Kind::PAREN_EXPRESSION) ||
                parentState->GetNode()->IsKindOf(Node::Kind::CAST_EXPRESSION)) {
                continue;
            }
            return parentState->GetNode();
        }
        return nullptr;
    }

    bool IsDereferenceOfPointer(DfaState& state)
    {
        bool isPointer = false;
        std::function<void(const Node&)> func = [&](const Node& node) {
            if (auto cast = Node::Cast<CastExpression>(&node); cast) {
                auto type = cast->GetType();
                isPointer = type.IsPointer() && type.GetPointedType().IsPointer();
            }
            node.TraverseChildren(func);
        };
        state.GetNode()->TraverseChildren(func);
        return isPointer;
    }

    bool IsAddrOfLocalVar(DfaState& state)
    {
        auto unary = state.GetNodeAs<UnaryExpression>();
        if (unary == nullptr || unary->GetOperation() != UnaryExpression::Operation::ADDR_OF) {
            return false;
        }

        if (IsArgOfCallExpr(state)) {
            return false;
        }

        auto decl = unary->GetDeclaration();
        if (decl == nullptr) {
            return false;  // LCOV_EXCL_LINE: HCAT-2779
        }

        auto varDecl = Node::Cast<VarDecl>(decl);
        if (varDecl != nullptr && !varDecl->IsStaticLocal() && varDecl->IsLocalVariableDeclaration() &&
            !varDecl->GetType().IsPointer()) {
            return true;
        }

        return false;
    }

    static bool IsArgOfCallExpr(DfaState& state)
    {
        for (auto currentState = state.GetParentState(); currentState != nullptr;
             currentState = currentState->GetParentState()) {
            auto node = currentState->GetNode();
            if (node->IsKindOf(Node::Kind::CALL_EXPRESSION)) {
                return true;
            } else if (!node->IsKindOf(Node::Kind::PAREN_EXPRESSION) && !node->IsKindOf(Node::Kind::CAST_EXPRESSION)) {
                return false;
            }
        }
        return false;
    }
};

std::unique_ptr<HCXX::Checker> CreateDoNotReturnAddrOfLocalVarChecker()
{
    return std::make_unique<DoNotReturnAddrOfLocalVarChecker>();
}
