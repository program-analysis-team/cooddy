/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of the simple checker which search for "division by zero" problems.
//

#include "Checker.h"
#include "ast/BinaryExpression.h"
#include "ast/CallExpression.h"
#include "ast/IntLiteralExpression.h"

using namespace HCXX;

class SimpleChecker : public Checker {
public:
    void CheckNode(const Node& node, ProblemsHolder& holder) override
    {
        if (node.GetKind() == Node::Kind::FUNCTION_DECL) {
            auto* function = Node::Cast<FunctionDecl>(&node);
            if (function != nullptr && function->GetName() == "main") {
                holder.RegisterProblem(*this, node, {{"Main declaration"}});
            }
        }

        auto* callExpr = Node::Cast<CallExpression>(&node);
        if (callExpr != nullptr) {
            auto* function = callExpr->GetFunction();
            if (function != nullptr && function->GetName() == "external_call" && function->GetBody() != nullptr) {
                holder.RegisterProblem(*this, *function->GetBody());
            } else if (function != nullptr && function->GetName() == "buildin_error") {
                holder.RegisterProblem(*this, node, {{"Buildin includes error"}});
            }
        }

        if (node.GetKind() == Node::Kind::BINARY_EXPRESSION) {
            auto* expression = Node::Cast<BinaryExpression>(&node);
            if (expression != nullptr && expression->GetOperation() == BinaryExpression::Operation::DIV) {
                if (const IntLiteralExpression* intLitExpr =
                        Node::Cast<IntLiteralExpression>(expression->GetRightOperand());
                    intLitExpr != nullptr && intLitExpr->GetSLimitedValue() == 0) {
                    holder.RegisterProblem(*this, node, {{"Division by zero"}});
                }
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateSimpleChecker()
{
    return std::make_unique<SimpleChecker>();
}
