/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of the pure checker to find various uncovered cases
//

#include "Checker.h"
#include "ast/ParenExpression.h"

using namespace HCXX;

class UncoveredEmptyChecker : public Checker {
public:
    void CheckNode(const Node& node, ProblemsHolder& holder) override
    {
        //  The class is outdated
        //  Please, try to implement your test based on HAST first(Examples are available in the
        //  test/test_cases/ParserTest/) The class may be used for tests only if your test requires functionality which
        //  isn't supported by HAST at the moment

        if (node.GetKind() == Node::Kind::PAREN_EXPRESSION) {
            const Node* subexpr = Node::Cast<ParenExpression>(&node)->GetSubExpr();
            if (subexpr == Node::Cast<ParenExpression>(&node)->GetNonParenSubExpr() &&
                node.GetParent()->GetKind() != Node::Kind::UNARY_EXPRESSION) {
                holder.RegisterProblem(*this, node);
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateUncoveredEmptyChecker()
{
    return std::make_unique<UncoveredEmptyChecker>();
}
