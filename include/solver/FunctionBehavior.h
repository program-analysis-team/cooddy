/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_SOLVER_FUNCTIONBEHAVIOR_H_
#define COODDY_ANALYZER_INCLUDE_SOLVER_FUNCTIONBEHAVIOR_H_

#include <cfg/Cfg.h>
#include <solver/SolverContext.h>
#include <utils/DataStream.h>

#include <functional>
#include <memory>

namespace HCXX {

enum class ExecutionResult {
    OK,
    SKIP,    // skip traversing the current cfg branch
    CANCEL,  // cancel execution
    REPEAT   // repeat execution of the current instruction (for example, to process the next virtual call)
};
using ExecutionCallback = std::function<ExecutionResult()>;
using ExecutionCallbackRef = const std::function<ExecutionResult()>&;

class FunctionBehavior {
public:
    virtual ~FunctionBehavior() = default;

    // store internal structures to stream
    virtual void StoreToStream(DataStream& stream) = 0;

    // get corresponding instruction
    virtual Instruction GetInstruction(const Node& node) = 0;

    // get source range of the function
    virtual SourceRange GetFunctionRange() const = 0;

    // get source range by instruction
    virtual SourceRange GetSourceRange(Instruction instruction) = 0;

    // get node kind by instruction
    virtual Node::Kind GetInstructionKind(Instruction instruction) = 0;

    // create execution context started from the specified instruction
    virtual std::unique_ptr<SolverContext> StartExecution() = 0;

    // execute to the specified instruction
    // return value is false if execution is not possible
    virtual void Execute(SolverContext& context, ExecutionCallbackRef callback) = 0;

    // Cleanup nodes mapping (after this call GetInstruction will return zero)
    virtual void CleanUpMapping() = 0;

    virtual uint32_t GetMaxInstruction() = 0;

    // create FunctionBehavior instance
    static std::unique_ptr<FunctionBehavior> Create(const Cfg& cfg);
    static std::unique_ptr<FunctionBehavior> Create(DataStream& stream);
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_SOLVER_FUNCTIONBEHAVIOR_H_
