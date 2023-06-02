/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_THROWEXTPRESSION_H
#define COODDY_THROWEXTPRESSION_H

#include "Node.h"

namespace HCXX {

class ThrowExpression : public HCXX::Node {
public:
    DECLARE_KIND(Node, Node::Kind::THROW_EXPRESSION);
    DECLARE_SERIALIZE(ThrowExpression, nullptr);
};

};  // namespace HCXX

#endif  // COODDY_THROWEXTPRESSION_H
