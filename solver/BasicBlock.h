/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_BASICBLOCK_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_BASICBLOCK_H_

#include <functional>

#include "InstructionProcessor.h"

using namespace HCXX;

class ExecutionContext;
class FunctionBehaviorImpl;

class BasicBlock {
public:
    BasicBlock(FunctionBehaviorImpl& owner, uint32_t position, const CfgBlock& block);
    BasicBlock(FunctionBehaviorImpl& owner, uint32_t position, DataStream& stream);
    ~BasicBlock() = default;

    void StoreToStream(DataStream& stream);

    uint32_t GetPosition() const
    {
        return myPosition;
    }

    uint32_t GetSuccessorsCount() const
    {
        return mySuccessorsCount;
    }

    bool IsEndlessLoopEntry() const
    {
        return myIsEndlessLoop;
    }

    bool IsLoopEntrance() const
    {
        return myIsLoopEntrance;
    }

    bool IsLoopExit() const
    {
        return myIsLoopExit;
    }

    bool IsInLoop() const
    {
        return myIsInLoop;
    }

    bool IsDoWhileLoop() const
    {
        return myIsDoWhileLoop;
    }

    uint32_t GetEndOfLoop() const
    {
        return myEndOfLoop;
    }

    uint32_t GetSuccessor(uint32_t pos) const;

    z3::expr Execute(ExecutionContext& context, ExecutionCallbackRef callback) const;

    z3::expr GetJoinCondition(ExecutionContext& context, uint32_t branch, z3::expr& terminator,
                              z3::expr& joinedCondition) const;

private:
    void CompileLoopIterators(InstructionProcessor::CompileContext& context, const Cfg& cfg);

    void ExecuteLoopIterators(InstructionProcessor::ExecutionContext& context) const;

    void InitLoop(const CfgBlock& block);

private:
    FunctionBehaviorImpl* myOwner;
    uint32_t myPosition;
    uint8_t myFirstStoredField = 0;
    uint32_t myIsDefaultStatement : 1;
    uint32_t myIsLoopEntrance : 1;
    uint32_t myIsEndlessLoop : 1;
    uint32_t myIsDoWhileLoop : 1;
    uint32_t myIsLoopExit : 1;
    uint32_t myIsInLoop : 1;
    uint32_t myEndOfLoop = 0;
    uint32_t myLabelInstructions = 0;
    uint32_t myInstructions = 0;
    uint32_t myTerminatorInstructions = 0;
    uint32_t mySuccessorsCount = 0;
    uint32_t mySuccessorsOffset = 0;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_BASICBLOCK_H_
