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

class LiteralExpression : public TypedNodeBase {
public:
    DECLARE_ENUM(LiteralType, INTEGER, FLOAT, STRING, NULLPTR, BOOL, CHAR);

    LiteralExpression(Type type) : myTypeFlags(type.GetFlags()) {}

    DECLARE_KIND(TypedNodeBase, Node::Kind::LITERAL_EXPRESSION);
    DECLARE_SERIALIZE(LiteralExpression, myTypeFlags);

    Type GetType() const override
    {
        return Type(myTypeFlags);
    }

    virtual LiteralType GetLiteralType() const
    {
        return LiteralType::NULLPTR;
    }

    /**
     * Supposed to return the way literal is written in source code.
     */
    std::string GetLiteral() const
    {
        return GetTranslationUnit()->GetSourceInRange(GetRange());
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Literal") {
            return GetLiteral();
        }
        if (attrName == "LiteralType") {
            return LiteralTypeToCStr(GetLiteralType());
        }
        return Base::GetAttribute(attrName);
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
        return literalExpression != nullptr && GetLiteralType() == literalExpression->GetLiteralType();
    }
    // LCOV_EXCL_STOP
private:
    uint64_t myTypeFlags = 0;
};

};  // namespace HCXX

#endif  // COODDY_LITERALEXPRESSION_H
