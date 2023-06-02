/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxConstructExpression.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/FunctionDecl.h>
#include <ast/MemberExpression.h>
#include <ast/RefExpression.h>
#include <ast/ReturnStatement.h>
#include <dfa/AnnotationUtils.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>
#include <dfa/DfaFunctionContext.h>

using namespace HCXX;

class CallArgsAnnotation : public BuildInDfaChecker {
    Annotation::Kind myAliasKind;

public:
    CallArgsAnnotation()
    {
        myAliasKind = Annotation::GetKind("Alias");
    }

    void InitState(DfaState& state) override
    {
        InitStateByCall(state, state.GetNodeAs<CallExpression>());

        InitStateByCall(state, state.GetParentAs<CallExpression>());
        auto memberExpr = state.GetParentAs<MemberExpression>();
        if (memberExpr != nullptr) {
            InitStateByCall(state, state.GetParentState()->GetParentAs<CallExpression>());
        }
    }

private:
    void InitStateByCall(DfaState& state, const CallExpression* call)
    {
        if (call == nullptr || call->GetFunction() == nullptr) {
            return;
        }
        auto& funcCtx = state.GetFuncState().GetContext();
        Instruction calleeInstruction = funcCtx.GetInstruction(*call);
        uint32_t argPos = GetAnnotationPosFromArgument(call, state.GetNode());
        DfaFunctionContext* calleeContext = funcCtx.GetCalleeContext(calleeInstruction);
        if (calleeContext == nullptr) {
            return;
        }
        size_t curIndex = 0;
        for (auto& annotation : calleeContext->GetAnnotation(argPos)) {
            AnnotateCallArgument(state, annotation, call, argPos, calleeInstruction, curIndex++, 0);
        }
        auto methodCall = Node::Cast<CxxMemberCallExpression>(call);
        if (methodCall != nullptr && methodCall->IsVirtualCall()) {
            auto& overriddenContexts = calleeContext->GetOverriddenContexts();
            for (auto i = 0; i < overriddenContexts.size(); ++i) {
                curIndex = 0;
                for (auto& annotation : overriddenContexts[i]->GetAnnotation(argPos)) {
                    AnnotateCallArgument(state, annotation, call, argPos, calleeInstruction, curIndex++, i + 1);
                }
            }
        }
    }

    /// Annotate a call argument from the respective parameter annotation.
    void AnnotateCallArgument(DfaState& state, const Annotation& annotation, const CallExpression* call,
                              uint32_t argPos, Instruction calleeInstruction, size_t indexInArgAnnotations,
                              size_t indexOfOverridden)
    {
        if (!Annotation::IsPropagatedFromParameter(annotation.GetKind())) {
            return;
        }

        if (annotation.GetKind() == myAliasKind) {
            auto aliasNode = GetArgumentFromAnnotationPos(call, annotation.GetArgPos());
            CopyAliasAnnotations(state, annotation, state.GetFuncState().GetState(aliasNode));
            return;
        }
        auto memOffset = annotation.GetMemoryOffset();
        bool isSourceRange =
            annotation.GetInstruction() == 0 && !Annotation::AnnotateCallInstruction(annotation.GetKind());
        Annotation::ArgInfo argInfo(argPos, indexInArgAnnotations, isSourceRange, indexOfOverridden);
        if (isSourceRange) {
            Annotation argAnnotation(annotation.GetKind(), argInfo, state.GetInstruction(), VirtualOffset(),
                                     annotation.GetUserData<>());
            state.GetSubState(memOffset).Annotate(argAnnotation, *state.GetNode());
        } else {
            Annotation argAnnotation(annotation.GetKind(), argInfo, calleeInstruction, memOffset,
                                     annotation.GetUserData<>());
            state.GetSubState(memOffset).Annotate(argAnnotation, *state.GetNode());
        }
    }

    /**
     * Annotation's UserData for Alias annotation contain memory offset inside the aliased node ( pointed by myArgPos ).
     * In example:
     *
     *      struct Foo
     *      {
     *          int *a; // suppose have offset 3
     *          int *b; // suppose have offset 4
     *      }
     *
     *      void function(Foo* foo, Foo* bar)
     *      {
     *          foo->a = bar->b;
     *      }
     *
     * Function annotations for function foo should contain the following annotations:
     * //myKind, myArgPos, myInstruction, myMemoryOffset, myUserData
     * [1]: [
     * { Write,  1,        x,             3,              n/a               }
     * ...]
     * [2]: [
     * { Alias,  1,        x,             4,              3                 }
     * ...]
     */
    void CopyAliasAnnotations(DfaState& state, const Annotation& aliasAnnotation, DfaState& aliasState)
    {
        if (!aliasAnnotation.IsSourceObject()) {
            VirtualOffset aliasOffset(aliasAnnotation.GetUserData());
            if (&state == &aliasState && aliasOffset == aliasAnnotation.GetMemoryOffset()) {
                return;
            }
            DfaState* destPtr = &state.GetSubState(aliasAnnotation.GetMemoryOffset());
            DfaState* srcPtr = &aliasState.GetSubState(aliasOffset);

            for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
                Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
                if (!Annotation::IsPropagatedByDecl(annotation)) {
                    continue;
                }
                srcPtr->CopyAnnotation(annotation, *destPtr, true);
            }
            return;
        }
        for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
            Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
            if (Annotation::IsPropagatedByPointer(annotation)) {
                aliasState.CopyAnnotation(annotation, state);
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateCallArgsAnnotation()
{
    return std::make_unique<CallArgsAnnotation>();
}
