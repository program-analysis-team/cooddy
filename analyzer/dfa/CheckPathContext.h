/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_DFA_CHECKPATHCONTEXT_H_
#define COODDY_ANALYZER_SOURCE_DFA_CHECKPATHCONTEXT_H_

#include "FunctionContext.h"

class CheckPathContext {
public:
    CheckPathContext(DataFlowAnalyzer& analyzer, const FunctionContext& owner, ProblemsHolder& holder)
        : myAnalyzer(analyzer), myCurFuncCtx(&owner), myHolder(holder){};
    ~CheckPathContext() = default;

    void CheckSuspiciousPaths(std::vector<SuspiciousPath>& suspPaths);

private:
    static constexpr uint32_t MAX_RECURSION_DEPTH = 4;  // execute all functions within this depth
    static constexpr uint32_t MAX_RECURSION_LIMIT = 8;  // don't execute functions deeper than this limit
    static constexpr uint32_t MAX_EXECUTED_INSTRUCTIONS = 300000;
    static constexpr uint64_t MAX_EXECUTION_TIME = 10e9;  // 10 seconds
    static constexpr uint64_t MAX_SMALL_FUNCTION_INSTRUCTIONS_COUNT = 100;

    // this value is defined so that it can be exceeded by untrusted data in symbolic execution
    static constexpr uint32_t UNDEF_ALLOCATION_SIZE = INT32_MAX - INT16_MAX;

    friend class TraceContext;

    struct PathToCheck;
    using PathPtr = std::shared_ptr<PathToCheck>;
    using PathArray = std::vector<PathPtr>;
    using PathsMapping = std::unordered_map<Instruction, PathArray>;

    struct PathContext {
        const SuspiciousPath& path;
        bool isSourceFound = false;
        bool canAddSink = false;
        std::vector<ExprId> sources;
        std::vector<ExprId> sinks;
    };

    struct PathKey {
        const PathContext* context = nullptr;
        Annotation annotation;

        bool operator==(const PathKey& key) const
        {
            return context == key.context && annotation == key.annotation;
        }
    };

    struct PathHash {
        std::size_t operator()(const PathKey& key) const noexcept
        {
            auto& a = key.annotation;
            return uint64_t(key.context) ^ a.GetInstruction() ^ a.GetMemoryOffset().GetRawValue();
        }
    };

    struct PathToCheck {
        const FunctionContext& owner;
        Annotation annotation;
        PathContext& context;
        SourceRange sourceRange;
        PathPtr parent;
        ExecId execId = 0;
        const FunctionContext* sinkFunction = nullptr;
    };

    struct CallInfo {
        const FunctionContext* funcCtx = nullptr;
        std::vector<Instruction> argInstructions;
    };

    struct OutArgSource {
        Instruction callInstruction = 0;
        uint32_t recursionDepth = 0;
        PathPtr sourcePath;
        VirtualOffset argOffset;
    };

private:
    void InitCallInfo(const FunctionContext& funcCtx);

    void ExecuteSolver(PathsMapping& paths);

    void CheckSolverPaths();

    void ProcessPath(PathPtr& path);

    bool CollectCalleePaths(FunctionContext& calleeCtx, PathPtr& path, PathsMapping& calleePaths);

    FunctionContext* ProcessCallInstruction(uint32_t& indexOfOverridden, PathArray& paths);

    void ProcessAllocAnnotations(FunctionContext& calleeCtx);

    void ProcessRetValueAnnotations(FunctionContext& calleeCtx);

    void ProcessRetValueConditions(FunctionContext& calleeCtx);

    void ProcessArgsAnnotations(FunctionContext* calleeCtx);

    void ProcessArgTaintAnnotations(FunctionContext& calleeCtx, uint32_t argPos, const Annotation& annotation);

    void ProcessSourceExpr(PathPtr& path, ExprId exprId);

    bool IsOutArgSource(PathPtr& path);

    bool ShouldProcessPath(PathPtr& path);

    void ProcessOutArgSources(Instruction callInstr);

    /// Mark arguments as untrusted sources if their respective parameters were annotated with TaintInput.
    void AddUntrustedSourcesForArguments();

    bool ShouldExecuteCallee(FunctionContext* callee, PathsMapping& curPaths, PathsMapping& calleePaths);

    void AddOutArgNullCondition(uint32_t argPos, VirtualOffset offset, Condition condition);

    void ExecuteCallee(FunctionContext& callee, PathsMapping& calleePaths);

    PathArray& GetPathArray(PathsMapping& paths, Instruction instr);

    bool IsRecursionLimit() const
    {
        return myRecursionDepth + 1 >= MAX_RECURSION_DEPTH;
    }

    void Dump();

private:
    DataFlowAnalyzer& myAnalyzer;
    ProblemsHolder& myHolder;
    const FunctionContext* myCurFuncCtx = nullptr;
    std::unique_ptr<SolverContext> myContext;
    std::vector<PathContext> myPaths;
    std::unordered_map<ExprId, PathPtr> myExprMapping;
    std::unordered_map<ExecId, CallInfo> myCallsMapping;
    std::unordered_set<PathKey, PathHash> myCalleePaths;
    std::vector<OutArgSource> myOutArgSources;
    uint32_t myExecutedInstructions = 0;
    Timer myExecutionTimer;
    bool myTimeoutIsReached = false;
    uint32_t myRecursionDepth = 0;
    uint32_t myCurIndexInPaths = 0;
};

#endif  // COODDY_ANALYZER_SOURCE_DFA_CHECKPATHCONTEXT_H_
