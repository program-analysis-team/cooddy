/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CXXCONSTRUCTEXPRESSION_H
#define COODDY_CXXCONSTRUCTEXPRESSION_H

#include <ast/CallExpression.h>

namespace HCXX {

class CxxConstructExpression : public CallExpression {
public:
    explicit CxxConstructExpression(Node* decl, std::vector<const Node*>&& arguments, Type classType)
        : CallExpression(nullptr, decl, std::move(arguments), classType)
    {}

    ~CxxConstructExpression() = default;

    DECLARE_KIND(CallExpression, Node::Kind::CXX_CONSTRUCT_EXPRESSION);
    DECLARE_SERIALIZE(CxxConstructExpression, nullptr);
};

}  // namespace HCXX

#endif  // COODDY_CXXCONSTRUCTEXPRESSION_H
