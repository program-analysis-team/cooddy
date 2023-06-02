/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_ALLOCSOURCECHECKER_H_
#define COODDY_ANALYZER_INCLUDE_DFA_ALLOCSOURCECHECKER_H_
#include <cstdint>

// COODDY_SUPPRESS FileNameAsTopLevelRecordNameChecker
struct AllocUserData {
    enum { ALLOC_FUNC = 1 };
    int allocType;
    bool isPassByRef = false;
    bool isStatic = false;
};

struct FreeUserData {
    bool isFromAliasState = false;
};

#endif  // COODDY_ANALYZER_INCLUDE_DFA_ALLOCSOURCECHECKER_H_
