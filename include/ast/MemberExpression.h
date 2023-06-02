/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Represent CXXRecordDecl's member usage in any expression.
//

#ifndef COODDY_MEMBEREXPRESSION_H
#define COODDY_MEMBEREXPRESSION_H

#include <ast/FieldDecl.h>
#include <ast/NamedNode.h>

#include <string>

namespace HCXX {

class MemberExpression : public NamedNode<TypedNode> {
public:
    MemberExpression(const Type& type, std::string&& name, const Node* base, const Node* value, bool isArrow)
        : Base(nullptr, std::move(name), type), myBase(base), myValue(value), myIsArrow(isArrow)
    {}

    DECLARE_KIND(NamedNode<TypedNode>, Node::Kind::MEMBER_EXPRESSION);
    DECLARE_SERIALIZE(MemberExpression, myBase << myValue << myIsArrow);

    bool IsArrow() const
    {
        return myIsArrow;
    }

    const Node* GetBase() const
    {
        return myBase;
    }

    const Node* GetValue() const
    {
        return myValue;
    }

    bool IsConstExpr() const override
    {
        auto typedValue = Node::Cast<TypedNode>(myValue);
        return typedValue != nullptr && typedValue->IsConstExpr();
    }

    int64_t GetSLimitedValue() const override
    {
        auto typedValue = Node::Cast<TypedNode>(myValue);
        return typedValue != nullptr && typedValue->GetSLimitedValue();
    }

    void Traverse(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myValue, callback);
        TypedNode::Traverse(callback);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myBase, callback);
    }

    const Node* GetDeclaration() const override
    {
        auto baseNode = Node::Cast<TypedNode>(myBase);
        return baseNode != nullptr ? baseNode->GetDeclaration() : nullptr;
    }

    std::optional<VirtualOffset> GetOffsetInDeclaration() const override
    {
        auto base = Node::Cast<TypedNode>(myBase);
        auto fieldNode = Node::Cast<FieldDecl>(myValue);
        if (base != nullptr && fieldNode != nullptr) {
            VirtualOffset result(VirtualOffset::Kind::INDEX, fieldNode->GetIndex());
            auto baseOffset = base->GetOffsetInDeclaration();
            return baseOffset ? *baseOffset + result : result;
        }
        return Base::GetOffsetInDeclaration();
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        if (myBase != nullptr) {
            myBase->TraverseDeclUsage(callback, usageFlags);
        }
    }
    void Print(std::string& source, int entryOffset) const override
    {
        Base::Print(source, entryOffset);
        if (myBase != nullptr) {
            auto r = myBase->GetRange();
            std::string name = myIsArrow ? "->" : ".";
            name += VarDecl::GetVarName(myValue);
            SafePrint(source, entryOffset, name, r, r.Size());
        }
    }
    // LCOV_EXCL_START
    virtual std::string Dump() const override
    {
        std::string res = Node::Dump();
        if (myValue != nullptr) {
            res += " value: " + myValue->Dump();
        }
        return res;
    }
    // LCOV_EXCL_STOP

private:
    const Node* myBase = nullptr;
    const Node* myValue = nullptr;
    bool myIsArrow = false;
};

};  // namespace HCXX

#endif  // COODDY_MEMBEREXPRESSION_H
