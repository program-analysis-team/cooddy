/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_ASTMANAGER_H_
#define COODDY_ANALYZER_INCLUDE_AST_ASTMANAGER_H_

#include <ast/Node.h>

#include <climits>
#include <functional>
#include <string>
#include <vector>

namespace HCXX {

class AstManager {
public:
    static std::atomic<uint64_t> nodesCount;
    static std::atomic<uint64_t> nodesSize;

    bool IsInited() const
    {
        return myIndiciesInited;
    }

    virtual Node* AllocateNode(uint32_t nodeSize);

    virtual void AddNode(Node& node, Node& parent, bool system, SourceRange range);

    void DumpTrUnitInfo() const;

    /* Find node that is closest to current one from the left. Probably it will be the previous sibling of current node
     * (same parent) or parent itself. */
    const Node* FindPrevOrParentNode(const Node& node) const;

    // Finds the nearest node which contains the specified location
    const Node* FindContainingNode(Location location) const;

    const std::vector<Node*>& GetNodes() const
    {
        return myNodes;
    }

    std::vector<const Node*> GetNodes(const std::function<bool(const Node*)>& op) const;

    std::vector<const Node*> GetNodesInRange(const SourceRange& range,
                                             const std::function<bool(const Node*)>& op = nullptr) const;

    Node*& GetParentByIndex(uint32_t idx)
    {
        return myParents[idx];
    }

protected:
    explicit AstManager(Node* root);
    ~AstManager();

    void Init();

    void SortNodes();

    void Clear();

    void Serialize(IOStream& stream);

    void DeserializeNode(IOStream& stream, Node*& node, uint32_t& curId, bool system);

private:
    Node* myRoot = nullptr;
    std::vector<Node*> myNodes;
    std::vector<Node*> mySystemNodes;
    std::vector<Node*> myParents;
    // COODDY_SUPPRESS UnusedMemberExpressionChecker (usage_in_assert_fp.cpp test-case)
    bool myIndiciesInited = false;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_ASTMANAGER_H_
