/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <TranslationUnit.h>
#include <ast/AstManager.h>
#include <ast/UndefinedNode.h>

#include <cassert>

using namespace std;

namespace HCXX {

using NodeCreatorsMap = std::unordered_map<Node::Kind, std::function<Node*()>>;

NodeCreatorsMap& GetNodesCreatorsMap()
{
    static NodeCreatorsMap nodeCreators;
    return nodeCreators;
}

void RegisterNodeCreator(Node::Kind kind, std::function<Node*()> creator)
{
    GetNodesCreatorsMap().emplace(kind, creator);
}

AstManager::AstManager(Node* root) : myRoot(root)
{
    myNodes.emplace_back(myRoot);
}

AstManager::~AstManager()
{
    Clear();
}

Node* AstManager::AllocateNode(uint32_t nodeSize)
{
    Node* node = static_cast<Node*>(operator new(nodeSize));
    node->myRefCount = 1;
    return node;
}

void AstManager::AddNode(Node& node, Node& parent, bool system, SourceRange range)
{
    node.myTU = static_cast<TranslationUnit*>(myRoot);
    node.mySourceRange = range;
    if (!system) {
        node.myParent = &parent;
        node.myNext = parent.myChildren;
        node.myIndexPos = myNodes.size() + 1;
        parent.myChildren = &node;
        myNodes.emplace_back(&node);
    } else {
        mySystemNodes.emplace_back(&node);
    }
    myIndiciesInited = false;
}

// LCOV_EXCL_START
void AstManager::DumpTrUnitInfo() const
{
    for (const Node* n : GetNodes()) {
        std::cout << n->Dump();
    }
}
// LCOV_EXCL_STOP

const Node* AstManager::FindPrevOrParentNode(const Node& node) const
{
    assert(myIndiciesInited);
    Node* actualParent = node.myParent;

    // LCOV_EXCL_START
    if (actualParent == nullptr) {
        return nullptr;
    }
    // LCOV_EXCL_STOP

    const HCXX::SourceRange nodeRange = node.GetRange();

    Node* maxNode = nullptr;
    Location maxRangeEnd = 0;

    Node* minParent = nullptr;
    Location minParentEnd = std::numeric_limits<Location>::max();

    for (int i = node.myIndexPos - 1; i > actualParent->myIndexPos; i--) {
        Node* candidate = myNodes[i - 1];
        const HCXX::SourceRange candidateRange = candidate->GetRange();

        // LCOV_EXCL_START
        if (!candidateRange.IsValid()) {
            continue;
        }
        // LCOV_EXCL_STOP

        if (candidateRange.end <= nodeRange.begin && candidateRange.end >= maxRangeEnd) {
            maxNode = candidate;
            maxRangeEnd = candidateRange.end;
        }

        if (candidateRange.begin <= nodeRange.begin && nodeRange.end <= candidateRange.end &&
            candidateRange.end < minParentEnd) {
            minParent = candidate;
            minParentEnd = candidateRange.end;
        }
    }

    return minParent != nullptr ? minParent : maxNode != nullptr ? maxNode : actualParent;
}

const Node* AstManager::FindContainingNode(Location location) const
{
    auto it = std::upper_bound(myNodes.begin(), myNodes.end(), location,
                               [](Location loc, const Node* node) { return loc < node->GetRange().begin; });
    if (it == myNodes.begin()) {
        return nullptr;  // LCOV_EXCL_LINE: HCAT-2777
    }
    for (const auto* node = *--it; node != nullptr; node = node->GetParent()) {
        if (node->GetRange().end >= location) {
            return node;
        }
    }
    return nullptr;
}

/* Sort nodes by source range. Sorting is stable, i.e. parent is still before child */
void AstManager::SortNodes()
{
    std::stable_sort(myNodes.begin(), myNodes.end(),
                     [](const Node* nodeA, const Node* nodeB) { return nodeA->GetRange() < nodeB->GetRange(); });

    for (size_t i = 0; i < myNodes.size(); ++i) {
        myNodes[i]->myIndexPos = i + 1;
    }
    myIndiciesInited = true;
}

std::vector<const Node*> AstManager::GetNodes(const function<bool(const Node*)>& op) const
{
    std::vector<const Node*> result;
    for (auto& node : myNodes) {
        if (op(node)) {
            result.push_back(node);
        }
    }
    return result;
}

std::vector<const Node*> AstManager::GetNodesInRange(const SourceRange& range,
                                                     const function<bool(const Node*)>& op) const
{
    assert(myIndiciesInited);

    auto beginIt = std::lower_bound(myNodes.begin(), myNodes.end(), range.begin,
                                    [](const Node* node, Location loc) { return node->GetRange().begin < loc; });

    std::vector<const Node*> nodesInRange;
    for (; beginIt != myNodes.end() && (*beginIt)->GetRange().end <= range.end; ++beginIt) {
        if (op == nullptr || op(*beginIt)) {
            nodesInRange.emplace_back(*beginIt);
        }
    }
    return nodesInRange;
}

void AstManager::Clear()
{
    for (auto& node : mySystemNodes) {
        node->Clear();
        node->Release();
    }
    myRoot->Clear();
    myRoot->Release();
    myNodes = std::vector<Node*>();
    mySystemNodes = std::vector<Node*>();
}

void AstManager::Init()
{
    for (auto& node : myNodes) {
        node->Init();
    }
    SortNodes();
}

void AstManager::DeserializeNode(IOStream& stream, Node*& node, uint32_t& curId, bool system)
{
    Node::Kind kind = Node::Kind::UNDEFINED;
    stream << kind;
    if (kind == Node::Kind::TRANSLATION_UNIT) {
        return;
    }
    auto creator = GetNodesCreatorsMap()[kind];
    if (creator == nullptr) {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "Can't find node creator: " << Node::KindToCStr(kind) << std::endl;
        node = new UndefinedNode();
        // LCOV_EXCL_STOP
    } else {
        node = creator();
    }
    node->myTU = static_cast<TranslationUnit*>(myRoot);
    node->myIndexPos = system ? 0 : curId - 1;
    node->myRefCount = 1;
    stream.AddIdMapping(curId, node);
}

void AstManager::Serialize(IOStream& stream)
{
    uint32_t curId = 0;
    if (stream.IsOut()) {
        for (auto& it : myNodes) {
            stream.AddPtrMapping(it, ++curId);
        }
        for (auto& it : mySystemNodes) {
            stream.AddPtrMapping(it, ++curId);
        }
        stream.Add<uint32_t>(myNodes.size());
        for (auto& it : myNodes) {
            Node::Kind kind = it->GetKind();
            stream << kind;
        }
        stream.Add<uint32_t>(mySystemNodes.size());
        for (auto& it : mySystemNodes) {
            Node::Kind kind = it->GetKind();
            stream << kind;
        }
    } else {
        myNodes.resize(stream.Get<uint32_t>());
        for (auto& it : myNodes) {
            DeserializeNode(stream, it, ++curId, false);
        }
        mySystemNodes.resize(stream.Get<uint32_t>());
        for (auto& it : mySystemNodes) {
            DeserializeNode(stream, it, ++curId, true);
        }
        myIndiciesInited = true;
    }
    for (auto& it : myNodes) {
        it->Serialize(stream);
    }
    for (auto& it : mySystemNodes) {
        it->Serialize(stream);
    }
}

};  // namespace HCXX
