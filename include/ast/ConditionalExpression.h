/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CONDITIONALEXPRESSION_H
#define COODDY_CONDITIONALEXPRESSION_H

#include <ast/Node.h>

namespace HCXX {

// Node for ternary operator "<cond> ? <expr> : <expr>" ("<cond> ?: <expr>")
class ConditionalExpression : public Node {
public:
    ConditionalExpression(Node* condExpr, Node* trueExpr, Node* falseExpr)
        : myCondExpr(condExpr), myTrueExpr(trueExpr), myFalseExpr(falseExpr)
    {}

    DECLARE_KIND(Node, Node::Kind::CONDITIONAL_EXPRESSION);
    DECLARE_SERIALIZE(ConditionalExpression, myCondExpr << myTrueExpr << myFalseExpr);

    Type GetType() const override
    {
        return myTrueExpr ? myTrueExpr->GetType() : (myFalseExpr ? myFalseExpr->GetType() : Type{});
    }

    const Node* GetCondition() const
    {
        return myCondExpr;
    }

    const Node* GetTrueExpression() const
    {
        return myTrueExpr;
    }

    const Node* GetFalseExpression() const
    {
        return myFalseExpr;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myCondExpr, callback);
        CALL_CALLBACK(myTrueExpr, callback);
        CALL_CALLBACK(myFalseExpr, callback);
    }

private:
    Node* myCondExpr = nullptr;
    Node* myTrueExpr = nullptr;
    Node* myFalseExpr = nullptr;
};

};  // namespace HCXX

#endif  // COODDY_CONDITIONALEXPRESSION_H
