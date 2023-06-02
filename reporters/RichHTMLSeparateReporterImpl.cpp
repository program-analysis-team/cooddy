/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "reporters/RichHtmlReporter.h"
namespace HCXX {
class RichHTMLSeparateReporterImpl : public RichHtmlReporter, public RichHTMLSeparateReporter {
public:
    explicit RichHTMLSeparateReporterImpl(const Workspace& workspace, const std::filesystem::path& reportPath)
        : RichHtmlReporter(workspace), myReportPath(reportPath)
    {
        myReportPath.append("html_traces");
        std::filesystem::remove_all(myReportPath);
        std::filesystem::create_directory(myReportPath);
    }
    void PreprocessTemplateStr() override
    {
        for (std::string excl :
             {"<script src=\"Deps/FileSaver.min.js\"></script>\n", "<script src=\"Deps/tabulator.min.js\"></script>\n",
              "<script src=\"Deps/jszip.min.js\"></script>\n",
              "<link rel=\"stylesheet\" href=\"Deps/tabulator-simple.min.css\">\n",
              "<script src=\"tabulatorInit.js\"></script>\n", "<script src=\"compilationIssuesInit.js\"></script>\n"}) {
            if (std::size_t pos = templateStrTail.find(excl); pos != std::string::npos) {
                templateStrTail.replace(pos, excl.length(), "");
            }
        }
    }

    std::string AddProblem(const Problem& problem) override
    {
        std::filesystem::path newReport(myReportPath);
        newReport.append("report-" + problem.uniqueId.ToString() + ".html");
        descriptor.problems.clear();
        descriptor.codeSnippets.clear();
        RegisterProblemImpl(problem);
        FlushStream(newReport);
        return newReport.string();
    }

private:
    std::filesystem::path myReportPath;
};

std::unique_ptr<RichHTMLSeparateReporter> RichHTMLSeparateReporter::Create(const Workspace& workspace,
                                                                           const std::string& reportPath)
{
    return std::make_unique<RichHTMLSeparateReporterImpl>(workspace, reportPath);
}
}  // namespace HCXX
