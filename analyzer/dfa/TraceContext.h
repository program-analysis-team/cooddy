/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_DFA_TRACECONTEXT_H_
#define COODDY_ANALYZER_SOURCE_DFA_TRACECONTEXT_H_

#include "CheckPathContext.h"

class TraceContext {
public:
    TraceContext(CheckPathContext& context, PathInfo& pathInfo) : myContext(context), myPathInfo(pathInfo) {}
    ~TraceContext() = default;

    bool Report(DataFlowAnalyzer& analyzer);

private:
    struct CallNode {
        ExecId execId = -1;
        const FunctionContext* funcCtx;
        std::map<ExecId, TraceNode> events;
    };

    using AddNodeCallback = std::function<void(CallNode& node)>;
    void AddEvent(ExecId execId, TraceNode&& node, const AddNodeCallback& callback);

    void BuildTrace(CallNode& callNode, int parentId);
    void AddPath(CheckPathContext::PathPtr& path);
    void AddReturns();
    void AddBranches();
    void AddUntrustedSourceEvent(const UntrustedSource& source);
    void AddUntrustedSourcePropagations(const UntrustedSource& source);
    void AddAdditionalUntrustedEvents(const UntrustedSource& source, bool& sourceAdded);
    bool AddDefaultUntrustedSource();
    bool AddSources();
    void FillDescriptions(Problem& problem, Annotation::Kind sinkKind);
    void FillSourceInfoAndSeverity(Problem& problem);
    void ReportProblem(DataFlowAnalyzer& analyzer);

    Annotation GetUntrustedSourceAnnotation(ExecId callerExecId, SourceId sourceId, uint32_t calleeArgPos);
    Instruction GetInstructionByArgPos(ExecId execId, uint32_t argPos, Instruction defValue = 0);

    std::string GetArgName(const DfaFunctionContext* funcCtx, const Annotation& annotation);
    std::string GetAdditionalDescription();
    std::string MakeSinkEventDescription(const DfaFunctionContext* funcCtx, const Annotation& annotation);
    std::string MakeSourceEventDescription(const DfaFunctionContext* funcCtx, const Annotation& annotation);
    std::string GetTaintProblemReasonCode(FunctionContext* fCtx, uint32_t argPos);

    CheckPathContext& myContext;
    CheckPathContext::PathPtr mySourcePath;
    CheckPathContext::PathPtr mySinkPath;
    PathInfo& myPathInfo;
    TracePath myTrace;
    ExecId mySinkExecId = UNDEF_EXEC_ID;
    std::string mySourceName;
    std::string mySourceDesc;
    std::map<ExecId, CallNode> myCallGraph;
    int myLastEventId = 0;
    const SuspiciousPath* mySuspPath = nullptr;
    const FunctionContext* myLastCallWithSource = nullptr;
    bool myAddAdditionalDesc = true;
};

#endif  // COODDY_ANALYZER_SOURCE_DFA_TRACECONTEXT_H_
