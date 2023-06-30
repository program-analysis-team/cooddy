/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PATHCHECKER_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PATHCHECKER_H_

#include <utils/Timer.h>

#include "ExecutionContext.h"

using ExpressionsMap = std::unordered_map<uint32_t, std::vector<ExprId>>;
using ReachableSinksMap = std::unordered_map<uintptr_t, std::unordered_set<uint32_t>>;
using PathSources = std::vector<ExprId>;

class PathChecker : public ExecutionContext {
public:
    explicit PathChecker(FunctionBehaviorImpl* owner, bool useMemMonitor)
        : ExecutionContext(owner, useMemMonitor), mySolver(myContext, z3::solver::simple()), myConstraints(myTrueExpr)
    {
        if (useMemMonitor) {
            myMemMonitor.Check(mySolver);
        }
    }
    ~PathChecker() = default;

private:
    struct StoredExpr {
        z3::expr expr;
        z3::expr condition;
        SymbolId symbolId;
        ExecId execId = UNDEF_EXEC_ID;
    };

    struct AccessInfo {
        const SymbolSizeInfo* memoryInfo;
        z3::expr beginOffsetExpr;
        z3::expr endOffsetExpr;
        ::AccessInfo evaledInfo;
    };

    struct CheckContext {
        const CheckPathParams& params;
        z3::expr_vector conditions;
        z3::expr orExpr;
        ReachableSinksMap& reachableSinks;
        ExpressionsMap sourceIds;
        std::vector<PathSources> pathSources;
    };

    struct MakePathContext {
        const SymbolId& sinkId;
        const ExpressionsMap& sourceIds;
        uint32_t checkFlags = 0;
        ExecId sinkExecId = UNDEF_EXEC_ID;
        std::unordered_map<uint32_t, z3::expr> processedExpr;
        PathSources pathSources;
        bool hasUntrustedSources = false;
        bool hasFreeSymbols = false;
    };

    ExprId StoreSymbolExpr(const z3::expr& expr, const SymbolId& symbolId = SymbolId(StackPos{}));

    void AddOffset(ExprId exprId, VirtualOffset offset) override;

    void SetCallArgSize(uint32_t argPos, VirtualOffset offset, ExprId sizeExprId, bool isAlloc) override;

    ExprId GetExprId(ExprLocation loc, int32_t param) override;

    bool AddCondition(ExprId exprId, const Condition& condition) override;

    void AddCondition(ExprId exprId, ExprId lhe, Condition::Operation op, ExprId rhe) override;

    void AddStringCondition(const std::string& condition, bool setResult, uint32_t argPos,
                            VirtualOffset offset) override;

    void ApplyOperation(ExprId exprId, Operation op, ExprId rhe) override;

    z3::expr CreateSymbolSizeExpr(const SymbolId& symbolId, const z3::expr& symbolExpr, bool strLen) override;

    z3::expr CreateExprByOp(Condition::Operation operation, const z3::expr& op1, const z3::expr& op2,
                            ExprId exprId = UNDEF_EXPR_ID);

    z3::expr CreateExprByCondition(const z3::expr& op, Condition condition, ExprId exprId = UNDEF_EXPR_ID);

    z3::expr CreateAccessCondition(const z3::expr& expr, const z3::expr& offset, ExprId exprId, bool isSignedType);

    void SetMemoryOffset(z3::expr& memoryOffset, z3::expr offset);

    const ::AccessInfo* GetAccessInfoByModel(z3::model& model, ExprId exprId, ExprId sourceId);

    bool IsReachable() override
    {
        return CheckConstraint(GetCurConstraint(), false) == z3::sat;
    }

    bool IsUntrustedSource(ExprId exprId) override;

    bool IsCallArgument(ExprId exprId) const override;

    z3::check_result CheckConstraint(const z3::expr& expr, bool useCache = true);

    bool CheckPaths(const std::vector<CheckPathParams>& params, uint64_t& duration, PathCallback callback) override;

    void FindSinkSameAsSource(const CheckPathParams& params, PathCallback callback);

    bool IsSourcePassedToCall(const SymbolId& sourceId, uint16_t callId);

    std::optional<PathInfo> FindReachablePath(CheckContext& context);
    std::optional<PathInfo> FindReachableSource(CheckContext& context);
    std::optional<PathInfo> FindReachableSource(CheckContext& context, uint32_t sinkPos);

    void MakeSinkConditions(CheckContext& context);
    void MakeUnreachableSinkConditions(CheckContext& context);

    bool MakeFieldCondition(const std::vector<ExprId>& sourceId, ExprId sinkId, VirtualOffset offset,
                            bool addDerefCondition) override;

    /// For specified sink build Path Condition to all reachable sources.
    z3::expr MakePathCondition(const z3::expr& sinkExpr, MakePathContext&& context, bool checkFreeSymbols = false);

    z3::expr MakePathConditionFromArgs(const z3::expr& expr, MakePathContext& context);

    z3::expr MergeItePathConditions(const z3::expr& iteExpr, MakePathContext& context);

    z3::expr CheckPathCondition(const z3::expr& expr, MakePathContext& context);

    z3::expr CheckTaintCondition(const z3::expr& expr, MakePathContext& context);

    /// Check if returned value may contain addresses of local variables as its field and return a condition to source
    /// if it is so. Return nullopt otherwise.
    std::optional<z3::expr> CheckReturnExpr(const z3::expr& sinkExpr, MakePathContext& context);

    void AddAddrOfConditions(const z3::expr& expr);

    void AddUnsignedCondition(const z3::expr& expr) override;

    void AddNotNullCondition(const z3::expr& expr) override;

    void AddIndexBoundaryCondition(const z3::expr& expr, const z3::expr& sizeExpr);

    const z3::expr& GetOffsetBoundaryCondition(const z3::expr& offsetExpr, const z3::expr& sizeExpr);

    void CheckPaths(size_t l, size_t r, PathCallback callback);

    bool CheckTimer()
    {
        return !myWasTimeout && myTimer.Duration() < myTimeout;
    }

    void AddUntrustedSourceByKind(ExprId expr, const UntrustedSource::SourceKind kind) override;

private:
    z3::solver mySolver;
    z3::expr myConstraints;
    Timer myTimer;
    uint64_t myTimeout = 0;
    bool myWasTimeout = false;
    std::vector<CheckContext> myContexts;
    std::vector<StoredExpr> myStoredExpressions;
    std::unordered_map<uint32_t, z3::expr> myProcessedAddrOff;
    std::unordered_map<uint32_t, AccessInfo> myAccessInfoMap;
    std::unordered_map<uint32_t, z3::check_result> myCachedVerdicts;
    std::unordered_map<uint32_t, z3::expr> myUnsignedExpr;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PATHCHECKER_H_
