/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class WrongStringIncrementChecker : public DfaChecker {
public:
    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        auto unary = state.GetNodeAs<UnaryExpression>();
        if (unary == nullptr || !IsIncrementOp(*unary) || !IsCharPointer(*unary) || unary->InMacro()) {
            return;
        }

        auto loopRange = state.GetFuncState().GetCfgBlock().FindNearestLoop();
        auto& block = state.GetFuncState().GetCfgBlock();
        auto& els = block.GetElements();
        if (loopRange == std::make_pair<uint32_t, uint32_t>(0, 0) || block.GetPredecessors().empty() ||
            block.GetCfg() == nullptr || els.empty()) {
            return;
        }

        auto operandTyped = Node::Cast<TypedNode>(unary->GetOperand());
        auto operandDecl = operandTyped != nullptr ? operandTyped->GetDeclaration() : nullptr;
        if (operandDecl == nullptr) {
            return;  // LCOV_EXCL_LINE: HCAT-2779
        }

        TraverseCallback checkUnaryIncIsTheSame = [&](const Node& node) {
            if (&node != unary && IsIncrementOfSameDecl(operandDecl, node)) {
                TracePath problemTraces;
                problemTraces.emplace_back(node.GetTranslationUnit(), node.GetRange(),
                                           StrLocales::GetStringLocale("FIRST_INC"));
                problemTraces.emplace_back(node.GetTranslationUnit(), unary->GetRange(),
                                           StrLocales::GetStringLocale("STRING.OVERFLOW_DESCRIPTION"));
                state.RegisterProblem(holder, *this, *unary, {{}, problemTraces});
            }
        };

        for (auto& predPos : block.GetPredecessors()) {
            // check that pos is correct and inside same loop
            if (block.GetCfg()->GetBlocks().size() < predPos ||
                (predPos < loopRange.first || predPos > loopRange.second)) {
                return;  // LCOV_EXCL_LINE
            }
            auto& pred = block.GetCfg()->GetBlocks()[predPos];
            if (!pred.GetElements().empty()) {
                pred.GetElements().back().GetNode()->Traverse(checkUnaryIncIsTheSame);
            }
            // check that terminator condition has increment of same value
            auto terCond = pred.GetTerminatorCondition();
            if (terCond != nullptr) {
                // TODO: somehow add check that reference was checked on '\0' in condition
                terCond->Traverse(checkUnaryIncIsTheSame);
            }
        }
    }

private:
    const uint8_t charSize = 8;
    bool IsIncrementOp(const UnaryExpression& unary)
    {
        return unary.GetOperation() == HCXX::UnaryExpression::Operation::PRE_INC ||
               unary.GetOperation() == HCXX::UnaryExpression::Operation::POST_INC;
    }

    bool IsIncrementOfSameDecl(const Node* leftDecl, const Node& right)
    {
        auto rightUnary = Node::Cast<UnaryExpression>(&right);
        if (rightUnary == nullptr || !IsIncrementOp(*rightUnary)) {
            return false;
        }
        auto operandTyped = Node::Cast<TypedNode>(rightUnary->GetOperand());
        return operandTyped != nullptr && operandTyped->GetDeclaration() == leftDecl;
    }

    bool IsCharPointer(const Node& node)
    {
        return node.GetType().IsPointer() && node.GetType().GetPointedType().GetSizeInBits() == charSize;
    }
};

std::unique_ptr<HCXX::Checker> CreateWrongStringIncrementChecker()
{
    return std::make_unique<WrongStringIncrementChecker>();
}
