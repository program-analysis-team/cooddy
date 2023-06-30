/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CompilersInfo.h"

#include <utils/LocaleUtils.h>
#include <utils/StrUtils.h>

#ifndef _MSC_VER
#include <unistd.h>
#else
#include <process.h>
#endif
#include <utils/EnvironmentUtils.h>
#include <utils/Log.h>
#include <utils/StrUtils.h>

#include <chrono>
#include <cstring>
#include <fstream>
#include <memory>
#include <thread>

namespace HCXX {

CompilersInfo::CompilersInfo() : myDefaultCCompilerName(GetBinaryPath("cc")) {}

std::string CompilersInfo::GetBinaryPath(const std::string& binaryName) const
{
    auto compilerPaths = EnvironmentUtils::GetExecutablePaths(binaryName, 1);
    return compilerPaths.empty() ? "" : compilerPaths[0];
}

bool CompilersInfo::IsCorrectCompilerBinary(const std::string& binary)
{
    if (binary.empty()) {
        return false;
    }
    auto absolutePath = GetBinaryPath(binary);
    return !absolutePath.empty() && std::filesystem::exists(absolutePath) &&
           !std::filesystem::is_directory(absolutePath);
}

bool CompilersInfo::ShouldAddCompilerMacro(const std::string& define)
{
    static std::array<const char*, 8> stdDefines{"__clang__", "__linux__",     "__GNU",      "_GNU_SOURCE",
                                                 "__SIZE",    "__GCC_ATOMIC_", "__INT_FAST", "__UINT_FAST"};
    for (auto& it : stdDefines) {
        if (HCXX::StrUtils::StartsWith(define, it)) {
            return true;
        }
    }
    return false;
}

CompilersInfo::CompilerInfo CompilersInfo::CollectNewCompilerInfo(const std::string& compiler,
                                                                  std::string&& extraOptions)
{
    std::filesystem::path compilerInfoPath = std::filesystem::temp_directory_path();

    std::stringstream ss;
    ss << std::this_thread::get_id() << "_" << std::time(nullptr) << "_" << getpid();
    compilerInfoPath.append("compiler_" + ss.str() + ".info");

    std::string cmdCompilerExec =
        "echo | \"" + GetBinaryPath(compiler) + "\" -v -E -dM -Wp,-v " + extraOptions + " - 2>";
    cmdCompilerExec += "\"" + compilerInfoPath.string() + "\"  1>&2";

    int result = std::system(cmdCompilerExec.c_str());
    if (result != 0) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "There were error, while running: " << cmdCompilerExec << std::endl;
        // LCOV_EXCL_STOP
    }

    std::ifstream optionsStream(compilerInfoPath);
    CompilerInfo info;
    bool foundIncludes = false;
    for (std::string sLine; std::getline(optionsStream, sLine);) {
        if (sLine == "End of search list.") {
            foundIncludes = false;
        }
        if (StrUtils::StartsWith(sLine, "#define")) {
            auto define = sLine.substr(8);
            if (HCXX::StrUtils::StartsWith(define, "__GNUC__ 1")) {
                define = "__GNUC__ 10";  // clang supports features up to 10 gnu version
            }
            if (auto p = define.find(' '); p != std::string::npos) {
                define[p] = '=';
            }
            if (ShouldAddCompilerMacro(define)) {
                info.defines.emplace_back(define);
            }
        }
        if (foundIncludes) {
            while (!sLine.empty() && sLine.front() == ' ') {
                sLine.erase(sLine.begin());
            }
            info.includes.emplace_back(sLine);
        } else if (sLine.find("#include <...>") != std::string::npos) {
            foundIncludes = true;
        } else if (StrUtils::StartsWith(sLine, "Target: ")) {
            info.triple = sLine.substr(std::strlen("Target: "));
        }
    }
    optionsStream.close();
    std::remove(compilerInfoPath.string().c_str());
    return info;
}

CompilersInfo::CompilerInfo& CompilersInfo::GetCompilerInfo(CompilerOptions& compilerOptions)
{
    std::string compilerPath = compilerOptions.compiler;
    if (!IsCorrectCompilerBinary(compilerPath)) {
        compilerPath = myDefaultCCompilerName;
    }
    auto stdOpt = compilerOptions.GetOptionValue("-std");
    auto langOpt = compilerOptions.HasOption("-xc")     ? "c"
                   : compilerOptions.HasOption("-xc++") ? "c++"
                                                        : compilerOptions.GetOptionValue("-x");
    if (langOpt.empty()) {
        langOpt = compilerOptions.compiler.find("++") != std::string::npos || stdOpt.find("++") != std::string::npos ||
                          !HCXX::StrUtils::EndsWith(compilerOptions.GetFilePath(), ".c")
                      ? "c++"
                      : "c";
        compilerOptions.options.insert(compilerOptions.options.end(), {"-x", langOpt});
    }
    std::string extraOptions(" -x " + langOpt);
    if (auto sysRoot = compilerOptions.GetOptionValue("--sysroot"); !sysRoot.empty()) {
        extraOptions += " --sysroot=" + sysRoot;
    }
    if (!stdOpt.empty()) {
        extraOptions += " -std=" + stdOpt;
    }
    if (compilerOptions.HasOption("-m32")) {
        extraOptions += " -m32";
    }
    std::lock_guard<std::mutex> lock(myMutex);
    auto it = myCompilers.emplace(compilerPath + extraOptions, CompilerInfo());
    if (it.second) {
        it.first->second = CollectNewCompilerInfo(compilerPath, std::move(extraOptions));
    }
    return it.first->second;
}

}  // namespace HCXX
