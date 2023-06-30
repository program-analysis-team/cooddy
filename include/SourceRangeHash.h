/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SOURCERANGEHASH_H
#define COODDY_SOURCERANGEHASH_H

#include <ast/Node.h>

#include "utils/HashImpl.h"

using namespace HCXX;

struct SourceRangeHash {
    size_t operator()(const SourceRange& t) const
    {
        return CalcHash(t.begin, t.end);
    }
};

#endif  // COODDY_SOURCERANGEHASH_H
