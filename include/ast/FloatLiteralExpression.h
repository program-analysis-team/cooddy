/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_FLOATLITERALEXPRESSION_H
#define COODDY_FLOATLITERALEXPRESSION_H

#include <ast/LiteralExpression.h>

namespace HCXX {

class FloatLiteralExpression : public LiteralExpression {
public:
    explicit FloatLiteralExpression(const LiteralExpression& base, double myValue)
        : LiteralExpression(base), myValue(myValue)
    {}

    DECLARE_KIND(LiteralExpression, Node::Kind::FLOAT_LITERAL_EXPRESSION);

    // LCOV_EXCL_START
    DECLARE_SERIALIZE(FloatLiteralExpression, myValue);
    // LCOV_EXCL_START

    double GetValue() const
    {
        return myValue;
    }

private:
    double myValue;
};

}  // namespace HCXX

#endif  // COODDY_FLOATLITERALEXPRESSION_H
