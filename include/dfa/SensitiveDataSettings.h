/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_SENSITIVEDATASETTINGS_H_
#define COODDY_ANALYZER_INCLUDE_DFA_SENSITIVEDATASETTINGS_H_

#include <utils/EnumMapping.h>
#include <utils/Log.h>
#include <utils/json-cpp.h>

struct SensitiveDataSettings {
    bool sensiInfoSummary;

    DECLARE_FIELDS("sensi_info_summary", sensiInfoSummary);  // LCOV_EXCL_LINE
};

#endif  // COODDY_ANALYZER_INCLUDE_DFA_SENSITIVEDATASETTINGS_H_
