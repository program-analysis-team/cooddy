/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_THISEXPRESSION_H
#define COODDY_THISEXPRESSION_H

#include <ast/CxxMethodDecl.h>

#include "TypedNode.h"

namespace HCXX {

class ThisExpression : public TypedNode {
public:
    explicit ThisExpression(const Type& type) : TypedNode(type) {}

    DECLARE_KIND(TypedNode, Node::Kind::THIS_EXPRESSION);
    DECLARE_SERIALIZE(ThisExpression, nullptr);

    const Node* GetDeclaration() const override
    {
        return CxxMethodDecl::GetThisDecl();
    }
};

}  // namespace HCXX
#endif  // COODDY_THISEXPRESSION_H
