/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_REFEXPRESSION_H
#define COODDY_REFEXPRESSION_H

#include <ast/TypedNode.h>

namespace HCXX {

class RefExpression : public TypedNodeBase {
public:
    explicit RefExpression(const Node* declaration, const Type& type) : myDeclaration(declaration) {}

    DECLARE_KIND(TypedNodeBase, Node::Kind::REF_EXPRESSION);
    DECLARE_SERIALIZE(RefExpression, myDeclaration);

    const Node* GetDeclaration() const override
    {
        return myDeclaration;
    }

    std::optional<VirtualOffset> GetOffsetInDeclaration() const override
    {
        return VirtualOffset(VirtualOffset::Kind::REF);
    }

    Type GetType() const override
    {
        return myDeclaration != nullptr ? myDeclaration->GetType() : Type();
    }

    void Traverse(TraverseCallback callback) const override
    {
        CALL_CALLBACK(GetType().GetPointedDeclaration(), callback);
        CALL_CALLBACK(myDeclaration, callback);
        TypedNodeBase::Traverse(callback);
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        if (myDeclaration != nullptr) {
            callback(*myDeclaration, usageFlags);
        }
    }

    bool IsConstExpr() const override
    {
        return CALL_TYPED_NODE(myDeclaration, IsConstExpr);
    }

    int64_t GetSLimitedValue() const override
    {
        return CALL_TYPED_NODE(myDeclaration, GetSLimitedValue);
    }

    bool IsEqualNode(const Node* comparedNode) const override
    {
        auto refExpression = CompareAndCast<RefExpression>(comparedNode);
        return refExpression != nullptr && GetDeclaration()->IsEqualNode(refExpression->GetDeclaration());
    }

    void Print(std::string& source, int entryOffset) const override;

private:
    const Node* myDeclaration = nullptr;
};
}  // namespace HCXX

#endif  // COODDY_REFEXPRESSION_H
