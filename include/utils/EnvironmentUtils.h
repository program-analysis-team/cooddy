/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_ENVIRONMENTUTILS_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_ENVIRONMENTUTILS_H_
#include <deps/span.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace HCXX::EnvironmentUtils {

/// Enable calling Argv() and an argv[0]-based name lookup for GetSelfExecutablePath.
void InitArgv(int argc, const char* const*& argv);

/// Pre-cache InitSelfExecutablePath and -Dir. nameHint allows to verify the executable name as a sanity check.
void InitSelfExecutablePath(std::string_view nameHint = "");

/// Search for binaryName, by iterating over environmental variable PATH
/// limit - is maximum number of required paths of binary. By default all found binaries from PATH will be returned
/// returns vector of found paths in same order, as declared in PATH
std::vector<std::string> GetExecutablePaths(const std::string& binaryName, uint32_t limit = UINT32_MAX);

/// Returns absolute path to executable file/directory.
const std::filesystem::path& GetSelfExecutablePath();
const std::filesystem::path& GetSelfExecutableDir();

/// returns the full path to jar-file installed with cooddy
std::string GetInstalledJarPath(const std::string& jarName);

std::string NormalizePath(const std::string& path);
std::string GetCanonicalPath(const std::string& path);

tcb::span<const char* const> Argv();

}  // namespace HCXX::EnvironmentUtils

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_ENVIRONMENTUTILS_H_
