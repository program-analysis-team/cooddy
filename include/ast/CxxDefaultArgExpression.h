/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CXXDEFAULTARGEXPRESSION_H
#define COODDY_CXXDEFAULTARGEXPRESSION_H

#include "Node.h"

namespace HCXX {

class CxxDefaultArgExpression : public HCXX::Node {
public:
    CxxDefaultArgExpression(const Node* param, const Node* expr) : myParam(param), myExpr(expr) {}

    DECLARE_KIND(Node, Node::Kind::CXX_DEFAULT_ARG_EXPRESSION);
    DECLARE_SERIALIZE(CxxDefaultArgExpression, myParam << myExpr);

    const Node* GetParam() const
    {
        return myParam;
    }
    const Node* GetExpr() const
    {
        return myExpr;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myParam, callback);
        CALL_CALLBACK(myExpr, callback);
    }

private:
    const Node* myParam = nullptr;
    const Node* myExpr = nullptr;
};

};  // namespace HCXX

#endif  // COODDY_CXXDEFAULTARGEXPRESSION_H
