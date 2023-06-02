/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ENUMDECL_H
#define COODDY_ENUMDECL_H

#include <ast/EnumConstantDecl.h>
#include <ast/NamedNode.h>
#include <ast/TypedNode.h>

namespace HCXX {

class EnumDecl : public NamedNode<TypedNode> {
public:
    using EnumMembers = std::vector<EnumConstantDecl*>;

    EnumDecl(Node* qualifier, const char* name, const Type& type, EnumMembers members)
        : Base(qualifier, name, type), myMembers(std::move(members))
    {}
    ~EnumDecl() = default;

    DECLARE_KIND(NamedNode<TypedNode>, Node::Kind::ENUM_DECL);
    DECLARE_SERIALIZE(EnumDecl, myMembers);

    const EnumMembers& GetMembers() const
    {
        return myMembers;
    }

    // LCOV_EXCL_START
    bool IsDeclaration() const override
    {
        return true;
    }
    // LCOV_EXCL_STOP

private:
    EnumMembers myMembers;
};

}  // namespace HCXX

#endif  // COODDY_ENUMDECL_H
