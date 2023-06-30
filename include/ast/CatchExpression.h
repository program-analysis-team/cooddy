/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CATCHEXTPRESSION_H
#define COODDY_CATCHEXTPRESSION_H

#include "Node.h"

namespace HCXX {

class CatchExpression : public HCXX::CompoundNode {
public:
    DECLARE_KIND(CompoundNode, Node::Kind::CATCH_EXPRESSION);
    DECLARE_SERIALIZE(CatchExpression, nullptr);
};

};  // namespace HCXX

#endif  // COODDY_CATCHEXTPRESSION_H
