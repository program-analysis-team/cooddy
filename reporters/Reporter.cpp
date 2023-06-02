/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "reporters/Reporter.h"

namespace HCXX {

Reporter::Reporter(const Workspace& workspace, const std::string& defExt) : ProblemsHolder(workspace), myDefExt(defExt)
{}

Reporter::~Reporter() {}

void Reporter::Init(Parser& parser, const std::string& resultPath, uint32_t initFlags)
{
    myInitFlags = initFlags;
    myParser = &parser;
    myResultPath = resultPath;
    if (std::filesystem::is_directory(myResultPath)) {
        myResultPath.append("cooddy_result");
    }
    if (!myResultPath.has_extension()) {
        myResultPath.replace_extension(myDefExt);
    }
    if (!myDefExt.empty()) {
        myFileStream.open(myResultPath);
    }
}

std::string Reporter::AddProblemTrace(const Problem& problem)
{
    if (!(myInitFlags & HTML_REPORT) || myParser == nullptr || problem.trace.empty()) {
        return {};
    }
    if (problem.traceReport.empty()) {
        if (myHTMLReporter == nullptr) {
            myHTMLReporter = RichHTMLSeparateReporter::Create(myWorkspace, myResultPath.parent_path().string());
        }
        const_cast<Problem&>(problem).traceReport = myHTMLReporter->AddProblem(problem);
    }
    return problem.traceReport;
}

std::string Reporter::CreateCodeSnippet(const FileEntry* fileEntry, uint64_t lineNum, uint64_t shift)
{
    if (fileEntry == nullptr || lineNum-- == 0) {
        return "";  // LCOV_EXCL_LINE (HCAT-2802)
    }

    uint64_t minLine = shift < lineNum ? lineNum - shift : 0;
    uint64_t maxLine = lineNum + shift + 1;
    if (maxLine > fileEntry->lineOffsets.size() - 1) {
        return fileEntry->fileSource.substr(fileEntry->lineOffsets[minLine]);
    }
    return fileEntry->fileSource.substr(fileEntry->lineOffsets[minLine],
                                        fileEntry->lineOffsets[maxLine] - fileEntry->lineOffsets[minLine]);
}

}  // namespace HCXX
