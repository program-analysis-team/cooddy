/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include <stdexcept>

#include "Cli.h"
#include "CrashHandler.h"
#include "utils/EnvironmentUtils.h"

// COODDY_SUPPRESS FunctionLengthChecker
int main(int argc, const char* const* argv)
{
    SetCrashHandler();
    try {
        HCXX::EnvironmentUtils::InitArgv(argc, argv);
        HCXX::EnvironmentUtils::InitSelfExecutablePath("cooddy");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to initialize environment: " << e.what() << "\n";
        return ExitStatus::FAILURE;
    }

    if (argc < 2) {
        std::cerr << "Parameter '--scope' should be provided\n"
                  << "Use --help to get more information." << std::endl;
        return ExitStatus::BAD_COMMANDLINE;
    }

    auto cmdConfig = ConstructOptions();
    cxxopts::ParseResult parsedArgs;

    try {
        parsedArgs = cmdConfig.parse(argc, argv);
    } catch (const std::exception& err) {
        //  cxxopts::Options::parse throws exception if unsupported argument is provided.
        std::cerr << err.what() << "\nUse --help to get more information.\n";
        return ExitStatus::FAILURE;
    }

    if (parsedArgs.count("help")) {
        std::cout << cmdConfig.help() << std::endl;
        return ExitStatus::SUCCESS;
    }

    std::stringstream versionInfo;
    versionInfo << "Version: " << GIT_VERSION;
    if (parsedArgs.count("version")) {
        std::cout << versionInfo.str() << std::endl;
        return ExitStatus::SUCCESS;
    }

    if (!HCXX::StrLocales::LoadLocale(parsedArgs["locale"].as<std::string>())) {
        return ExitStatus::BAD_COMMANDLINE;
    }

    SetLogLevel(parsedArgs["log"].as<std::string>());
    Log(LogLevel::INFO) << versionInfo.str() << std::endl;

    ReportUnknownArgs(parsedArgs);

    HCXX::Workspace::Init();
    return RunAnalyses(parsedArgs);
}
