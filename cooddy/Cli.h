/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_TOOL_CLI_H_
#define COODDY_ANALYZER_TOOL_CLI_H_

#define CXXOPTS_NO_RTTI

#include <Analyzer.h>
#include <CompilerOptionsList.h>
#include <dfa/TaintSettings.h>
#include <utils/CsvUtils.h>
#include <utils/LocaleUtils.h>
#include <utils/Log.h>
#include <workspace/Profile.h>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iostream>
#include <regex>
#include <unordered_set>

#include "CallChainConsumer.h"
#include "CallGraphConsumer.h"
#include "EntryFuncConsumer.h"
#include "MangledNameFinder.h"
#include "ProblemsHolder.h"
#include "TaintMacroCliDefinition.h"
#include "deps/cxxopts.hpp"
#include "reporters/CompositeReporter.h"
#include "reporters/OutReporter.h"
#include "workspace/GenAnnoSettings.h"

// COODDY_SUPPRESS FunctionLengthChecker
inline cxxopts::Options ConstructOptions()
{
    // clang-format off

    cxxopts::Options options("Cooddy", "Static analyzer of C, C++  and Java source code");
    options.set_width(120);  // Wrap lines in --help at 120 col
    // Line width for description appears to be 88 chars
    options.add_options()
    ("scope", "path to file, folder, compile_commands.json or ast-file to scan", cxxopts::value<std::string>())
    ("compile-options", "<-flag1>,<-flag2>,...,<-flagN>", cxxopts::value<vector<string>>()->default_value(""))
    ("gen-annotations", "control for which functions annotations should be generated:\n"
                        "  none - generation of annotation is turned off;\n"
                        "  all  - all functions;\n"
                        "  u    - functions that do not have a definition in the TU;\n"
                        "  t    - functions are the source of untrusted data;\n"
                        "  s    - functions from system libraries;\n",
        cxxopts::value<std::string>()->default_value("none"))
    ("gen-annotations-path", "path for generated annotations",
        cxxopts::value<std::string>()->default_value(std::string{WorkspaceOptions::GEN_ANNOTATIONS_DEFAULT_PATH}))
    ("log", "<off|fatal|error|warn|warning|info|debug|trace|all>", cxxopts::value<std::string>()->default_value("info"))
    ("checkers", "<checker1>,<checker2>,...,<checkerN>",
        cxxopts::value<std::vector<std::string>>()->default_value("all"))
    ("compiler", "path to compiler executable", cxxopts::value<std::string>()->default_value(""))
    ("reporter",
        "name of the one of built-in reporter: <out|json|json-html|json-code|csv|csv-html> or combination of reporters with comma separator. Also name os custom reporter is possible: in smaller case to represent dynamic library included in th project. For example: \"--reporter=sample\" to represent libSampleReporter.dll.",
        cxxopts::value<vector<string>>()->default_value("out"))
    ("gen-callchain", "generate call graph paths for the specified entry point",
        cxxopts::value<std::string>()->default_value(""))
    ("callchain-depth", "max depth of the call chain path",
        cxxopts::value<int>()->default_value("100"))
    ("results-path", "path where report will be stored",
        cxxopts::value<std::string>()->default_value(std::filesystem::current_path().string()))
    ("profile", "path to file with inspections profile",
        cxxopts::value<std::string>()->default_value("default"))
    ("min-problem-severity", "minimal severity of problem to report.\n  possible values: <error|warning|notify>",
        cxxopts::value<std::string>()->default_value("notify"))
    ("project-root", "path to project root in report",
        cxxopts::value<std::string>()->default_value(""))
    ("incremental",
        "incremental analysis options:\n"
        "  off - incremental mode is turned off;\n"
        "  b   - use baseline analysis results for incremental analysis;\n"
        "  n   - report only problems not found in baseline results;\n"
        "  u   - update analysis results as a baseline results;\n"
        "pattern: <off|[b|n][u]>",
        cxxopts::value<std::string>()->default_value("off"))
    ("taint-options",
        "control which functions can be considered \"untrusted\":\n"
        "  none - taint analysis is turned off;\n"
        "  u    - functions that do not have a definition in the TU;\n"
        "  s    - functions marked with a macro specified in --taint-macro;\n"
        "  p    - public API functions (with a definition, unused from this TU, ext. linkage);\n"
        "  all  - all functions.\n"
        "pattern: <none|[u][s][p]|all>",
        cxxopts::value<std::string>()->default_value(std::string(HCXX::TaintSettings::DEFAULT_FLAGS)))
    ("taint-macro", "set macro used as an annotation to tainted functions",
        cxxopts::value<std::string>()->default_value("EXTER_ATTACK"))
    ("taint-macro-summary", "print summary of issues found in functions annotated by macro",
        cxxopts::value<bool>()->default_value("true"))
    ("taint-sde", "set macro used as an annotation to sensitive data exposure sources",
        cxxopts::value<std::string>()->default_value("SENSI_INFO"))
    ("analyze-with-fatal-errors",
        "analyze source file if the number of fatal parse errors not greater than the value of this option",
        cxxopts::value<uint32_t>()->default_value("20"))
    ("ignore-suppresses", "show suppressed warnings", cxxopts::value<bool>()->default_value("false"))
    ("parse-errors-log","path to log with parse errors", cxxopts::value<std::string>()->default_value(""))
    ("ast-dump", "serialize parsed source code in ast-file", cxxopts::value<bool>()->default_value("false"))
    ("export-call-graph", "Export call graph. No checkers will be executed.",
        cxxopts::value<bool>()->default_value("false"))
    ("entry-function", "Run analysis starting from specified entry function.\n"
             "Option 'call-graph-path' should be specified with call graph exported in csv format.",
        cxxopts::value<std::string>()->default_value(""))
    ("check-path-line", "Check paths with the join point on the specified line.\n"
             "This option is ignored if entry-function is not specified.", cxxopts::value<int>()->default_value("0"))
    ("mangle",
        "Find all functions with the specified name and output their mangled names.\n"
        "Mangled names can be used in annotations.json when two functions are not\n"
        "distinguishable by their name alone, such as with overloaded functions. Function name\n"
        "specified in this option follows C++ rules of name lookup:\n"
        "1. It can be an unqualified name, like \"begin\", in which case it will match all\n"
        "   \"begin\" functions in any class or namespace;\n"
        "2. It can be a partially-qualified name, such as \"iterator::begin\", in which case it\n"
        "   will match all \"begin\" functions in an \"iterator\" class or namespace anywhere;\n"
        "3. It can be a fully-qualified name, such as \"::std::begin\", in which case it will\n"
        "   only match \"begin\" functions in a global \"std\" scope.\n"
        "Analysis is not performed when this option is specified.",
        cxxopts::value<std::string>()->default_value(""))
    ("call-graph-path", "Path to a file to write call graph", cxxopts::value<std::string>()->default_value(""))
    ("locale", "setup localization of Cooddy, english by default", cxxopts::value<std::string>()->default_value("EN"))
    ("lang", "one of following value: <c|c++|java> ", cxxopts::value<std::string>()->default_value("default"))
    ("rhs-servers", "List of RHS server address", cxxopts::value<std::vector<std::string>>()->default_value("default"))
    ("v,version","Print version")
    ("h,help","Print usage");
    // clang-format on
    return options;
}

inline std::unordered_set<std::string> InitCheckersList(const std::vector<std::string>& checkers)
{
    std::unordered_set<std::string> result;

    if (checkers.size() == 1 && *checkers.begin() == "all") {
        return result;
    }

    for (const auto& checker : checkers) {
        result.emplace(checker);
    }

    return result;
}

inline void SetLogLevel(const string& level)
{
    HCXX::LogLevel logLevel = HCXX::LogLevel::INFO;
    if (level == "off") {
        logLevel = HCXX::LogLevel::OFF;
    } else if (level == "fatal") {
        logLevel = HCXX::LogLevel::FATAL;
    } else if (level == "error") {
        logLevel = HCXX::LogLevel::ERROR;
    } else if (level == "warn" || level == "warning") {
        logLevel = HCXX::LogLevel::WARNING;
    } else if (level == "info") {
        logLevel = HCXX::LogLevel::INFO;
    } else if (level == "debug") {
        logLevel = HCXX::LogLevel::DEBUG;
    } else if (level == "trace" || level == "all") {
        logLevel = HCXX::LogLevel::TRACE;
    }
    HCXX::Logger::SetLevel(logLevel);
}

struct ExitStatus {
    enum : std::uint8_t {
        SUCCESS = 0,
        FAILURE = 1,
        BAD_COMMANDLINE = 2,
    };
};

std::string GetSourceTextLine(const std::string& filePath, uint32_t line, uint32_t linesOfContext)
{
    auto file = HCXX::FileEntriesCache::GetInstance().GetFileEntry(filePath);

    if (file == nullptr || file->lineOffsets.empty() || line >= file->lineOffsets.size()) {
        return "";  // LCOV_EXCL_LINE
    }

    auto& lineOffsets = file->lineOffsets;

    uint32_t startPos = line > linesOfContext ? lineOffsets[line - linesOfContext] : lineOffsets[0];
    uint32_t endPos = line + linesOfContext >= lineOffsets.size() ? lineOffsets[lineOffsets.size() - 1]
                                                                  : lineOffsets[line + linesOfContext];
    return file->fileSource.substr(startPos, endPos - startPos);
}

inline void SaveParserErrorLog(HCXX::Parser& parser, const std::string& parseErrorLog)
{
    auto& issues = parser.statistics.compilationIssues;
    if (issues.empty()) {
        return;
    }
    std::filesystem::path logPath(parseErrorLog);
    if (logPath.is_relative()) {
        logPath = HCXX::EnvironmentUtils::GetSelfExecutableDir() / logPath;
    }
    if (std::filesystem::is_directory(logPath)) {
        logPath = logPath / "parse_errors.csv";
    }
    static constexpr uint32_t linesOfContext = 5;
    std::ofstream os(logPath);
    HCXX::CsvUtils::WriteRow(os, {"Translation unit", "File", "Line", "Column", "Severity", "Message", "Source code"});
    for (auto& [tu, issues] : issues) {
        for (auto& issue : issues) {
            HCXX::CsvUtils::WriteRow(
                os, {tu, issue.file, std::to_string(issue.line), std::to_string(issue.column), issue.severity,
                     issue.message, GetSourceTextLine(issue.file, issue.line, linesOfContext)});
        }
    }
}

// Wrapper for analysis to add AR_ENABLE flag to consumer
inline void RunAnalysis(const cxxopts::ParseResult& parsedArgs, std::unique_ptr<HCXX::Analyzer> analyzer,
                        const CompilerOptionsList& unitsOptions, ProblemsHolder& holder, Analyzer::Consumer& consumer)
{
    if (parsedArgs["ast-dump"].as<bool>()) {
        consumer.SetParseFlags(Parser::DUMP_AST | consumer.GetParseFlags());
    }
    analyzer->Analyze(unitsOptions, holder, consumer);
}

inline Problem::Severity ConvertCLIOpToSeverity(const std::string& op)
{
    return Problem::SeverityFromStr(HCXX::StrUtils::BigSnakeCaseToCamelCase(StrUtils::ToUpper(op)));
}

// COODDY_SUPPRESS FunctionLengthChecker LargeStackAllocationChecker
inline int RunAnalyses(cxxopts::ParseResult& parsedArgs)
{
    if (parsedArgs.count("scope") == 0) {
        //  'scope' is the only one mandatory parameter,
        //  so we have to check explicitly that it was provide before reading it.
        //  Otherwise, cxxopts library throws domain_error if the parameter isn't provided by user.
        std::cerr << HCXX::StrLocales::GetStringLocale("PARAMETER_SCOPE_SHOULD_BE_PROVIDED")
                  << HCXX::StrLocales::GetStringLocale("USE_HELP_TO_GET_MORE_INFORMATION") << "\n";
        return ExitStatus::BAD_COMMANDLINE;
    }
    std::filesystem::path analyzeScope(parsedArgs["scope"].as<std::string>());
    if (!std::filesystem::exists(analyzeScope)) {
        std::cerr << HCXX::StrLocales::GetStringLocale("SCOPE_DOES_NOT_EXIST") << ": " << analyzeScope << "\n"
                  << HCXX::StrLocales::GetStringLocale("USE_HELP_TO_GET_MORE_INFORMATION") << "\n";
        return ExitStatus::BAD_COMMANDLINE;
    }

    std::string profile = parsedArgs["profile"].as<std::string>();
    auto genAnnoSettings = GenAnnoSettings::FromCliArg(parsedArgs["gen-annotations"].as<std::string>());
    WorkspaceOptions workspaceOptions = {
        false,
        Problem::SeverityFromCaseInsensitiveString(parsedArgs["min-problem-severity"].as<std::string>()),
        genAnnoSettings,
        parsedArgs["gen-annotations-path"].as<std::string>(),
        parsedArgs["project-root"].as<std::string>(),
    };
    workspaceOptions.incrementalSettings.FromCliArg(parsedArgs["incremental"].as<std::string>());
    workspaceOptions.commandLine = StrUtils::Join(EnvironmentUtils::Argv().subspan(1), ' ');
    workspaceOptions.profile = profile;
    auto servers = parsedArgs["rhs-servers"].as<std::vector<std::string>>();
    if (!servers.empty() && servers[0] != "default") {
        workspaceOptions.settings.rhsServers = servers;
    }
    auto checkers = InitCheckersList(parsedArgs["checkers"].as<std::vector<std::string>>());
    HCXX::Workspace workspace(analyzeScope.string(), checkers, workspaceOptions, profile);

    auto taintFlagsArg = parsedArgs["taint-options"].as<std::string>();
    if (!workspace.GetProfile().GetTaintOptions().empty() && parsedArgs["taint-options"].has_default()) {
        // if taint-option is not defined in cmd, then use value from profile
        taintFlagsArg = workspace.GetProfile().GetTaintOptions();
    }
    bool taintMacroSummaryArg = parsedArgs["taint-macro-summary"].as<bool>();
    auto taintSettings = TaintSettings{taintFlagsArg, taintMacroSummaryArg};
    workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(taintSettings));

    if (workspace.GetProfile().IsEmpty()) {
        std::cout << HCXX::StrLocales::GetStringLocale("PROFILE_FILE") << profile
                  << HCXX::StrLocales::GetStringLocale("CAN_NOT_BE_LOADED") << std::endl;
        return ExitStatus::BAD_COMMANDLINE;
    }

    for (auto& checker : checkers) {
        if (!workspace.GetProfile().IsCheckerEnabled(checker)) {
            std::cout << checker << HCXX::StrLocales::GetStringLocale("IS_NOT_PRESENTED") << std::endl;
            return ExitStatus::BAD_COMMANDLINE;
        }
    }

    CompilerOptions extraOptions{parsedArgs["compile-options"].as<vector<string>>()};

    auto compileOptions = workspace.GetCompilerOptions();
    compileOptions.AddExtraOptions(extraOptions.options);

    auto taintMacro = parsedArgs["taint-macro"].as<std::string>();
    auto taintSde = parsedArgs["taint-sde"].as<std::string>();
    CompilerOptions parserOptions{{TaintMacroCliDefinition(taintMacro, "__cooddy_security_risk"),
                                   TaintMacroCliDefinition(taintSde, "__cooddy_security_sde")},
                                  parsedArgs["compiler"].as<std::string>()};

    std::string lang = parsedArgs["lang"].as<string>();
    if (lang == "default") {
        lang = extraOptions.GetOptionValue("-x");
    }
    if (!lang.empty()) {
        parserOptions.options.push_back("-x=" + lang);
    }

    // Create Parser
    std::unique_ptr<HCXX::Parser> parser = HCXX::Parser::Create(parserOptions, analyzeScope.string());
    parser->statistics.maxFatalErrorCount = parsedArgs["analyze-with-fatal-errors"].as<uint32_t>();

    // Create Analyzer
    std::unique_ptr<HCXX::Analyzer> analyzer = HCXX::Analyzer::Create(*parser, workspace);

    // Check if it is call chain generation mode
    auto callChainEntry = parsedArgs["gen-callchain"].as<std::string>();
    if (!callChainEntry.empty()) {
        CallChainConsumer consumer(callChainEntry, parsedArgs["callchain-depth"].as<int>());
        OutReporter reporter(workspace);
        RunAnalysis(parsedArgs, std::move(analyzer), compileOptions, reporter, consumer);

        // Out call chain to json
        consumer.OutReport(parsedArgs["results-path"].as<std::string>());
    } else {
        auto parsedReps = parsedArgs["reporter"].as<std::vector<std::string>>();
        CompositeReporter reporter(parsedReps, workspace);
        uint32_t initFlags = 0;
        for (auto& repArg : parsedReps) {
            if (repArg != "html" && repArg.find("html") != std::string::npos) {
                initFlags |= Reporter::HTML_REPORT;
            }
            if (repArg.find("code") != std::string::npos) {
                initFlags |= Reporter::CODE_IN_REPORT;
            }
        }
        reporter.Init(*parser, parsedArgs["results-path"].as<std::string>(), initFlags);

        std::string mangleQuery = parsedArgs["mangle"].as<std::string>();
        if (!mangleQuery.empty()) {
            MangledNameFinder mangler(mangleQuery, *parser);
            RunAnalysis(parsedArgs, std::move(analyzer), compileOptions, reporter, mangler);
            Log(LogLevel::INFO) << mangler.FormatResult() << "\n";
            return ExitStatus::SUCCESS;
        }

        // Check if it is call graph generation mode
        auto callGraphFileName = parsedArgs["call-graph-path"].as<std::string>();
        if (callGraphFileName.empty()) {
            Analyzer::Consumer consumer;
            RunAnalysis(parsedArgs, std::move(analyzer), compileOptions, reporter, consumer);
        } else if (std::filesystem::path(callGraphFileName).extension() == ".csv") {
            EntryFuncConsumer consumer;
            bool exportMode = parsedArgs["export-call-graph"].as<bool>();
            if (!exportMode) {
                consumer.SetEntryFunc(parsedArgs["entry-function"].as<std::string>(), callGraphFileName,
                                      parsedArgs["check-path-line"].as<int>());
            }
            RunAnalysis(parsedArgs, std::move(analyzer), compileOptions, reporter, consumer);
            if (exportMode) {
                consumer.StoreCallGraph(callGraphFileName);
            }
        } else {
            CallGraphConsumer consumer(parsedArgs["export-call-graph"].as<bool>());
            std::filesystem::path path = analyzeScope.string();
            consumer.SetPath(path);
            RunAnalysis(parsedArgs, std::move(analyzer), compileOptions, reporter, consumer);
            consumer.OutReport(callGraphFileName);
        }
    }

    auto parseErrorLog = parsedArgs["parse-errors-log"].as<std::string>();
    if (!parseErrorLog.empty()) {
        SaveParserErrorLog(*parser, parseErrorLog);
    }

    return ExitStatus::SUCCESS;
}

inline void ReportUnknownArgs(const cxxopts::ParseResult& args)
{
    for (auto& arg : args.unmatched()) {
        std::stringstream ss;
        ss << HCXX::StrLocales::GetStringLocale("UNEXPECTED_POSITIONAL_ARGUMENT_IGNORED") << ": " << arg << "\n";

        if (StrUtils::StartsWith(arg, "–") ||  // En dash
            StrUtils::StartsWith(arg, "—")) {  // Em dash
            ss << HCXX::StrLocales::GetStringLocale("SUGGESTION") << ": "
               << HCXX::StrLocales::GetStringLocale("LONG_DASH_SUGGESTION") << "\n";
        }

        Log(LogLevel::ERROR) << ss.str();
    }
}

#endif  // COODDY_ANALYZER_TOOL_CLI_H_
