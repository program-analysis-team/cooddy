/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/CxxRecordDecl.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class AllocSourceChecker : public BuildInDfaChecker {
    Annotation::Kind myAllocSourceKind;
    Annotation::Kind myFreeSinkKind;
    Annotation::Kind myFreeSourceKind;
    Annotation::Kind myPassByRef;
    Annotation::Kind myCreateObjectKind;

public:
    AllocSourceChecker()
    {
        Annotation::RegisterKind("ZeroMemory",
                                 Annotation::PROPAGATED_BY_DECL | Annotation::NOT_PROPAGATED_THROUGH_LOOP);
        Annotation::RegisterKind("LockResource", Annotation::PROPAGATED_BY_POINTER);
        Annotation::RegisterKind("UnlockResource", Annotation::PROPAGATED_BY_DECL);
        Annotation::RegisterKind("AllocDescriptor", Annotation::PROPAGATED_BY_POINTER);
        Annotation::RegisterKind("FreeDescriptor", Annotation::PROPAGATED_BY_DECL);
        myAllocSourceKind = Annotation::RegisterKind("AllocSource", 0);
        myFreeSinkKind =
            Annotation::RegisterKind("FreeSink", Annotation::PROPAGATED_BY_DECL | Annotation::PROPAGATED_BY_RETVALUE);
        myFreeSourceKind =
            Annotation::RegisterKind("FreeSource", Annotation::PROPAGATED_BY_POINTER | Annotation::PROPAGATED_BY_ALIAS);
        myPassByRef = Annotation::GetKind("PassByRef");
        myCreateObjectKind = Annotation::RegisterKind("CreateObject", Annotation::NOT_PROPAGATED_FROM_PARAMETER);
    }

    void CheckUndefFunction(const DfaFunctionContext& funcCtx) override
    {
        for (auto& annotation : funcCtx.GetAnnotation(0)) {
            if (annotation.GetKind() == myCreateObjectKind) {
                auto& templateTypes = funcCtx.GetFunction()->GetTemplateTypes();
                if (templateTypes.empty()) {
                    continue;  // LCOV_EXCL_LINE
                }
                auto recordDecl = Node::Cast<CxxRecordDecl>(templateTypes.front().GetDeclaration());
                if (recordDecl != nullptr) {
                    annotation.GetUserData() = recordDecl->GetUniqueId();
                }
            }
        }
    }

    void InitState(DfaState& state) override
    {
        auto binExpr = state.GetParentAs<BinaryExpression>();
        if (binExpr != nullptr && binExpr->IsArithmeticOp() && state.GetNode() == binExpr->GetLeftOperand() &&
            state.GetParentState()->HasAnnotation(myFreeSinkKind)) {
            state.GetParentState()->CopyAnnotation(myFreeSinkKind, state);
        }
        if (state.GetLeafAnnotation(myFreeSinkKind)) {
            state.CloneAnnotation(myFreeSinkKind, myFreeSourceKind);
        }
        // check for static variable
        if (state.HasAnnotation(myAllocSourceKind)) {
            auto leafAnnotation = state.GetLeafAnnotation(myAllocSourceKind);
            if (leafAnnotation != std::nullopt) {
                MarkStateAsStatic(state, leafAnnotation->GetUserData<AllocUserData>());
            }
        }

        if (state.HasAnnotation(myPassByRef) && state.HasAnnotation(myAllocSourceKind)) {
            for (auto& [anno, node] : state.GetAnnotationSources(myAllocSourceKind)) {
                auto userData = anno.GetUserData<AllocUserData>();
                userData.isPassByRef = true;
                state.Annotate(myAllocSourceKind, userData, true);
                return;
            }
        }
    }

    void ExitState(DfaState& state) override
    {
        if (!state.HasAnnotation(myFreeSinkKind)) {
            for (auto [annotation, node] : state.GetAnnotationSources(myFreeSourceKind)) {
                auto& userData = annotation.GetUserData<FreeUserData>();
                userData.isFromAliasState = true;
                state.ClearAnnotation(myFreeSourceKind);
                state.Annotate(annotation, *node);
                break;
            }
        }
        if (state.HasAnnotation(myAllocSourceKind) && state.HasAnnotation(myPassByRef)) {
            state.ClearAnnotation(myAllocSourceKind);
        }
    }

private:
    void MarkStateAsStatic(DfaState& state, AllocUserData userData) const
    {
        auto varDecl = state.GetParentOfType<VarDecl>();
        if (varDecl == nullptr) {
            auto binary = state.GetParentOfType<BinaryExpression>();
            if (binary != nullptr && binary->IsAssignOp()) {
                auto typed = Node::Cast<TypedNode>(binary->GetLeftOperand());
                if (typed != nullptr) {
                    varDecl = Node::Cast<VarDecl>(typed->GetDeclaration());
                }
            }
        }
        if (varDecl != nullptr && (varDecl->IsStaticLocal() || varDecl->IsStaticDataMember())) {
            userData.isStatic = true;
            state.Annotate(myAllocSourceKind, userData, true);
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateAllocSourceChecker()
{
    return std::make_unique<AllocSourceChecker>();
}
