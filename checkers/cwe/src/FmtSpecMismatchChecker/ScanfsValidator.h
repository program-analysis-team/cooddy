/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_SCANFSVALIDATOR_H_
#define COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_SCANFSVALIDATOR_H_
#include <utils/StrUtils.h>

#include <deps/ctre.hpp>

#include "ScanfValidator.h"

struct ScanfsValidator : ScanfValidator {
    std::vector<ParseResult> ParseFormatString(std::string_view formatString) override
    {
        using namespace FmtTypes;
        auto result = ScanfValidator::ParseFormatString(formatString);

        // make a mutable pointer out of an argument
        auto p = [](FmtType type) constexpr {
            type.quals = FmtType::Quals::MUTABLE | FmtType::Quals::POINTER;
            return type;
        };

        // For each of the c/s/[ format parameters, expect a USIZE with the buffer size afterwards.
        for (int i = 0; i < result.size(); ++i) {
            if (result[i].type == p(CHAR) || result[i].type == p(WCHAR)) {
                result.insert(result.begin() + i + 1, {USIZE, result[i].match, true});
                ++i;
            }
        }

        return result;
    }

    Detect Validate(FmtType actual, FmtType expected) override
    {
        // Handle the auxiliary argument for string size
        if (expected == FmtTypes::USIZE) {
            return ValidateUnsignedInt(actual, expected);
        }

        return ScanfValidator::Validate(actual, expected);
    }
};

#endif  // COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_SCANFSVALIDATOR_H_
