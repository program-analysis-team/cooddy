/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_CFG_CFG_H_
#define COODDY_ANALYZER_INCLUDE_CFG_CFG_H_

#include <ast/FunctionDecl.h>
#include <cfg/CfgBlock.h>

#include <map>

namespace HCXX {
class FunctionDecl;

class Cfg {
public:
    Cfg(const FunctionDecl& function, std::vector<CfgBlock>&& blocks)
        : myFunction(&function), myBlocks(std::move(blocks))
    {}
    Cfg(const FunctionDecl& function, IOStream& stream);
    ~Cfg();

    const FunctionDecl& GetFunction() const
    {
        return *myFunction;
    }

    const std::vector<CfgBlock>& GetBlocks() const
    {
        return myBlocks;
    }

    const std::vector<CfgBlock::Range>& GetLoops() const
    {
        return myLoops;
    }

    std::optional<uint32_t> IsNodeModifiedInRange(const Node* decl, const CfgBlock::Range& range) const;

    auto& GetModifiedNodes() const
    {
        return myModifiedNodes;
    }

    uint32_t GetExitBlock() const
    {
        return myExitBlock;
    }

    uint32_t GetEntryBlock() const
    {
        return myEntryBlock;
    }

    uint32_t GetMemorySize() const
    {
        return myMemorySize;
    }

    void Init();
    void DumpCFGToDot(std::ofstream& resultFile) const;

    void Serialize(IOStream& stream);

private:
    void GetTopologicalOrder(std::vector<uint32_t>& order, std::vector<uint32_t>& backOrder);

    void AddCtorInitializers(std::vector<HCXX::CfgElement>& elements);

    void AddCatchEdges();

    using GlobalsMap = std::map<uint32_t, const Node*>;
    std::vector<HCXX::CfgElement> ProcessElementsOfBlock(CfgBlock& block, uint32_t blockPos, bool isEntryBlock,
                                                         GlobalsMap& globals);

    friend class CfgBuilder;

    NodePtr<FunctionDecl> myFunction;
    std::vector<CfgBlock> myBlocks;
    std::vector<CfgBlock::Range> myLoops;
    std::map<std::pair<const Node*, uint32_t>, uint32_t> myModifiedNodes;
    uint32_t myEntryBlock = 0;
    uint32_t myExitBlock = 0;
    uint32_t myMemorySize = 0;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_CFG_CFG_H_
