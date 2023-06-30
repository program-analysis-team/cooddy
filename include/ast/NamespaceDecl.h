/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_NAMESPACEDECL_H
#define COODDY_NAMESPACEDECL_H

#include <ast/NamedNode.h>

#include <string>

namespace HCXX {

class NamespaceDecl : public NamedNode<CompoundNode> {
public:
    NamespaceDecl(Node* qualifier, std::string&& name) : Base(qualifier, std::move(name)) {}

    DECLARE_KIND(NamedNode<CompoundNode>, Node::Kind::NAMESPACE);
    DECLARE_SERIALIZE(NamespaceDecl, nullptr);
};

};  // namespace HCXX

#endif  // COODDY_NAMESPACEDECL_H
