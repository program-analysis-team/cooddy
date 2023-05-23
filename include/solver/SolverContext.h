/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_INCLUDE_SOLVER_SOLVERCONTEXT_H_
#define COODDY_ANALYZER_INCLUDE_SOLVER_SOLVERCONTEXT_H_

#include <ast/Node.h>
#include <ast/VirtualOffset.h>

#include <deque>

namespace HCXX {

// This alias is used to identify AST statement in the solver's representation
// The mapping between AST node and Instruction is created during creation of FunctionBehavior instance
// FunctionBehavior::GetInstruction can be used to get Instruction for AST node
using Instruction = uint32_t;
using ExprId = uint32_t;
using ExecId = uint32_t;
using SourceId = uintptr_t;
const int32_t UNDEFINED_VALUE = 0x0BADDE;
const ExprId UNDEF_EXPR_ID = UINT32_MAX;
const ExecId UNDEF_EXEC_ID = UINT32_MAX;

struct ExecutedBranchResult {
    ExecId execId;
    std::string result;
};
struct ExecutedReturnResult {
    ExecId execId;
    bool weakAssumption;
};

struct IntTypeInfo {
    uint16_t isUnsigned : 1;
    uint16_t sizeInBits : 14;  // may be reduced to 7; possible values: 8, 16, 32, 64, exact value for enum
    uint16_t isEnum : 1;

    uint64_t GetMaxValue() const
    {
        if (isEnum) {
            return sizeInBits;
        }
        uint64_t bitsToShift = isUnsigned ? sizeInBits : sizeInBits - 1;
        return bitsToShift == 64 ? UINT64_MAX : (1ULL << bitsToShift) - 1ULL;
    }
    int64_t GetMinValue() const
    {
        return (isUnsigned || isEnum) ? 0LL : -GetMaxValue() - 1;
    }
};

struct Condition {
    DECLARE_ENUM(Operation, NONE, EQ, NE, GT, LE, GE, LT, ACCESS, ACCESS_UNSINGED, INT_OVERFLOW, INT_MAX_VAL,
                 INT_MIN_VAL);

    void FromString(std::string_view str);
    std::string ToString() const;

    Operation operation = Operation::NONE;
    union {
        int32_t operand = 0;
        IntTypeInfo intTypeInfo;
    };
};

enum class ExprLocation { CURRENT, CURRENT_SIZE, CONSTANT, CALL_ARG, CALL_ARG_SIZE, OPERAND, CONDITION };

struct ArgType {
    uint8_t isPointer : 1;
    uint8_t isReference : 1;
    uint8_t isConst : 1;
    uint8_t isBuffer : 1;
    uint8_t isIntegral : 1;
    IntTypeInfo intInfo;
};

struct ArgInfo {
    ArgType type = {0};
    Instruction instruction = 0;
};

struct CheckPathParams {
    enum Flags {
        USE_EXIT_SYMBOLS_AS_SINKS = 1,
        USE_UNTRUSTED_SOURCES = 2,
        SINK_IS_THE_SAME_AS_SOURCE = 4,
        CHECK_UNREACHABLE_SINK = 8,
        USE_POINTER_ARITHMETIC = 16,
        USE_MEMORY_ADDRESSES = 32,
        USE_CONSTANTS_ONLY = 64,
        USE_INT_TYPE_LIMITS = 128,
        SINK_CAN_BE_BEFORE_SOURCE = 256
    };

    uint32_t flags = 0;
    std::vector<ExprId>& sinks;
    std::vector<ExprId>& sources;
    const Condition& sinkCondition;
    uintptr_t sinkKind = 0;
};

struct UntrustedSource {
    enum SourceKind { NONE, CALL_ARG, ITERATOR, PROPAGATION, BOUNDARY_CONDITION, STRING_LENGTH, SENSITIVE_DATA };

    SourceKind sourceKind = SourceKind::NONE;
    SourceId sourceId = 0;
    ExecId execId = UNDEF_EXEC_ID;
    uint32_t argPos = 0;
    const UntrustedSource* propagation = nullptr;

    const UntrustedSource& GetRoot() const
    {
        return propagation != nullptr ? propagation->GetRoot() : *this;
    }
};

struct AccessInfo {
    uint64_t allocatedSize = UNDEFINED_VALUE;
    int64_t accessOffsetBegin = UNDEFINED_VALUE;
    int64_t accessOffsetEnd = UNDEFINED_VALUE;
    int64_t sourceExprValue = UNDEFINED_VALUE;
};

struct PathInfo {
    ExprId sinkExpr = UNDEF_EXPR_ID;
    ExprId sourceExpr = UNDEF_EXPR_ID;
    std::vector<UntrustedSource> untrustedSources;
    std::vector<ExecutedReturnResult> returns;
    std::vector<ExecutedBranchResult> branches;
    uint64_t checkDuration = 0;
    const AccessInfo* accessInfo = nullptr;

    bool IsSinkIdDefined()
    {
        return sinkExpr != UNDEF_EXPR_ID;
    }
    bool IsSourceIdDefined()
    {
        return sourceExpr != UNDEF_EXPR_ID;
    }
};

struct ExecInfo {
    ExecId callId = 0;
    Instruction instruction = 0;
};

struct CallExecInfo : public ExecInfo {
    bool isVirtualCall = false;
    uint32_t recordId = 0;
};

// solver context (contains the current state of the execution)
struct SolverContext {
    virtual ~SolverContext() = default;

    // Get current processing instruction
    virtual Instruction GetCurInstruction() const = 0;

    // Get current instruction kind
    virtual Node::Kind GetCurKind() const = 0;

    // Returns unique id for the current execution location
    virtual ExecId GetCurExecId() = 0;

    // Returns instruction of the CallExpression is being called
    virtual Instruction GetCurCallInstruction() const = 0;

    // Returns unique id for the expression defined by location
    virtual ExprId GetExprId(ExprLocation loc = ExprLocation::CURRENT, int32_t param = 0) = 0;

    // Returns execution info by ExecId
    virtual ExecInfo GetExecInfo(ExecId execId) = 0;

    // Returns true if field condition was created
    virtual bool MakeFieldCondition(const std::vector<ExprId>& sourceIds, ExprId sinkId, VirtualOffset offset,
                                    bool addDerefCondition) = 0;

    // Add offset to exprId's symbol and expression
    virtual void AddOffset(ExprId exprId, VirtualOffset offset) = 0;

    // Returns true if current instruction is reachable
    virtual bool IsReachable() = 0;

    // Add condition to exprId's condition (if exprId == UNDEF_EXPR_ID, add condition to the global scope)
    virtual bool AddCondition(ExprId exprId, const Condition& condition) = 0;

    // Add condition ('lhe' op 'rhe') to exprId's condition (if exprId == UNDEF_EXPR_ID, add condition to the global
    // scope)
    virtual void AddCondition(ExprId exprId, ExprId lhe, Condition::Operation op, ExprId rhe) = 0;

    // Add string condition from annotation file
    virtual void AddStringCondition(const std::string& condition, bool setResult, uint32_t argPos,
                                    VirtualOffset offset) = 0;

    // Apply operation 'op' to expression 'exprId' with operand 'rhe' and save result to 'exprId'
    enum class Operation { SUB, ADD, MUL, DIV, ASSIGN };
    virtual void ApplyOperation(ExprId exprId, Operation op, ExprId rhe) = 0;

    // Returns true if this check is finished without timeout reached passed in duration
    // Reachable paths will be passed through callback argument
    using PathCallback = const std::function<bool(PathInfo&&)>&;
    virtual bool CheckPaths(const std::vector<CheckPathParams>& params, uint64_t& duration, PathCallback callback) = 0;

    // Returns true if current instruction has call instruction kind
    // If result is true, callExecInfo will contain information about call function instruction
    virtual bool IsCallExpr(CallExecInfo* callExecInfo = nullptr) const = 0;

    // Returns true if current instruction is call argument expression
    virtual bool IsCallArgument(ExprId exprId) const = 0;

    // Returns count of arguments for call instruction
    virtual size_t GetCallArgsCount() const = 0;

    // This method can be called if IsCallExpr returned true
    virtual ArgInfo GetCallArgInfo(uint32_t argPos) const = 0;

    // Copies src argument's expression to dst argument
    virtual void CopyCallArg(uint32_t srcPos, uint32_t dstPos, VirtualOffset dstOffset,
                             VirtualOffset srcOffset = VirtualOffset()) = 0;

    // Assign specific value to the argument's expression with specified offset
    // If value = UNDEFINED_VALUE, free variable will be assigned
    virtual void AssignCallArgValue(uint32_t argPos, VirtualOffset offset, int64_t value = UNDEFINED_VALUE) = 0;

    // Set argument as strlen expression for string argument
    virtual void AssignSizeExpr(uint32_t argPos, VirtualOffset offset, uint32_t arrayArgPos) = 0;

    // Set specified argument as untrusted source
    virtual void AddCallArgUntrustedSource(uint32_t argPos, SourceId sourceId) = 0;

    // Set dstArgPos argument as untrusted source if srcArgPos is untrusted
    virtual void CopyCallArgUntrustedSource(uint32_t dstArgPos, uint32_t srcArgPos) = 0;

    // Returns true if expression contains untrusted sources symbols and|or constants
    virtual bool IsUntrustedSource(ExprId exprId) = 0;

    // Set size for specified argument
    virtual void SetCallArgSize(uint32_t argPos, VirtualOffset offset, ExprId sizeExprId, bool isAlloc) = 0;

    // Add throw/exit to the executing basic block
    virtual void AddThrowExpr() = 0;

    // Add record id for the current expression
    virtual void AddRecordId(uint32_t recordId) = 0;

    // Create unique offset
    virtual VirtualOffset CreateUniqueOffset() = 0;

    // Get full execution array
    virtual const std::vector<ExecInfo>& GetExecInfoArray() = 0;

    virtual void AddUntrustedSourceByKind(ExprId expr, const UntrustedSource::SourceKind kind) = 0;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_SOLVER_SOLVERCONTEXT_H_
