/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CatchExpression.h>
#include <ast/CxxCtorDecl.h>
#include <ast/RefExpression.h>
#include <ast/TryExpression.h>
#include <cfg/Cfg.h>

#include <fstream>
#include <unordered_set>

#include "CfgBuilder.h"

using namespace HCXX;

namespace HCXX {

Cfg::Cfg(const FunctionDecl& function, IOStream& stream) : myFunction(&function)
{
    auto numBlocks = stream.Get<uint32_t>();
    for (auto i = 0; i < numBlocks; ++i) {
        myBlocks.emplace_back(CfgBlock(i, this)).Serialize(stream);
    }
    Init();
}

Cfg::~Cfg()
{
    for (auto& block : myBlocks) {
        if (block.GetTerminatorCondition() != nullptr) {
            block.GetTerminatorCondition()->Traverse([](const Node& node) { node.Release(); });
        }
        for (auto& elem : block.GetElements()) {
            elem.GetNode()->Traverse([](const Node& node) { node.Release(); });
        }
    }
    myFunction->Traverse([&](const Node& node) { node.Release(); });
}

void Cfg::Serialize(IOStream& stream)
{
    stream.Add<uint32_t>(myBlocks.size());
    for (auto& block : myBlocks) {
        block.Serialize(stream);
    }
}

std::vector<HCXX::CfgElement> Cfg::ProcessElementsOfBlock(CfgBlock& block, uint32_t blockPos, bool isEntryBlock,
                                                          GlobalsMap& globals)
{
    std::unordered_set<const Node*> elemSet;
    auto callback = [&](const Node& node) {
        if (!node.IsDeclaration()) {
            elemSet.emplace(&node);
        }
        node.AddRef();
    };
    auto declUsageCallback = [&](const Node& node, uint32_t flags) {
        auto varDecl = Node::Cast<VarDecl>(&node);
        if (varDecl != nullptr && !varDecl->IsLocalVariableDeclaration() && varDecl->GetType().IsArray()) {
            globals.emplace(varDecl->GetRange().begin, varDecl);
        }
        if (flags != 0) {
            myModifiedNodes[{&node, blockPos}] |= flags;
        }
    };
    if (block.GetTerminatorCondition() != nullptr) {
        block.GetTerminatorCondition()->Traverse(callback);
        block.GetTerminatorCondition()->TraverseDeclUsage(declUsageCallback);
    }

    std::vector<HCXX::CfgElement> elements;
    for (auto el = block.GetElements().rbegin(); el != block.GetElements().rend(); ++el) {
        if (el->GetNode() == nullptr || !elemSet.emplace(el->GetNode()).second) {
            continue;  // LCOV_EXCL_LINE
        }
        elements.emplace_back(*el);
        el->GetNode()->Traverse(callback);
        el->GetNode()->TraverseDeclUsage(declUsageCallback);
    }

    if (isEntryBlock) {
        AddCtorInitializers(elements);
        elements.reserve(elements.size() + globals.size());
        for (auto& global : globals) {
            global.second->Traverse(callback);
            elements.emplace_back(CfgElement(CfgElement::Kind::STATEMENT, global.second));
        }
    }
    std::reverse(elements.begin(), elements.end());
    return elements;
}

void Cfg::Init()
{
    myMemorySize = sizeof(Cfg) + myBlocks.capacity() * sizeof(CfgBlock);
    myEntryBlock = myBlocks.size() - 1;
#ifdef CODE_COVERAGE
    myBlocks.clear();  // clear existing Cfg in code coverage mode to test CfgBuilder
#endif
    if (myBlocks.empty()) {
        CfgBuilder(*this).Build();
    }
    AddCatchEdges();
    std::vector<uint32_t> order;
    std::vector<uint32_t> backOrder(myBlocks.size());
    GetTopologicalOrder(order, backOrder);

    GlobalsMap globals;
    std::vector<HCXX::CfgBlock> blocks;
    blocks.reserve(order.size());
    for (auto& b : order) {
        auto& block = myBlocks[b];
        HCXX::CfgBlock::BlocksRef successors;
        auto& blockSuccessors = block.GetSuccessors();
        successors.reserve(blockSuccessors.size());
        for (auto& successor : blockSuccessors) {
            successors.emplace_back(backOrder[successor]);
        }
        HCXX::CfgBlock::BlocksRef predecessors;
        auto& blockPredecessors = block.GetPredecessors();
        predecessors.reserve(blockPredecessors.size());
        for (auto& predecessor : blockPredecessors) {
            uint32_t predBlock = backOrder[predecessor];
            if (predBlock != 0 && predBlock < blocks.size()) {
                myLoops.emplace_back(predBlock, blocks.size());
            }
            predecessors.emplace_back(predBlock);
        }
        auto elements = ProcessElementsOfBlock(block, blocks.size(), b == order.back(), globals);

        myMemorySize += (successors.capacity() + predecessors.capacity()) * sizeof(CfgBlock::BlocksRef) +
                        elements.capacity() * sizeof(CfgElement);

        blocks.emplace_back(blocks.size(), std::move(elements), std::move(successors), std::move(predecessors),
                            block.GetTerminatorCondition(), block.GetLabelStatement(), block.GetTerminatorStatement(),
                            block.IsEndlessLoop(), this);
    }
    myBlocks.swap(blocks);
    myEntryBlock = myBlocks.size() - 1;
    myExitBlock = 0;

    myFunction->Traverse([&](const Node& node) { node.AddRef(); });
}

void Cfg::AddCtorInitializers(std::vector<HCXX::CfgElement>& elements)
{
    auto ctorDecl = Node::Cast<CxxCtorDecl>(myFunction);
    if (ctorDecl == nullptr) {
        return;
    }
    auto& initList = ctorDecl->GetInitList();
    for (auto it = initList.rbegin(); it != initList.rend(); ++it) {
        (*it)->Traverse([&](const Node& node) { node.AddRef(); });
        elements.emplace_back(CfgElement(CfgElement::Kind::INITIALIZER, *it));
    }
}

void Cfg::GetTopologicalOrder(std::vector<uint32_t>& order, std::vector<uint32_t>& backOrder)
{
    std::vector<bool> visited(myBlocks.size());
    uint32_t curOrderPos = 0;

    std::function<void(uint32_t)> dfs = [&](uint32_t block) {
        if (visited[block]) {
            return;
        }
        visited[block] = true;
        myBlocks[block].Init(*this);
        auto& successors = myBlocks[block].GetSuccessors();
        for (auto it = successors.rbegin(); it != successors.rend(); ++it) {
            dfs(*it);
        }
        order.emplace_back(block);
        backOrder[block] = curOrderPos++;
    };

    dfs(GetEntryBlock());
}

// LCOV_EXCL_START
void Cfg::AddCatchEdges()
{
    std::unordered_map<const Node*, uint32_t> tryBlocks;
    for (size_t i = 0; i < myBlocks.size(); ++i) {
        auto terminator = myBlocks[i].GetTerminatorStatement();
        if (Node::Cast<TryExpression>(terminator) != nullptr) {
            tryBlocks.emplace(terminator, i);
        }
    }
    for (size_t i = 0; i < myBlocks.size(); ++i) {
        auto& block = myBlocks[i];
        if (block.GetElements().size() == 0 || Node::Cast<CatchExpression>(block.GetLabelStatement()) != nullptr) {
            continue;
        }
        auto node = block.GetElements().back().GetNode();
        auto tryNode = node != nullptr ? node->GetParentOfType<TryExpression>() : nullptr;
        if (tryNode == nullptr) {
            continue;
        }
        auto it = tryBlocks.find(tryNode);
        if (it == tryBlocks.end()) {
            continue;
        }
        auto& tryBlock = myBlocks[it->second];
        tryBlocks.erase(it);
        for (auto& trySucc : tryBlock.GetSuccessors()) {
            auto label = myBlocks[trySucc].GetLabelStatement();
            if (Node::Cast<CatchExpression>(label) != nullptr) {
                block.AddSuccessor(trySucc);
                myBlocks[trySucc].AddPredecessor(i);
            }
        }
    }
}

void Cfg::DumpCFGToDot(std::ofstream& resultFile) const
{
    if (!resultFile.is_open()) {
        return;
    }
    resultFile << "digraph{\n";
    resultFile << "label=\"Function " << myFunction->GetName() << "\"\n";
    resultFile << "labelloc = \"t\"\n";
    // write Description of each block
    for (auto& block : myBlocks) {
        resultFile << block.GetPos() << "[label=\"" << block.GetPos() << ".\n";
        resultFile << "Elements:\n";
        for (auto& el : block.GetElements()) {
            resultFile << el.GetNode()->GetSourceText() << "\n";
        }

        if (block.GetTerminatorStatement() != nullptr) {
            resultFile << "Terminator statement:\n";
            resultFile << block.GetTerminatorCondition()->GetSourceText() << "\n";
        }
        resultFile << "\"]\n";
    }

    for (auto& block : myBlocks) {
        for (auto& successor : block.GetSuccessors()) {
            resultFile << block.GetPos() << " -> " << successor << "\n";
        }
    }
    resultFile << "}";
}
// LCOV_EXCL_STOP

std::optional<uint32_t> Cfg::IsNodeModifiedInRange(const Node* decl, const CfgBlock::Range& range) const
{
    auto it = myModifiedNodes.lower_bound(std::make_pair(decl, range.first));
    if (it != myModifiedNodes.end() && it->first.first == decl && it->first.second <= range.second) {
        return it->second;
    }
    return std::nullopt;
}

}  // namespace HCXX
