/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#include "reporters/JsonReporter.h"

#include <ctime>
#include <iomanip>

#include "utils/Utc.h"

namespace HCXX {
std::string FindGit(const std::string& root)
{
    // LCOV_EXCL_START
    try {
        std::filesystem::path path(root);
        while (!path.empty() && !std::filesystem::exists(path / ".git/HEAD")) {
            if (path == path.parent_path()) {
                break;
            }
            path = path.parent_path();
        }
        if (std::filesystem::exists(path / ".git/HEAD")) {
            std::ifstream t(path / ".git/HEAD");
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            if (StrUtils::StartsWith(str, "ref: ")) {
                try {
                    str = StrUtils::TrimWhitespace(str);
                    std::ifstream tt(path / ".git" / str.substr(5));
                    std::string str2((std::istreambuf_iterator<char>(tt)), std::istreambuf_iterator<char>());
                    str += " " + str2;
                } catch (...) {
                }
            }
            Log(LogLevel::INFO) << "Git found: " << str << "\n";
            return str;
        }
    } catch (...) {
    }
    return "Git not found";
    // LCOV_EXCL_STOP
}
JsonReporter::JsonReporter(const Workspace& workspace) : Reporter(workspace, ".json")
{
    descriptor.profile = workspace.GetOptions().profile;
    descriptor.commandLine = workspace.GetOptions().commandLine;
    descriptor.gitCommit = GIT_COMMIT;
    descriptor.gitVersion = GIT_VERSION;
    descriptor.startTime = to_string(utc_clock::now());
    descriptor.startTimeStamp = std::time(0);
    descriptor.projectGitSummary = FindGit(GetWorkspace().GetProjectRoot());
}

JsonReporter::~JsonReporter()
{
    Flush();
}

void JsonReporter::Flush()
{
    if (flushed) {
        return;
    }
    flushed = true;
    descriptor.endTime = to_string(utc_clock::now());
    descriptor.endTimeStamp = std::time(0);
    if (myFileStream.is_open()) {
        myFileStream.close();
    }
    myFileStream.open(myResultPath, std::ios_base::out | std::ios_base::trunc);
    myFileStream.seekp(std::ios_base::beg);
    myFileStream << jsoncpp::to_string(descriptor, "\n");
    myFileStream.flush();
    myFileStream.close();
    Reporter::Flush();
}

ProblemTrace JsonReporter::ConvertProblemTrace(const TracePath& problemTrace)
{
    ProblemTrace trace;
    for (const auto& it : problemTrace) {
        if (it.kind == TraceNode::Kind::BRANCH || it.kind == TraceNode::Kind::WEAKASSUMPTION) {
            continue;  // LCOV_EXCL_LINE
        }
        ProblemTraceEvent event;
        event.file = GetWorkspace().GetRelativePath(it.filename, true);
        event.column = it.column;
        event.line = it.line;
        event.endLine = it.endLine;
        event.endColumn = it.endColumn;
        event.length = it.length;
        event.desc = it.description;
        trace.push_back(event);
    }
    return trace;
}

std::string JsonReporter::ConvertProblemSeverity(Problem::Severity severity)
{
    switch (severity) {
        case Problem::Severity::ERROR:
            return "ERROR";
        case Problem::Severity::NOTIFY:
            return "NOTIFY";
        default:
            return "WARNING";
    }
}

void JsonReporter::RegisterProblemImpl(const Problem& problem)
{
    auto& problems = descriptor.problems;
    ProblemClass problemClass = {problem.kind, ConvertProblemSeverity(problem.severity), problem.profileName,
                                 problem.checkerName};
    ProblemTrace problemTrace = ConvertProblemTrace(problem.trace);
    ProblemDescriptor desc = {GetWorkspace().GetRelativePath(problem.filename, true),
                              problem.line,
                              problem.column,
                              problem.length,
                              std::move(problemClass),
                              "",
                              problem.description,
                              problem.codeLanguage,
                              problem.sourceId,
                              problem.reasonCode,
                              problem.uniqueId.ToString(),
                              std::move(problemTrace),
                              Reporter::AddProblemTrace(problem),
                              problem.solverDuration,
                              problem.functionName,
                              problem.sinkFunction};

    if (myInitFlags & CODE_IN_REPORT) {
        desc.codeSnippet = Reporter::CreateCodeSnippet(problem.filename, desc.line);
    }
    problems.push_back(desc);
}
}  // namespace HCXX
