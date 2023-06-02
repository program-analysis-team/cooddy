/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "utils/PathUtils.h"

#include <filesystem>

#include "utils/StrUtils.h"

namespace fs = std::filesystem;

namespace HCXX {
namespace PathUtils {

bool IsAbsolute(const fs::path& p)
{
    return p.is_absolute() || IsUnc(p);
}

bool IsRelative(const fs::path& p)
{
    return !IsAbsolute(p);
}

bool IsUnc(const fs::path& p)
{
    return StrUtils::StartsWith(p.string(), "\\\\");
}

std::string FileUri(std::string_view path, PathFormat format)
{
    std::string result = "file://";

    std::string commonSegment;
    switch (format) {
        case PathFormat::POSIX: {
            commonSegment = path;
        }
        case PathFormat::WINDOWS: {
            bool isAbsolute = path.size() >= 2 && path[1] == ':';

            if (isAbsolute) {
                result.push_back('/');                    // Forward slash indicating absolute path
                result.push_back(std::toupper(path[0]));  // Drive letter
                result.push_back(path[1]);                // Drive colon (appended as-is per windows rules)

                commonSegment = path.substr(2);
            } else {
                commonSegment = path;
            }

            commonSegment = StrUtils::Replace(commonSegment, "\\", "/");

            break;
        }
        // LCOV_EXCL_START: Unhandled PathFormat value
        default:
            assert(false);
            __builtin_unreachable();
            // LCOV_EXCL_STOP
    }

    result += UrlEncode(commonSegment, PATH_RESERVED);
    return result;
}

template <>
std::string FileUri<std::filesystem::path, void>(const std::filesystem::path& path, PathFormat format)
{
    return FileUri(path.string(), format);
}

}  // namespace PathUtils
}  // namespace HCXX
