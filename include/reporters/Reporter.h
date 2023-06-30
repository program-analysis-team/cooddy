/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_TOOL_REPORTER_H_
#define COODDY_ANALYZER_TOOL_REPORTER_H_
#include <Analyzer.h>
#include <ProblemsHolder.h>

#include <filesystem>
#include <fstream>

#include "RichHTMLSeparateReporter.h"
#include "workspace/Workspace.h"

namespace HCXX {
class Reporter : public ProblemsHolder {
public:
    enum InitFlags { HTML_REPORT = 1, CODE_IN_REPORT = 2, PARSE_ERRORS_IN_REPORT = 4 };

    Reporter(const Workspace& workspace, const std::string& defExt);
    virtual ~Reporter();

    virtual void Init(Parser& parser, const std::string& resultPath, uint32_t initFlags = 0);
    std::string AddProblemTrace(const Problem& problem);

    void SetParseErrorsPath(const std::string& path)
    {
        myParseErrorsPath = path;
    }

    struct CompilationIssue {
        struct Error {
            std::string file;
            uint32_t line = 0;
            uint32_t column = 0;
            std::string message;
            std::string codeSnippet;

            DECLARE_FIELDS("file", file, "line", line, "column", column, "message", message, "codeSnippet",
                           codeSnippet);
        };

        std::string unitFile;
        std::string reason;
        std::vector<Error> errors;

        DECLARE_FIELDS("unit", unitFile, "reason", reason, "errors", errors);
    };

    struct ParseInfo {
        struct Summary {
            uint32_t totalParsedCount = 0;
            uint32_t succeedCount = 0;
            uint32_t failedCount = 0;
            uint32_t partiallyParsedCount = 0;
            double successRate = 0;

            DECLARE_FIELDS("units_total", totalParsedCount, "units_success", succeedCount, "units_failed", failedCount,
                           "units_partially_parsed", partiallyParsedCount, "success_rate", successRate);
        };
        Summary summary;
        std::vector<CompilationIssue> compilationIssues;

        DECLARE_FIELDS("summary", summary, "compilation_issues", compilationIssues);
    };

    ParseInfo GetParseInfo(const std::function<std::string(LocationInfo&)>& snippetCreator = {});

protected:
    friend class CompositeReporter;

    void Flush() override;

    static std::string CreateCodeSnippet(const std::string& filePath, uint64_t lineNum, uint64_t shift = 3);
    void ReportParseInfo();
    void StoreParseInfo();

    uint32_t myInitFlags = 0;
    Parser* myParser = nullptr;
    std::string myDefExt;
    std::filesystem::path myResultPath;
    std::filesystem::path myParseErrorsPath;
    std::ofstream myFileStream;
    std::unique_ptr<RichHTMLSeparateReporter> myHTMLReporter;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_TOOL_REPORTER_H_
