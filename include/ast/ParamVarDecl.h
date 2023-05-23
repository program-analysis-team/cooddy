/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_PARAMDECL_H
#define COODDY_PARAMDECL_H

#include <ast/NamedNode.h>
#include <ast/Type.h>
#include <ast/TypedNode.h>

#include <string>
#include <string_view>

namespace HCXX {
// TODO: add more information of ParamVarDecl
class ParamVarDecl : public NamedNode<TypedNode> {
public:
    ParamVarDecl(std::string&& name, const Type& type, std::string&& typeName, const Node* arraySizeExpr = nullptr)
        : Base(nullptr, std::move(name), type), myTypeName(std::move(typeName)), myArraySizeExpr(arraySizeExpr)
    {}

    DECLARE_KIND(NamedNode<TypedNode>, Node::Kind::PARAM_VAR_DECL);
    DECLARE_SERIALIZE(ParamVarDecl, nullptr);

    bool IsDeclaration() const override
    {
        return true;
    }

    std::string_view GetTypeName() const
    {
        return myTypeName;
    }

    const Node* GetArraySizeExpr() const
    {
        return myArraySizeExpr;
    }
    bool IsMemoryBuffer(uint32_t& sizeofBuffer) const override;

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myArraySizeExpr, callback);
    }

private:
    std::string myTypeName;
    NodePtr<Node> myArraySizeExpr;
};

};  // namespace HCXX

#endif  // COODDY_PARAMDECL_H
