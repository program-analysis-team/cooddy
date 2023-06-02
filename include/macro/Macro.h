/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_MACRO_H
#define COODDY_MACRO_H

#include <macro/MacroExpansion.h>
#include <utils/EnumMapping.h>

namespace HCXX {

struct Macro {
    SourceRange declSourceRange;
    SourceRange nameSourceRange;
    unsigned numParams = 0;
    std::vector<MacroExpansion> macroExpansions;
    bool isSystem = false;
};

}  // namespace HCXX

#endif  // COODDY_MACRO_H
