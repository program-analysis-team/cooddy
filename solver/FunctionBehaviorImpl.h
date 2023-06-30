/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_FUNCTIONBEHAVIORIMPL_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_FUNCTIONBEHAVIORIMPL_H_

#include <deque>
#include <unordered_map>

#include "BasicBlock.h"
#include "InstructionProcessor.h"
#include "SymbolsContext.h"
#include "utils/DataStream.h"

using NodeToInstructionMap = std::unordered_map<const HCXX::Node*, uint32_t>;

class FunctionBehaviorImpl : public FunctionBehavior {
public:
    explicit FunctionBehaviorImpl(const Cfg& cfg);
    explicit FunctionBehaviorImpl(DataStream& stream);
    ~FunctionBehaviorImpl() = default;

    void StoreToStream(DataStream& stream) override;

    Instruction GetInstruction(const HCXX::Node& node) override
    {
        auto it = myInstructionsMapping.find(&node);
        return it != myInstructionsMapping.end() ? it->second : 0;
    }

    Instruction AddInstruction(Instruction instruction, const HCXX::Node& node, bool replaceExisting = false)
    {
        auto it = myInstructionsMapping.insert(std::make_pair(&node, instruction));
        if (replaceExisting) {
            it.first->second = instruction;
        }
        return it.second ? 0 : it.first->second;
    }

    SourceRange GetSourceRange(Instruction instruction) override;

    Node::Kind GetInstructionKind(Instruction instruction) override;

    DataStream& GetInstructionsSet()
    {
        // COODDY_SUPPRESS ReturnMemberAddressChecker
        return myIntructionsSet;
    }

    SourceRange GetFunctionRange() const override
    {
        return mySourceRange;
    }

    std::unique_ptr<SolverContext> StartExecution() override;

    void Execute(SolverContext& context, ExecutionCallbackRef callback) override;

    void CleanUpMapping() override;

    const std::vector<ArgType>& GetArgTypes() const;

    ArgType GetArgType(uint32_t argPos) const;

    static ArgType MakeArgType(const Type& type, const Node* argNode = nullptr);

    static Instruction GetThisInstruction()
    {
        return ExecutionContext::THIS_INSTRUCTION + sizeof(InstructionHeader);
    }

    static SymbolId GetArgSymbolId(StackPos stackPos, uint32_t argPos)
    {
        return SymbolId(stackPos, GetThisInstruction() + sizeof(InstructionHeader) * argPos);
    }

    static uint32_t GetArgPos(const SymbolId& symbolId)
    {
        return (symbolId.GetVarInstr() - ExecutionContext::THIS_INSTRUCTION) / sizeof(InstructionHeader) - 1;
    }

    bool IsExitSymbol(const SymbolId& symbol) const;

    const std::deque<BasicBlock>& GetBlocks() const
    {
        return myBlocks;
    }

    std::optional<Instruction> GetInstructionOffset(Instruction instruction) const;

    InstructionHeader* GetInstructionHeader(Instruction instruction);

    uint32_t GetMaxInstruction() override
    {
        return myIntructionsSet.Count();
    }

    std::string GetName(Instruction instr, GetCalleeNameCb& getCalleeNameCb,
                        GetSourceInRangeCb& getSourceInRangeCb) override;

private:
    void ProcessBlockSuccessors(ExecutionContext& execCtx, const BasicBlock& block,
                                std::vector<SymbolsContext::Ptr>& symbolsContext, z3::expr& terminatorExpr);
    static SymbolsContext::Ptr& GetSymbolsContext(std::vector<SymbolsContext::Ptr>& symbolsContext, uint32_t position,
                                                  ExecutionContext& execCtx);

    static constexpr uint32_t MAX_SUCCESSORS_COUNT = 1024;

    std::deque<BasicBlock> myBlocks;
    NodeToInstructionMap myInstructionsMapping;
    DataStream myIntructionsSet;
    SourceRange mySourceRange;
    std::vector<ArgType> myArgTypes;
    ArgType myResultType;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_FUNCTIONBEHAVIORIMPL_H_
