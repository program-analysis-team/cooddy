/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_CXXDTOREXPRESSION_H_
#define COODDY_ANALYZER_INCLUDE_AST_CXXDTOREXPRESSION_H_

#include <ast/CxxMemberCallExpression.h>

namespace HCXX {

class CxxDtorExpression : public CxxMemberCallExpression {
public:
    explicit CxxDtorExpression(CallExpression&& callExpr, const Node* object)
        : CxxMemberCallExpression(std::move(callExpr), object)
    {}

    DECLARE_KIND(CxxMemberCallExpression, Node::Kind::CXX_DTOR_EXPRESSION);
    DECLARE_SERIALIZE(CxxDtorExpression, nullptr);
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_CXXDTOREXPRESSION_H_
