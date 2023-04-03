/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_PATHUTILS_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_PATHUTILS_H_
#include <filesystem>

namespace HCXX {
namespace PathUtils {

/// Supercedes std::filesystem::path::is_absoulute, recognises UNC paths (as absoulute).
bool IsAbsolute(const std::filesystem::path& p);

/// Supercedes std::filesystem::path::is_relative, recognises UNC paths (as absoulute).
bool IsRelative(const std::filesystem::path& p);

/**
 * @brief a UNC path is a Windows standard for absolute paths pointing to network resources.
 *
 * See https://learn.microsoft.com/en-us/dotnet/standard/io/file-path-formats#unc-paths
 */
bool IsUnc(const std::filesystem::path& p);

}  // namespace PathUtils
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_PATHUTILS_H_
