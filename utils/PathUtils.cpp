/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
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

}  // namespace PathUtils
}  // namespace HCXX
