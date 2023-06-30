/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TRYEXTPRESSION_H
#define COODDY_TRYEXTPRESSION_H

#include <ast/CatchExpression.h>

#include "Node.h"

namespace HCXX {

class TryExpression : public HCXX::CompoundNode {
public:
    TryExpression(const Node* body, std::vector<const CatchExpression*>&& handlers) : myBody(body), myHandlers(handlers)
    {}
    ~TryExpression() = default;

    DECLARE_KIND(CompoundNode, Node::Kind::TRY_EXPRESSION);
    DECLARE_SERIALIZE(TryExpression, myBody << myHandlers);

    const Node* GetBody() const
    {
        return myBody;
    }

    auto& GetHandlers() const
    {
        return myHandlers;
    }

private:
    const Node* myBody = nullptr;
    std::vector<const CatchExpression*> myHandlers;
};

}  // namespace HCXX

#endif  // COODDY_THROWEXTPRESSION_H
