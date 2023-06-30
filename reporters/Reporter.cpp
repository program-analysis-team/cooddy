/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "reporters/Reporter.h"

#include <utils/CsvUtils.h>

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
    auto parentDir = myResultPath.parent_path();
    if (!parentDir.empty() && (!std::filesystem::is_directory(parentDir) || !std::filesystem::exists(parentDir))) {
        std::error_code ec;
        if (!std::filesystem::create_directories(parentDir, ec)) {
            Log(LogLevel::ERROR) << "Can not create results path directory " << parentDir << "\n";  // LCOV_EXCL_LINE
        }
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

std::string Reporter::CreateCodeSnippet(const std::string& filePath, uint64_t lineNum, uint64_t shift)
{
    const FileEntry* fileEntry = FileEntriesCache::GetInstance().GetFileEntry(filePath);
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

void Reporter::Flush()
{
    if (myParser != nullptr && !myParseErrorsPath.empty()) {
        StoreParseInfo();
    }
}

Reporter::ParseInfo Reporter::GetParseInfo(const std::function<std::string(LocationInfo&)>& snippetCreator)
{
    ParseInfo info;
    info.summary.totalParsedCount = myParser->statistics.totalParsedCount;
    info.summary.succeedCount = myParser->statistics.succeedCount;
    info.summary.failedCount = myParser->statistics.failedCount;
    info.summary.partiallyParsedCount = myParser->statistics.partiallyParsedCount;
    info.summary.successRate = 100 * myParser->statistics.succeedCount / double(myParser->statistics.totalParsedCount);

    for (auto& [unit, issue] : myParser->statistics.compilationIssues) {
        auto& reportIssue =
            info.compilationIssues.emplace_back(CompilationIssue{unit->GetMainFileName(), issue.reason});

        for (auto i = 0; i < issue.errors.size(); ++i) {
            auto locInfo = unit->GetLocationInfo(SourceRange{unit->GetParseErrors()[i], unit->GetParseErrors()[i]});
            CompilationIssue::Error error{locInfo.filename, locInfo.line, locInfo.column, issue.errors[i]};
            if (snippetCreator != nullptr) {
                error.codeSnippet = snippetCreator(locInfo);
            }
            reportIssue.errors.emplace_back(std::move(error));
        }
    }
    return info;
}

void Reporter::StoreParseInfo()
{
    ParseInfo info = GetParseInfo();
    if (myParseErrorsPath.is_relative()) {
        myParseErrorsPath = std::filesystem::current_path() / myParseErrorsPath;
    }
    if (std::filesystem::is_directory(myParseErrorsPath)) {
        myParseErrorsPath = myParseErrorsPath / "parse_errors.csv";
    }

    std::ofstream stream(myParseErrorsPath);
    if (myParseErrorsPath.extension() != ".csv") {
        stream << jsoncpp::to_string(info, "\n");
        return;
    }
    HCXX::CsvUtils::WriteRow(stream, {"File", "Reason", "Log"});

    for (auto& issue : info.compilationIssues) {
        std::string errorLog;
        for (auto& error : issue.errors) {
            std::stringstream ss;
            if (!error.file.empty()) {
                ss << error.file << ':' << error.line << ':' << error.column << ": ";
            }
            ss << error.message << "\n";
            errorLog += ss.str();
        }
        HCXX::CsvUtils::WriteRow(stream, {issue.unitFile, issue.reason, errorLog});
    }
}

}  // namespace HCXX
