/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <dfa/TaintedChecker.h>
using namespace HCXX;

class BufferMaxSizeChecker : public TaintedChecker {
    Annotation::Kind myBuffSizeKind;
    Annotation::Kind myMemorySizeKind;
    Annotation::Kind myCharMemSizeKind;
    Annotation::Kind myAllocKind;

    static constexpr uint32_t MEMORY_CHECK_PRIORITY = 1;

    struct MemorySizeInfo {
        uint32_t memPtrArgPos : 16;
        uint32_t operation : 14;
        uint32_t isPointer : 1;
        uint32_t isCharMemorySize : 1;
        Instruction instructionId;
    };

public:
    BufferMaxSizeChecker()
    {
        myBuffSizeKind =
            Annotation::RegisterKind("BuffSize", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
        myMemorySizeKind =
            Annotation::RegisterKind("MemorySize", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
        myCharMemSizeKind =
            Annotation::RegisterKind("CharMemSize", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
        myAllocKind = Annotation::GetKind("AllocSource");
    }
    void InitState(DfaState& state) override
    {
        FillBuffSizeAnnotationsData(state);
        std::vector<std::pair<Annotation, const Node*>> annots;
        bool nodeFound = false;
        const Node* buffNode = nullptr;
        auto isCharSizeKind = state.HasAnnotation(myCharMemSizeKind);
        if (isCharSizeKind) {
            state.CloneAnnotation(myCharMemSizeKind, myMemorySizeKind, KEEP_USER_DATA);
            state.ClearAnnotation(myCharMemSizeKind);
        }
        for (auto [annotation, node] : state.GetAnnotationSources(myMemorySizeKind)) {
            auto& userData = annotation.GetUserData<MemorySizeInfo>();
            if (node == state.GetNode()) {
                nodeFound = true;
                buffNode = GetNodeByArg(userData, state, userData.instructionId == 0);
                if (buffNode != nullptr) {
                    userData.instructionId =
                        state.GetFuncState().GetContext().GetInstruction(*buffNode->GetInnerNode());
                }
                userData.isPointer = node->GetType().IsPointer();
            }
            userData.isCharMemorySize = isCharSizeKind;
            annots.emplace_back(std::make_pair(annotation, node));
        }
        if (isCharSizeKind || nodeFound) {
            state.ClearAnnotation(myMemorySizeKind);
            for (auto& [annot, node] : annots) {
                state.Annotate(annot, *node);
            }
            if (nodeFound && buffNode != nullptr) {
                state.CopyAnnotation(myMemorySizeKind, state.GetFuncState().GetState(buffNode), false);
            }
        }
    }

    void CheckMemoryBuffer(DfaState& state, VirtualOffset offset)
    {
        uint32_t memoryOffset = 0;
        auto varDecl = state.GetNodeAs<VarDecl>();
        if (varDecl == nullptr || !varDecl->IsMemoryBuffer(memoryOffset) ||
            !state.HasAnnotation(myMemorySizeKind, offset)) {
            return;
        }
        SuspiciousPath path{*this, myMemorySizeKind, 0, StrLocales::GetStringLocale("MEMORY_BUFFER")};
        path.checkPathFlags = CheckPathParams::USE_MEMORY_ADDRESSES;
        path.sourceOffset = offset;
        state.AddSuspiciousPath(std::move(path));
    }

    void CheckGlobalVar(DfaState& state, ProblemsHolder& holder) override
    {
        CheckMemoryBuffer(state, VirtualOffset());
    }

    void CheckDeclaration(DfaState& state, ProblemsHolder& holder) override
    {
        TaintedChecker::CheckDeclaration(state, holder);
        CheckMemoryBuffer(state, VirtualOffset(VirtualOffset::Kind::ADDR_OF));
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation(myUntrustedSourceKind)) {
            if (state.HasAnnotation(myMemorySizeKind)) {
                state.AddSuspiciousPath({*this,
                                         myMemorySizeKind,
                                         myUntrustedSourceKind,
                                         StrLocales::GetStringLocale("MEMORY_SIZE"),
                                         {},
                                         CheckPathParams::USE_MEMORY_ADDRESSES});
            }
            if (state.HasAnnotation(myBuffSizeKind)) {
                state.AddSuspiciousPath(
                    {*this, myBuffSizeKind, myUntrustedSourceKind, StrLocales::GetStringLocale("BUFFER_SIZE")});
            }
        }
        if (state.HasAnnotation(myAllocKind) && state.HasAnnotation(myMemorySizeKind)) {
            state.AddSuspiciousPath({*this,
                                     myMemorySizeKind,
                                     myAllocKind,
                                     StrLocales::GetStringLocale("MEMORY_BUFFER"),
                                     {},
                                     CheckPathParams::USE_MEMORY_ADDRESSES});
        }
    }

    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        auto& solverCtx = sinkInfo.context;
        if (sinkInfo.annotation.GetKind() == myBuffSizeKind) {
            auto userDataMemInfo = sinkInfo.annotation.GetUserData<MemorySizeInfo>();
            auto dstExprId = solverCtx.GetExprId(ExprLocation::CALL_ARG, userDataMemInfo.memPtrArgPos - 1);
            if (!sinkInfo.context.IsUntrustedSource(dstExprId)) {
                return false;
            }
            sinkInfo.context.AddCondition(sinkInfo.exprId, sinkInfo.exprId,
                                          static_cast<Condition::Operation>(userDataMemInfo.operation), dstExprId);
            return true;
        }
        auto& memorySizeInfo = sinkInfo.annotation.GetUserData<MemorySizeInfo>();
        auto memoryExprId = solverCtx.GetExprId(ExprLocation::CALL_ARG, memorySizeInfo.memPtrArgPos - 1);
        if (memorySizeInfo.isPointer) {
            solverCtx.ApplyOperation(sinkInfo.exprId, SolverContext::Operation::ASSIGN,
                                     solverCtx.GetExprId(ExprLocation::CURRENT_SIZE));
        }
        if (memorySizeInfo.isCharMemorySize) {
            solverCtx.ApplyOperation(sinkInfo.exprId, SolverContext::Operation::ADD,
                                     solverCtx.GetExprId(ExprLocation::CONSTANT, 1));
        }
        solverCtx.AddCondition(sinkInfo.exprId, memoryExprId, Condition::Operation::ACCESS, sinkInfo.exprId);
        solverCtx.ApplyOperation(memoryExprId, SolverContext::Operation::ADD, sinkInfo.exprId);
        solverCtx.ApplyOperation(sinkInfo.exprId, SolverContext::Operation::ASSIGN, memoryExprId);
        return true;
    }

    bool OnReportProblem(ProblemInfo& problem) override
    {
        auto& sinkAnnotation = problem.trace.back().annotation;
        problem.replacements.resize(2);
        if (sinkAnnotation.GetKind() == myBuffSizeKind) {
            problem.kind = "MEM.BUF.SIZE.OVERFLOW";
            auto& lastTrace = problem.trace.back();
            FillBufferSizeReplacements(sinkAnnotation, lastTrace.funcCtx, lastTrace, problem.replacements);
            return true;
        }
        auto userDataMemInfo = sinkAnnotation.GetUserData<MemorySizeInfo>();
        problem.kind = userDataMemInfo.isCharMemorySize ? "CHAR.BUF.SIZE.OVERFLOW" : "MEM.BUF.SIZE.UNTRUSTED";
        return FillMemorySizeDescription(problem);
    }

private:
    const Node* GetNodeByArg(MemorySizeInfo& memorySizeInfo, DfaState& state, bool extraCondition = true)
    {
        auto callNode = state.GetParentOfType<CallExpression>();
        if (extraCondition && callNode != nullptr && callNode->GetArguments().size() >= memorySizeInfo.memPtrArgPos) {
            return callNode->GetArguments()[memorySizeInfo.memPtrArgPos - 1];
        }
        return nullptr;
    }

    void FillBuffSizeAnnotationsData(DfaState& state)
    {
        auto buffSizeSources = state.GetAnnotationSources(myBuffSizeKind);
        if (buffSizeSources.empty()) {
            return;
        }
        auto annotation = buffSizeSources.begin()->first;
        auto& userDataMemInfo = annotation.GetUserData<MemorySizeInfo>();
        auto conditionData = annotation.GetUserData<Condition>();
        if (userDataMemInfo.operation == 0) {
            userDataMemInfo.memPtrArgPos = conditionData.operand;
            userDataMemInfo.operation = static_cast<uint32_t>(conditionData.operation);
            userDataMemInfo.isPointer = state.GetNode()->GetType().IsPointer();
            if (auto sizeNode = GetNodeByArg(userDataMemInfo, state); sizeNode != nullptr) {
                userDataMemInfo.instructionId = state.GetFuncState().GetContext().GetInstruction(*sizeNode);
            }
            state.ClearAnnotation(myBuffSizeKind);
            state.Annotate(annotation, *buffSizeSources.begin()->second);
        }
    }

    bool FillMemorySizeDescription(ProblemInfo& problem)
    {
        auto& sinkAnnotation = problem.trace.back().annotation;
        auto funcCtx = problem.trace.back().funcCtx;
        if (funcCtx == nullptr || problem.accessInfo == nullptr) {
            return false;  // LCOV_EXCL_LINE
        }
        auto info = sinkAnnotation.GetUserData<MemorySizeInfo>();
        int64_t memSize = problem.accessInfo->allocatedSize - problem.accessInfo->accessOffsetBegin;
        if (memSize <= -UNDEFINED_VALUE) {
            return false;  // LCOV_EXCL_LINE
        }
        std::string memName = funcCtx->GetArgName(info.instructionId, info.memPtrArgPos - 1);

        std::string argSizeName = funcCtx->GetArgName(sinkAnnotation.GetInstruction(), sinkAnnotation.GetArgPos() - 1);
        auto accessSize = problem.accessInfo->accessOffsetEnd - problem.accessInfo->accessOffsetBegin;
        std::string argSizeVal = std::to_string(info.isCharMemorySize ? accessSize - 1 : accessSize);

        problem.description = StrLocales::GetStringLocale(
            "MEM.BUF.SIZE.UNTRUSTED",
            {std::move(memName), std::to_string(memSize), std::move(argSizeName), std::move(argSizeVal)});
        if (info.isCharMemorySize) {
            problem.description += StrLocales::GetStringLocale("CHAR.BUF.SIZE.OVERFLOW_ENDING");
        }
        return true;
    }

    void FillBufferSizeReplacements(Annotation& sinkAnnotation, const DfaFunctionContext* functionCtx,
                                    TraceNode& lastTrace, Replacements& reps)
    {
        reps[0] = StrLocales::GetStringLocale("DESTINATION") + ' ' + StrLocales::GetStringLocale("BUFFER");
        if (functionCtx != nullptr) {
            auto dstSizeName = functionCtx->GetTranslationUnit().GetSourceInRange(
                functionCtx->GetSourceRange(sinkAnnotation.GetUserData<MemorySizeInfo>().instructionId));
            if (!dstSizeName.empty()) {
                reps[0] += " '" + dstSizeName + "'";
            }
        }
        auto paraName = lastTrace.tu->GetSourceInRange(lastTrace.range);
        if (!paraName.empty()) {
            reps[1] = "'" + paraName + "'";
        } else {
            reps[1] = StrLocales::GetStringLocale("THIS");  // LCOV_EXCL_LINE
        }

        reps[1] += StrLocales::GetStringLocale("MEM.BUF.SIZE.OVERFLOW_ENDING");
    }
};

std::unique_ptr<HCXX::Checker> CreateBufferMaxSizeChecker()
{
    return std::make_unique<BufferMaxSizeChecker>();
}
