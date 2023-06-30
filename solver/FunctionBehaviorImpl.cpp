/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "FunctionBehaviorImpl.h"

#include <ast/CxxMethodDecl.h>
#include <ast/CxxRecordDecl.h>
#include <ast/EnumDecl.h>
#include <ast/FunctionDecl.h>
#include <ast/UnaryExpression.h>
#include <utils/Log.h>

#include "GetNameContext.h"
#include "PathChecker.h"
#include "SymbolsContext.h"

FunctionBehaviorImpl::FunctionBehaviorImpl(const Cfg& cfg)
{
    mySourceRange = cfg.GetFunction().GetRange();
    myIntructionsSet.GetData(ExecutionContext::THIS_INSTRUCTION);

    auto& blocks = cfg.GetBlocks();
    InstructionProcessor::CompileContext context{*this, myIntructionsSet};

    auto cxxMethod = Node::Cast<CxxMethodDecl>(&cfg.GetFunction());
    if (cxxMethod != nullptr && !cxxMethod->IsStatic()) {
        context.Compile(nullptr);
        myArgTypes.emplace_back(ArgType{1});
    }
    auto& params = cfg.GetFunction().GetParams();
    for (int i = 0; i < params.size(); ++i) {
        context.Compile(params[i]);
        myArgTypes.emplace_back(MakeArgType(params[i]->GetType()));
    }
    myResultType = MakeArgType(cfg.GetFunction().GetRetType());

    // add blocks to instruction set in the reverse order
    // the first block will be added first
    for (int i = static_cast<int>(blocks.size()) - 1; i >= 0; i--) {
        myBlocks.emplace_front(*this, i, blocks[i]);
    }
}

FunctionBehaviorImpl::FunctionBehaviorImpl(DataStream& stream)
{
    mySourceRange = stream.Get<SourceRange>();
    uint32_t instrSetSize = stream.Get<uint32_t>();
    stream.GetBuffer(myIntructionsSet.GetData(instrSetSize), instrSetSize);

    uint32_t blocksCount = stream.Get<uint32_t>();
    for (auto i = 0; i < blocksCount; ++i) {
        myBlocks.emplace_back(*this, i, stream);
    }
}

void FunctionBehaviorImpl::StoreToStream(DataStream& stream)
{
    stream.Add<SourceRange>(mySourceRange);
    stream.Add<uint32_t>(myIntructionsSet.Count());
    stream.AddBuffer(myIntructionsSet.GetData(), myIntructionsSet.Count());
    stream.Add<uint32_t>(myBlocks.size());
    for (auto& block : myBlocks) {
        block.StoreToStream(stream);
    }
}

SourceRange FunctionBehaviorImpl::GetSourceRange(Instruction instruction)
{
    auto header = GetInstructionHeader(instruction);
    if (!header) {
        return {};
    }
    auto rangeBegin = mySourceRange.begin + header->rangeOffset;
    if (header->rangeOffset == UINT16_MAX) {
        rangeBegin = myIntructionsSet.Get<uint32_t>(&instruction);
    }
    return {rangeBegin, rangeBegin + header->rangeSize};
}

Node::Kind FunctionBehaviorImpl::GetInstructionKind(Instruction instruction)
{
    auto header = GetInstructionHeader(instruction);
    return header ? static_cast<Node::Kind>(header->kind) : Node::Kind::NODE;
}

std::unique_ptr<SolverContext> FunctionBehaviorImpl::StartExecution()
{
    return std::make_unique<PathChecker>(this, true);
}

void FunctionBehaviorImpl::ProcessBlockSuccessors(ExecutionContext& execCtx, const BasicBlock& block,
                                                  std::vector<SymbolsContext::Ptr>& symbolsContext,
                                                  z3::expr& terminatorExpr)
{
    z3::expr joinedCondition = execCtx.CreateBoolExpr(false);
    uint32_t numSuccessors = std::min(block.GetSuccessorsCount(), MAX_SUCCESSORS_COUNT);
    for (uint32_t i = 0; i < numSuccessors; ++i) {
        uint32_t successor = block.GetSuccessor(i);
        if (successor > block.GetPosition()) {
            if (!execCtx.GetSuccessorForLastBlockInLoop(successor)) {
                continue;  // LCOV_EXCL_LINE
            }
        }
        z3::expr joinCondition =
            execCtx.GetJoinCondition(block, myBlocks[successor], i, terminatorExpr, joinedCondition);
        GetSymbolsContext(symbolsContext, successor, execCtx)
            ->Join(GetSymbolsContext(symbolsContext, block.GetPosition(), execCtx), joinCondition);
    }
}

void FunctionBehaviorImpl::Execute(SolverContext& context, ExecutionCallbackRef callback)
{
    if (myBlocks.empty()) {
        return;  // LCOV_EXCL_LINE
    }

    auto& execCtx = static_cast<ExecutionContext&>(context);
    std::vector<SymbolsContext::Ptr> symbolsContext(myBlocks.size());

    auto& entrySymbols = GetSymbolsContext(symbolsContext, myBlocks.size() - 1, execCtx);
    execCtx.EnterFunction(*this, entrySymbols, myBlocks.back().GetPosition());

    auto exitSymbols = GetSymbolsContext(symbolsContext, 0, execCtx);

    auto block = execCtx.GetNextBlockForExecution();
    // For top-level (entrypoint) functions, execute pseudo-instructions for parameters (not arguments, since
    // there are no arguments for a top-level function).
    if (execCtx.GetCallStackSize() == 0) {
        InstructionProcessor::ExecutionContext ctx{myIntructionsSet, ExecutionContext::THIS_INSTRUCTION, execCtx,
                                                   callback};
        for (auto& it : myArgTypes) {
            (void)it;
            ctx.Execute();
        }
    }

    for (; block != nullptr; block = execCtx.GetNextBlockForExecution()) {
        auto& curSymbols = GetSymbolsContext(symbolsContext, block->GetPosition(), execCtx);
        try {
            auto terminatorExpr = execCtx.Execute(*block, curSymbols, callback);
            ProcessBlockSuccessors(execCtx, *block, symbolsContext, terminatorExpr);
            curSymbols.reset();
        } catch (const ExecutionException& ex) {
            execCtx.AdjustCallStack();
            if (ex.result == ExecutionResult::CANCEL) {
                break;
            }
        }
    }
    execCtx.LeaveFunction(std::move(exitSymbols));
}

void FunctionBehaviorImpl::CleanUpMapping()
{
    myInstructionsMapping = NodeToInstructionMap();
}

const std::vector<ArgType>& FunctionBehaviorImpl::GetArgTypes() const
{
    return myArgTypes;
}

ArgType FunctionBehaviorImpl::GetArgType(uint32_t argPos) const
{
    return argPos >= myArgTypes.size() ? myResultType : myArgTypes[argPos];
}

static inline bool IsAddrOf(const Node& node)
{
    auto unaryExpr = Node::Cast<UnaryExpression>(node.GetInnerNode());
    return unaryExpr != nullptr && unaryExpr->GetOperation() == UnaryExpression::Operation::ADDR_OF;
}

ArgType FunctionBehaviorImpl::MakeArgType(const Type& type, const Node* argNode)
{
    ArgType result{type.IsPointer(), type.IsReference(), 0, 0, 0, {0, 0}};
    if (type.IsPointer()) {
        result.isConst = type.GetPointedType().IsConstant() || type.IsRvalueReferenceType();
        result.isBuffer = !type.IsDeclaration() && type.GetPointedType().GetSizeInBits() == 8;
    }
    result.isIntegral = type.IsIntegralType() && !type.IsDeclaration();

    if (type.IsTrivial() || !result.isConst && argNode != nullptr && IsAddrOf(*argNode)) {
        Type varType = type.IsPointer() ? type.GetPointedType() : type;
        result.intInfo.isUnsigned = varType.IsUnsigned();
        result.intInfo.sizeInBits = varType.IsTrivial() ? varType.GetSizeInBits() : 0;
    }
    if (auto enumDecl = Node::Cast<EnumDecl>(type.GetPointedDeclaration()); enumDecl != nullptr) {
        result.intInfo.isUnsigned = 1;
        result.intInfo.isEnum = 1;
        result.intInfo.sizeInBits = enumDecl->GetMembers().size() - 1;
    }
    return result;
}

std::optional<Instruction> FunctionBehaviorImpl::GetInstructionOffset(Instruction instruction) const
{
    SymbolId::ClearFlags(instruction);
    if (instruction < sizeof(InstructionHeader) || instruction >= myIntructionsSet.Count()) {
        return std::nullopt;
    }
    return instruction - sizeof(InstructionHeader);
}

InstructionHeader* FunctionBehaviorImpl::GetInstructionHeader(Instruction instruction)
{
    auto offset = GetInstructionOffset(instruction);
    return offset ? myIntructionsSet.GetPtr<InstructionHeader>(&offset.value()) : nullptr;
}

std::unique_ptr<FunctionBehavior> FunctionBehavior::Create(const Cfg& cfg)
{
    return std::make_unique<FunctionBehaviorImpl>(cfg);
}

std::unique_ptr<FunctionBehavior> FunctionBehavior::Create(DataStream& stream)
{
    return std::make_unique<FunctionBehaviorImpl>(stream);
}

bool FunctionBehaviorImpl::IsExitSymbol(const SymbolId& symbol) const
{
    auto argPos = GetArgPos(symbol);
    return symbol.IsGlobal() || symbol.GetVarInstr() == ExecutionContext::RET_INSTRUCTION ||
           (argPos < myArgTypes.size() && symbol.IsOutputArg(myArgTypes[argPos]));
}

SymbolsContext::Ptr& FunctionBehaviorImpl::GetSymbolsContext(std::vector<SymbolsContext::Ptr>& symbolsContext,
                                                             uint32_t position, ExecutionContext& execCtx)
{
    if (symbolsContext[position] == nullptr) {
        symbolsContext[position] = std::make_shared<SymbolsContext>(execCtx.CreateBoolExpr(false));
    }
    return symbolsContext[position];
}

std::string FunctionBehaviorImpl::GetName(Instruction instr, GetCalleeNameCb& getCalleeNameCb,
                                          GetSourceInRangeCb& getSourceByRangeCb)
{
    auto offset = GetInstructionOffset(instr);
    if (!offset) {
        return {};  // LCOV_EXCL_LINE
    }

    PathChecker execCtx(this, false);
    std::vector<SymbolsContext::Ptr> symbolsContext(2);
    auto& entrySymbols = GetSymbolsContext(symbolsContext, 1, execCtx);
    execCtx.EnterFunction(*this, entrySymbols, 0);

    InstructionTree instrTree;
    InstructionProcessor::ExecutionContext ctx{myIntructionsSet, *offset, execCtx};
    ExecutionCallback cb = [&]() {
        instrTree.emplace_back(InstructionTreeNode{ctx->GetCurInstruction(), ctx.curDepth});
        return ExecutionResult::OK;
    };
    ctx.callback = cb;

    ctx.Execute();
    auto& exitSymbols = GetSymbolsContext(symbolsContext, 0, execCtx);
    execCtx.LeaveFunction(exitSymbols);

    std::reverse(instrTree.begin(), instrTree.end());
    GetNameContext nameCtx(instrTree, *this, getCalleeNameCb, getSourceByRangeCb);
    return nameCtx.GetName();
}
