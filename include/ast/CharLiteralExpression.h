/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_CHARLITERALEXPRESSION_H_
#define COODDY_ANALYZER_INCLUDE_AST_CHARLITERALEXPRESSION_H_

#include <ast/LiteralExpression.h>
#include <ast/TypedNode.h>

#include <cstdint>
#include <optional>
#include <string>

namespace HCXX {

class CharLiteralExpression : public LiteralExpression {
public:
    DECLARE_ENUM(CharKind, ASCII, WIDE, UTF8, UTF16, UTF32);

    CharLiteralExpression(const Type& type, uint32_t value, CharKind kind)
        : LiteralExpression(type), myValue(value), myKind(kind)
    {}

    DECLARE_KIND(LiteralExpression, Node::Kind::CHAR_LITERAL_EXPRESSION);
    DECLARE_SERIALIZE(CharLiteralExpression, myValue << myKind);

    virtual LiteralType GetLiteralType() const
    {
        return LiteralType::CHAR;
    }

    // LCOV_EXCL_START
    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Value") {
            return std::to_string(myValue);
        }

        return LiteralExpression::GetAttribute(attrName);
    }
    // LCOV_EXCL_STOP

    int64_t GetSLimitedValue() const override
    {
        return myValue;
    }

    CharKind GetCharKind() const
    {
        return myKind;
    }

private:
    uint32_t myValue = 0;
    CharKind myKind = CharKind::ASCII;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_CHARLITERALEXPRESSION_H_
