/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_TEMPORARYEXPRESSION_H_
#define COODDY_ANALYZER_INCLUDE_AST_TEMPORARYEXPRESSION_H_

#include "Node.h"

namespace HCXX {

class TemporaryExpression : public TypedNode {
public:
    explicit TemporaryExpression(const Node* subExpr, const Type& type = Type()) : TypedNode(type), mySubExpr(subExpr)
    {}

    DECLARE_KIND(TypedNode, Node::Kind::TEMPORARY_EXPRESSION);
    DECLARE_SERIALIZE(TemporaryExpression, mySubExpr);

    const Node* GetInnerNode() const override
    {
        return mySubExpr != nullptr ? mySubExpr->GetInnerNode() : this;
    }

    const Node* GetSubExpr() const
    {
        return mySubExpr;
    }

    const Node* GetDeclaration() const override
    {
        auto baseNode = Node::Cast<TypedNode>(mySubExpr);
        return baseNode != nullptr ? baseNode->GetDeclaration() : nullptr;
    }

    Type GetType() const override
    {
        return mySubExpr != nullptr ? mySubExpr->GetType() : TypedNode::GetType();
    };

    std::optional<VirtualOffset> GetOffsetInDeclaration() const override
    {
        auto baseNode = Node::Cast<TypedNode>(mySubExpr);
        return baseNode != nullptr ? baseNode->GetOffsetInDeclaration() : std::nullopt;
    }

    bool IsConstExpr() const override
    {
        auto typedExpr = Node::Cast<TypedNode>(mySubExpr);
        return typedExpr != nullptr ? typedExpr->IsConstExpr() : false;
    }

    int64_t GetSLimitedValue() const override
    {
        auto typedExpr = Node::Cast<TypedNode>(mySubExpr);
        return typedExpr != nullptr ? typedExpr->GetSLimitedValue() : TypedNode::GetSLimitedValue();
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(mySubExpr, callback);
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        if (mySubExpr != nullptr) {
            mySubExpr->TraverseDeclUsage(callback, usageFlags);
        }
    }

private:
    const Node* mySubExpr = nullptr;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_TEMPORARYEXPRESSION_H_
