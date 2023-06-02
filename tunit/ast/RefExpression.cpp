/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/RefExpression.h>

#include "ast/VarDecl.h"

namespace HCXX {
void RefExpression::Print(std::string& source, int entryOffset) const
{
    std::string name = VarDecl::GetVarName(myDeclaration);
    SafePrint(source, entryOffset, name, GetRange());
}
};  // namespace HCXX
