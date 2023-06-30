/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
// Implementation of rules:
//  MEM.LEAK: Missing Release of Memory after Effective Lifetime
//  TODO: implement CWE-404: Improper Resource Shutdown or Release

#include <ast/CallExpression.h>
#include <ast/CxxConstructExpression.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxNewExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class MemoryAndResourceLeakChecker : public DfaChecker {
    Annotation::Kind myLockResourceKind;
    Annotation::Kind myUnlockResourceKind;
    Annotation::Kind myAllocSourceKind;
    Annotation::Kind myFreeSinkKind;
    Annotation::Kind myPassedToUndefFuncKind;
    Annotation::Kind myPassByRef;
    Annotation::Kind myAliasKind;
    Annotation::Kind myAllocDescriptorKind;
    Annotation::Kind myFreeDescriptorKind;

public:
    MemoryAndResourceLeakChecker()
    {
        myLockResourceKind = Annotation::GetKind("LockResource");
        myUnlockResourceKind = Annotation::GetKind("UnlockResource");
        myPassByRef = Annotation::GetKind("PassByRef");
        myAllocSourceKind = Annotation::GetKind("AllocSource");
        myAllocDescriptorKind = Annotation::GetKind("AllocDescriptor");
        myFreeDescriptorKind = Annotation::GetKind("FreeDescriptor");
        myFreeSinkKind = Annotation::GetKind("FreeSink");
        myAliasKind = Annotation::GetKind("Alias");
        myPassedToUndefFuncKind = Annotation::RegisterKind(
            "PassedToUndefFuncKind", Annotation::PROPAGATED_BY_DECL | Annotation::PROPAGATED_TO_FIELDS);
    }

    void InitState(DfaState& state) override
    {
        auto callExpr = state.GetParentAs<CallExpression>();
        if (callExpr) {
            if (state.GetFuncState().GetContext().IsCalleeDefined(callExpr)) {
                return;
            }
            for (auto [n, s] : state.GetAnnotationSources(myFreeSinkKind)) {
                if (s == state.GetNode()) {
                    return;
                }
            }
            auto argPos = callExpr->FindArgument(*state.GetNode());
            if (!argPos) {
                return;
            }
            Type argType = callExpr->GetArgumentType(*argPos);
            if (argType.IsPointer() && !argType.IsConstant() && !argType.IsPointedToConstant()) {
                state.Annotate(myPassedToUndefFuncKind);
            }
            return;
        }
    }
    void TryAddSuspiciousPath(DfaState& state, Annotation::Kind sink, Annotation::Kind source, const char* sourceName,
                              uint32_t flags = 0)
    {
        if (state.HasAnnotation(source)) {
            flags |= CheckPathParams::Flags::CHECK_UNREACHABLE_SINK | CheckPathParams::USE_POINTER_ARITHMETIC;
            state.AddSuspiciousPath({*this, sink, source, StrLocales::GetStringLocale(sourceName), {}, flags});
        }
    }
    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (SkipState(state)) {
            return;
        }
        TryAddSuspiciousPath(state, myUnlockResourceKind, myLockResourceKind, "RESOURCE_SOURCE");
        if (SkipStateAlloc(state)) {
            return;
        }
        TryAddSuspiciousPath(state, myFreeDescriptorKind, myAllocDescriptorKind, "DESCRIPTOR_SOURCE",
                             CheckPathParams::Flags::USE_EXIT_SYMBOLS_AS_SINKS);
        TryAddSuspiciousPath(state, myFreeSinkKind, myAllocSourceKind, "ALLOC_SOURCE",
                             CheckPathParams::Flags::USE_EXIT_SYMBOLS_AS_SINKS);
    }
    bool OnSourceExecuted(const SourceExecInfo& sourceInfo) override
    {
        auto& solverCtx = sourceInfo.context;
        if (sourceInfo.path.sourceKind == myLockResourceKind && solverCtx.IsCallArgument(sourceInfo.exprId)) {
            // ignore lock sources if lock object is passed as a parameter of the function
            return false;
        }
        solverCtx.AddCondition(sourceInfo.exprId, {Condition::Operation::GT, 0});
        return true;
    }

    bool ShouldProcessAnnotation(const Annotation& annotation, bool defResult, bool sourceFound) override
    {
        if (annotation.GetKind() != myFreeSinkKind && annotation.GetKind() != myUnlockResourceKind &&
            annotation.GetKind() != myFreeDescriptorKind) {
            return defResult;
        }
        return sourceFound;
    }

    bool OnReportProblem(ProblemInfo& problemInfo) override
    {
        if (problemInfo.trace.front().annotation.GetKind() == myAllocDescriptorKind) {
            problemInfo.kind = "DESCRIPTOR.LEAK";
            problemInfo.replacements.push_back(StrLocales::GetStringLocale("DESCRIPTOR_LEAK"));
            problemInfo.trace.front().description = StrLocales::GetStringLocale("DESCRIPTOR_SOURCE");
        } else if (problemInfo.trace.front().annotation.GetKind() == myLockResourceKind) {
            problemInfo.kind = "RESOURCE.LEAK";
            problemInfo.replacements.push_back(StrLocales::GetStringLocale("RESOURCE_LEAK"));
            problemInfo.trace.front().description = StrLocales::GetStringLocale("RESOURCE_SOURCE");
        } else {
            problemInfo.kind = "MEM.LEAK";
            problemInfo.replacements.push_back(StrLocales::GetStringLocale("MEM_LEAK"));
        }

        if (problemInfo.trace.back().kind == TraceNode::Kind::ASSUMPTION) {
            problemInfo.trace.back().description = problemInfo.replacements.front();
        }
        return true;
    }

private:
    bool SkipStateAlloc(DfaState& state) const
    {
        for (auto& [anno, node] : state.GetAnnotationSources(myAllocSourceKind)) {
            if (anno.GetUserData() == 0) {
                return true;
            }
        }
        for (auto& [anno, node] : state.GetAnnotationSources(myPassedToUndefFuncKind)) {
            if (!anno.IsSourceRange()) {
                return true;
            }
        }
        return false;
    }
    bool SkipState(DfaState& state) const
    {
        for (auto& [anno, node] : state.GetAnnotationSources(myAliasKind)) {
            auto aliasOffset = anno.GetMemoryOffset();
            auto& aliasState = state.GetFuncState().GetState(node);
            for (auto& [passedToUndef, passedToUndefNode] : aliasState.GetAnnotationSources(myPassedToUndefFuncKind)) {
                if (passedToUndefNode != state.GetNode() && aliasOffset.IsIndex() &&
                    aliasOffset.IsSubOffsetOf(passedToUndef.GetMemoryOffset())) {
                    return true;
                }
            }
        }

        // LCOV_EXCL_START
        if (state.HasAnnotation(myPassByRef) || state.IsDeclHasNestedAnnotation(myPassByRef)) {
            return true;
        }
        for (auto& [anno, node] : state.GetAnnotationSources(myAllocSourceKind)) {
            auto userData = anno.GetUserData<AllocUserData>();
            if (userData.isPassByRef || userData.isStatic) {
                return true;
            }
        }

        // skip allocations into an array member as a parameter (&array[index])
        auto unaryExpr = Node::Cast<UnaryExpression>(state.GetNode()->GetInnerNode());
        if (unaryExpr != nullptr && unaryExpr->GetOperation() == UnaryExpression::Operation::ADDR_OF) {
            auto operand = unaryExpr->GetOperand();
            return operand != nullptr && operand->GetInnerNode()->IsKindOf(Node::Kind::ARRAY_SUBSCRIPT_EXPRESSION);
        }
        // LCOV_EXCL_STOP
        // skip replacement new expressions
        if (auto newExpr = state.GetNodeAs<CxxNewExpression>(); newExpr != nullptr && newExpr->IsReplacementNew()) {
            return true;
        }

        // FIXME: should be removed after fix HCAT-1738
        // skip annotation on declaration
        if (state.GetNodeAs<VarDecl>() != nullptr || state.GetNodeAs<CxxMemberCallExpression>() != nullptr ||
            state.GetNodeAs<CxxConstructExpression>() != nullptr) {
            return true;
        }
        return false;
    }
};

std::unique_ptr<HCXX::Checker> CreateMemoryAndResourceLeakChecker()
{
    return std::make_unique<MemoryAndResourceLeakChecker>();
}
