/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CfgBuilder.h"

#include <cfg/Cfg.h>

namespace HCXX {

void CfgBuilder::Build()
{
    AddScope(Scope::FUNCTION, [&]() {
        AddNewBlock();  // add entry block
        AddScope(Scope::DEFAULT, [&]() { AddStatement(myCfg.myFunction->GetBody()); });
        AddNewBlock();  // add exit block
    });
    for (auto it : myBlocksToReverse) {
        std::reverse(myBlocks[it].mySuccessors.begin(), myBlocks[it].mySuccessors.end());
    }
    myCfg.myEntryBlock = 0;
    myCfg.myExitBlock = myBlocks.size() - 1;
}

void CfgBuilder::AddScope(Scope::Kind kind, const std::function<void()>& scopeBuilder)
{
    Scope scope{kind, myCurScope};
    EnterScope(scope);
    scopeBuilder();
    LeaveScope(scope);
}

void CfgBuilder::EnterScope(Scope& scope)
{
    if (myCurScope != nullptr) {
        myCurScope->isTerminator = true;
        auto& joinBlocks = scope.kind != Scope::ELSE_PART ? myCurScope->joinBlocks : myCurScope->elseJoinBlocks;
        std::swap(scope.joinBlocks, joinBlocks);
        if (scope.kind != Scope::SWITCH && scope.joinBlocks.empty()) {
            AddJoinBlock(&scope);
        }
    }
    myCurScope = &scope;
}

void CfgBuilder::LeaveScope(Scope& scope)
{
    myCurScope = scope.parent;
    if (myCurScope == nullptr) {
        return;
    }
    auto& joinBlocks = myCurScope->joinBlocks;
    joinBlocks.insert(joinBlocks.end(), scope.joinBlocks.begin(), scope.joinBlocks.end());
    if (!scope.isTerminator) {
        joinBlocks.emplace_back(scope.curBlock);
    }
}

void CfgBuilder::AddJoinBlock(Scope* scope, bool isElse)
{
    if (scope != nullptr) {
        auto& joinBlocks = isElse ? scope->elseJoinBlocks : scope->joinBlocks;
        joinBlocks.emplace_back(myCurScope->curBlock);
    }
}

void CfgBuilder::AddJoinedEdges(uint32_t toBlock)
{
    for (auto& joinBlock : myCurScope->joinBlocks) {
        AddEdge(joinBlock, toBlock);
    }
    myCurScope->joinBlocks.clear();
}

void CfgBuilder::MergeElseBlocks()
{
    auto& blocks = myCurScope->elseJoinBlocks;
    myCurScope->joinBlocks.insert(myCurScope->joinBlocks.end(), blocks.begin(), blocks.end());
    blocks.clear();
}

void CfgBuilder::AddEdge(uint32_t fromBlock, uint32_t toBlock)
{
    myBlocks[fromBlock].mySuccessors.emplace_back(toBlock);
    myBlocks[toBlock].myPredecessors.emplace_back(fromBlock);
}

void CfgBuilder::AddNewBlock()
{
    myCurScope->curBlock = myBlocks.size();
    CfgBlock block(myCurScope->curBlock, {}, {}, {}, nullptr, nullptr, nullptr, false, &myCfg);
    myBlocks.emplace_back(std::move(block));

    AddJoinedEdges(myCurScope->curBlock);
    myCurScope->isTerminator = false;
}

void CfgBuilder::AddLinkedBlock()
{
    bool isBeforeTerminated = myCurScope->isTerminator;
    AddNewBlock();
    if (!isBeforeTerminated) {
        AddEdge(myCurScope->curBlock - 1, myCurScope->curBlock);
    }
}

void CfgBuilder::AddCfgElement(const Node* node)
{
    if (node == nullptr) {
        return;
    }
    if (auto tempNode = Node::Cast<TemporaryExpression>(node); tempNode) {
        return AddCfgElement(tempNode->GetSubExpr());
    }
    node->Traverse([&](const Node& it) {
        auto condExpr = Node::Cast<ConditionalExpression>(&it);
        if (&it != node && condExpr != nullptr) {
            AddConditionalExpression(*condExpr);
        }
    });
    if (myCurScope->isTerminator) {
        AddNewBlock();
    }
    CfgElement cfgElem(CfgElement::Kind::STATEMENT, node);
    myBlocks[myCurScope->curBlock].myElements.emplace_back(std::move(cfgElem));
}

void CfgBuilder::AddTerminator(const Node* statement, const Node* condition, bool addCondition)
{
    if (addCondition) {
        AddCondition(statement, condition);
    } else if (myCurScope->isTerminator) {
        AddNewBlock();
    }
    myCurScope->isTerminator = true;
    myBlocks[myCurScope->curBlock].myTerminatorStatement = statement;
    myBlocks[myCurScope->curBlock].myTerminatorCondition = condition;
}

void CfgBuilder::AddCondition(const Node* statement, const Node* condition)
{
    auto binExpr = condition != nullptr ? Node::Cast<BinaryExpression>(condition->GetInnerNode()) : nullptr;
    if (binExpr == nullptr || !binExpr->IsLogicalAndOrOp()) {
        AddTerminator(statement, condition);
        AddJoinBlock(myCurScope, false);
        AddJoinBlock(myCurScope, true);
        return;
    }
    if (binExpr->GetOperation() == BinaryExpression::Operation::L_AND) {
        AddCondition(binExpr, binExpr->GetLeftOperand());
        AddCondition(binExpr, binExpr->GetRightOperand());
        return;
    }
    std::vector<uint32_t> savedElseBlocks;
    std::swap(savedElseBlocks, myCurScope->elseJoinBlocks);
    AddCondition(binExpr, binExpr->GetLeftOperand());

    std::vector<uint32_t> savedBlocks;
    std::swap(savedBlocks, myCurScope->joinBlocks);
    std::swap(myCurScope->joinBlocks, myCurScope->elseJoinBlocks);
    myBlocksToReverse.emplace_back(myCurScope->curBlock);
    AddCondition(binExpr, binExpr->GetRightOperand());

    myCurScope->joinBlocks.insert(myCurScope->joinBlocks.end(), savedBlocks.begin(), savedBlocks.end());
    myCurScope->elseJoinBlocks.insert(myCurScope->elseJoinBlocks.end(), savedElseBlocks.begin(), savedElseBlocks.end());
}

void CfgBuilder::AddLabel(const Node* label)
{
    AddLinkedBlock();
    myBlocks[myCurScope->curBlock].myLabelStatement = label;
}

void CfgBuilder::AddStatement(const Node* node)
{
    if (node == nullptr) {
        return AddNewBlock();
    }
#define STMT_CASE(TKind, TType)                       \
    case Node::Kind::TKind:                           \
        Add##TType(*static_cast<const TType*>(node)); \
        break

    switch (node->GetKind()) {
        STMT_CASE(COMPOUND_STATEMENT, CompoundStatement);
        STMT_CASE(IF_STATEMENT, IfStatement);
        STMT_CASE(LOOP_STATEMENT, LoopStatement);
        STMT_CASE(TRY_EXPRESSION, TryExpression);
        STMT_CASE(SWITCH_STATEMENT, SwitchStatement);
        STMT_CASE(CASE_STATEMENT, SwitchCaseStatement);
        STMT_CASE(DEFAULT_STATEMENT, SwitchCaseStatement);
        STMT_CASE(LABEL_STATEMENT, LabelStatement);
        STMT_CASE(GOTO_STATEMENT, GotoStatement);
        STMT_CASE(BREAK_STATEMENT, BreakStatement);
        STMT_CASE(CONTINUE_STATEMENT, ContinueStatement);
        STMT_CASE(RETURN_STATEMENT, ReturnStatement);
        STMT_CASE(CONDITIONAL_EXPRESSION, ConditionalExpression);
        default:
            AddCfgElement(node);
    }
}

void CfgBuilder::AddCompoundStatement(const CompoundStatement& stmt)
{
    auto children = stmt.GetChildren();
    if (children.empty()) {
        return AddNewBlock();
    }
    for (auto& it : children) {
        AddStatement(it);
    }
}

void CfgBuilder::AddIfStatement(const IfStatement& stmt)
{
    if (stmt.GetInitPart() != nullptr) {
        AddStatement(stmt.GetInitPart());
    }
    AddTerminator(&stmt, stmt.GetCondition(), true);
    AddScope(Scope::DEFAULT, [&]() { AddStatement(stmt.GetThenPart()); });
    if (stmt.GetElsePart() != nullptr) {
        AddScope(Scope::ELSE_PART, [&]() { AddStatement(stmt.GetElsePart()); });
    }
    MergeElseBlocks();
}

void CfgBuilder::AddReturnStatement(const ReturnStatement& stmt)
{
    AddCfgElement(&stmt);
    AddJoinBlock(myCurScope->FindScope(Scope::FUNCTION));
    AddTerminator();
}

void CfgBuilder::AddLoopStatement(const LoopStatement& stmt)
{
    AddCfgElement(stmt.GetInit());
    AddLinkedBlock();  // add entry block
    auto entryBlock = myCurScope->curBlock;

    if (stmt.GetLoopKind() == LoopStatement::LoopKind::DO_WHILE) {
        AddScope(Scope::LOOP, [&]() {
            AddScope(Scope::DEFAULT, [&]() { AddStatement(stmt.GetBody()); });
            AddNewBlock();  // add loop exit block
            AddTerminator(&stmt, stmt.GetCondition(), true);
            AddJoinedEdges(entryBlock);  // add loop edges
            MergeElseBlocks();
        });

    } else {
        AddTerminator(&stmt, stmt.GetCondition(), true);
        AddScope(Scope::LOOP, [&]() {
            AddScope(Scope::DEFAULT, [&]() { AddStatement(stmt.GetBody()); });
            AddNewBlock();  // add loop exit block
            AddCfgElement(stmt.GetIncrement());
            AddTerminator();
            AddEdge(myCurScope->curBlock, entryBlock);  // add loop edge
        });
        MergeElseBlocks();
    }
}

void CfgBuilder::AddLabelStatement(const LabelStatement& stmt)
{
    AddLabel(&stmt);
    AddStatement(stmt.GetSubStatement());
    auto& block = myLabeledBlocks[stmt.GetName()];
    block.block = myCurScope->curBlock;
    for (auto& gotoBlock : block.gotoBlocks) {
        AddEdge(gotoBlock, myCurScope->curBlock);
    }
}

void CfgBuilder::AddGotoStatement(const GotoStatement& stmt)
{
    AddTerminator(&stmt);
    auto& block = myLabeledBlocks[stmt.GetLabel()->GetName()];
    if (block.block == 0) {
        block.gotoBlocks.emplace_back(myCurScope->curBlock);
    } else {
        AddEdge(myCurScope->curBlock, block.block);
    }
}

void CfgBuilder::AddSwitchStatement(const SwitchStatement& stmt)
{
    AddTerminator(&stmt, stmt.GetCondition());
    AddScope(Scope::SWITCH, [&]() { AddStatement(stmt.GetBody()); });

    // move 'default' successor to the end of the list
    auto& successors = myBlocks[myCurScope->curBlock].mySuccessors;
    auto it = std::find_if(successors.begin(), successors.end(), [&](size_t i) {
        return Node::Cast<DefaultStatement>(myBlocks[i].myLabelStatement) != nullptr;
    });
    if (it != successors.end()) {
        std::swap(*it, successors.back());
    }
}

void CfgBuilder::AddSwitchCaseStatement(const SwitchCaseStatement& stmt)
{
    AddLabel(&stmt);
    AddEdge(myCurScope->parent->curBlock, myCurScope->curBlock);
    if (stmt.GetBody() != nullptr) {
        AddStatement(stmt.GetBody());
    }
}

void CfgBuilder::AddBreakStatement(const BreakStatement& stmt)
{
    AddCfgElement(&stmt);
    auto scope = myCurScope->FindScope(Scope::SWITCH | Scope::LOOP);
    if (scope != nullptr) {
        AddJoinBlock(scope->parent);
    }
    AddTerminator(&stmt);
}

void CfgBuilder::AddContinueStatement(const ContinueStatement& stmt)
{
    AddCfgElement(&stmt);
    AddJoinBlock(myCurScope->FindScope(Scope::LOOP));
    AddTerminator(&stmt);
}

void CfgBuilder::AddConditionalExpression(const ConditionalExpression& stmt)
{
    AddTerminator(&stmt, stmt.GetCondition(), true);
    AddScope(Scope::DEFAULT, [&]() { AddStatement(stmt.GetTrueExpression()); });
    AddScope(Scope::ELSE_PART, [&]() { AddStatement(stmt.GetFalseExpression()); });
}

void CfgBuilder::AddTryExpression(const TryExpression& stmt)
{
    AddStatement(stmt.GetBody());
    for (auto& handler : stmt.GetHandlers()) {
        AddStatement(handler);
    }
}

}  // namespace HCXX
