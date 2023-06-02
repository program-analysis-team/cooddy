/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxNewExpression.h>

namespace HCXX {

uint32_t CxxNewExpression::GetNumOfElementsInArray() const
{
    if (myArrayElemCount != nullptr) {
        auto node = myArrayElemCount->GetInnerNode();
        if (node != nullptr) {
            auto intLiteral = Node::Cast<TypedNode>(node);
            if (intLiteral != nullptr) {
                return intLiteral->GetSLimitedValue();
            }
        }
    }
    return 0;  // LCOV_EXCL_LINE: HCAT-2777
}
};  // namespace HCXX
