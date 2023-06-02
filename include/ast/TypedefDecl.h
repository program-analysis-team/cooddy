/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef AST_TYPEDEF_DECL_HEADER_FILE
#define AST_TYPEDEF_DECL_HEADER_FILE

#include <ast/NamedNode.h>
#include <ast/TypedNode.h>

#include <string_view>

namespace HCXX {

class TypedefDecl : public NamedNode<TypedNode> {
public:
    TypedefDecl(Node* qualifier, const Type& type, std::string_view name) : Base(qualifier, name, type) {}
    ~TypedefDecl() = default;

    void Init() override
    {
        auto record = Node::Cast<RecordDecl>(GetType().GetDeclaration());
        if (record != nullptr && record->GetUniqueId() == 0) {
            record->InitUniqueId(GetName());
        }
    }

    DECLARE_KIND(NamedNode<TypedNode>, Node::Kind::TYPEDEF_DECL);
    DECLARE_SERIALIZE(TypedefDecl, nullptr);
};

}  //  namespace HCXX

#endif  // #ifndef AST_TYPEDEF_DECL_HEADER_FILE
