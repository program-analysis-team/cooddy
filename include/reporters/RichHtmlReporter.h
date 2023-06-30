/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#ifndef COODDY_ANALYZER_INCLUDE_RICHHTMLREPORTER_H_
#define COODDY_ANALYZER_INCLUDE_RICHHTMLREPORTER_H_
#include "JsonReporter.h"

namespace HCXX {
class RichHtmlReporter : public JsonReporter {
public:
    explicit RichHtmlReporter(const Workspace& workspace) : JsonReporter(workspace)
    {
        myDefExt = ".html";
        folderPath = HCXX::EnvironmentUtils::GetSelfExecutableDir() / ".cooddy/HtmlReportTemplate";
    }
    virtual ~RichHtmlReporter()
    {
        Flush();
    }
    void Flush() override;
    virtual void PreprocessTemplateStr();

protected:
    std::string templateStrHead;
    std::string templateStrTail;
    std::filesystem::path folderPath;

    ProblemTrace ConvertProblemTrace(const TracePath& problemTrace) override;
    void FlushStream(const std::filesystem::path& fileName);

private:
    void InvalidateTemplateStr();
    void InvalidateConfigurationFiles();
    void InvalidateScripts();
    void InvalidateRhsServers();
    string PackData(const string& source);
    string PackFile(const std::filesystem::directory_entry& entry);
    string PackJsFile(const std::filesystem::directory_entry& entry);
    string CreateLocation(const LocationInfo& location, const Workspace& workspace);
    void EmbedExpansions(const TraceNode& it, CodeSnippet& snippet);
    void EmbedExpansion(const TraceNode& it, CodeSnippet& snippet, const SourceRange& codeRange,
                        const SourceRange& descriptionRange, const string& description);
    void UpdateDescriptor();
    void GetCompilationIssues();
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_RICHHTMLREPORTER_H_
