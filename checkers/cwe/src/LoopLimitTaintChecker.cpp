/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/LoopStatement.h>
#include <ast/MemberExpression.h>
#include <dfa/TaintedChecker.h>

using namespace HCXX;

class LoopLimitTaintChecker : public TaintedChecker {
    Annotation::Kind myLoopConditionKind;

public:
    LoopLimitTaintChecker()
    {
        myLoopConditionKind =
            Annotation::RegisterKind("LoopCondition", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
    }

    void CheckTerminator(const Node& terminator, DfaFunctionState& state, ProblemsHolder& holder) override
    {
        auto& cfgBlock = state.GetCfgBlock();
        auto terminatorCondition = cfgBlock.GetTerminatorCondition();
        if (terminatorCondition == nullptr) {
            return;  // LCOV_EXCL_LINE: HCAT-2779
        }
        auto terminatorStatement = Node::Cast<LoopStatement>(cfgBlock.GetTerminatorStatement());
        if (terminatorStatement == nullptr) {
            return;
        }
        AnnotateVars(state, *terminatorCondition, cfgBlock.GetCfg(), cfgBlock.FindNearestLoop());
    }

private:
    void AnnotateVars(DfaFunctionState& state, const Node& checkedNode, const Cfg* cfg, CfgBlock::Range loopRange)
    {
        std::function<std::vector<const Node*>(const Node& node)> traverseCallback = [&](const Node& node) {
            std::vector<const Node*> result;
            if (node.IsKindOf(Node::Kind::CALL_EXPRESSION) || node.IsKindOf(Node::Kind::ARRAY_SUBSCRIPT_EXPRESSION) ||
                node.IsKindOf(Node::Kind::CONDITIONAL_EXPRESSION)) {
                return result;
            }

            if (auto binExpr = Node::Cast<BinaryExpression>(&node); binExpr != nullptr) {
                auto left = traverseCallback(*binExpr->GetLeftOperand());
                auto right = traverseCallback(*binExpr->GetRightOperand());
                // FP a&&b. Do not propagate LoopCondition annotation if untrusted data affects only on one part
                // of the expression. Other cases which lead to FP are not found in test projects code.
                if (binExpr->GetOperation() != BinaryExpression::Operation::L_AND ||
                    (left.size() > 0 && right.size() > 0)) {
                    result.insert(result.end(), left.begin(), left.end());
                    result.insert(result.end(), right.begin(), right.end());
                }
                return result;
            }
            auto memberExpr = Node::Cast<MemberExpression>(&node);
            if (memberExpr != nullptr) {
                result.emplace_back(memberExpr);
                return result;
            }

            auto ref = Node::Cast<RefExpression>(&node);
            if (ref == nullptr) {
                std::vector<const Node*> allNodes;
                node.TraverseChildren([&](const Node& node) {
                    auto nodes = traverseCallback(node);
                    allNodes.insert(allNodes.end(), nodes.begin(), nodes.end());
                });
                return allNodes;
            }

            auto decl = ref->GetDeclaration();
            if (decl != nullptr && !cfg->IsNodeModifiedInRange(decl, loopRange)) {
                result.emplace_back(ref);
            }
            return result;
        };
        auto states = traverseCallback(checkedNode);
        for (auto s : states) {
            state.GetState(s).Annotate(myLoopConditionKind);
        }
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation(myLoopConditionKind) && state.HasAnnotation(myUntrustedSourceKind)) {
            state.AddSuspiciousPath({*this, myLoopConditionKind, myUntrustedSourceKind});
        }
    }

    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        auto& solverCtx = sinkInfo.context;
        solverCtx.AddCondition(sinkInfo.exprId, {Condition::Operation::GE, INT32_MAX});
        return true;
    }
};

std::unique_ptr<HCXX::Checker> CreateLoopLimitTaintChecker()
{
    return std::make_unique<LoopLimitTaintChecker>();
}
