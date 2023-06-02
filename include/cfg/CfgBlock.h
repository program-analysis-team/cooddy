/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_CFG_CFGBLOCK_H_
#define COODDY_ANALYZER_INCLUDE_CFG_CFGBLOCK_H_

#include <cfg/CfgElement.h>

namespace HCXX {
class Cfg;
class CfgBuilder;

class CfgBlock {
public:
    using BlocksRef = std::vector<uint32_t>;
    using Range = std::pair<uint32_t, uint32_t>;

    CfgBlock(uint32_t pos, std::vector<CfgElement>&& elements, BlocksRef&& successors, BlocksRef&& predecessors,
             const Node* terminatorCondition, const Node* labelStatement, const Node* terminatorStatement,
             bool isEndlessLoop, const Cfg* cfg)
        : myPos(pos),
          myElements(std::move(elements)),
          mySuccessors(std::move(successors)),
          myPredecessors(std::move(predecessors)),
          myTerminatorCondition(terminatorCondition),
          myLabelStatement(labelStatement),
          myTerminatorStatement(terminatorStatement),
          myIsEndlessLoop(isEndlessLoop),
          myCfg(cfg)
    {}
    CfgBlock(uint32_t pos, const Cfg* cfg) : myPos(pos), myCfg(cfg) {}

    void Init(const Cfg& cfg);

    void Serialize(IOStream& stream)
    {
        stream << myElements << mySuccessors << myPredecessors << myTerminatorCondition << myLabelStatement
               << myTerminatorStatement << myIsEndlessLoop;
    }

    const uint32_t GetPos() const
    {
        return myPos;
    }

    const std::vector<CfgElement>& GetElements() const
    {
        return myElements;
    }

    const BlocksRef& GetSuccessors() const
    {
        return mySuccessors;
    }

    const BlocksRef& GetPredecessors() const
    {
        return myPredecessors;
    }

    const Node* GetTerminatorCondition() const
    {
        return myTerminatorCondition;
    }

    const Node* GetLabelStatement() const
    {
        return myLabelStatement;
    }

    const Node* GetTerminatorStatement() const
    {
        return myTerminatorStatement;
    }

    // LCOV_EXCL_START
    void AddSuccessor(uint32_t successor)
    {
        mySuccessors.emplace_back(successor);
    }

    void AddPredecessor(uint32_t predecessor)
    {
        myPredecessors.emplace_back(predecessor);
    }
    // LCOV_EXCL_STOP

    bool IsEndlessLoop() const
    {
        return myIsEndlessLoop;
    }

    const Cfg* GetCfg() const
    {
        return myCfg;
    }

    Range FindNearestLoop() const;

    /// Returns true if cfg block can contain loop iterators
    bool HasLoopIterators() const;

private:
    friend class CfgBuilder;

    const Cfg* myCfg;
    uint32_t myPos;
    std::vector<CfgElement> myElements;
    BlocksRef mySuccessors;
    BlocksRef myPredecessors;
    NodePtr<Node> myTerminatorCondition;
    NodePtr<Node> myLabelStatement;
    NodePtr<Node> myTerminatorStatement;
    bool myIsEndlessLoop = false;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_CFG_CFGBLOCK_H_
