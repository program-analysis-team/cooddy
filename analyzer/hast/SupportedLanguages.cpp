/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "SupportedLanguages.h"

namespace HCXX {

std::optional<SupportedLanguages> SupportedLanguages::FromArr(const std::vector<std::string>& vec)
{
    unsigned int read = 0;
    for (const auto& value : vec) {
        unsigned int item = 0;
        if (value == "c") {
            item = LANG_C;  // LCOV_EXCL_LINE: there is no HAST-based checker for c lang
        } else if (value == "cpp") {
            item = LANG_CPP;
        } else {
            //  LCOV_EXCL_START
            return std::nullopt;
            //  LCOV_EXCL_STOP
        }

        if (read & item) {
            //  At least one language is specified twice
            //  LCOV_EXCL_START
            return std::nullopt;
            //  LCOV_EXCL_STOP
        }

        read |= item;
    }

    return SupportedLanguages(read);
}

SupportedLanguages::SupportedLanguages(int mask) : myMask(mask) {}

bool SupportedLanguages::IsCppLanguageSupported() const
{
    return myMask & LANG_CPP;
}

bool SupportedLanguages::IsPureCLanguageSupported() const
{
    return myMask & LANG_C;
}

}  //  namespace HCXX
