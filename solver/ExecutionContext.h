/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_EXECUTIONCONTEXT_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_EXECUTIONCONTEXT_H_

#include <optional>
#include <stack>
#include <unordered_set>

#include "BasicBlock.h"
#include "CallStack.h"
#include "ExecBranchesInfo.h"
#include "MemoryMonitor.h"
#include "OperandsArray.h"
#include "SymbolAliases.h"
#include "UntrustedSources.h"

using namespace HCXX;
class SymbolsContext;

struct ExecutionException : public std::exception {
    explicit ExecutionException(ExecutionResult result) : result(result) {}
    ExecutionResult result;
};

class ExecutionContext : public SolverContext, public CallStack, public OperandsArray {
public:
    using SymbolsContextPtr = std::shared_ptr<SymbolsContext>;
    using SymbolsSet = std::unordered_set<SymbolId, SymbolId::Hash>;

    static const Instruction RET_INSTRUCTION = 1;
    static const Instruction THROW_INSTRUCTION = 2;
    static const Instruction THIS_INSTRUCTION = 3;
    static const SymbolId THROW_SYMBOL_ID;

    explicit ExecutionContext(FunctionBehaviorImpl* owner, bool useMemMonitor);
    ~ExecutionContext();

    void EnterFunction(FunctionBehaviorImpl& function, SymbolsContextPtr entrySymbols, uint32_t entryBlock);

    void LeaveFunction(SymbolsContextPtr exitSymbols);

    z3::expr Execute(const BasicBlock& block, SymbolsContextPtr symbols, ExecutionCallbackRef callback);

    const z3::expr& SetSymbol(const SymbolId& symbolId, const z3::expr& symbolExpr, const SymbolId* srcId = nullptr);

    const z3::expr& GetSymbolExpr(const SymbolId& symbolId);

    const z3::expr& GetSymbol(SymbolId& symbolId);

    const z3::expr& GetSubSymbol(SymbolId& symbolId, VirtualOffset offset);

    std::optional<z3::expr> GetOffsetExprFromPointerOffset(const z3::expr& expr);

    /// Convert an expression "arr + (N * S)" into "arr[N]"
    /// ...where N is the element index, S is the size of one element (ptr arithmetic is always is bytes)
    std::optional<SymbolId> ConvertPointerOffsetIntoIndex(const z3::expr& expr);

    /// Convert an expression "arr[N]" into "arr + (N * S)"
    /// ...where N is the element index, S is the size of one element (ptr arithmetic is always is bytes)
    std::optional<z3::expr> ConvertIndexIntoPointerOffset(SymbolId& symbolId, uint32_t elementSize);

    SymbolId ResolveSymbolId(const SymbolId& symbolId);

    bool IsExitSymbol(const SymbolId& symbol);

    void SetSymbolSize(const z3::expr& symbol, const z3::expr& symbolSize, bool isLiteral = false);

    virtual z3::expr CreateSymbolSizeExpr(const SymbolId& symbolId, const z3::expr& symbolExpr, bool strLen) = 0;

    z3::expr CreateSymbolExpr(const SymbolId& symbolId);

    z3::expr CreateIntegerExpr(int64_t value);

    z3::expr CreateUIntegerExpr(uint64_t value);

    z3::expr CreateIntegerCondition(z3::expr opExpr, IntTypeInfo opInfo);

    z3::expr CreateBoolExpr(bool value);

    VirtualOffset CreateUniqueOffset() override;

    void AddThrowExpr(const z3::expr* addCondition);

    void AddThrowExpr() override
    {
        AddThrowExpr(nullptr);
    }

    z3::expr& CastToBV(z3::expr& expr);

    z3::expr& CastToBool(z3::expr& expr);

    bool IsCallExpr(CallExecInfo* callExecInfo) const override;

    void CopyCallArg(uint32_t srcPos, uint32_t dstPos, VirtualOffset dstOffset, VirtualOffset srcOffset) override;

    void AssignCallArgValue(uint32_t argPos, VirtualOffset offset, int64_t value) override;

    void AssignSizeExpr(uint32_t argPos, VirtualOffset offset, uint32_t arrayArgPos) override;

    /**
     * @brief Create or find a z3 expression for this argument or return value.
     * @param argPos argument index or uint32_t(-1) for the return value.
     * @return z3 expression for the argument, or for return value if argPos >= argument count.
     */
    z3::expr GetArgExprByPos(uint32_t argPos, VirtualOffset* offset = nullptr);

    void AddUntrustedSource(UntrustedSourceExpr&& source);

    void AddCallArgUntrustedSource(uint32_t argPos, SourceId sourceId) override;
    void CopyCallArgUntrustedSource(uint32_t dstArgPos, uint32_t srcArgPos) override;

    void CopySubOffsetUntrustedSource(const z3::expr& dstExpr, const z3::expr& srcExpr);

    size_t GetCallArgsCount() const override;

    ::ArgInfo GetCallArgInfo(uint32_t argPos) const override;

    StackPos GetStackPos() const
    {
        return {static_cast<uint16_t>(myCurStack.size() - 1), myCurStack.back().callId};
    }

    Node::Kind GetCurKind() const override
    {
        return myCurInstrKind;
    }

    Instruction GetCurInstruction() const override
    {
        return myCurInstruction;
    }

    z3::expr GetCurConstraint();

    Instruction GetCurCallInstruction() const override
    {
        return CallStack::GetCurCallInstruction();
    }

    ExecId GetCurExecId() override
    {
        return myExecInfo.size() - 1;
    }

    ExecInfo GetExecInfo(ExecId execId) override
    {
        return execId < myExecInfo.size() ? myExecInfo[execId] : ExecInfo{};
    }

    void SetCurExecInfo(Instruction curInst, Node::Kind curInstrKind, z3::expr& curExpr, SymbolId& curSymbolId);

    VirtualOffset GetIndexOffset(z3::expr& indexExpr);

    void UpdateLoopsState(const BasicBlock& block);

    void AddLoopExitCondition();

    void AddLoopIterator(Instruction loopInstr, Instruction iterInstr, int32_t increment, IntTypeInfo typeInfo);

    z3::expr GetJoinCondition(const BasicBlock& src, const BasicBlock& dst, uint32_t branch, z3::expr& terminator,
                              z3::expr& joinedCondition);

    void AdjustCallStack()
    {
        CallStack::ClearCallStack(myCurStack.back().callStackPos);
    }

    void AddRecordId(uint32_t recordId, const SymbolId& symbolId)
    {
        myRecordIds.emplace(symbolId, recordId);
    }

    void AddRecordId(uint32_t recordId) override
    {
        AddRecordId(recordId, *myCurSymbolId);
    }

    virtual void AddUnsignedCondition(const z3::expr& expr) = 0;

    virtual void AddNotNullCondition(const z3::expr& expr) = 0;

    const BasicBlock* GetNextBlockForExecution();

    bool GetSuccessorForLastBlockInLoop(uint32_t& successor);

    SymbolId CreateGlobalSymbolId(uint32_t uniqueId, uint32_t instr);

    // LCOV_EXCL_START
    const std::vector<ExecInfo>& GetExecInfoArray() override
    {
        return myExecInfo;
    }
    // LCOV_EXCL_STOP

    z3::expr GetExprFromCache(Instruction instruction);

    void AddExprToCache(Instruction instruction, const z3::expr& expr)
    {
        myCurStack.back().exprCache.emplace(instruction, expr);
    }

    // returns SymbolId if it presented in symbols cache, otherwise adds it to symbols cache and returns nullptr
    const SymbolId* GetSymbolBySolverId(uint32_t id, const SymbolId& symbol)
    {
        auto it = myCurStack.back().symbolsCache.emplace(id, symbol);
        return it.second ? nullptr : &it.first->second;
    }

    void AddArrayMaxValue(const z3::expr& arrExpr, int64_t maxVal)
    {
        myArrMaxValue.emplace(arrExpr.id(), std::make_pair(arrExpr, maxVal));
    }

    const z3::expr& GetArrayExpr(SymbolId& arrSymbolId, const z3::expr& arrExpr, z3::expr& idxExpr);

    struct SymbolSizeInfo {
        z3::expr expr;
        z3::expr sizeExpr;
        z3::expr condition;
        bool isUnknownSize = false;
        bool isLiteral = false;
        bool isAlloc = false;
    };

    const SymbolSizeInfo* FindSizeExpr(const z3::expr& expr);

protected:
    struct LoopNode {
        struct IteratorInfo {
            SymbolId symbolId;
            int32_t increment;
            IntTypeInfo typeInfo;
            z3::expr boundCondition;
        };
        const BasicBlock& entranceBlock;
        z3::expr iterStepExpr;
        z3::expr_vector iterExpr;
        z3::expr_vector initExpr;
        z3::expr breakCondition;
        std::vector<IteratorInfo> iterInfo;
        bool shouldExecute = true;
        bool addedItersAsUntrusted = false;
    };

    bool ShouldExecuteLoop(uint32_t& successor);

    bool CheckLoopCondition(z3::expr& condition, LoopNode& loop, bool loopEntrance);

    void ModifyLoopCondition(z3::expr& condition, LoopNode& loop, bool loopEntrance, uint32_t branch);

    void AddLoopItersAsUntrusted(const z3::expr& boundCondition, LoopNode& loop,
                                 const UntrustedSourceExpr* untrustedSource);

    struct StackNode {
        uint16_t callId = 0;
        ExecId callExecId = 0;
        z3::expr endlessLoopExitCondition;
        uint32_t block = 0;
        uint32_t callStackPos = 0;
        std::vector<LoopNode> loopsStack;
        SymbolsSet setSymbols;
        SymbolsContextPtr symbols;
        std::unordered_map<Instruction, z3::expr> exprCache;
        std::unordered_map<uint32_t, SymbolId> symbolsCache;
    };

    struct CallInfo {
        uint16_t stackPos;
        ExecId execId;
        FunctionBehaviorImpl* function;
    };

    struct IndexInfo {
        z3::expr expr;
        VirtualOffset offset;
        z3::expr boundaryCondition;
        bool isConditionInited = false;
    };

    MemoryMonitor myMemMonitor;
    z3::context myContext;
    z3::expr myTrueExpr;
    std::vector<StackNode> myCurStack;
    std::vector<ExecInfo> myExecInfo;
    std::vector<CallInfo> myCallInfo;
    std::vector<z3::expr> myLogicalOperands;
    std::unordered_map<uint32_t, ExecId> myTerminatorsExecIds;
    std::unordered_map<uint32_t, SymbolSizeInfo> mySymbolSizes;
    std::unordered_map<uint32_t, IndexInfo> myIndexOffsets;
    std::unordered_map<uint32_t, std::pair<z3::expr, int64_t>> myArrMaxValue;
    std::unordered_map<SymbolId, uint32_t, SymbolId::Hash> myRecordIds;
    std::unordered_map<FunctionBehaviorImpl*, uint32_t> myGlobalIds;
    std::unordered_map<uint64_t, VirtualOffset> myUniqueOffsets;
    uint32_t myCurUniqueOffset = 0;
    Instruction myCurInstruction = 0;
    Node::Kind myCurInstrKind = Node::Kind::UNDEFINED;
    z3::expr* myCurExpr = nullptr;
    SymbolId* myCurSymbolId = nullptr;
    ExecBranchesInfo myExecBranchesInfo;
    SymbolAliases mySymbolAliases;
    FunctionBehaviorImpl* myFunctionBehavior = nullptr;
    SymbolsContextPtr myExitSymbols = nullptr;
    SymbolsSet myExitSetSymbols;
    UntrustedSources myUntrustedSources;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_EXECUTIONCONTEXT_H_
