/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "ExecBranchesInfo.h"

void ExecBranchesInfo::EnterFunction(uint32_t entryBlock, uint16_t callId)
{
    if (myEntryPoint.IsUninitialized()) {
        myEntryPoint = {entryBlock, callId};
    }
    myBlocks.emplace_back(std::vector<ExecutedBlock>(entryBlock + 1));
}

void ExecBranchesInfo::AddBranchBlock(z3::expr& joinCondition, const BasicBlock& src, z3::expr& terminator,
                                      uint32_t currentPos, uint32_t successorPos, uint16_t callId)
{
    if (successorPos >= currentPos) {
        return;
    }
    ExecutedBlockId currentBlockId(currentPos, callId);
    ExecutedBlockId successorBlockId(successorPos, callId);

    auto& currentBlock = myBlocks[currentBlockId.callId][currentBlockId.blockPos];
    if (currentBlockId.callId > myPreviousBlockId.callId) {
        myBlockCallStack.push_back(currentBlockId);
    } else if (currentBlockId.callId < myPreviousBlockId.callId) {
        std::vector<ExecutedBlockId> calls;
        while (!myBlockCallStack.empty() && currentBlockId.callId < myBlockCallStack.back().callId) {
            calls.emplace_back(myBlockCallStack.back());
            myBlockCallStack.pop_back();
        }
        for (auto it = calls.rbegin(); it != calls.rend(); ++it) {
            currentBlock.calls.push_back(*it);
        }
    }

    currentBlock.transitions.emplace_back(Transition{src, terminator, joinCondition, successorBlockId});

    myPreviousBlockId = currentBlockId;
}
void ExecBranchesInfo::ProcessBranch(PathInfo& pathInfo, const z3::model& model, Transition* transition,
                                     const ExecutedBlock& pBlock)
{
    if (transition != nullptr && transition->src.GetSuccessorsCount() > 1 && pBlock.lastExecId != UINT32_MAX) {
        auto ev = model.eval(transition->terminatorCondition);
        int64_t res = 0;
        if (ev.is_numeral_i64(res)) {
            pathInfo.branches.emplace_back(ExecutedBranchResult{pBlock.lastExecId, std::to_string(res)});
        } else if (ev.is_bool() && (ev.is_true() || ev.is_false())) {
            pathInfo.branches.emplace_back(ExecutedBranchResult{pBlock.lastExecId, ev.to_string()});
        }
    }
}
void ExecBranchesInfo::ProcessExecutedBranches(PathInfo& pathInfo, const z3::model& model,
                                               const std::function<bool(uint16_t)>& predicate)
{
    std::vector<ExecId> lastExecIdForCall(myBlocks.size(), UINT32_MAX);

    // LCOV_EXCL_START: GCOV bug
    std::function<void(ExecutedBlockId, bool)> traverseExecutedBranches = [&](ExecutedBlockId blockId,
                                                                              bool weakAssumption) {
        // LCOV_EXCL_STOP
        if (blockId.IsUninitialized()) {
            return;  // LCOV_EXCL_LINE: HCAT-2778
        }
        auto& block = myBlocks[blockId.callId][blockId.blockPos];
        if (block.lastExecId != UINT32_MAX) {
            lastExecIdForCall[blockId.callId] = block.lastExecId;
        }

        for (auto& callBlock : block.calls) {
            traverseExecutedBranches(callBlock, weakAssumption || !predicate(callBlock.callId));
        }

        for (auto& trans : block.transitions) {
            auto result = model.eval(trans.joinCondition);
            if (result.is_true()) {
                ProcessBranch(pathInfo, model, &trans, block);
                traverseExecutedBranches(trans.block, weakAssumption);
                return;
            }
        }
        if (!block.transitions.empty()) {
            ProcessBranch(pathInfo, model, &block.transitions.front(), block);
            traverseExecutedBranches(block.transitions.front().block, weakAssumption);
        } else if (lastExecIdForCall[blockId.callId] != UINT32_MAX) {
            pathInfo.returns.emplace_back(ExecutedReturnResult{lastExecIdForCall[blockId.callId], weakAssumption});
        }
    };
    traverseExecutedBranches(myEntryPoint, false);
}
