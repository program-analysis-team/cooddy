/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_VARDECL_H
#define COODDY_VARDECL_H

#include <ast/NamedNode.h>
#include <ast/Type.h>
#include <ast/TypedNode.h>

#include <string>

namespace HCXX {

// TODO: push down to NamedNode
class DeclName : public Node {
public:
    DECLARE_KIND(Node, Node::Kind::DECL_NAME);
    DECLARE_SERIALIZE(DeclName, nullptr);
};

// TODO: push down to NamedNode
class DeclQualifier : public Node {
public:
    DECLARE_KIND(Node, Node::Kind::DECL_QUALIFIER);
    DECLARE_SERIALIZE(DeclQualifier, nullptr);
};

class VarDecl : public NamedNode<TypedNode> {
public:
    DECLARE_ENUM(VariableDeclKind, UNDEFINED, CXX_CLASS_MEMBER, LOCAL_DECLARATION, STATIC_DATA_MEMBER, STATIC_LOCAL)

    enum AttributeIndex {
        IS_VOLATILE = 0,
        IS_EXTERNAL = 1,
        IS_CONST_EXPR = 2,
        IS_ANONYMOUS = 3,
        IS_MATRIX = 4,
        IS_VA_ARG = 5
    };
    using Attributes = std::bitset<16>;

    VarDecl(Node* qualifier, VariableDeclKind varKind, std::string&& name, const Type& type,
            const DeclQualifier* declQualifier, const DeclName* declName, const Node* initializer,
            Attributes attributes, const Node* arraySizeExpr, uint32_t uniqueId = 0)
        : Base(qualifier, std::move(name), type),
          myVarKind(varKind),
          myDeclQualifier(declQualifier),
          myDeclName(declName),
          myInitializer(initializer),
          myAttributes(attributes),
          myArraySizeExpr(arraySizeExpr),
          myUniqueId(uniqueId)
    {}

    void Init() override;

    DECLARE_KIND(NamedNode<TypedNode>, Node::Kind::VAR_DECL);
    DECLARE_SERIALIZE(VarDecl,
                      myVarKind << myInitializer << myConstValue << myAttributes << myArraySizeExpr << myUniqueId);

    const DeclName* GetDeclName() const
    {
        return myDeclName;
    }
    const DeclQualifier* GetDeclQualifier() const
    {
        return myDeclQualifier;
    }
    const SourceRange& GetTypeSpecSourceRange() const
    {
        return myTypeSpecSourceRange;
    }
    void SetTypeSpecSourceRange(const SourceRange& typeSpecSourceRange)
    {
        myTypeSpecSourceRange = typeSpecSourceRange;
    }
    bool IsCxxClassMember() const
    {
        return myVarKind == VariableDeclKind::CXX_CLASS_MEMBER || IsStaticDataMember();
    }
    bool IsLocalVariableDeclaration() const
    {
        return myVarKind == VariableDeclKind::LOCAL_DECLARATION || IsStaticLocal();
    }
    bool IsStaticDataMember() const
    {
        return myVarKind == VariableDeclKind::STATIC_DATA_MEMBER;
    }
    bool IsStaticLocal() const
    {
        return myVarKind == VariableDeclKind::STATIC_LOCAL;
    }
    const Node* GetInitializer() const override
    {
        return myInitializer;
    }
    bool IsDeclaration() const override
    {
        return true;
    }
    bool IsConstExpr() const override;

    bool IsConstExprAttr() const
    {
        return myAttributes[IS_CONST_EXPR];
    }
    bool IsExternal() const
    {
        return myAttributes[IS_EXTERNAL];
    }
    int64_t GetSLimitedValue() const override;

    void SetCfgBlock(uint32_t blockPos) const
    {
        myCfgBlock = blockPos;
    }
    uint32_t GetCfgBlock() const
    {
        return myCfgBlock;
    }
    uint32_t GetUniqueId() const
    {
        return myUniqueId;
    }
    std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "IsClassMember") {
            return std::to_string(IsCxxClassMember());
        }

        if (attrName == "IsLocal") {
            return std::to_string(IsLocalVariableDeclaration());
        }

        return Base::GetAttribute(attrName);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myArraySizeExpr, callback);
        CALL_CALLBACK(myInitializer, callback);
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        callback(*this, GetInitializer() != nullptr ? UsageFlags::WRITE : UsageFlags::READ);
    }

    bool IsVolatileQualified() const
    {
        return myAttributes[IS_VOLATILE];
    }

    uint32_t GetNumOfElementsInArray() const;

    const Node* GetArraySizeExpr() const
    {
        return myArraySizeExpr;
    }

    bool IsMatrixArray() const
    {
        return myAttributes[IS_MATRIX];
    }

    bool IsVAArg() const
    {
        return myAttributes[IS_VA_ARG];
    }

    VariableDeclKind GetVarKind() const
    {
        return myVarKind;
    }

    bool IsEqualNode(const Node* comparedNode) const override
    {
        auto varDecl = CompareAndCast<VarDecl>(comparedNode);
        return varDecl != nullptr && GetVarKind() == varDecl->GetVarKind();
    }

    bool IsMemoryBuffer(uint32_t& sizeofBuffer) const override;

    void Print(std::string& source, int entryOffset) const override;
    std::string Dump() const override;

    static std::string GetVarName(const Node* decl);

protected:
    Attributes myAttributes;

private:
    uint32_t myUniqueId = 0;
    VariableDeclKind myVarKind = VariableDeclKind::UNDEFINED;
    const DeclQualifier* myDeclQualifier = nullptr;
    const DeclName* myDeclName = nullptr;
    const Node* myInitializer = nullptr;
    SourceRange myTypeSpecSourceRange;
    NodePtr<Node> myArraySizeExpr;
    int64_t myConstValue = TypedNode::UNDEF_VALUE;
    mutable uint32_t myCfgBlock = UINT32_MAX;
};

};  // namespace HCXX

#endif  // COODDY_VARDECL_H
