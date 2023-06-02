/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/ArraySubscriptExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/TaintedChecker.h>
#include <solver/FunctionBehavior.h>
using namespace HCXX;

class OutOfBoundsChecker : public TaintedChecker {
    Annotation::Kind myAccessKind;
    Annotation::Kind myAllocKind;
    std::string myMemoryAddress;

    static constexpr uint32_t MEMORY_CHECK_PRIORITY = 1;

    struct AccessInfo {
        uint32_t sizeOfArrayEl : 30;
        uint32_t isAccessByIndex : 1;
        uint32_t isUnsigned : 1;
        uint32_t indexInstrOffset : 8;
        uint32_t baseInstrOffset : 24;
    };

public:
    OutOfBoundsChecker()
    {
        myAllocKind = Annotation::GetKind("AllocSource");
        myAccessKind = Annotation::RegisterKind("Access", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
        myMemoryAddress = StrLocales::GetStringLocale("MEMORY_ADDRESS");
    }

    bool IsArraySubscriptionAddrOf(DfaState& state, const TypedNode* idxNode)
    {
        auto parentNode = state.GetParentAs<UnaryExpression>();
        if (idxNode == nullptr || parentNode == nullptr ||
            parentNode->GetOperation() != UnaryExpression::Operation::ADDR_OF) {
            return false;
        }
        return idxNode->IsConstExpr() && !state.GetParentState()->HasAnnotation(myAccessKind);
    }

    void InitState(DfaState& state) override
    {
        auto unaryExpression = state.GetParentAs<UnaryExpression>();
        if (unaryExpression != nullptr && unaryExpression->GetOperation() == HCXX::UnaryExpression::Operation::DEREF) {
            state.Annotate(myAccessKind, AccessInfo{unaryExpression->GetSizeOfType(), 0, 0, 0, 0});
            return;
        }
        auto arrayExpression = state.GetNodeAs<ArraySubscriptExpression>();
        if (arrayExpression != nullptr && arrayExpression->GetIdx() != nullptr &&
            !IsArraySubscriptionAddrOf(state, Node::Cast<TypedNode>(arrayExpression->GetIdx()))) {
            bool isUnsigned = arrayExpression->GetIdx()->GetType().IsUnsigned();
            auto& funcCtx = state.GetFuncState().GetContext();
            auto arrInstr = funcCtx.GetInstruction(*arrayExpression);
            auto idxInstrOffset = funcCtx.GetInstruction(*arrayExpression->GetIdx()) - arrInstr;
            auto baseInstrOffset = funcCtx.GetInstruction(*arrayExpression->GetBase()) - arrInstr;
            state.Annotate(myAccessKind, AccessInfo{arrayExpression->GetSizeOfType(), 1, isUnsigned, idxInstrOffset,
                                                    baseInstrOffset});
            return;
        }
    }

    void CheckGlobalVar(DfaState& state, ProblemsHolder& holder) override
    {
        CheckState(state, holder);
    }

    bool IsMemorySource(DfaState& state)
    {
        if (state.HasAnnotation(myAllocKind) || state.GetNode()->GetType().IsConstantArray()) {
            return true;
        }
        uint32_t memorySize = 0;
        auto typedNode = state.GetNodeAs<TypedNode>();
        return typedNode != nullptr && typedNode->IsMemoryBuffer(memorySize);
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (!state.HasAnnotation(myAccessKind)) {
            return;
        }
        if (IsMemorySource(state)) {
            Annotation::Kind sourceKind = state.HasAnnotation(myAllocKind) ? myAllocKind : 0;
            AddSuspiciousPath(state, {*this, myAccessKind, sourceKind, myMemoryAddress});
        } else if (state.HasAnnotation(myUntrustedSourceKind)) {
            AddSuspiciousPath(
                state, {*this, myAccessKind, myUntrustedSourceKind, StrLocales::GetStringLocale("MEMORY_OFFSET")});
        }
    }

    void AddSuspiciousPath(DfaState& state, SuspiciousPath&& path)
    {
        if (path.sourceKind != myUntrustedSourceKind ||
            state.GetNodeAs<ParamVarDecl>() != nullptr && state.GetNode()->GetType().IsPointer()) {
            path.checkPriority = MEMORY_CHECK_PRIORITY;
        }
        path.checkPathFlags = CheckPathParams::USE_MEMORY_ADDRESSES | CheckPathParams::USE_INT_TYPE_LIMITS;
        state.AddSuspiciousPath(std::move(path));
    }

    std::string GetInstrName(const DfaFunctionContext& func, Instruction instr)
    {
        return func.GetTranslationUnit().GetSourceInRange(func.GetBehavior()->GetSourceRange(instr));
    }

    bool OnReportProblem(ProblemInfo& problem) override
    {
        auto annotation = problem.trace.back().annotation;
        if (annotation.GetKind() != myAccessKind) {
            return true;  // LCOV_EXCL_LINE: HCAT-2779
        }
        auto accessInfo = annotation.GetUserData<AccessInfo>();
        problem.kind = accessInfo.isAccessByIndex == 0 ? "MEM.OFFSET.BOUNDS" : "MEM.BUF.INDEX.BOUNDS";

        auto func = problem.trace.back().funcCtx;
        if (problem.accessInfo == nullptr || !accessInfo.isAccessByIndex || func == nullptr ||
            problem.accessInfo->allocatedSize >= UNDEFINED_VALUE) {
            return true;
        }

        auto accessSize = problem.accessInfo->accessOffsetEnd - problem.accessInfo->accessOffsetBegin;
        auto arrName = GetInstrName(*func, annotation.GetInstruction() + accessInfo.baseInstrOffset);
        auto idxName = GetInstrName(*func, annotation.GetInstruction() + accessInfo.indexInstrOffset);

        if (accessSize != 0 && (problem.accessInfo->allocatedSize % accessSize) == 0 && arrName != idxName) {
            std::string accessType = problem.accessInfo->accessOffsetBegin < 0 ? "Underrunning" : "Overrunning";
            std::string arrSize = std::to_string(problem.accessInfo->allocatedSize / accessSize);
            std::string idxValue = std::to_string(problem.accessInfo->accessOffsetBegin / accessSize);

            problem.description = StrLocales::GetStringLocale(
                "INDEX_BOUNDS", {std::move(accessType), std::move(arrName), std::move(arrSize), std::move(idxName),
                                 std::move(idxValue)});
        }
        return true;
    }

    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        auto& solverCtx = sinkInfo.context;
        Condition condition{Condition::Operation::ACCESS, 0};
        auto accessInfo = sinkInfo.annotation.GetUserData<AccessInfo>();
        condition.operand = accessInfo.sizeOfArrayEl;
        if (accessInfo.isUnsigned) {
            condition.operation = Condition::Operation::ACCESS_UNSINGED;
        }
        if (accessInfo.isAccessByIndex != 0) {
            auto idxExpr = solverCtx.GetExprId(ExprLocation::OPERAND, 1);
            solverCtx.ApplyOperation(idxExpr, SolverContext::Operation::MUL,
                                     solverCtx.GetExprId(ExprLocation::CONSTANT, accessInfo.sizeOfArrayEl));

            auto baseExpr = solverCtx.GetExprId(ExprLocation::OPERAND, 0);
            solverCtx.ApplyOperation(sinkInfo.exprId, SolverContext::Operation::ASSIGN, baseExpr);
            solverCtx.ApplyOperation(sinkInfo.exprId, SolverContext::Operation::ADD, idxExpr);
        }
        return solverCtx.AddCondition(sinkInfo.exprId, condition);
    }
};

std::unique_ptr<HCXX::Checker> CreateOutOfBoundsChecker()
{
    return std::make_unique<OutOfBoundsChecker>();
}
