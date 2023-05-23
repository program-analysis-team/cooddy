/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include "BasicBlock.h"

#include <ast/BinaryExpression.h>
#include <ast/BreakStatement.h>
#include <ast/ConditionalExpression.h>
#include <ast/LoopStatement.h>
#include <ast/SwitchCaseStatement.h>
#include <ast/UnaryExpression.h>
#include <utils/Log.h>

#include "FunctionBehaviorImpl.h"

BasicBlock::BasicBlock(FunctionBehaviorImpl& owner, uint32_t position, const CfgBlock& block)
    : myOwner(&owner),
      myPosition(position),
      myIsEndlessLoop(block.IsEndlessLoop()),
      myIsDefaultStatement(0),
      myIsLoopEntrance(0),
      myIsDoWhileLoop(0),
      myIsLoopExit(0),
      myIsInLoop(0)
{
    mySuccessorsCount = block.GetSuccessors().size();
    auto& instrSet = myOwner->GetInstructionsSet();

    InstructionProcessor::CompileContext context{*myOwner, instrSet};
    myLabelInstructions = instrSet.Count();
    auto caseStatement = Node::Cast<SwitchCaseStatement>(block.GetLabelStatement());
    if (caseStatement != nullptr) {
        myIsDefaultStatement = caseStatement->IsDefault();
        context.Compile(caseStatement->GetCaseExpr());
    }
    myInstructions = instrSet.Count();
    InitLoop(block);
    if (IsLoopEntrance()) {
        CompileLoopIterators(context, *block.GetCfg());
    }
    auto& elements = block.GetElements();
    for (auto& cfgEl : elements) {
        context.Compile(cfgEl.GetNode());
    }
    if (!elements.empty() || block.GetTerminatorStatement() != nullptr) {
        auto node = !elements.empty() ? elements.back().GetNode() : block.GetTerminatorStatement();
        myIsInLoop = node != nullptr && node->GetParentOfType<LoopStatement>() != nullptr;
    }
    myTerminatorInstructions = instrSet.Count();
    if (mySuccessorsCount > 1) {
        auto& succBlock = block.GetCfg()->GetBlocks()[block.GetSuccessors().front()];
        myIsLoopExit = Node::Cast<BreakStatement>(succBlock.GetTerminatorStatement()) != nullptr;
        context.Compile(block.GetTerminatorCondition());
    }

    mySuccessorsOffset = instrSet.Count();
    for (auto it : block.GetSuccessors()) {
        instrSet.Add(it);
    }
}

BasicBlock::BasicBlock(FunctionBehaviorImpl& owner, uint32_t position, DataStream& stream)
    : myOwner(&owner), myPosition(position)
{
    stream.GetBuffer(&myFirstStoredField, sizeof(*this) - offsetof(BasicBlock, myFirstStoredField));
}

void BasicBlock::StoreToStream(DataStream& stream)
{
    stream.AddBuffer(&myFirstStoredField, sizeof(*this) - offsetof(BasicBlock, myFirstStoredField));
}

const Node* GetUnaryIncrement(const Node& node, int32_t& increment)
{
    auto unaryExpr = Node::Cast<UnaryExpression>(&node);
    auto op = unaryExpr != nullptr ? unaryExpr->GetOperation() : UnaryExpression::Operation::UNKNOWN;
    if (op == UnaryExpression::Operation::PRE_INC || op == UnaryExpression::Operation::POST_INC) {
        increment = 1;
        return unaryExpr->GetOperand();
    }
    if (op == UnaryExpression::Operation::PRE_DEC || op == UnaryExpression::Operation::POST_DEC) {
        increment = -1;
        return unaryExpr->GetOperand();
    }
    return nullptr;
}

const Node* GetBinaryIncrement(const Node& node, int32_t& increment)
{
    auto binaryExpr = Node::Cast<BinaryExpression>(&node);
    if (binaryExpr == nullptr) {
        return nullptr;
    }
    auto rightOperand = Node::Cast<TypedNode>(binaryExpr->GetRightOperand());
    if (rightOperand == nullptr || !rightOperand->IsConstExpr()) {
        return nullptr;
    }
    if (binaryExpr->GetOperation() == BinaryExpression::Operation::ADD_ASSIGN) {
        increment = rightOperand->GetSLimitedValue();
        return binaryExpr->GetLeftOperand();
    }
    if (binaryExpr->GetOperation() == BinaryExpression::Operation::SUB_ASSIGN) {
        increment = -rightOperand->GetSLimitedValue();
        return binaryExpr->GetLeftOperand();
    }
    return 0;
}

using LoopIterMap = std::unordered_map<Instruction, std::pair<int32_t, IntTypeInfo>>;

void CollectLoopIterators(FunctionBehaviorImpl& func, const CfgBlock& block, LoopIterMap& loopIters)
{
    if (Node::Cast<BreakStatement>(block.GetTerminatorStatement()) != nullptr) {
        return;
    }
    for (auto& el : block.GetElements()) {
        el.GetNode()->Traverse([&](const Node& node) {
            auto increment = 0;
            auto incrementNode = GetUnaryIncrement(node, increment);
            if (incrementNode == nullptr) {
                incrementNode = GetBinaryIncrement(node, increment);
            }
            auto refExpr = Node::Cast<RefExpression>(incrementNode);
            if (refExpr == nullptr || increment == 0) {
                return;
            }
            auto decl = refExpr->GetDeclaration();
            auto declInstr = decl != nullptr ? func.GetInstruction(*decl) : 0;
            if (declInstr != 0) {
                auto& iterInfo = loopIters[declInstr];
                if (iterInfo.first == 0) {
                    iterInfo.second = func.MakeArgType(decl->GetType()).intInfo;
                }
                iterInfo.first += increment;
            }
        });
    }
}

void BasicBlock::CompileLoopIterators(InstructionProcessor::CompileContext& context, const Cfg& cfg)
{
    LoopIterMap loopIters;
    auto& blocks = cfg.GetBlocks();

    CollectLoopIterators(context.owner, blocks[myPosition], loopIters);
    CollectLoopIterators(context.owner, blocks[myEndOfLoop], loopIters);
    auto lastLoopBlock = IsDoWhileLoop() ? (myPosition + 1u) : (myEndOfLoop + 1u);
    CollectLoopIterators(context.owner, blocks[lastLoopBlock], loopIters);

    context.Add<uint16_t>(loopIters.size());
    for (auto& [instr, info] : loopIters) {
        context.Add<Instruction>(instr);
        context.Add<int32_t>(info.first);
        context.Add<IntTypeInfo>(info.second);
    }
}

void BasicBlock::ExecuteLoopIterators(InstructionProcessor::ExecutionContext& context) const
{
    auto loopInstr = context.curInstr;
    auto itersCount = context.Get<uint16_t>();
    for (auto i = 0; i < itersCount; ++i) {
        auto instr = context.Get<Instruction>();
        auto increment = context.Get<int32_t>();
        auto typeInfo = context.Get<IntTypeInfo>();
        context->AddLoopIterator(loopInstr, instr, increment, typeInfo);
    }
}

uint32_t BasicBlock::GetSuccessor(uint32_t pos) const
{
    uint32_t successorPos = mySuccessorsOffset + pos * sizeof(uint32_t);
    return myOwner->GetInstructionsSet().Get<uint32_t>(&successorPos);
}

z3::expr BasicBlock::Execute(ExecutionContext& context, ExecutionCallbackRef callback) const
{
    InstructionProcessor::ExecutionContext execCtx{myOwner->GetInstructionsSet(), myInstructions, context, callback};
    if (IsLoopEntrance()) {
        ExecuteLoopIterators(execCtx);
    }

    while (execCtx.curInstr < myTerminatorInstructions) {
        execCtx.Execute();
        execCtx.ExecutePostOperations();
    }
    if (mySuccessorsCount <= 1) {
        // for one successor just return true expression
        return context.CreateBoolExpr(true);
    }
    return execCtx.Execute();
}

z3::expr BasicBlock::GetJoinCondition(ExecutionContext& context, uint32_t branch, z3::expr& terminator,
                                      z3::expr& joinedCondition) const
{
    if (myLabelInstructions == myInstructions || terminator.is_bool()) {
        z3::expr result = context.CastToBool(terminator);
        return branch == 0 ? result : !result;
    }
    if (myIsDefaultStatement) {
        return !joinedCondition;
    }
    InstructionProcessor::ExecutionContext execCtx{myOwner->GetInstructionsSet(), myLabelInstructions, context};
    z3::expr caseExpr = execCtx.Execute();
    z3::expr result = context.CastToBV(caseExpr) == context.CastToBV(terminator);
    joinedCondition = joinedCondition || result;
    return result;
}

void BasicBlock::InitLoop(const CfgBlock& block)
{
    if (mySuccessorsCount <= 1) {
        return;
    }
    auto loopStmt = Node::Cast<LoopStatement>(block.GetTerminatorStatement());
    if (loopStmt != nullptr) {
        myIsDoWhileLoop = loopStmt->GetLoopKind() == LoopStatement::LoopKind::DO_WHILE;
    }
    auto loop = block.FindNearestLoop();

    if (loop.first != 0 && (loop.second == block.GetPos() || myIsDoWhileLoop)) {
        myIsLoopEntrance = 1;
        myEndOfLoop = loop.first;
    }
}
