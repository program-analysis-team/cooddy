/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TYPEALIASDECL_H
#define COODDY_TYPEALIASDECL_H

#include <ast/NamedNode.h>
#include <ast/TypedNode.h>

#include <string_view>

namespace HCXX {

class TypeAliasDecl : public NamedNode<TypedNode> {
public:
    TypeAliasDecl(const Type& type, std::string_view name) : Base(nullptr, name, type) {}
    ~TypeAliasDecl() = default;

    DECLARE_KIND(NamedNode<TypedNode>, Node::Kind::TYPE_ALIAS_DECL);
    DECLARE_SERIALIZE(TypeAliasDecl, nullptr);
};

}  // namespace HCXX

#endif  // COODDY_TYPEALIASDECL_H
