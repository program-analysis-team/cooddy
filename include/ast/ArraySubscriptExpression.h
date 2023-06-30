/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ARRAYSUBSCRIPTEXPRESSION_H
#define COODDY_ARRAYSUBSCRIPTEXPRESSION_H

#include <ast/IntLiteralExpression.h>
#include <ast/Type.h>
#include <ast/TypedNode.h>

namespace HCXX {

class ArraySubscriptExpression : public TypedNode {
public:
    ArraySubscriptExpression(Node* lhs, Node* rhs, Node* base, const Type& type)
        : TypedNode(type), myLHS(lhs), myRHS(rhs), myReveresed(rhs == base)
    {}

    DECLARE_KIND(TypedNode, Node::Kind::ARRAY_SUBSCRIPT_EXPRESSION);
    DECLARE_SERIALIZE(ArraySubscriptExpression, myLHS << myRHS << myReveresed);

    const Node* GetBase() const
    {
        return myReveresed ? myRHS : myLHS;
    }

    const Node* GetIdx() const
    {
        return myReveresed ? myLHS : myRHS;
    }

    // LCOV_EXCL_START
    const Node* GetLeftHandSide() const
    {
        return myLHS;
    };

    const Node* GetRightHandSide() const
    {
        return myRHS;
    };
    // LCOV_EXCL_STOP

    const Node* GetDeclaration() const override
    {
        auto baseNode = Node::Cast<TypedNode>(GetBase());
        auto idx = Node::Cast<IntLiteralExpression>(GetIdx());
        return baseNode != nullptr && idx != nullptr ? baseNode->GetDeclaration() : nullptr;
    }

    std::optional<VirtualOffset> GetOffsetInDeclaration() const override
    {
        auto base = Node::Cast<TypedNode>(GetBase());
        auto idx = Node::Cast<IntLiteralExpression>(GetIdx());
        if (base != nullptr && idx != nullptr) {
            auto idxVal = idx->GetSLimitedValue();
            VirtualOffset result(idxVal != 0 ? VirtualOffset::Kind::INDEX : VirtualOffset::Kind::DEREF, idxVal);
            auto baseOffset = base->GetOffsetInDeclaration();
            return baseOffset ? *baseOffset + result : result;
        }
        return TypedNode::GetOffsetInDeclaration();
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myLHS, callback);
        CALL_CALLBACK(myRHS, callback);
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        if (GetBase() != nullptr) {
            GetBase()->TraverseDeclUsage(callback, usageFlags);
        }
        if (GetIdx() != nullptr) {
            GetIdx()->TraverseDeclUsage(callback, UsageFlags::READ);
        }
    }
    void Print(std::string& source, int entryOffset) const override
    {
        if (myRHS != nullptr) {
            auto r = myRHS->GetRange();
            SafePrint(source, entryOffset, "[", r, -1);
            SafePrint(source, entryOffset, "]", r, r.Size());
        }
        Base::Print(source, entryOffset);
    }
    // LCOV_EXCL_START
    std::string Dump() const override
    {
        return Base::Dump() + " Base:" + GetBase()->GetInnerNode()->GetType().Dump();
    }
    // LCOV_EXCL_STOP

private:
    NodePtr<Node> myLHS;
    NodePtr<Node> myRHS;
    bool myReveresed = false;
};

}  // namespace HCXX

#endif  // COODDY_ARRAYSUBSCRIPTEXPRESSION_H
