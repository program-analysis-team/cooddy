/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include <ast/AsmStatement.h>
#include <ast/BinaryExpression.h>
#include <ast/CompoundStatement.h>
#include <ast/CxxDtorExpression.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxMethodDecl.h>
#include <ast/CxxOperatorCallExpression.h>
#include <ast/FieldDecl.h>
#include <ast/MemberExpression.h>
#include <ast/ReturnStatement.h>
#include <ast/ThisExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>

#include "ast/TemporaryExpression.h"

using namespace HCXX;

class ReadWriteChecker : public BuildInDfaChecker {
    Annotation::Kind myWriteKind;
    Annotation::Kind myReadKind;
    Annotation::Kind myNonConstCallKind;
    Annotation::Kind myPassByRef;
    Annotation::Kind myChangedKind;

public:
    ReadWriteChecker()
    {
        myWriteKind = Annotation::RegisterKind("Write", Annotation::JOINED_BY_AND);
        myReadKind = Annotation::RegisterKind("Read", Annotation::PROPAGATED_BY_DECL);
        myNonConstCallKind = Annotation::RegisterKind("NonConstCall", 0);
        myPassByRef = Annotation::RegisterKind("PassByRef", Annotation::PROPAGATED_BY_DECL);
        myChangedKind =
            Annotation::RegisterKind("Changed", Annotation::PROPAGATED_BY_POINTER | Annotation::UNTRUSTED_SINK);
    }

    void InitState(DfaState& state) override
    {
        if (IsIndirectCallArg(state)) {
            state.GetSubState(VirtualOffset(VirtualOffset::Kind::DEREF)).Annotate(myWriteKind);
        }

        AnnotateNonConstArgsFromUndefFunc(state);
        AnnotateObjectFromNonConstUndefMethod(state);
        AnnotatePassByRef(state);

        const Node* rhePart = FindRHE(state);
        if (rhePart != nullptr || IsInAsmStatement(state)) {
            state.Annotate(myWriteKind);
        }
        auto typedRhe = Node::Cast<TypedNode>(rhePart);
        if (typedRhe != nullptr) {
            CopyRheAliasAnnotations(state, *typedRhe);
        }

        auto typedNode = state.GetNodeAs<TypedNode>();
        if (typedNode != nullptr && typedNode->GetDeclaration() != nullptr) {
            if (!IsDestructorCall(state) && !IsPassedAsRefArg(state) && !IsAddressGathering(state) &&
                !IsCastToVoid(state) && !IsInAsmStatement(state) && state.GetNodeAs<CastExpression>() == nullptr &&
                state.GetNodeAs<CallExpression>() == nullptr && !state.GetNode()->GetType().IsConstantArray()) {
                state.Annotate(myReadKind, 0, true);
            }
            auto& declState = state.GetFuncState().GetState(typedNode->GetDeclaration());
            if ((declState.HasAnnotation(myWriteKind) || declState.HasAnnotation(myNonConstCallKind)) &&
                !declState.HasAnnotation(myChangedKind)) {
                declState.Annotate(myChangedKind);
            }
        }
    }

    bool IsDestructorCall(DfaState& state)
    {
        if (state.GetNodeAs<MemberExpression>() != nullptr && state.GetParentAs<CxxDtorExpression>() != nullptr) {
            return true;
        }
        if (state.GetNodeAs<RefExpression>() != nullptr && state.GetParentAs<MemberExpression>() != nullptr &&
            state.GetParentOfType<CxxDtorExpression>() != nullptr) {
            return true;
        }
        return false;
    }

    /// Set annotation ChangedObject on object if called method is undefined and non-const
    void AnnotateObjectFromNonConstUndefMethod(DfaState& state)
    {
        auto methodCall = state.GetParentAs<CxxMemberCallExpression>();
        auto methodDecl = methodCall != nullptr ? Node::Cast<CxxMethodDecl>(methodCall->GetFunction()) : nullptr;
        if (methodDecl == nullptr || state.GetFuncState().GetContext().IsCalleeDefined(methodCall) ||
            methodCall->GetObject() == nullptr) {
            return;
        }

        if (methodDecl->IsConst() || methodDecl->IsVirtual()) {
            return;
        }

        auto typeNode = Node::Cast<TypedNode>(methodCall->GetObject());
        if (typeNode == nullptr || typeNode->GetDeclaration() == nullptr) {
            return;  // LCOV_EXCL_LINE
        }
        auto& objState = state.GetFuncState().GetState(typeNode);

        auto offset = VirtualOffset(VirtualOffset::Kind::REF);
        if (typeNode->GetType().IsPointer() && !typeNode->GetType().IsReference()) {
            offset = VirtualOffset(VirtualOffset::Kind::DEREF);
        }
        auto& subState = objState.GetSubState(offset);
        if (!subState.HasAnnotation(myNonConstCallKind)) {
            subState.Annotate(myNonConstCallKind);
        }
    }

    void CopyRheAliasAnnotations(DfaState& state, const TypedNode& rhe)
    {
        auto subOffset = rhe.GetOffsetInDeclaration();
        VirtualOffset rheOffset = subOffset ? *subOffset : VirtualOffset();

        auto& rheDeclState = state.GetFuncState().GetState(rhe.GetDeclaration());

        for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
            if (!Annotation::IsPropagatedByAlias(i)) {
                continue;
            }
            for (auto& it : rheDeclState.GetAnnotationSources(i)) {
                if (it.first.GetMemoryOffset() == rheOffset) {
                    rheDeclState.CopyAnnotation(i, state);
                }
            }
        }
    }

    bool IsCastToVoid(DfaState& ref)
    {
        // don't annotate references with cast for suppressing unused warning
        // int x;
        // (void)x;
        auto cast = ref.GetNodeAs<CastExpression>();
        if (cast == nullptr) {
            cast = Node::Cast<CastExpression>(GetNonParenExprParent(ref));
        }
        return cast != nullptr && cast->IsExplicit() && cast->GetCastKind() == CastExpression::Kind::TO_VOID;
    }

    static const Node* GetNonParenExprParent(DfaState& state)
    {
        for (auto parent = state.GetParentState(); parent != nullptr; parent = parent->GetParentState()) {
            if (parent->GetNode()->IsKindOf(Node::Kind::PAREN_EXPRESSION)) {
                continue;
            }
            return parent->GetNode();
        }
        return nullptr;
    }

    void MoveAnnotations(DfaState& state, DfaState* rheState, bool moveOnlyByDeclAnnotations,
                         bool moveOnlyIfStateHasAnnotation, bool moveOnlyDeclAnnotations)
    {
        for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
            auto annotation = static_cast<Annotation::Kind>(i);
            if (moveOnlyByDeclAnnotations && !Annotation::IsPropagatedByDecl(annotation)) {
                continue;
            }
            if (moveOnlyIfStateHasAnnotation && !state.HasAnnotation(annotation)) {
                continue;
            }
            if (!moveOnlyDeclAnnotations && rheState != nullptr) {
                state.MoveAnnotation(annotation, *rheState, true);
            } else {
                state.MoveDeclAnnotation(annotation, rheState, true, false);
            }
        }
    }

    void ProcessPointerAnnotations(DfaState& state, DfaState* rheState)
    {
        // move annotation from object if it dereference of pointer
        auto unary = state.GetNodeAs<UnaryExpression>();
        if (unary != nullptr && unary->GetOperation() == UnaryExpression::Operation::DEREF) {
            auto& unaryOperandState = state.GetFuncState().GetState(unary->GetOperand());
            MoveAnnotations(unaryOperandState, rheState, true, false, true);
        }

        if (rheState == nullptr || !state.GetNode()->GetType().IsPointer()) {
            return;
        }

        if (state.IsDeclHasNestedAnnotation(myWriteKind)) {
            auto& subState = rheState->GetSubState(VirtualOffset(VirtualOffset::Kind::DEREF));
            MoveAnnotations(subState, nullptr, false, true, true);
            subState.Annotate(myWriteKind);
        }
        state.CopySubOffsetAnnotations(myWriteKind, rheState);
    }
    void ExitState(DfaState& state) override
    {
        const Node* rhe = FindRHE(state);
        DfaState* rheState = nullptr;
        if (rhe != nullptr) {
            rheState = &state.GetFuncState().GetState(rhe);
        }
        if (!state.HasAnnotation(myWriteKind)) {
            return;
        }

        ProcessPointerAnnotations(state, rheState);

        MoveAnnotations(state, rheState, true, true, false);
    }

    void AnnotateNonConstArgsFromUndefFunc(DfaState& state)
    {
        auto callExpr = state.GetParentAs<CallExpression>();
        auto function = callExpr != nullptr ? callExpr->GetFunction() : nullptr;
        if (function == nullptr || state.GetFuncState().GetContext().IsCalleeDefined(callExpr)) {
            return;
        }
        // TODO: Possibly only && is required ignoring. Future research required. HCAT-651
        if (function->GetFuncKind() == FunctionDecl::FunctionKind::CONSTRUCTOR) {
            return;
        }
        if (function->IsIntinsic()) {
            return;
        }
        auto argPos = callExpr->FindArgument(*state.GetNode());
        if (!argPos || *argPos == 0 && function->IsOperator()) {
            return;
        }
        Type argType = callExpr->GetArgumentType(*argPos);
        if (argType.IsPointer() && !argType.IsConstant() && !argType.IsPointedToConstant()) {
            VirtualOffset offset = argType.IsReference() ? VirtualOffset(VirtualOffset::Kind::REF)
                                                         : VirtualOffset(VirtualOffset::Kind::DEREF);
            state.GetSubState(offset).Annotate(myWriteKind);
        }
    }

    bool IsAddressGathering(DfaState& ref)
    {
        auto parent = GetNonParenExprParent(ref);
        if (parent == nullptr) {
            return false;
        }
        auto unary = Node::Cast<UnaryExpression>(parent);
        return unary != nullptr && unary->GetOperation() == UnaryExpression::Operation::ADDR_OF;
    }

    void AnnotatedAssignedNodeAsPassByRef(DfaState& state)
    {
        auto binExpr = state.GetParentAs<BinaryExpression>();
        if (binExpr == nullptr || !binExpr->IsAssignOp() || binExpr->GetRightOperand() != state.GetNode()) {
            return;
        }
        auto typedLhs = Node::Cast<TypedNode>(binExpr->GetLeftOperand());
        if (typedLhs == nullptr) {
            return;
        }
        if (typedLhs->GetType().IsReference() || typedLhs->GetDeclaration() == nullptr) {
            state.Annotate(myPassByRef);

        } else if (auto memExprLhs = Node::Cast<MemberExpression>(typedLhs->GetInnerNode()); memExprLhs != nullptr) {
            auto base = Node::Cast<TypedNode>(memExprLhs->GetBase());
            auto baseDecl = base != nullptr ? Node::Cast<VarDecl>(base->GetDeclaration()) : nullptr;
            if (Node::Cast<ThisExpression>(base) == nullptr &&
                (memExprLhs->IsArrow() || baseDecl == nullptr || !baseDecl->IsLocalVariableDeclaration())) {
                // annotate assignment value to all member expressions, except 'this' and local variable members
                state.Annotate(myPassByRef);
            }
        }
        auto offset = state.GetMemoryOffset();
        if (state.GetDeclState() != nullptr && offset.ExtractSubOffset().first == VirtualOffset::Kind::INDEX &&
            offset.ExtractSubOffset().first == VirtualOffset::Kind::ADDR_OF) {
            state.GetDeclState()->Annotate(myPassByRef);
        }
    }

    void AnnotatePassByRef(DfaState& state)
    {
        AnnotatedAssignedNodeAsPassByRef(state);

        if (auto typedNode = state.GetNodeAs<TypedNode>(); typedNode != nullptr) {
            if (auto decl = typedNode->GetDeclaration(); decl != nullptr && decl->GetType().IsReference()) {
                state.GetFuncState().GetState(decl).Annotate(myPassByRef);
                state.Annotate(myPassByRef);
            }
        }

        if (IsPassedAsRefArg(state)) {
            state.Annotate(myPassByRef);
        }
    }

    bool IsPassedAsRefArg(DfaState& state)
    {
        // passed arg reference arg
        auto callExpr = state.GetParentAs<CallExpression>();
        auto function = callExpr != nullptr ? callExpr->GetFunction() : nullptr;

        if (function == nullptr) {
            return false;
        }

        auto argPos = callExpr->FindArgument(*state.GetNode());
        if (!argPos || *argPos == 0 && function->IsOperator()) {
            return false;
        }
        Type argType = callExpr->GetArgumentType(*argPos);
        return argType.IsReference();
    }

    bool IsIndirectCallArg(DfaState& state)
    {
        auto callExpr = state.GetParentAs<CallExpression>();
        if (callExpr == nullptr || callExpr->GetFunction() != nullptr) {
            return false;
        }
        return state.GetNode()->GetType().IsPointer();
    }

    bool IsInAsmStatement(DfaState& state)
    {
        return state.GetParentOfType<AsmStatement>() != nullptr;
    }

    const Node* FindRHE(DfaState& state)
    {
        auto binExpr = state.GetParentAs<BinaryExpression>();
        if (binExpr != nullptr && binExpr->GetLeftOperand() == state.GetNode() && binExpr->IsAssignOp()) {
            return binExpr->GetRightOperand();
        }

        auto assignOp = state.GetParentAs<CxxOperatorCallExpression>();
        if (assignOp != nullptr && assignOp->GetOperator() == CxxOperatorCallExpression::OperatorKind::EQUAL &&
            assignOp->GetArguments().size() >= 2 && state.GetNode() == assignOp->GetArguments()[0]) {
            return assignOp->GetArguments()[1];
        }

        auto typedNode = state.GetNodeAs<TypedNode>();
        if (typedNode != nullptr && typedNode->GetInitializer() != nullptr) {
            return typedNode->GetInitializer();
        }
        return nullptr;
    }
};

std::unique_ptr<HCXX::Checker> CreateReadWriteChecker()
{
    return std::make_unique<ReadWriteChecker>();
}
