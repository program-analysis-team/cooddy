/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_FUNCTIONDECL_H
#define COODDY_FUNCTIONDECL_H

#include <ast/DeclarationNameInfo.h>
#include <ast/Node.h>
#include <ast/ParamVarDecl.h>
#include <ast/RefExpression.h>
#include <ast/Type.h>
#include <ast/VarDecl.h>

#include <bitset>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace HCXX {

DECLARE_ENUM(FunctionQualifiersIndex, COPY, MOVE, EXPLICIT, DEFAULT, IMPLICIT, DEFINED, DELETED, DEFAULTED, PURE,
             OVERRIDE, STATIC, VARIADIC, TEMPLATED, VARIADIC_TEMPLATE, NO_RETURN, CONST, INLINE, VIRTUAL,
             ANNOTATED_TAINTED, INTRINSIC, LAST = 31);

using FunctionQualifiers = std::bitset<32>;

class FunctionDecl : public Node {
public:
    DECLARE_ENUM(FunctionKind, FUNCTION, METHOD, CONSTRUCTOR, DESTRUCTOR, CONVERSION);

    DECLARE_ENUM(AccessKind, PUBLIC, PROTECTED, PRIVATE, NONE);

    enum SignatureFormatFlags : unsigned int {
        SFF_NONE = 0,
        SFF_WITH_RETURN_TYPE = 0b01,
        SFF_WITH_PARAM_NAMES = 0b10,

        SFF_DEFAULT = SFF_WITH_RETURN_TYPE | SFF_WITH_PARAM_NAMES,
    };

    explicit FunctionDecl(DeclarationNameInfo&& declNameInfo, const Type& retType, std::string&& retTypeName,
                          std::vector<NodePtr<ParamVarDecl>>&& param, Node* body, FunctionKind funcKind,
                          FunctionQualifiers qualifiers, std::string&& mangledName, std::string&& qualifiedName,
                          AccessKind accessKind, FunctionDecl* definition, FunctionDecl* templateInstPattern,
                          std::vector<SourceRange>&& commentBlocks, std::vector<Type>&& templateTypes)
        : myDeclNameInfo(std::move(declNameInfo)),
          myRetType(retType),
          myRetTypeName(std::move(retTypeName)),
          myParams(std::move(param)),
          myBody(body),
          myFuncKind(funcKind),
          myQualifiers(std::move(qualifiers)),
          myAccessKind(accessKind),
          myMangledName(std::move(mangledName)),
          myQualifiedName(std::move(qualifiedName)),
          myDefinition(definition),
          myTemplateInstPattern(templateInstPattern),
          myCommentBlocks(std::move(commentBlocks)),
          myTemplateTypes(std::move(templateTypes))
    {}

    DECLARE_KIND(Node, Node::Kind::FUNCTION_DECL);
    DECLARE_SERIALIZE(FunctionDecl, myQualifiers << myAccessKind << myParams << myBody << myRetType << myRetTypeName
                                                 << myFuncKind << myMangledName << myQualifiedName << myDefinition
                                                 << myTemplateInstPattern);

    const string& GetName() const override
    {
        return myDeclNameInfo.GetName();
    }

    const vector<NodePtr<ParamVarDecl>>& GetParams() const
    {
        return myParams;
    }

    const Type& GetRetType() const
    {
        return myRetType;
    }

    std::string_view GetRetTypeName() const
    {
        return myRetTypeName;
    }

    const Node* GetBody() const
    {
        return myBody;
    }

    const SourceRange& GetNameRange() const
    {
        return myDeclNameInfo.GetRange();
    }

    bool IsInline() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::INLINE)];
    }

    bool IsIntinsic() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::INTRINSIC)];
    }

    bool IsStatic() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::STATIC)];
    }

    bool IsDefined() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::DEFINED)];
    }

    bool IsDeleted() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::DELETED)];
    }

    bool IsDefaulted() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::DEFAULTED)];
    }

    bool IsPure() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::PURE)];
    }

    bool IsImplicit() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::IMPLICIT)];
    }

    bool IsVariadic() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::VARIADIC)];
    }

    bool IsTemplated() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::TEMPLATED)];
    }

    bool IsVariadicTemplate() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::VARIADIC_TEMPLATE)];
    }

    bool IsNoReturn() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::NO_RETURN)];
    }

    bool IsOperator() const
    {
        return myDeclNameInfo.GetName().find("operator") == 0;
    }

    bool IsConst() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::CONST)];
    }

    bool IsVirtual() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::VIRTUAL)];
    }

    /// True if this function was annotated with special macro (see cli arg --taint-macro).
    bool IsAnnotatedTainted() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::ANNOTATED_TAINTED)];
    }

    Type GetType() const override
    {
        return myRetType;
    }

    std::string GetSignature(unsigned int formatFlags = SFF_DEFAULT) const;

    const std::string& GetMangledName() const
    {
        return myMangledName;
    }

    std::string GetQualifiedName() const
    {
        return myQualifiedName;
    }

    FunctionKind GetFuncKind() const
    {
        return myFuncKind;
    }

    AccessKind GetAccessKind() const
    {
        return myAccessKind;
    }

    const FunctionQualifiers& GetFunctionQualifiers() const
    {
        return myQualifiers;
    }

    //  Some remarks about function behaviour
    //  Use cases 1:
    //  struct A
    //  {
    //      explicit A(int x); IsDefinition => False(because of the body below)
    //      explicit A(char x); IsDefinition => True(There aren't any body)
    //  };
    //
    //  A::A(int x) { ... }
    //
    //  Use cases 2:
    //  struct B
    //  {
    //      B() = default; IsDefinition => True
    //  };
    //  Use cases 3:
    //  struct C
    //  {
    //      B() = delete; IsDefinition => True
    //  };
    bool IsDefinition() const
    {
        return myDefinition == nullptr;
    }

    const FunctionDecl* GetDefinition() const
    {
        return myDefinition;
    }

    const FunctionDecl* GetTemplateInstantiationPattern() const
    {
        return myTemplateInstPattern;
    }

    const std::vector<SourceRange>& GetCommentBlocks() const
    {
        return myCommentBlocks;
    }

    const std::vector<Type>& GetTemplateTypes() const
    {
        return myTemplateTypes;
    }

    bool IsDeclaration() const override
    {
        return true;
    }
    std::optional<std::string> GetAttribute(std::string_view attrName) const override;

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myBody, callback);
        CALL_CALLBACK(myDefinition, callback);
        for (auto& node : myParams) {
            CALL_CALLBACK(node, callback);
        }
    }

    std::vector<const RefExpression*> FindDeclarationUsages(const Node& declNode) const;
    void Print(std::string& source, int entryOffset) const override;

protected:
    FunctionQualifiers myQualifiers;
    AccessKind myAccessKind = AccessKind::NONE;

private:
    DeclarationNameInfo myDeclNameInfo;
    vector<NodePtr<ParamVarDecl>> myParams;
    NodePtr<Node> myBody;
    Type myRetType;
    std::string myRetTypeName;
    FunctionKind myFuncKind = FunctionKind::FUNCTION;
    std::string myMangledName;
    std::string myQualifiedName;
    const FunctionDecl* myDefinition = nullptr;
    NodePtr<FunctionDecl> myTemplateInstPattern;
    std::vector<SourceRange> myCommentBlocks;
    std::vector<Type> myTemplateTypes;
};

};  // namespace HCXX

#endif  // COODDY_FUNCTIONDECL_H
