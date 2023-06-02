/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_COMMENT_H
#define COODDY_COMMENT_H

#include <ast/Node.h>
#include <utils/EnumMapping.h>

namespace HCXX {

struct Comment {
    DECLARE_ENUM(Kind, COMMON, C_STYLE, CXX_STYLE, SUPPRESS);
};

}  // namespace HCXX

#endif  // COODDY_COMMENT_H
