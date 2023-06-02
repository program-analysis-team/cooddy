/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_BOOLLITERALEXPRESSION_H_
#define COODDY_ANALYZER_INCLUDE_AST_BOOLLITERALEXPRESSION_H_

#include <ast/LiteralExpression.h>

namespace HCXX {

class BoolLiteralExpression : public LiteralExpression {
public:
    BoolLiteralExpression(const LiteralExpression& base, bool value) : LiteralExpression(base), myValue(value) {}

    DECLARE_KIND(LiteralExpression, Node::Kind::BOOL_LITERAL_EXPRESSION);
    DECLARE_SERIALIZE(BoolLiteralExpression, myValue);

    bool GetValue() const
    {
        return myValue;
    }

    int64_t GetSLimitedValue() const override
    {
        return int64_t(myValue);
    }
    void Print(std::string& source, int entryOffset) const override
    {
        SafePrint(source, entryOffset, myValue ? "true" : "false", GetRange());
    }

private:
    bool myValue = false;
};

};      // namespace HCXX
#endif  // COODDY_ANALYZER_INCLUDE_AST_BOOLLITERALEXPRESSION_H_
