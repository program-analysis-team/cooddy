/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ENUMCONSTANTDECL_H
#define COODDY_ENUMCONSTANTDECL_H

#include <ast/NamedNode.h>
#include <ast/Node.h>

namespace HCXX {

class EnumConstantDecl : public NamedNode<TypedNode> {
public:
    EnumConstantDecl(Node* qualifier, std::string_view name, const Type& type, int64_t value)
        : Base(qualifier, name, type), myValue(value)
    {}
    ~EnumConstantDecl() = default;

    DECLARE_KIND(NamedNode<TypedNode>, Node::Kind::ENUM_CONSTANT_DECL);
    DECLARE_SERIALIZE(EnumConstantDecl, myValue);
    bool IsDeclaration() const override
    {
        return true;
    }

    bool IsConstExpr() const override
    {
        return true;
    }

    int64_t GetSLimitedValue() const override
    {
        return myValue;
    }

    std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Value") {
            return std::to_string(GetValue());
        }

        return Base::GetAttribute(attrName);
    }

public:
    //  Returns numeric value for enum const declaration
    //  Example 1:
    //  enum JustEnum { RED, GREEN, BLUE };
    //  -   0 is returned for RED
    //  -   1 is returned for GREEN
    //  -   2 is returned for BLUE
    //  Example 2:
    //  enum EnumClassWithValue { RED = 2, GREEN, BLUE };
    //  -   2 is returned for RED
    //  -   3 is returned for GREEN
    //  -   4 is returned for BLUE
    int64_t GetValue() const
    {
        return myValue;
    }

private:
    int64_t myValue = 0;
};

}  // namespace HCXX

#endif  // COODDY_ENUMCONSTANTDECL_H
