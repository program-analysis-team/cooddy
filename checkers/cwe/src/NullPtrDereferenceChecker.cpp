/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include <ast/BinaryExpression.h>
#include <ast/ImplicitValueInitExpression.h>
#include <ast/InitListExpression.h>
#include <ast/LiteralExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/TaintedChecker.h>
#include <solver/FunctionBehavior.h>

#include "ast/LoopStatement.h"

using namespace HCXX;

class NullPtrDereferenceChecker : public TaintedChecker {
    Annotation::Kind myInitNullKind;
    Annotation::Kind myDerefKind;
    Annotation::Kind myZeroMemoryKind;
    Annotation::Kind myZeroMemorySourceKind;
    Annotation::Kind myComparedWithNull;

    enum ConditionKind { NOCONDITION, FOR, IF };
    struct ConditionData {
        bool isInMacro;
        bool isInForLoopCond;
    };

public:
    NullPtrDereferenceChecker()
    {
        myInitNullKind = Annotation::RegisterKind("InitNull", Annotation::NOT_PROPAGATED_BY_STATIC_VAR);
        myComparedWithNull = Annotation::RegisterKind(
            "ComparedWithNull", Annotation::PROPAGATED_BY_DECL | Annotation::NOT_PROPAGATED_FROM_PARAMETER |
                                    Annotation::NOT_PROPAGATED_THROUGH_LOOP);
        myDerefKind = Annotation::GetKind("Deref");
        myZeroMemoryKind = Annotation::GetKind("ZeroMemory");
        myZeroMemorySourceKind = Annotation::RegisterKind("ZeroMemorySource", 0);
    }

    void CheckDeclaration(DfaState& state, ProblemsHolder& holder) override
    {
        if (!state.HasAnnotation(myDerefKind)) {
            return;
        }
        if (state.GetNodeAs<ParamVarDecl>() != nullptr && state.GetLeafAnnotation(myUntrustedSourceKind)) {
            state.AddSuspiciousPath({*this,
                                     myDerefKind,
                                     myUntrustedSourceKind,
                                     StrLocales::GetStringLocale("NULL_POINTER"),
                                     {Condition::Operation::EQ},
                                     CheckPathParams::USE_UNTRUSTED_SOURCES});
        }
        if (state.HasAnnotation(myComparedWithNull)) {
            state.AddSuspiciousPath({*this,
                                     myDerefKind,
                                     myComparedWithNull,
                                     StrLocales::GetStringLocale("NULL_POINTER"),
                                     {Condition::Operation::EQ, 0},
                                     CheckPathParams::SINK_CAN_BE_BEFORE_SOURCE});
        }
    }

    void AnnotatePureVirtualFunction(DfaState& state)
    {
        const auto* callExpr = state.GetNodeAs<CallExpression>();
        if (callExpr == nullptr) {
            return;
        }
        auto func = callExpr->GetFunction();
        auto type = callExpr->GetType();
        if (func == nullptr || !func->IsPure() || !type.IsPointer() || type.IsReference() ||
            type.GetPointedDeclaration() == nullptr) {
            return;
        }
        auto callee = state.GetFuncState().GetContext().GetCalleeContext(state.GetInstruction());
        if (callee != nullptr && callee->GetOverriddenContexts().size() <= 1) {
            state.Annotate(myInitNullKind);
        }
    }

    void InitState(DfaState& state) override
    {
        const auto* literal = state.GetNodeAs<LiteralExpression>();
        if (literal != nullptr && literal->IsTreatedAsNullPtr()) {
            state.Annotate(myInitNullKind);
            return;
        }
        if (const auto* initExpr = state.GetNodeAs<ImplicitValueInitExpression>(); initExpr != nullptr) {
            auto type = initExpr->GetType();
            if (type.IsIntegralType() || type.IsPointer()) {
                state.Annotate(myInitNullKind);
            }
        }
        auto conditionKind = GetConditionKind(state);
        if (IsComparingWithNull(state) && conditionKind != ConditionKind::NOCONDITION) {
            auto node = state.GetNode();
            state.Annotate(myComparedWithNull,
                           ConditionData{node != nullptr && node->InMacro(), conditionKind == ConditionKind::FOR});
        }
        AnnotatePureVirtualFunction(state);
    }

    bool IsTmpNode(const Node* node) const
    {
        return node != nullptr &&
               (node->IsKindOf(Node::Kind::PAREN_EXPRESSION) || node->IsKindOf(Node::Kind::CAST_EXPRESSION) ||
                node->IsKindOf(Node::Kind::LITERAL_EXPRESSION));
    }

    // returns true in case &(((Foo*)0)->field)
    bool IsNullInMemberExpression(DfaState* state)
    {
        if (state == nullptr) {
            return false;  // LCOV_EXCL_LINE: HCAT-2779
        }
        if (IsTmpNode(state->GetNode())) {
            return IsNullInMemberExpression(state->GetParentState());
        }
        return state->GetNode()->IsKindOf(Node::Kind::MEMBER_EXPRESSION);
    }

    bool IsNullNode(const Node* node)
    {
        const auto* literal = Node::Cast<LiteralExpression>(node != nullptr ? node->GetInnerNode() : nullptr);
        return literal != nullptr && literal->IsTreatedAsNullPtr();
    }

    bool IsComparingWithNull(DfaState& state)
    {
        auto binExpr = state.GetParentAs<BinaryExpression>();
        if (binExpr != nullptr) {
            auto op = binExpr->GetOperation();
            if (op == BinaryExpression::Operation::EQ || op == BinaryExpression::Operation::NE) {
                return IsNullNode(binExpr->GetLeftOperand()) || IsNullNode(binExpr->GetRightOperand());
            }
        }
        auto unaryExpr = state.GetParentAs<UnaryExpression>();
        return unaryExpr != nullptr && unaryExpr->GetOperation() == UnaryExpression::Operation::LNOT;
    }

    ConditionKind GetConditionKind(DfaState& state)
    {
        auto stNode = state.GetFuncState().GetCfgBlock().GetTerminatorStatement();

        if (stNode != nullptr) {
            auto loopStatement = Node::Cast<LoopStatement>(stNode);
            auto isIfStatement = stNode->IsKindOf(Node::Kind::IF_STATEMENT);
            if (state.GetTopState()->GetNode() == state.GetFuncState().GetCfgBlock().GetTerminatorCondition() &&
                (isIfStatement ||
                 loopStatement != nullptr && loopStatement->GetLoopKind() == LoopStatement::LoopKind::FOR)) {
                return isIfStatement ? ConditionKind::IF : ConditionKind::FOR;
            }
        }
        return ConditionKind::NOCONDITION;
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation((myZeroMemorySourceKind)) && !state.GetNode()->GetType().IsArray()) {
            auto derefAnnotations = state.GetDeclNestedAnnotations(myDerefKind, state.GetMemoryOffset());
            if (derefAnnotations.size() > 0) {
                SuspiciousPath path(*this, myDerefKind, myZeroMemorySourceKind);
                path.sinkAnnotations = std::move(derefAnnotations);
                state.AddSuspiciousPath(std::move(path));
            }
        }
        if (!state.HasAnnotation(myDerefKind)) {
            return;
        }
        if (!state.HasAnnotation(myInitNullKind) || IsNullInMemberExpression(&state)) {
            return;
        }
        if (!state.HasSameAnnotationSource(myDerefKind, myInitNullKind, true)) {
            state.AddSuspiciousPath({*this,
                                     myDerefKind,
                                     myInitNullKind,
                                     StrLocales::GetStringLocale("NULL_POINTER"),
                                     {Condition::Operation::EQ}});
        }
    }

    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        if (sinkInfo.path.sourceKind == myZeroMemorySourceKind && sinkInfo.annotation.GetKind() == myDerefKind) {
            auto offset = sinkInfo.annotation.GetMemoryOffset();
            if (offset.ExtractSubOffset().first == VirtualOffset::Kind::INDEX && sinkInfo.sources.size() > 0) {
                auto sinkExprId = sinkInfo.exprId;
                sinkInfo.context.AddCondition(sinkExprId, {Condition::Operation::EQ, 0});
                return sinkInfo.context.MakeFieldCondition(sinkInfo.sources, sinkExprId,
                                                           sinkInfo.annotation.GetMemoryOffset(), false);
            }
        }
        return true;
    }

    void CheckUndefFunction(const DfaFunctionContext& funcCtx) override
    {
        // Set ZeroMemorySource annotation at args were ZeroMemory annotation present
        // ZeroMemorySource annotation does not propagate by decl, and propagates by dereference as memset(&a,...)
        size_t annoSize = funcCtx.GetSizeOfAnnotations();
        for (size_t i = 0; i < annoSize; i++) {
            ParamAnnotation annots;
            for (auto& argAnno : funcCtx.GetAnnotation(i)) {
                if (argAnno.GetKind() == myZeroMemoryKind && argAnno.GetMemoryOffset().IsSourceObject()) {
                    annots.emplace_back(Annotation(myZeroMemorySourceKind, argAnno.GetArgInfo(),
                                                   argAnno.GetInstruction(), VirtualOffset(), argAnno.GetUserData()));
                    annots.emplace_back(Annotation(myZeroMemorySourceKind, argAnno.GetArgInfo(),
                                                   argAnno.GetInstruction(), VirtualOffset(VirtualOffset::Kind::DEREF),
                                                   argAnno.GetUserData()));
                }
            }
            if (!annots.empty()) {
                funcCtx.AddAnnotation(i, annots);
            }
        }
    }

    static std::string GetVarName(const TraceNode& traceNode)
    {
        auto funcCtx = traceNode.funcCtx;
        return funcCtx ? funcCtx->GetVarName(traceNode.annotation.GetInstruction()) : "";
    }

    static void GetComparingWithNullDesc(const char* locName, TraceNode& traceNode)
    {
        traceNode.description = StrLocales::GetStringLocale(locName, {GetVarName(traceNode)});
    }

    bool OnReportProblem(ProblemInfo& problemInfo) override
    {
        problemInfo.replacements.push_back(GetVarName(problemInfo.trace.back()));

        auto kind = problemInfo.trace.front().annotation.GetKind();
        if (kind == myUntrustedSourceKind) {
            problemInfo.kind = "NULL.UNTRUSTED.DEREF";
        } else if (kind == myComparedWithNull) {
            auto anot = problemInfo.trace.front().annotation;
            if (anot.GetUserData<ConditionData>().isInForLoopCond) {
                return false;
            }
            GetComparingWithNullDesc("FORWARD_NULL_DESC", problemInfo.trace.front());
        } else {
            auto anot = problemInfo.trace.back().annotation;
            if (anot.GetKind() == myComparedWithNull) {
                if (anot.GetUserData<ConditionData>().isInMacro) {
                    return false;
                }
                GetComparingWithNullDesc("REVERSE_NULL_DESC", problemInfo.trace.back());
                problemInfo.trace.insert(problemInfo.trace.begin(), problemInfo.trace.back());
                while (problemInfo.trace.back().annotation.GetKind() != myDerefKind) {
                    problemInfo.trace.pop_back();
                }
            }
        }
        return true;
    }
};

std::unique_ptr<HCXX::Checker> CreateNullPtrDereferenceChecker()
{
    return std::make_unique<NullPtrDereferenceChecker>();
}
