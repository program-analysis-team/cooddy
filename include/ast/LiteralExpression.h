/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_LITERALEXPRESSION_H
#define COODDY_LITERALEXPRESSION_H

#include <TranslationUnit.h>
#include <ast/TypedNode.h>

#include <string>

namespace HCXX {

class LiteralExpression : public TypedNode {
public:
    DECLARE_ENUM(LiteralType, UNKNOWN, INTEGER, FLOAT, DOUBLE, STRING, NULLPTR, BOOL,
                 CHAR);  //  TODO: Do we need UNKNOWN here?

    LiteralExpression(SourceRange spellingRange, LiteralType literalType, std::string&& literal, const Type& type)
        : TypedNode(type), mySpellingRange(spellingRange), myLiteralType(literalType), myLiteral(std::move(literal))
    {}

    DECLARE_KIND(TypedNode, Node::Kind::LITERAL_EXPRESSION);
    DECLARE_SERIALIZE(LiteralExpression, myLiteralType);

    /**
     * Supposed to return the way literal is written in source code. This string is not normalized. I.e. for long
     * it can return either "0x2A", "42", "42L" or "42l" depending on what exactly is present in source.
     *
     * Currently returns empty string for strings.
     */
    const std::string& GetLiteral() const
    {
        return myLiteral;
    }

    LiteralType GetLiteralType() const
    {
        return myLiteralType;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Literal") {
            return GetLiteral();
        }
        if (attrName == "LiteralType") {
            return LiteralTypeToCStr(GetLiteralType());
        }

        return TypedNode::GetAttribute(attrName);
    }

    const SourceRange& GetSpellingRange() const
    {
        return mySpellingRange;
    }

    virtual bool IsTreatedAsNullPtr() const
    {
        return GetLiteralType() == LiteralType::NULLPTR;
    }

    bool IsConstExpr() const override
    {
        return true;
    }
    // LCOV_EXCL_START
    bool IsEqualNode(const Node* comparedNode) const override
    {
        auto literalExpression = CompareAndCast<LiteralExpression>(comparedNode);
        return literalExpression != nullptr && myLiteralType == literalExpression->GetLiteralType() &&
               myLiteral == literalExpression->GetLiteral();
    }
    // LCOV_EXCL_STOP

private:
    SourceRange mySpellingRange;
    LiteralType myLiteralType = LiteralType::UNKNOWN;
    std::string myLiteral;
};

};  // namespace HCXX

#endif  // COODDY_LITERALEXPRESSION_H
