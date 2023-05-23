/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include "utils/EnvironmentUtils.h"

#include <codecvt>
#include <cwchar>
#include <fstream>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "deps/whereami.h"
#include "utils/Log.h"
#include "utils/StrUtils.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN 1
#define WIN32_EXTRA_LEAN 1

#define NOGDICAPMASKS 1
#define NOVIRTUALKEYCODES 1
#define NOWINMESSAGES 1
#define NOWINSTYLES 1
#define NOSYSMETRICS 1
#define NOMENUS 1
#define NOICONS 1
#define NOKEYSTATES 1
#define NOSYSCOMMANDS 1
#define NORASTEROPS 1
#define NOSHOWWINDOW 1
#define OEMRESOURCE 1
#define NOATOM 1
#define NOCLIPBOARD 1
#define NOCOLOR 1
#define NOCTLMGR 1
#define NODRAWTEXT 1
#define NOGDI 1
#define NOKERNEL 1
#define NOUSER 1
#define NONLS 1
#define NOMB 1
#define NOMEMMGR 1
#define NOMETAFILE 1
#define NOMINMAX 1
#define NOMSG 1
#define NOOPENFILE 1
#define NOSCROLL 1
#define NOSERVICE 1
#define NOSOUND 1
#define NOTEXTMETRIC 1
#define NOWH 1
#define NOWINOFFSETS 1
#define NOCOMM 1
#define NOKANJI 1
#define NOHELP 1
#define NOPROFILER 1
#define NODEFERWINDOWPOS 1
#define NOMCX 1

struct IUnknown;
#include <Windows.h>
#include <libloaderapi.h>
#include <shellapi.h>
#endif

namespace fs = std::filesystem;

namespace HCXX {
namespace EnvironmentUtils {
namespace {

/// Set from Init() for retrieving via Argv().
std::vector<const char*> g_argv = {};

/// Convert argv to UTF-8 on all platforms.
// LCOV_EXCL_START
void ArgvToUnicode(const char* const*& argv)
{
#ifdef _WIN32
    static auto argvVec = [] {
        static auto argvStrings = [] {
            auto deleter = [](wchar_t** ptr) { LocalFree(ptr); };

            int argc;
            std::unique_ptr<wchar_t* [], decltype(deleter)> wargv {
                CommandLineToArgvW(GetCommandLineW(), &argc), deleter
            };

            if (!wargv) {
                throw std::runtime_error("CommandLineToArgvW failed");
            }

            std::vector<std::string> argvStringsResult;
            argvStringsResult.reserve(argc);
            for (int i = 0; i < argc; ++i) {
                argvStringsResult.push_back(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(
                    wargv[i], wargv[i] + std::wcslen(wargv[i])));
            }

            return argvStringsResult;
        }();

        std::vector<const char*> argvVecResult;
        argvVecResult.reserve(argvStrings.size());

        for (auto& arg : argvStrings) {
            argvVecResult.push_back(arg.data());
        }

        return argvVecResult;
    }();

    argv = argvVec.data();
#endif
}
// LCOV_EXCL_STOP

/// Try retrieving path using whereami library (will fail in chroot environments)
fs::path GetSelfExecutablePathViaWAI()
{
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    if (length > 0) {
        std::string result(length + 1, '\0');
        wai_getExecutablePath(result.data(), length, nullptr);
        return fs::canonical(result);
    }

    return {};  // LCOV_EXCL_LINE
}

// LCOV_EXCL_START
/// Try retrieving path using argv[0] (will fail when calling from env or when argv is modified)
fs::path GetSelfExecutablePathViaArgv()
{
    if (Argv().empty()) {
        std::cerr << "Warning: GetSelfExecutablePathViaArgv called but argv was not initialized\n";
        return {};
    }

    std::error_code ec;
    fs::path result = fs::canonical(Argv()[0], ec);
    if (!ec) {
        return result;
    }

    return {};
}

/// Try retrieving path using getenv("_") (will fail if the shell does not support this functionality)
fs::path GetSelfExecutablePathViaGetenv()
{
    const char* envValue = std::getenv("_");
    if (envValue) {
        std::error_code ec;
        fs::path result = fs::canonical(envValue, ec);
        if (!ec) {
            return result;
        }
    }

    return {};
}
// LCOV_EXCL_STOP

const fs::path& GetSelfExecutablePathImpl(std::string_view nameHint)
{
    static const fs::path result = [nameHint] {
        fs::path p;

        p = GetSelfExecutablePathViaWAI();
        if (!p.empty()) {
            if (nameHint.empty() || p.stem() == nameHint) {
                return p;
            }

            // LCOV_EXCL_START
            std::cerr << "Warning: Executable path from WAI does not make sense: " << p << "\n";
        }

        p = GetSelfExecutablePathViaArgv();
        if (!p.empty()) {
            if (nameHint.empty() || p.stem() == nameHint) {
                return p;
            }

            std::cerr << "Warning: Executable path from argv[0] does not make sense: " << p << "\n";
        }

        p = GetSelfExecutablePathViaGetenv();
        if (!p.empty()) {
            if (nameHint.empty() || p.stem() == nameHint) {
                return p;
            }

            std::cerr << "Warning: Executable path from getenv(\"_\") does not make sense: " << p << "\n";
        }

        throw std::runtime_error(
            "could not obtain executable path.\n"
            "Suggestion: try launching cooddy by specifying an absolute path in the command line.");
        // LCOV_EXCL_STOP
    }();

    return result;
}

}  // namespace

// LCOV_EXCL_START
void InitArgv(int argc, const char* const*& argv)
{
    ArgvToUnicode(argv);
    g_argv.assign(argv, argv + argc);
}

void InitSelfExecutablePath(std::string_view nameHint)
{
    GetSelfExecutablePathImpl(nameHint);
}

const fs::path& GetSelfExecutablePath()
{
    return GetSelfExecutablePathImpl("");
}

const fs::path& GetSelfExecutableDir()
{
    static const fs::path result = GetSelfExecutablePath().parent_path();
    return result;
}

std::vector<std::string> GetExecutablePaths(const std::string& binaryName, uint32_t limit)
{
    std::vector<std::string> result;
    if (fs::path(binaryName).is_absolute()) {
        result.push_back(binaryName);
        return result;
    }

    auto pathEnv = std::getenv("PATH");
    if (pathEnv == nullptr) {
        // LCOV_EXCL_START
        return result;
        // LCOV_EXCL_STOP
    }
    char separator = ':';
    std::string extension;
#ifdef WIN32
    separator = ';';
    if (fs::path(binaryName).extension().empty()) {
        extension = ".exe";
    }
#endif

    for (const auto& path : StrUtils::Split(pathEnv, separator)) {
        fs::path tmpPath(path);
        tmpPath.append(binaryName + extension);
        if (fs::exists(tmpPath)) {
            result.push_back(tmpPath.string());
            if (result.size() >= limit) {
                break;
            }
        }
    }

    return result;
}

std::string NormalizePath(const std::string& path)
{
    std::string result;
    result.reserve(path.size());
    for (auto c : path) {
        result.push_back(c == '\\' ? '/' : c);
    }
    return result;
}

std::string GetCanonicalPath(const std::string& path)
{
    fs::path filePath(path);
    std::error_code ec;
    auto canonicalPath = fs::canonical(filePath, ec);
    if (ec) {
        canonicalPath = path;
    }
    return canonicalPath.string();
}

std::string GetInstalledJarPath(const std::string& jarName)
{
    auto execPath = HCXX::EnvironmentUtils::GetSelfExecutablePath();
    auto jarPath = execPath.parent_path() / ".cooddy" / jarName;  // TODO: replace this hardcode with parser options
    return jarPath.string();
}

// LCOV_EXCL_START
tcb::span<const char* const> Argv()
{
    return g_argv;
}
// LCOV_EXCL_STOP

}  // namespace EnvironmentUtils

#ifdef _WIN32
uint32_t GetMaxThreadsCount()
{
    return std::thread::hardware_concurrency();
}
#else
/*
 * Returns the number of concurrent threads supported by the OS environment.
 * Also this function takes into account total available RAM and ensures that each thread can use at least 2Gb of RAM
 * For example, if there are 64 CPUs and 32Gb of RAM, than this function will return 17 (=min(64, 32/2 + 1)).
 */
uint32_t GetMaxThreadsCount()
{
    uint64_t totalMemSize = UINT64_MAX;
    std::ifstream memInfo("/proc/meminfo");
    for (std::string token; memInfo >> token; memInfo.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
        if (token == "MemTotal:") {
            memInfo >> totalMemSize;
            totalMemSize *= 1024;
            break;
        }
    }
    uint64_t memLimitSize = UINT64_MAX;
    std::ifstream memLimit("/sys/fs/cgroup/memory/memory.limit_in_bytes");
    memLimit >> memLimitSize;

    uint32_t maxThreads = std::thread::hardware_concurrency();
    Log(LogLevel::INFO) << "System info: mem_size=" << totalMemSize << ", mem_limit=" << memLimitSize
                        << ", cpu_count=" << maxThreads << "\n";

    return std::min(maxThreads, uint32_t(std::min(totalMemSize, memLimitSize) / INT32_MAX) + 1);
}
#endif

}  // namespace HCXX
