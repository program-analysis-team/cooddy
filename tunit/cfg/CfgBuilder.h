/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CFG_CFGBUILDER_H_
#define COODDY_ANALYZER_SOURCE_CFG_CFGBUILDER_H_

#include <ast/BinaryExpression.h>
#include <ast/BreakStatement.h>
#include <ast/CompoundStatement.h>
#include <ast/ConditionalExpression.h>
#include <ast/ContinueStatement.h>
#include <ast/GotoStatement.h>
#include <ast/IfStatement.h>
#include <ast/LabelStatement.h>
#include <ast/LoopStatement.h>
#include <ast/ReturnStatement.h>
#include <ast/SwitchCaseStatement.h>
#include <ast/SwitchStatement.h>
#include <ast/TemporaryExpression.h>
#include <ast/TryExpression.h>
#include <cfg/Cfg.h>

namespace HCXX {

class CfgBuilder {
public:
    explicit CfgBuilder(Cfg& cfg) : myCfg(cfg), myBlocks(cfg.myBlocks) {}
    ~CfgBuilder() = default;

    void Build();

private:
    struct Scope {
        enum Kind { DEFAULT = 0, FUNCTION = 1, LOOP = 2, SWITCH = 4, ELSE_PART = 8 };

        Kind kind = Kind::DEFAULT;
        Scope* parent = nullptr;
        uint32_t curBlock = 0;
        bool isTerminator = true;
        std::vector<uint32_t> joinBlocks;
        std::vector<uint32_t> elseJoinBlocks;

        Scope* FindScope(uint32_t kindFlags)
        {
            return (kind & kindFlags) ? this : (parent != nullptr ? parent->FindScope(kindFlags) : nullptr);
        }
    };

    struct LabeledBlock {
        uint32_t block = 0;
        std::vector<uint32_t> gotoBlocks;
    };

    void EnterScope(Scope& scope);
    void LeaveScope(Scope& scope);
    void AddScope(Scope::Kind kind, const std::function<void()>& scopeBuilder);
    void AddJoinBlock(Scope* scope, bool isElse = false);
    void AddJoinedEdges(uint32_t toBlock);
    void MergeElseBlocks();
    void AddNewBlock();
    void AddLinkedBlock();
    void AddStatement(const Node* node);
    void AddCfgElement(const Node* node);
    void AddEdge(uint32_t fromBlock, uint32_t toBlock);
    void AddTerminator(const Node* statement = nullptr, const Node* condition = nullptr, bool addCondition = false);
    void AddCondition(const Node* statement, const Node* condition);
    void AddLabel(const Node* label);

    void AddCompoundStatement(const CompoundStatement& stmt);
    void AddIfStatement(const IfStatement& stmt);
    void AddLoopStatement(const LoopStatement& stmt);
    void AddTryExpression(const TryExpression& stmt);
    void AddSwitchStatement(const SwitchStatement& stmt);
    void AddSwitchCaseStatement(const SwitchCaseStatement& stmt);
    void AddLabelStatement(const LabelStatement& stmt);
    void AddGotoStatement(const GotoStatement& stmt);
    void AddBreakStatement(const BreakStatement& stmt);
    void AddContinueStatement(const ContinueStatement& stmt);
    void AddReturnStatement(const ReturnStatement& stmt);
    void AddConditionalExpression(const ConditionalExpression& stmt);

private:
    Cfg& myCfg;
    std::vector<CfgBlock>& myBlocks;
    Scope* myCurScope = nullptr;
    std::unordered_map<std::string, LabeledBlock> myLabeledBlocks;
    std::vector<uint32_t> myBlocksToReverse;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_SOURCE_CFG_CFGBUILDER_H_
