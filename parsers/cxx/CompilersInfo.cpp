/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include "CompilersInfo.h"

#include <utils/LocaleUtils.h>

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

CompilersInfo::CompilersInfo()
    : myDefaultCCompilerName(GetBinaryPath("cc")), myDefaultCxxCompilerName(GetBinaryPath("c++"))
{}

std::string CompilersInfo::GetBinaryPath(const std::string& binaryName) const
{
    auto compilerPaths = EnvironmentUtils::GetExecutablePaths(binaryName, 1);
    return compilerPaths.empty() ? "" : compilerPaths[0];
}

bool CompilersInfo::IsCorrectCompilerBinary(const std::string& binary)
{
    auto absolutePath = GetBinaryPath(binary);
    return !absolutePath.empty() && std::filesystem::exists(absolutePath) &&
           !std::filesystem::is_directory(absolutePath);
}

bool CompilersInfo::IsCPPCompiler(const std::string& compiler) const
{
    return std::filesystem::path(compiler).filename().string().find("++") != std::string::npos;
}

CompilersInfo::CompilerInfo CompilersInfo::CollectNewCompilerInfo(const std::string& compiler,
                                                                  const std::string& sysRoot)
{
    std::filesystem::path curPath = std::filesystem::current_path();
    std::filesystem::path compilerInfoPath = curPath;

    std::stringstream ss;
    ss << std::this_thread::get_id() << "_" << std::time(nullptr) << "_" << getpid();

    compilerInfoPath.append("compiler_" + ss.str() + ".info");

    std::string extraOptions = IsCPPCompiler(compiler) ? "-x c++" : "-x c";
    if (!sysRoot.empty()) {
        extraOptions += " --sysroot=" + sysRoot;
    }
    std::string cmdCompilerExec = "echo | \"" + GetBinaryPath(compiler) + "\" -v -E -Wp,-v " + extraOptions + " - 2>";
    cmdCompilerExec += "\"" + compilerInfoPath.string() + "\"  1>&2";

    int result = std::system(cmdCompilerExec.c_str());
    if (result != 0) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "There were error, while running: " << cmdCompilerExec << std::endl;
        // LCOV_EXCL_STOP
    }

    std::ifstream optionsStream(compilerInfoPath);
    std::vector<std::string> compilerIncludes;
    std::string compilerVersion;
    std::string target;
    bool foundIncludes = false;
    for (std::string sLine; std::getline(optionsStream, sLine);) {
        if (sLine == "End of search list.") {
            break;
        }
        if (foundIncludes) {
            while (!sLine.empty() && sLine.front() == ' ') {
                sLine.erase(sLine.begin());
            }
            compilerIncludes.emplace_back(sLine);
        } else if (sLine.find("#include <...>") != std::string::npos) {
            foundIncludes = true;
        } else if (auto pos = sLine.find("version "); pos != std::string::npos) {
            auto n = std::string("version ").size();
            compilerVersion = sLine.substr(pos + n, sLine.find(".", pos + n) - pos - n);
        } else if (StrUtils::StartsWith(sLine, "Target: ")) {
            target = sLine.substr(std::strlen("Target: "));
        }
    }
    optionsStream.close();
    std::remove(compilerInfoPath.string().c_str());

    if (!foundIncludes) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "Buildin compiler includes not found" << std::endl;
        // LCOV_EXCL_STOP
    }

    return CompilerInfo{std::move(compilerIncludes), std::move(compilerVersion), std::move(target),
                        IsCPPCompiler(compiler)};
}

std::string CompilersInfo::GetCompilerBinary(const CompilerOptions& compilerOptions)
{
    if (compilerOptions.compiler.empty() || !IsCorrectCompilerBinary(compilerOptions.compiler)) {
        const char* envPath = std::getenv("CXX_COMPILER_PATH");
        std::string compilerPath = envPath != nullptr ? envPath : "";
        if (!compilerPath.empty() && IsCorrectCompilerBinary(compilerPath)) {
            return compilerPath;  // LCOV_EXCL_LINE
        }

        if (!compilerOptions.GetFilePath().empty() && !std::filesystem::is_directory(compilerOptions.GetFilePath()) &&
            (std::filesystem::path(compilerOptions.GetFilePath()).extension() == ".c" ||
             !IsCorrectCompilerBinary(myDefaultCxxCompilerName))) {
            return myDefaultCCompilerName;

        } else {
            return myDefaultCxxCompilerName;
        }
    }
    return compilerOptions.compiler;  // LCOV_EXCL_LINE
}

CompilersInfo::CompilerInfo& CompilersInfo::GetCompilerInfo(const CompilerOptions& compilerOptions)
{
    std::string compilerPath = compilerOptions.compiler;
    if (compilerPath.empty() || !IsCorrectCompilerBinary(compilerPath)) {
        compilerPath = GetCompilerBinary(compilerOptions);
    }
    auto sysRoot = compilerOptions.GetOptionValue("--sysroot");

    std::lock_guard<std::mutex> lock(myMutex);
    auto it = myCompilers.emplace(compilerPath + sysRoot, CompilerInfo());
    if (it.second) {
        it.first->second = CollectNewCompilerInfo(compilerPath, sysRoot);
    }
    return it.first->second;
}

}  // namespace HCXX
