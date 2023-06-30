/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#ifndef COODDY_ANALYZER_INCLUDE_JSONREPORTER_H_
#define COODDY_ANALYZER_INCLUDE_JSONREPORTER_H_

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "Parser.h"
#include "Reporter.h"
#include "utils/json-cpp.h"

namespace HCXX {

struct ProblemTraceEvent : public LocationInfoBase {
    std::string file;
    std::string desc;
    std::string snippetPath;
    int parentEventId;
    int id;
    string kind;

    DECLARE_FIELDS("file", file, "line", line, "col", column, "end_line", endLine, "end_col", endColumn, "length",
                   length, "desc", desc, "snippet_path", snippetPath, "parentEventId", parentEventId, "id", id, "kind",
                   kind);

    bool operator==(const ProblemTraceEvent& other) const
    {
        return file == other.file && line == other.line && column == other.column && endLine == other.endLine &&
               endColumn == other.endColumn && desc == other.desc;
    }
};

using ProblemTrace = std::vector<ProblemTraceEvent>;

class JsonReporter : public Reporter {
public:
    explicit JsonReporter(const Workspace& workspace);
    virtual ~JsonReporter();

    virtual ProblemTrace ConvertProblemTrace(const TracePath& problemTrace);
    virtual void Flush() override;

    struct ProblemClass {
        std::string name;
        std::string severity;
        std::string profileName;
        std::string inspectionName;

        DECLARE_FIELDS("name", name, "severity", severity, "profile", profileName, "inspection_name", inspectionName);

        bool operator==(const ProblemClass& other) const
        {
            return inspectionName == other.inspectionName && severity == other.severity && name == other.name &&
                   profileName == other.profileName;
        }
    };
    using CodeSnippetHighlightedLocations = std::unordered_map<std::string, std::unordered_set<std::string>>;
    struct CodeSnippet {
        uint32_t line = UINT32_MAX;
        uint32_t column = UINT32_MAX;
        uint32_t endLine = 0;
        uint32_t endColumn = 0;
        std::string filename;
        std::string code;
        CodeSnippetHighlightedLocations highlightedLocations;

        CodeSnippet() {}  // LCOV_EXCL_LINE
        CodeSnippet(const LocationInfo& info, const string& code)
            : line(info.line),
              column(info.column),
              endLine(info.endLine),
              endColumn(info.endColumn),
              filename(info.filename),
              code(code)
        {}
        DECLARE_FIELDS("file", filename, "line", line, "column", column, "end_line", endLine, "end_column", endColumn,
                       "code", code, "highlighted_locations", highlightedLocations);

        bool operator==(const CodeSnippet& other) const
        {
            return filename == other.filename && line == other.line && endLine == other.endLine &&
                   column == other.column && endColumn == other.endColumn;
        }
    };
    struct ProblemDescriptor {
        std::string file;
        uint32_t line;
        uint32_t offset;
        uint32_t length;
        ProblemClass problemClass;
        std::string highlightedElement;
        std::string description;
        std::string codeLanguage;
        std::string source;
        std::string reason;
        std::string uniqueId;
        ProblemTrace problemTrace;
        std::string htmlReport;
        uint32_t solverDuration = 0;
        std::string functionName;
        std::string sinkFunction;
        std::string codeSnippet;

        DECLARE_FIELDS("file", file, "line", line, "offset", offset, "length", length, "problem_class", problemClass,
                       "highlighted_element", highlightedElement, "description", description, "language", codeLanguage,
                       "source", source, "reason", reason, "unique_id", uniqueId, "trace", problemTrace, "html_trace",
                       htmlReport, "solver_duration", solverDuration, "function", functionName, "sink_function",
                       sinkFunction, "code_snippet", codeSnippet);

        bool operator==(const ProblemDescriptor& other) const
        {
            return file == other.file && line == other.line && offset == other.offset && length == other.length &&
                   highlightedElement == other.highlightedElement && description == other.description &&
                   uniqueId == other.uniqueId && problemClass == other.problemClass &&
                   functionName == other.functionName && sinkFunction == other.sinkFunction &&
                   codeSnippet == other.codeSnippet;
        }
    };

    struct ReportDescriptor {
        std::string profile;
        std::string commandLine;
        std::string gitCommit;
        std::string gitVersion;
        std::string projectGitSummary;
        std::string startTime;
        std::string endTime;
        time_t startTimeStamp = 0;
        time_t endTimeStamp = 0;
        std::vector<ProblemDescriptor> problems;
        std::unordered_map<std::string, std::string> configurations;
        std::unordered_map<string, CodeSnippet> codeSnippets;
        std::vector<CompilationIssue> compilationIssues;
        std::string taintManUrl;

        DECLARE_FIELDS("start_time", startTime, "end_time", endTime, "start_timestamp", startTimeStamp, "end_timestamp",
                       endTimeStamp, "profile", profile, "command_line", commandLine, "git_commit", gitCommit,
                       "git_version", gitVersion, "project_git_summary", projectGitSummary, "problems", problems,
                       "code_snippets", codeSnippets, "configurations", configurations, "taint_man_url", taintManUrl,
                       "compilation_issues", compilationIssues);
    };

    std::string ConvertProblemSeverity(Problem::Severity severity);

    void RegisterProblemImpl(const Problem& problem) override;

    ReportDescriptor descriptor;
    bool flushed = false;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_JSONREPORTER_H_
