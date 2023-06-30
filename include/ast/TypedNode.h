/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TYPEDNODE_H
#define COODDY_TYPEDNODE_H

#include <ast/Node.h>
#include <ast/Type.h>
#include <ast/VirtualOffset.h>

/// Declaration of node with type
namespace HCXX {
class TypedNodeBase : public Node {
public:
    explicit TypedNodeBase() {}

    DECLARE_KIND(Node, Node::Kind::TYPED_NODE);

    virtual const Node* GetInitializer() const
    {
        return nullptr;
    }

    virtual const Node* GetDeclaration() const
    {
        return nullptr;
    }

    virtual std::optional<VirtualOffset> GetOffsetInDeclaration() const
    {
        return std::nullopt;
    }

    virtual bool IsConstExpr() const
    {
        return false;
    }

    /**
     * @param sizeofBuffer the reference of the argument presenting the full byte size of the buffer
     * @return returns true if variable is a memory buffer
     */
    virtual bool IsMemoryBuffer(uint32_t& sizeofBuffer) const
    {
        return false;
    }

    // Returns exact value if it can fit int64_t, otherwise returns LLONG_MIN / LLONG_MAX.
    virtual int64_t GetSLimitedValue() const
    {
        return UNDEF_VALUE;
    }

    static const int64_t UNDEF_VALUE = INT64_MAX;
};

class TypedNode : public TypedNodeBase {
public:
    explicit TypedNode(const Type& type) : myType(type) {}

    DECLARE_SERIALIZE(TypedNode, myType);

    Type GetType() const override
    {
        return myType;
    }

    void Traverse(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myType.GetPointedDeclaration(), callback);
        Node::Traverse(callback);
    }

private:
    Type myType;
};

#define CALL_TYPED_NODE(node, method) \
    (Node::Cast<TypedNode>(node) != nullptr ? Node::Cast<TypedNode>(node)->method() : TypedNodeBase::method())

}  // namespace HCXX
#endif  // COODDY_TYPEDNODE_H
