/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxMethodDecl.h>
#include <ast/CxxRecordDecl.h>
#include <ast/FunctionDecl.h>
#include <ast/MemberExpression.h>
#include <ast/RefExpression.h>
#include <ast/ReturnStatement.h>
#include <dfa/AnnotationUtils.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>
#include <dfa/DfaFunctionContext.h>

using namespace HCXX;

class FuncDeclAnnotation : public BuildInDfaChecker {
    Annotation::Kind myAliasKind;

public:
    FuncDeclAnnotation()
    {
        myAliasKind = Annotation::RegisterKind("Alias", Annotation::PROPAGATED_BY_DECL);
    }

private:
    void InitState(DfaState& state) override
    {
        auto offset = state.GetMemoryOffset();
        if (state.GetParentAs<ReturnStatement>() != nullptr) {
            Annotation a{myAliasKind, Annotation::ArgInfo(0), 0, VirtualOffset(), offset.GetRawValue()};
            state.Annotate(a, *state.GetNode());
            return;
        }
        auto typedNode = state.GetNodeAs<TypedNode>();
        if (typedNode != nullptr && typedNode->GetDeclaration() != nullptr) {
            Annotation a{myAliasKind, Annotation::ArgInfo(), 0, VirtualOffset(), offset.GetRawValue()};
            state.Annotate(a, *typedNode->GetDeclaration());
        }
    }

    bool IsStaticVarReference(const Node& node)
    {
        auto ref = Node::Cast<RefExpression>(node.GetInnerNode());
        auto var = Node::Cast<VarDecl>(ref != nullptr ? ref->GetDeclaration() : nullptr);
        return var != nullptr && (var->IsStaticLocal() || var->IsStaticDataMember());
    }

    void ExitState(DfaState& state) override
    {
        for (auto& alias : state.GetAnnotationSources(myAliasKind)) {
            for (int i = 0; i < Annotation::GetAnnotationsCount(); ++i) {
                Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
                if (!state.HasAnnotation(annotation) || !Annotation::IsPropagatedByRetValue(annotation)) {
                    continue;
                }
                if (!Annotation::IsPropagatedByStaticVar(annotation) && IsStaticVarReference(*alias.second)) {
                    continue;
                }
                if (alias.first.GetArgPos() == 0) {
                    // store annotations for return value in the function state)
                    auto function = state.GetFuncState().GetContext().GetFunction();
                    auto& functionState = state.GetFuncState().GetState(function);
                    for (auto& it : state.GetAnnotationSources(annotation)) {
                        functionState.Annotate(Annotation(it.first, VirtualOffset()), *it.second);
                    }
                }

                if ((!Annotation::IsPropagatedByPointer(annotation) || Annotation::IsPropagatedByDecl(annotation)) &&
                    alias.second->IsKindOf(HCXX::Node::Kind::PARAM_VAR_DECL)) {
                    // store annotations propagated by pointer in the alias declaration states
                    auto& aliasState = state.GetFuncState().GetState(alias.second);
                    for (auto& it : state.GetAnnotationSources(annotation)) {
                        aliasState.Annotate(Annotation(it.first, VirtualOffset(alias.first.GetUserData())), *it.second);
                    }
                }
            }
        }
        if (state.GetNodeAs<ParamVarDecl>() == nullptr) {
            state.ClearAnnotation(myAliasKind, false);
        }
    }

    /// This class aggregates annotations for specific parameter of the function and than store them into the function
    /// summary. The main purpose of this class to support uniqueness and order of annotations
    struct AnnotateContext {
        DfaFunctionState& state;
        std::unordered_map<Annotation, size_t, DfaState::AnnotationHash> annotations;
        uint32_t paramPos = 0;

        /// Add the current parameter's annotation to the map
        void AddAnnotation(Annotation&& annotation)
        {
            annotations.emplace(annotation, annotations.size());
        }

        /// Convert map with annotations to ParamAnnotation and store them to the function summary
        void Store()
        {
            ParamAnnotation annotation(annotations.size());
            for (auto& it : annotations) {
                annotation[it.second] = it.first;
            }
            state.GetContext().AddAnnotation(paramPos++, annotation);
            annotations.clear();
        }
    };

    void CheckFunction(DfaFunctionState& state, ProblemsHolder& holder) override
    {
        auto function = state.GetContext().GetFunction();

        // add annotations for return value
        AnnotateContext context{state};
        AddAnnotations(context, state.GetState(function), Type());
        context.Store();

        // add annotations for 'this'
        if (auto methodDecl = Node::Cast<CxxMethodDecl>(function); methodDecl != nullptr && !methodDecl->IsStatic()) {
            AddThisAnnotations(context, state.GetState(methodDecl->GetThisDecl()));
            context.Store();
        }
        // add annotations for params
        for (auto& param : function->GetParams()) {
            auto& paramState = state.GetState(param);
            for (auto& alias : paramState.GetAnnotationSources(myAliasKind)) {
                AddAlias(context, state.GetContext(), alias);
            }
            AddAnnotations(context, paramState, param->GetType());
            context.Store();
        }
    }

    void AddThisAnnotations(AnnotateContext& context, const DfaState& state)
    {
        for (int i = 0; i < Annotation::GetAnnotationsCount(); ++i) {
            auto annotation = static_cast<Annotation::Kind>(i);
            for (auto& it : state.GetAnnotationSources(annotation)) {
                if (!it.first.IsSourceObject()) {
                    context.AddAnnotation(Annotation(it.first));
                }
            }
        }
    }

    void AddAnnotations(AnnotateContext& context, const DfaState& state, const Type& paramType)
    {
        for (int i = 0; i < Annotation::GetAnnotationsCount(); ++i) {
            Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
            if (!state.HasAnnotation(annotation) || annotation == myAliasKind) {
                continue;
            }
            if (paramType.IsValid() && !paramType.IsPointer() && !Annotation::IsPropagatedByDecl(annotation)) {
                // shouldn't propagate this annotations by value in argument
                continue;
            }
            bool propagateObjectAnnotations =
                !paramType.IsValid() || paramType.IsReference() || Annotation::IsPropagatedByPointer(annotation);

            for (const auto& [anno, _] : state.GetAnnotationSources(annotation)) {
                if ((propagateObjectAnnotations || !anno.IsSourceObject()) && anno.GetInstruction() != 0) {
                    context.AddAnnotation(Annotation(anno));
                }
            }
        }
    }

    void AddAlias(AnnotateContext& context, const DfaFunctionContext& funcContext,
                  const std::pair<const Annotation, const Node*>& alias)
    {
        const Node* aliasNode = alias.second;
        auto aliasOffset = alias.first.GetMemoryOffset();
        if (alias.first.GetArgPos() == 0) {  // return value
            context.AddAnnotation({myAliasKind, Annotation::ArgInfo(0), 0, aliasOffset});
        } else if (Node::Cast<ParamVarDecl>(aliasNode) != nullptr) {
            auto nodeOffset = alias.first.GetUserData();
            auto paramPos = GetParamPosInFunction(*funcContext.GetFunction(), aliasNode);
            if (paramPos && (nodeOffset == 0 || !aliasOffset.IsSourceObject())) {
                context.AddAnnotation({myAliasKind, Annotation::ArgInfo(*paramPos), 0, aliasOffset, nodeOffset});
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateFuncDeclAnnotation()
{
    return std::make_unique<FuncDeclAnnotation>();
}
