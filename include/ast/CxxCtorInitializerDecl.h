/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Represent CXXRecordDecl's member initialization for both cases
// - definition + declaration: isDefaultInit() returns true
// - definition with c-tor initializer list: isDefaultInit() returns false
//

#ifndef COODDY_CXXCTORINITIALIZERDECL_H
#define COODDY_CXXCTORINITIALIZERDECL_H

#include <ast/CxxMethodDecl.h>
#include <ast/FieldDecl.h>
#include <ast/FunctionDecl.h>

using std::string;
using std::vector;

namespace HCXX {

class CxxCtorInitializerDecl : public TypedNode {
public:
    CxxCtorInitializerDecl(const CxxMethodDecl* methodDecl, const FieldDecl* fieldDecl, const Node* initializer,
                           const std::string& name)
        : TypedNode(Type()), myMethodDecl(methodDecl), myFieldDecl(fieldDecl), myInitializer(initializer), myName(name)
    {}

    DECLARE_KIND(TypedNode, Node::Kind::CXX_CTOR_INITIALIZER);
    DECLARE_SERIALIZE(CxxCtorInitializerDecl, myMethodDecl << myFieldDecl << myInitializer);

    const string& GetName() const override
    {
        return myName;
    }

    const FieldDecl* GetFieldDecl() const
    {
        return myFieldDecl;
    }

    const Node* GetInitializer() const override
    {
        return myInitializer;
    }

    const Node* GetDeclaration() const override
    {
        return myMethodDecl != nullptr ? myMethodDecl->GetThisDecl() : nullptr;
    }

    std::optional<VirtualOffset> GetOffsetInDeclaration() const override
    {
        if (myFieldDecl == nullptr) {
            return std::nullopt;
        }
        return VirtualOffset(VirtualOffset::Kind::INDEX, myFieldDecl->GetIndex());
    }

    void Traverse(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myFieldDecl, callback);
        TypedNode::Traverse(callback);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myInitializer, callback);
    }

private:
    const CxxMethodDecl* myMethodDecl = nullptr;
    const FieldDecl* myFieldDecl = nullptr;
    const Node* myInitializer = nullptr;
    std::string myName;
};

};  // namespace HCXX

#endif  // COODDY_CXXCTORINITIALIZERDECL_H
