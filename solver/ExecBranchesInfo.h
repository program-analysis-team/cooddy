/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_EXECBRANCHESINFO_H
#define COODDY_EXECBRANCHESINFO_H

#include <deque>
#include <functional>
#include <unordered_map>

#include "BasicBlock.h"
#include "solver/SolverContext.h"
#include "utils/HashImpl.h"
#include "z3++.h"

using namespace HCXX;

class ExecBranchesInfo {
public:
    void EnterFunction(uint32_t entryBlock, uint16_t callId);

    void AddBranchBlock(z3::expr& joinCondition, const BasicBlock& src, z3::expr& terminator, uint32_t currentPos,
                        uint32_t successorPos, uint16_t callId);

    void SetCurExecId(uint32_t pos, uint16_t callId, ExecId execId)
    {
        myBlocks[callId][pos].lastExecId = execId;
    }

    void ProcessExecutedBranches(PathInfo& pathInfo, const z3::model& model,
                                 const std::function<bool(uint16_t)>& predicate);

private:
    struct ExecutedBlockId {
        ExecutedBlockId(uint32_t blockPos = UINT32_MAX, uint16_t callId = UINT16_MAX)
            : blockPos(blockPos), callId(callId)
        {}
        uint32_t blockPos;
        uint16_t callId;
        bool IsUninitialized() const
        {
            return blockPos == UINT32_MAX && callId == UINT16_MAX;
        }
        bool operator==(const ExecutedBlockId& b) const
        {
            return blockPos == b.blockPos && callId == b.callId;
        }
    };

    struct Transition {
        const BasicBlock& src;
        z3::expr terminatorCondition;
        z3::expr joinCondition;
        ExecutedBlockId block;
    };

    struct ExecutedBlock {
        std::vector<Transition> transitions;
        std::vector<ExecutedBlockId> calls;
        ExecId lastExecId = UINT32_MAX;
    };
    void ProcessBranch(PathInfo& pathInfo, const z3::model& model, Transition* transition, const ExecutedBlock& pBlock);

    std::vector<std::vector<ExecutedBlock>> myBlocks;
    std::deque<ExecutedBlockId> myBlockCallStack;
    ExecutedBlockId myPreviousBlockId;
    ExecutedBlockId myEntryPoint;
};

#endif  // COODDY_EXECBRANCHESINFO_H
