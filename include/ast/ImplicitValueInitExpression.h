/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_IMPLICITVALUEINITLISTEXPRESSION_H
#define COODDY_IMPLICITVALUEINITLISTEXPRESSION_H

#include <ast/TypedNode.h>

#include <string>

namespace HCXX {

class ImplicitValueInitExpression : public TypedNode {
public:
    explicit ImplicitValueInitExpression(const Type& type) : TypedNode(type) {}

    DECLARE_KIND(Node, Node::Kind::IMPLICIT_VALUE_INIT_LIST_EXPRESSION);
    DECLARE_SERIALIZE(ImplicitValueInitExpression, nullptr);
};

};  // namespace HCXX

#endif  // COODDY_IMPLICITVALUEINITLISTEXPRESSION_H
