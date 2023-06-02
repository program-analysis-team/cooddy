/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_TOOL_REPORTER_H_
#define COODDY_ANALYZER_TOOL_REPORTER_H_
#include <ProblemsHolder.h>

#include <filesystem>
#include <fstream>

#include "RichHTMLSeparateReporter.h"
#include "workspace/Workspace.h"

namespace HCXX {
class Reporter : public ProblemsHolder {
public:
    enum InitFlags { HTML_REPORT = 1, CODE_IN_REPORT = 2 };

    Reporter(const Workspace& workspace, const std::string& defExt);
    virtual ~Reporter();

    virtual void Init(Parser& parser, const std::string& resultPath, uint32_t initFlags = 0);
    std::string AddProblemTrace(const Problem& problem);

protected:
    friend class CompositeReporter;

    static std::string CreateCodeSnippet(const FileEntry* fileEntry, uint64_t lineNum, uint64_t shift = 3);
    uint32_t myInitFlags = 0;
    Parser* myParser = nullptr;
    std::string myDefExt;
    std::filesystem::path myResultPath;
    std::ofstream myFileStream;
    std::unique_ptr<RichHTMLSeparateReporter> myHTMLReporter;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_TOOL_REPORTER_H_
