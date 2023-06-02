/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_CXXOPERATORCALLEXPRESSION_H_
#define COODDY_ANALYZER_INCLUDE_AST_CXXOPERATORCALLEXPRESSION_H_

#include <ast/CallExpression.h>

namespace HCXX {

class CxxOperatorToken : public Node {
public:
    DECLARE_KIND(Node, Node::Kind::CXX_OPERATOR_TOKEN);
    DECLARE_SERIALIZE(CxxOperatorToken, nullptr);
};

class CxxOperatorCallExpression : public CallExpression {
public:
    DECLARE_ENUM(OperatorKind, NONE, NEW, DELETE, ARRAY_NEW, ARRAY_DELETE, PLUS, MINUS, STAR, SLASH, PERCENT, CARET,
                 AMP, PIPE, TILDE, EXCLAIM, EQUAL, LESS, GREATER, PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL,
                 PERCENT_EQUAL, CARET_EQUAL, AMP_EQUAL, PIPE_EQUAL, LESS_LESS, GREATER_GREATER, LESS_LESS_EQUAL,
                 GREATER_GREATER_EQUAL, EQUAL_EQUAL, EXCLAIM_EQUAL, LESS_EQUAL, GREATER_EQUAL, SPACE_SHIP, AMP_AMP,
                 PIPE_PIPE, PLUS_PLUS, MINUS_MINUS, COMMA, ARROW_STAR, ARROW, CALL, SUBSCRIPT, CONDITIONAL, COAWAIT);

    explicit CxxOperatorCallExpression(CallExpression&& callExpr, const CxxOperatorToken* operatorToken,
                                       OperatorKind operatorKind, bool isAssignmentOp)
        : CallExpression(callExpr),
          myOperatorToken(operatorToken),
          myOperator(operatorKind),
          myIsAssignmentOp(isAssignmentOp)
    {}

    DECLARE_KIND(CallExpression, Node::Kind::CXX_OPERATOR_CALL_EXPRESSION);
    DECLARE_SERIALIZE(CxxOperatorCallExpression, myOperator << myOperatorToken << myIsAssignmentOp);

    OperatorKind GetOperator() const
    {
        return myOperator;
    }
    const CxxOperatorToken* GetOperatorToken() const
    {
        return myOperatorToken;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Operator") {
            return OperatorKindToCStr(GetOperator());
        }

        return CallExpression::GetAttribute(attrName);
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        CallExpression::TraverseDeclUsage(callback, usageFlags);
        if (!myIsAssignmentOp) {
            return;
        }
        auto& args = GetArguments();
        if (args.empty()) {
            return;  // LCOV_EXCL_LINE: HCAT-2777
        }
        auto funcDecl = GetFunction();
        if (funcDecl != nullptr && !funcDecl->IsConst()) {
            args[0]->TraverseDeclUsage(callback, UsageFlags::WRITE);
        }
    }

private:
    OperatorKind myOperator = OperatorKind::NONE;
    const CxxOperatorToken* myOperatorToken = nullptr;
    bool myIsAssignmentOp = false;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_CXXOPERATORCALLEXPRESSION_H_
