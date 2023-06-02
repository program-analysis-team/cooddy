/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_LOCALEUTILS_H
#define COODDY_LOCALEUTILS_H

#include <string>
#include <string_view>
#include <vector>

namespace HCXX {

namespace StrLocales {

std::string GetStringLocale(std::string_view id,
                            const std::vector<std::string>& replacements = std::vector<std::string>(),
                            const bool hideOutput = false);

/**
 * @brief Load locale into an internal string map from a language identifier, such as "EN".
 *
 * Language identifier must be present in the first row of Strings.csv.
 *
 * @return true if either a locale or a fallback locale was loaded successfully.
 */
bool LoadLocale(std::string_view locale);

}  // namespace StrLocales
}  // namespace HCXX

#endif  // COODDY_LOCALEUTILS_H
