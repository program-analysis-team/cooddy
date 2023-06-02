/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_PATHUTILS_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_PATHUTILS_H_
#include <cassert>
#include <filesystem>
#include <string>
#include <string_view>

#include "StrUtils.h"
#include "UriEncoding.h"

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

enum class PathFormat {
    POSIX,
    WINDOWS,
    NATIVE =
#ifdef _WIN32
        WINDOWS,
#else
        POSIX,
#endif
};

std::string FileUri(std::string_view path, PathFormat format = PathFormat::NATIVE);

template <class T, class = std::enable_if_t<std::is_same_v<T, std::filesystem::path>>>
std::string FileUri(const T& path, PathFormat format = PathFormat::NATIVE);

template <>
std::string FileUri<std::filesystem::path, void>(const std::filesystem::path& path, PathFormat format);

}  // namespace PathUtils
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_PATHUTILS_H_
