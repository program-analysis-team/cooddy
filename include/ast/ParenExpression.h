/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_PARENEXPRESSION_H
#define COODDY_PARENEXPRESSION_H

#include <ast/TemporaryExpression.h>

namespace HCXX {

class ParenExpression : public TemporaryExpression {
public:
    ParenExpression(const Node* subExpr, const Type& type) : TemporaryExpression(subExpr, type) {}

    DECLARE_KIND(TemporaryExpression, Node::Kind::PAREN_EXPRESSION);
    DECLARE_SERIALIZE(ParenExpression, nullptr);

    const Node* GetNonParenSubExpr() const
    {
        auto subExpr = GetSubExpr();
        while (subExpr != nullptr && subExpr->GetKind() == Node::Kind::PAREN_EXPRESSION) {
            // LCOV_EXCL_START
            // msg sent successfully from here during test run, but coverage still empty
            subExpr = static_cast<const ParenExpression*>(subExpr)->GetSubExpr();
            // LCOV_EXCL_STOP
        }
        return subExpr;
    };

    bool IsConstExpr() const override
    {
        return CALL_TYPED_NODE(GetSubExpr(), IsConstExpr);
    }

    int64_t GetSLimitedValue() const override
    {
        return CALL_TYPED_NODE(GetSubExpr(), GetSLimitedValue);
    }
    void Print(std::string& source, int entryOffset) const override
    {
        auto range = GetRange();
        SafePrint(source, entryOffset, "(", range);
        SafePrint(source, entryOffset, ")", range, range.Size() - 1);
        Base::Print(source, entryOffset);
    }
};

};  // namespace HCXX

#endif  // COODDY_PARENEXPRESSION_H
