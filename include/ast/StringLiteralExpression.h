/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_STRINGLITERALEXPRESSION_H
#define COODDY_STRINGLITERALEXPRESSION_H

#include <ast/LiteralExpression.h>
#include <ast/TypedNode.h>

namespace HCXX {

class StringLiteralExpression : public LiteralExpression {
public:
    StringLiteralExpression(const Type& type, std::string&& value)
        : LiteralExpression({}, LiteralExpression::LiteralType::STRING, {}, type), myValue(std::move(value))
    {}

    DECLARE_KIND(LiteralExpression, Node::Kind::STRING_LITERAL_EXPRESSION);
    DECLARE_SERIALIZE(StringLiteralExpression, nullptr);

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Value") {
            return myValue;
        }

        return LiteralExpression::GetAttribute(attrName);
    }

    const std::string& GetValue() const
    {
        return myValue;
    }

    void Print(std::string& source, int entryOffset) const override
    {
        SafePrint(source, entryOffset, "\"" + GetValue() + "\"", GetRange());
        Base::Print(source, entryOffset);
    }

private:
    std::string myValue;
};

};  // namespace HCXX

#endif  // COODDY_STRINGLITERALEXPRESSION_H
