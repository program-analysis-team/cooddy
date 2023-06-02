/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_DELETEEXPR_H
#define COODDY_DELETEEXPR_H

#include "CallExpression.h"

namespace HCXX {

class DeleteExpression : public HCXX::CallExpression {
public:
    DeleteExpression(const Node* objectBeingDeleted, const Node* deleter)
        : HCXX::CallExpression(objectBeingDeleted, deleter, {objectBeingDeleted}, {}),
          myObjectBeingDeleted(objectBeingDeleted)
    {}

    DECLARE_KIND(CallExpression, Node::Kind::DELETE_EXPRESSION);
    DECLARE_SERIALIZE(DeleteExpression, myObjectBeingDeleted);

    const Node* GetObjectBeingDeleted() const
    {
        return myObjectBeingDeleted;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myObjectBeingDeleted, callback);
    }

private:
    const Node* myObjectBeingDeleted = nullptr;
};

}  // namespace HCXX

#endif  // COODDY_DELETEEXPR_H
