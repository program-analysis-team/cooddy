/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_INTLITERALEXPRESSION_H
#define COODDY_INTLITERALEXPRESSION_H

#include <ast/LiteralExpression.h>
#include <ast/TypedNode.h>

#include <algorithm>
#include <limits>

namespace HCXX {

class IntLiteralExpression : public LiteralExpression {
public:
    IntLiteralExpression(const Type& type, uint64_t myValue) : LiteralExpression(type), myValue(myValue) {}

    DECLARE_KIND(LiteralExpression, Node::Kind::INT_LITERAL_EXPRESSION);
    DECLARE_SERIALIZE(IntLiteralExpression, myValue);

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "SLimitedValue") {
            return std::to_string(GetSLimitedValue());
        }

        return LiteralExpression::GetAttribute(attrName);
    }

    int64_t GetSLimitedValue() const override
    {
        return myValue <= uint64_t(std::numeric_limits<int64_t>::max()) ? myValue : UNDEF_VALUE;
    }

    uint64_t GetValue() const
    {
        return myValue;
    }

    LiteralType GetLiteralType() const override
    {
        return LiteralType::INTEGER;
    }

    virtual bool IsTreatedAsNullPtr() const override
    {
        return myValue == 0;
    }

    bool IsEqualNode(const Node* comparedNode) const override
    {
        auto intLiteralExpression = CompareAndCast<IntLiteralExpression>(comparedNode);
        return intLiteralExpression != nullptr && myValue == intLiteralExpression->GetValue();
    }

    // LCOV_EXCL_START
    std::string Dump() const override
    {
        return Node::Dump() + ", value: " + std::to_string(myValue);
    }
    // LCOV_EXCL_STOP

    void Print(std::string& source, int entryOffset) const override
    {
        SafePrint(source, entryOffset, std::to_string(GetValue()), GetRange());
        Base::Print(source, entryOffset);
    }

private:
    uint64_t myValue = 0;
};

};  // namespace HCXX

#endif  // COODDY_INTLITERALEXPRESSION_H
