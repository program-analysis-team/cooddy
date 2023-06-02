/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/InitListExpression.h>
#include <ast/ParamVarDecl.h>
#include <ast/StringLiteralExpression.h>

namespace HCXX {
bool ParamVarDecl::IsMemoryBuffer(uint32_t& sizeofBuffer) const
{
    auto typedNode = Node::Cast<TypedNode>(myArraySizeExpr);
    if (typedNode == nullptr || !typedNode->IsConstExpr()) {
        return false;
    }
    sizeofBuffer = typedNode->GetSLimitedValue() * GetType().GetPointedType().GetSizeInBits() / 8;
    return true;
}
};  // namespace HCXX
