/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/LoopStatement.h>

#include "cfg/Cfg.h"

namespace HCXX {

void CfgBlock::Init(const Cfg& cfg)
{
    if (Node::Cast<LoopStatement>(myTerminatorStatement) != nullptr) {
        auto cond = Node::Cast<TypedNode>(myTerminatorCondition);
        if (myTerminatorCondition == nullptr ||
            cond != nullptr && cond->IsConstExpr() && cond->GetSLimitedValue() != 0) {
            myIsEndlessLoop = true;
        }
    }
    if (myIsEndlessLoop) {
        if (mySuccessors.size() > 1) {
            mySuccessors.pop_back();
        }
        mySuccessors.emplace_back(cfg.GetExitBlock());
    }
}

CfgBlock::Range CfgBlock::FindNearestLoop() const
{
    for (auto& loopRange : myCfg->GetLoops()) {
        if (GetPos() >= loopRange.first && GetPos() <= loopRange.second) {
            return loopRange;
        }
    }
    return {0, 0};
}

bool CfgBlock::HasLoopIterators() const
{
    auto loop = FindNearestLoop();
    if (loop.second == 0 || loop.first == 0) {
        return false;
    }
    auto lastBlock = loop.first;
    auto& blocks = myCfg->GetBlocks();
    while (lastBlock != loop.second && blocks[lastBlock].myElements.empty()) {
        lastBlock++;
    }
    return lastBlock == myPos;
}

}  // namespace HCXX
