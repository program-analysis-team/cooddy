/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/ArraySubscriptExpression.h>
#include <ast/BinaryExpression.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxRecordDecl.h>
#include <ast/InitListExpression.h>
#include <ast/IntLiteralExpression.h>
#include <ast/MemberExpression.h>
#include <ast/ReturnStatement.h>
#include <ast/StringLiteralExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/AnnotationUtils.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>
#include <dfa/TaintSettings.h>
#include <solver/FunctionBehavior.h>
#include <utils/Configuration.h>

#include <cassert>

using namespace HCXX;

class UntrustedSourceChecker : public BuildInDfaChecker {
    Annotation::Kind myTaintPropagationKind;
    Annotation::Kind myTaintInputKind;
    Annotation::Kind myTaintOutputKind;
    Annotation::Kind myUntrustedSourceKind;
    Annotation::Kind myAllocKind;
    Annotation::Kind myAliasKind;
    Annotation::Kind myTaintAliasKind;
    const uint32_t myVariadicTaintParams = 10;

    TaintSettings mySettings;

    struct TaintedFunctions : public Context {
        std::vector<const DfaFunctionContext*> functions;
    };
    std::mutex myMutex;

public:
    UntrustedSourceChecker()
    {
        myTaintPropagationKind = Annotation::RegisterKind("TaintPropagation", 0);
        myTaintInputKind = Annotation::RegisterKind("TaintInput", Annotation::NOT_PROPAGATED_FROM_PARAMETER);
        myTaintOutputKind = Annotation::RegisterKind("TaintOutput", Annotation::NOT_PROPAGATED_FROM_PARAMETER);
        myUntrustedSourceKind = Annotation::RegisterKind("UntrustedSource", 0);
        myTaintAliasKind =
            Annotation::RegisterKind("TaintAlias", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK |
                                                       Annotation::NOT_PROPAGATED_FROM_PARAMETER);
        myAllocKind = Annotation::GetKind("AllocSource");
        myAliasKind = Annotation::GetKind("Alias");
    }

private:
    bool Initialize(const Checker::Config& config) override
    {
        if (config == nullptr) {
            // No config found, use default values
            return true;
        }

        return LoadConfiguration(mySettings, config, GetName());
    }

    bool AnnotateConstExpr(DfaState& state)
    {
        auto typedNode = state.GetNodeAs<TypedNode>();
        if (typedNode == nullptr || !typedNode->IsConstExpr()) {
            return false;
        }
        for (auto parent = &state; parent != nullptr; parent = parent->GetParentState()) {
            if (!parent->HasAnnotation(myUntrustedSourceKind) || parent->GetNodeAs<InitListExpression>() != nullptr) {
                continue;
            }
            if (auto anno = parent->GetLeafAnnotation(myUntrustedSourceKind); anno && anno->GetUserData() == 0) {
                return false;
            }
        }
        state.Annotate(myUntrustedSourceKind);
        return true;
    }

    void AnnotateTaintedAlias(DfaState& state)
    {
        auto offset = state.GetMemoryOffset();
        if (state.GetParentAs<ReturnStatement>() != nullptr) {
            Annotation a{myTaintAliasKind, Annotation::ArgInfo(0), 0, offset, offset.GetRawValue()};
            state.Annotate(a, *state.GetNode());
            return;
        }
        auto typedNode = state.GetNodeAs<TypedNode>();
        if (typedNode != nullptr && typedNode->GetDeclaration() != nullptr) {
            Annotation a{myTaintAliasKind, Annotation::ArgInfo(), 0, offset, offset.GetRawValue()};
            state.Annotate(a, *typedNode->GetDeclaration());
        }
    }

    void CopyUntrustedSinkAnnotations(DfaState& source, DfaState& dest, bool copyDeclAnnotations = false)
    {
        for (int i = 0; i < Annotation::GetAnnotationsCount(); ++i) {
            if (Annotation::IsUntrustedSink(i)) {
                source.CopyAnnotation(i, dest, copyDeclAnnotations);
            }
        }
    }

    bool IsPartOfArithmetic(DfaState& state)
    {
        auto binary = state.GetParentAs<BinaryExpression>();
        if (binary != nullptr &&
            (binary->IsArithmeticOp() || binary->IsArithmeticAssignOp() || binary->IsBitwiseOp())) {
            return true;
        }
        auto unary = state.GetParentAs<UnaryExpression>();
        if (unary != nullptr && unary->IsArithmetic()) {
            return true;
        }
        return false;
    }

    void PropagateNestedAnnotations(DfaState& state)
    {
        auto untrustedAnno = state.GetLeafAnnotation(myUntrustedSourceKind);
        if (!untrustedAnno || state.GetDeclState() == nullptr) {
            return;
        }
        auto stateOffset = state.GetMemoryOffset();
        if (stateOffset.IsAddrOf()) {
            stateOffset = stateOffset + VirtualOffset(VirtualOffset::Kind::DEREF);
        }
        for (auto& [annotation, node] : state.GetDeclState()->GetAnnotationSources(myTaintAliasKind)) {
            auto offset = annotation.GetMemoryOffset();
            if (!node->IsKindOf(Node::Kind::PARAM_VAR_DECL) || offset == stateOffset ||
                !offset.IsSubOffsetOf(stateOffset)) {
                continue;
            }
            auto& paramState = state.GetFuncState().GetState(node);
            paramState.Annotate(Annotation(*untrustedAnno, VirtualOffset(annotation.GetUserData())), *state.GetNode());
        }
    }

    void ProcessTaintPropagation(DfaState& state)
    {
        if (state.GetParentAs<ParamVarDecl>() != nullptr) {
            return;  // LCOV_EXCL_LINE
        }
        for (auto& [annotation, node] : state.GetAnnotationSources(myTaintPropagationKind)) {
            if (!annotation.IsSourceRange() || state.GetNode() != node) {
                continue;
            }
            const CallExpression* callExpr = nullptr;
            if (annotation.GetArgPos() == 0) {
                callExpr = state.GetNodeAs<CallExpression>();
            } else if (state.GetParentAs<MemberExpression>() != nullptr) {
                callExpr = state.GetParentState()->GetParentAs<CxxMemberCallExpression>();
            } else {
                callExpr = state.GetParentAs<CallExpression>();
            }
            if (callExpr == nullptr || callExpr->IsKindOf(Node::Kind::CXX_NEW_EXPRESSION) ||
                GetArgumentFromAnnotationPos(callExpr, annotation.GetArgPos()) != state.GetNode()) {
                continue;
            }

            const Node* dstArgNode = GetArgumentFromAnnotationPos(callExpr, annotation.GetUserData());
            if (dstArgNode == nullptr && !callExpr->GetFunction()->IsVariadic()) {
                // LCOV_EXCL_START
                Log(LogLevel::ERROR) << "Function \"" << callExpr->GetFunction()->GetQualifiedName()
                                     << "\" contains a TaintPropagation annotation which references argument "
                                     << (annotation.GetUserData() - GetIndexingOffset(*callExpr) + 1)
                                     << " but the call expression only has " << callExpr->GetArguments().size()
                                     << " agruments.\n";
                // LCOV_EXCL_STOP
            } else {
                DfaState& dstState = state.GetFuncState().GetState(dstArgNode);
                if (state.GetMemoryOffset() == VirtualOffset(VirtualOffset::Kind::ADDR_OF)) {
                    auto& srcState = state.GetSubState(VirtualOffset(VirtualOffset::Kind::DEREF));
                    CopyUntrustedSinkAnnotations(srcState, dstState, true);
                } else {
                    CopyUntrustedSinkAnnotations(state, dstState);
                }
            }
        }
    }

    DfaState& FindCompareOperandState(DfaState& state)
    {
        auto binExpr = Node::Cast<BinaryExpression>(state.GetNode()->GetInnerNode());
        if (binExpr == nullptr || !binExpr->IsArithmeticOp()) {
            return state;
        }
        if (auto node = Node::Cast<TypedNode>(binExpr->GetLeftOperand()); node != nullptr && node->IsConstExpr()) {
            return FindCompareOperandState(state.GetFuncState().GetState(binExpr->GetRightOperand()->GetInnerNode()));
        }
        if (auto node = Node::Cast<TypedNode>(binExpr->GetRightOperand()); node != nullptr && node->IsConstExpr()) {
            return FindCompareOperandState(state.GetFuncState().GetState(binExpr->GetLeftOperand()->GetInnerNode()));
        }
        return state;
    }

    void ProcessCompareOperator(DfaState& state)
    {
        auto binExpr = state.GetParentAs<BinaryExpression>();
        if (binExpr == nullptr || !binExpr->IsCompareOp()) {
            return;
        }
        auto otherNode = binExpr->GetLeftOperand();
        if (otherNode == state.GetNode()) {
            otherNode = binExpr->GetRightOperand();
        }
        if (otherNode->GetType().IsPointer()) {
            return;
        }
        auto copyState = &state;
        if (auto node = Node::Cast<TypedNode>(otherNode); node != nullptr && node->IsConstExpr()) {
            copyState = &FindCompareOperandState(state);
        }
        auto& otherState = state.GetFuncState().GetState(otherNode);
        CopyUntrustedSinkAnnotations(*copyState, otherState);
    }

    void ProcessAddressOfArrayElement(DfaState& state)
    {
        auto unaryExpression = state.GetNodeAs<UnaryExpression>();
        if (unaryExpression == nullptr || unaryExpression->GetOperation() != UnaryExpression::Operation::ADDR_OF) {
            return;
        }
        auto arraySubscription = Node::Cast<ArraySubscriptExpression>(unaryExpression->GetOperand());
        if (arraySubscription == nullptr) {
            return;
        }
        auto& arraySubscriptionState = state.GetFuncState().GetState(arraySubscription);
        CopyUntrustedSinkAnnotations(state, arraySubscriptionState);
    }

    void InitState(DfaState& state) override
    {
        AnnotateConstExpr(state);
        AnnotateTaintedAlias(state);

        auto arrExpr = state.GetParentAs<ArraySubscriptExpression>();
        if (arrExpr != nullptr || IsPartOfArithmetic(state) || state.GetParentAs<MemberExpression>() != nullptr ||
            state.GetParentAs<InitListExpression>() != nullptr) {
            CopyUntrustedSinkAnnotations(*state.GetParentState(), state);
        }
        if (arrExpr != nullptr && arrExpr->GetBase() == state.GetNode()) {
            for (int i = 0; i < Annotation::GetAnnotationsCount(); ++i) {
                if (Annotation::IsUntrustedSink(i) && i != myTaintAliasKind) {
                    state.CopyAnnotation(i, *state.GetParentState());
                }
            }
        }
        ProcessCompareOperator(state);
        ProcessTaintPropagation(state);
        PropagateNestedAnnotations(state);
        ProcessAddressOfArrayElement(state);
    }

    void ExitState(DfaState& state) override
    {
        if (!state.HasAnnotation(myUntrustedSourceKind)) {
            return;
        }
        for (auto& alias : state.GetAnnotationSources(myTaintAliasKind)) {
            if (alias.first.IsRetValueSource()) {
                auto& retState = state.GetFuncState().GetState(state.GetFuncState().GetContext().GetFunction());
                for (auto& it : state.GetAnnotationSources(myUntrustedSourceKind)) {
                    retState.Annotate(Annotation(it.first, alias.first.GetMemoryOffset()), *it.second);
                }
            }
        }
    }

    bool ShouldTreatArgsAsUntrusted(const DfaFunctionContext& funcCtx)
    {
        if ((mySettings.taintFlags & TaintSettings::UNDEFINED) && funcCtx.IsUndefined()) {
            return true;
        }
        if ((mySettings.taintFlags & TaintSettings::DEFINED) && !funcCtx.IsUndefined()) {
            return true;
        }
        if ((mySettings.taintFlags & TaintSettings::PUBLIC_API) && funcCtx.IsPublicApi()) {
            return true;
        }
        if ((mySettings.taintFlags & TaintSettings::ANNOTATED) && funcCtx.IsAnnotatedTainted()) {
            return true;
        }

        return false;
    }

    static SourceId GetFunctionSourceId(const DfaFunctionContext& funcCtx)
    {
        // COODDY_SUPPRESS
        return reinterpret_cast<SourceId>(&funcCtx);
    }

    void InitFunction(DfaFunctionState& state) override
    {
        auto& ctx = state.GetContext();
        auto* decl = ctx.GetFunction();

        if ((mySettings.taintFlags & TaintSettings::ANNOTATED) && ctx.IsAnnotatedTainted()) {
            std::unique_lock<std::mutex> lock(myMutex);
            GetContext<TaintedFunctions>(ctx.GetTranslationUnit()).functions.emplace_back(&ctx);
        }

        InitTaintAnnotations(ctx);
        auto& params = decl->GetParams();
        for (const auto& param : params) {
            auto paramPos = GetParamPosInFunction(*decl, param);
            if (!paramPos) {
                break;  // LCOV_EXCL_LINE: HCAT-2777
            }
            for (auto& anno : state.GetContext().GetAnnotation(*paramPos)) {
                if (anno.GetKind() == myTaintInputKind) {
                    anno.GetUserData<uint64_t>() = 1;
                    auto& paramState = state.GetState(param);
                    Annotation annotation{myUntrustedSourceKind, Annotation::ArgInfo(*paramPos),
                                          paramState.GetInstruction(), paramState.GetMemoryOffset()};
                    ctx.AddAnnotation(*paramPos, annotation);
                    paramState.Annotate(std::move(annotation), *paramState.GetNode());
                    break;
                }
            }
        }
        if (ShouldTreatArgsAsUntrusted(ctx)) {
            for (const auto& param : params) {
                auto paramPos = GetParamPosInFunction(*decl, param);
                if (!paramPos) {
                    break;  // LCOV_EXCL_LINE: HCAT-2777
                }
                state.GetState(param).Annotate(myUntrustedSourceKind);
                ParamAnnotation paramAnnotation = {myTaintInputKind};
                ctx.AddAnnotation(*paramPos, paramAnnotation);
            }
        }
    }

    bool CanAnnotateAsUntrustedSource(const DfaFunctionContext& funcCtx, int annotationIndex) const
    {
        auto& paramAnnotation = funcCtx.GetAnnotation(annotationIndex);
        for (auto& it : paramAnnotation) {
            auto kind = it.GetKind();
            if (kind == myTaintPropagationKind || kind == myAllocKind || kind == myUntrustedSourceKind) {
                return false;
            }
        }
        return !HasAlias(annotationIndex, funcCtx);
    }

    /**
     * @brief Initialize annotations defined in json-configs.
     *
     * For UntrustedSource set SourceId and replicate TaintPropagation for variadic parameters.
     */
    void InitTaintAnnotations(const DfaFunctionContext& funcCtx)
    {
        auto func = funcCtx.GetFunction();
        size_t annoSize = funcCtx.GetSizeOfAnnotations();

        for (size_t i = 0; i < annoSize; i++) {
            ParamAnnotation variadicTaint;
            for (auto& argAnno : funcCtx.GetAnnotation(i)) {
                if (argAnno.GetKind() == myUntrustedSourceKind) {
                    argAnno.GetUserData() = GetFunctionSourceId(funcCtx);
                    continue;
                }
                if (argAnno.GetKind() == myTaintPropagationKind && func->IsVariadic() &&
                    argAnno.GetUserData() == func->GetParams().size() + 1) {
                    variadicTaint.emplace_back(argAnno);
                }
            }
            if (!variadicTaint.empty()) {
                for (size_t j = 1; j < myVariadicTaintParams; j++) {
                    variadicTaint[0].GetUserData()++;
                    funcCtx.AddAnnotation(i, variadicTaint);
                }
            }
        }
    }

    /**
     * @brief Initialize annotations defined in json-configs specifically for undefined functions.
     *
     * Convert parameter-bound TaintOutput annotations into value-bound UntrustedSource annotations.
     */
    void InitTaintAnnotationsForUndefined(const DfaFunctionContext& funcCtx)
    {
        size_t size = funcCtx.GetSizeOfAnnotations();
        for (uint32_t i = 0; i < size; i++) {
            for (auto& annotation : funcCtx.GetAnnotation(i)) {
                if (annotation.GetKind() == myTaintOutputKind) {
                    Annotation untrustedSourceAnnotation(myUntrustedSourceKind, Annotation::ArgInfo{i}, 0,
                                                         annotation.GetMemoryOffset(), annotation.GetUserData());
                    funcCtx.AddAnnotation(i, untrustedSourceAnnotation);
                    break;
                }
            }
        }

        InitTaintAnnotations(funcCtx);
    }

    bool HasAlias(uint32_t annoIdx, const DfaFunctionContext& funcCtx) const
    {
        size_t annoSize = funcCtx.GetSizeOfAnnotations();
        for (size_t i = 1; i < annoSize; i++) {
            for (auto& argAnno : funcCtx.GetAnnotation(i)) {
                if (argAnno.GetKind() == myAliasKind && argAnno.GetArgPos() == annoIdx) {
                    return true;
                }
            }
        }
        return false;
    }

    void CheckDeclaration(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.GetNodeAs<ParamVarDecl>() == nullptr) {
            return;
        }
        auto function = state.GetFuncState().GetContext().GetFunction();
        auto paramPos = GetParamPosInFunction(*function, state.GetNode());
        if (!paramPos) {
            return;  // LCOV_EXCL_LINE: HCAT-2777
        }
        // Annotate function parameters with TaintPropagation annotation using TaintAlias annotation
        for (auto& alias : state.GetAnnotationSources(myTaintAliasKind)) {
            if (alias.first.IsRetValueSource()) {
                Annotation taintAnn(myTaintPropagationKind, Annotation::ArgInfo(0), 0, state.GetMemoryOffset(),
                                    *paramPos);
                state.GetFuncState().GetContext().AddAnnotation(0, ParamAnnotation{taintAnn});
            } else if (alias.second->IsKindOf(Node::Kind::PARAM_VAR_DECL) && state.GetNode() != alias.second) {
                auto aliasParamPos = GetParamPosInFunction(*function, alias.second);
                if (aliasParamPos) {
                    Annotation taintAnn(myTaintPropagationKind, Annotation::ArgInfo(*aliasParamPos), 0,
                                        state.GetMemoryOffset(), *paramPos);
                    state.GetFuncState().GetContext().AddAnnotation(*aliasParamPos, ParamAnnotation{taintAnn});
                }
            }
        }
    }

    void CheckUndefFunction(const DfaFunctionContext& funcCtx) override
    {
        InitTaintAnnotationsForUndefined(funcCtx);

        if (!ShouldTreatArgsAsUntrusted(funcCtx)) {
            return;
        }

        Annotation annotation{myUntrustedSourceKind};
        annotation.GetUserData<SourceId>() = GetFunctionSourceId(funcCtx);

        auto function = funcCtx.GetFunction();
        bool hasThis = Node::Cast<CxxMethodDecl>(function) != nullptr && !function->IsStatic();
        bool isClassMember = hasThis && Node::Cast<CxxCtorDecl>(function) == nullptr;

        auto& params = funcCtx.GetFunction()->GetParams();
        if (ShouldAnnotateUndefRetValueAsUntrusted(funcCtx, hasThis, params)) {
            funcCtx.AddAnnotation(0, ParamAnnotation{annotation});
        }
        if (isClassMember) {
            Annotation taintAnn(myTaintPropagationKind, Annotation::ArgInfo(0), 0, VirtualOffset(), 1);
            funcCtx.AddAnnotation(0, ParamAnnotation{taintAnn});
        }

        for (int i = 0; i < params.size(); ++i) {
            auto& param = params[i];
            auto type = param->GetType();
            auto annoPos = i + 1 + hasThis;
            if (isClassMember && type.IsPointer() && !function->IsConst() && !function->IsOperator()) {
                Annotation taintAnn(myTaintPropagationKind, Annotation::ArgInfo(annoPos), 0, VirtualOffset(), annoPos);
                funcCtx.AddAnnotation(1, ParamAnnotation{taintAnn});
            }
            if (type.IsPointer() && !type.IsPointedToConstant() && !type.IsRvalueReferenceType() &&
                CanAnnotateAsUntrustedSource(funcCtx, annoPos) && !TemplateTypeIsRValue(*function, i)) {
                funcCtx.AddAnnotation(annoPos, ParamAnnotation{annotation});
                if (!type.IsReference()) {
                    funcCtx.AddAnnotation(
                        annoPos, ParamAnnotation{Annotation(annotation, VirtualOffset(VirtualOffset::Kind::DEREF))});
                }
            }
        }
    }

    /// When --taint-macro-summary option is specified, report information via detects on whether issues were found in
    /// annotated functions.
    void PostCheck(std::vector<TranslationUnitPtr>& units, ProblemsHolder& holder) override
    {
        if (!mySettings.taintMacroSummary) {
            return;
        }

        // For each translation unit...
        for (auto& unit : units) {
            // ...and each EXTER_ATTACK-annotated function inside...
            for (auto& func : GetContext<TaintedFunctions>(*unit).functions) {
                // ...create a problem with a kind of either "did find issues" or "did not find issues".
                Problem problem;
                problem.checkerName = "NotAnIssue";
                problem.kind = func->IsTaintProblemFound() && func->IsAnnotatedTainted() ? "IssuesFound" : "NoIssues";
                problem.description = StrLocales::GetStringLocale("FUNCTION_ANNOTATED_BY_MACRO");
                problem.severity = Problem::Severity::ERROR;
                func->SetTaintProblemInfo(problem);
                holder.RegisterProblem(*this, *unit, func->GetBehavior()->GetFunctionRange(), std::move(problem));
            }
        }
    }

    bool ShouldAnnotateUndefRetValueAsUntrusted(const DfaFunctionContext& funcCtx, bool hasThis,
                                                const vector<NodePtr<ParamVarDecl>>& params) const
    {
        return !(!params.empty() && params[0]->GetType().GetPointedDeclaration() != nullptr) &&
               CanAnnotateAsUntrustedSource(funcCtx, 0) && !hasThis && mySettings.taintFlags & TaintSettings::UNDEFINED;
    }

    bool TemplateTypeIsRValue(const FunctionDecl& func, int paramPos)
    {
        if (auto temp = func.GetTemplateInstantiationPattern(); temp != nullptr) {
            auto& params = temp->GetParams();
            return paramPos < params.size() ? params[paramPos]->GetType().IsRvalueReferenceType() : false;
        }
        return false;
    }
};

std::unique_ptr<HCXX::Checker> CreateUntrustedSourceChecker()
{
    return std::make_unique<UntrustedSourceChecker>();
}
