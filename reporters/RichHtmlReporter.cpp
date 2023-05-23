/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include "reporters/RichHtmlReporter.h"

#include <deps/miniz.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "deps/base64.h"
#include "deps/jsmin.h"
#include "deps/sha1.h"
#include "utils/EnvironmentUtils.h"
#include "utils/json-cpp.h"

namespace HCXX {
void RichHtmlReporter::PreprocessTemplateStr() {}
void RichHtmlReporter::InvalidateTemplateStr()
{
    std::ifstream t(folderPath / "ReportTemplate.html");
    auto templateString = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    string crScriptHead = "<script id='cooddyResults' type='text/json'>";
    if (std::size_t pos = templateString.find(crScriptHead); pos != std::string::npos) {
        pos += crScriptHead.length();
        templateStrHead = templateString.substr(0, pos);
        std::size_t endPos = templateString.find("</script>", pos);
        templateStrTail = templateString.substr(endPos);
    } else {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "Template '.cooddy/HtmlReportTemplate/ReportTemplate.html' file is invalid.\n";
        // LCOV_EXCL_STOP
    }
    PreprocessTemplateStr();
    InvalidateRhsServers();
    InvalidateConfigurationFiles();
    InvalidateScripts();
}
void RichHtmlReporter::InvalidateRhsServers()
{
    auto& rhsServers = myWorkspace.GetOptions().settings.rhsServers;
    if (!rhsServers.empty()) {
        string crRhsServers = "<script>var rhsServers = [];</script>";
        if (std::size_t pos = templateStrTail.find(crRhsServers); pos != std::string::npos) {
            templateStrTail.replace(
                pos, crRhsServers.length(),
                "<script>var rhsServers = [" +
                    std::accumulate(rhsServers.begin() + 1, rhsServers.end(), '\'' + rhsServers[0] + '\'',
                                    [](std::string x, std::string y) { return x + ",\'" + y + '\''; }) +
                    "];</script>");
        }
    }
}

void RichHtmlReporter::InvalidateScripts()
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {
        std::error_code ec;
        auto relativePath = std::filesystem::relative(entry.path(), folderPath, ec).string();
        relativePath = StrUtils::Replace(relativePath, "\\", "/");
        if (entry.path().extension() == ".js") {
            string temp = "<script src=\"" + relativePath + "\"></script>";
            if (std::size_t pos = templateStrTail.find(temp); pos != std::string::npos) {
                templateStrTail.replace(pos, temp.length(),
                                        "<script type='text/json' class='packed'>" + PackJsFile(entry) + "</script>");
            }
        } else if (entry.path().extension() == ".css") {
            string temp = "<link rel=\"stylesheet\" href=\"" + relativePath + "\">";
            if (std::size_t pos = templateStrTail.find(temp); pos != std::string::npos) {
                templateStrTail.replace(
                    pos, temp.length(),
                    "<script type='text/json' class='packedStyle'>" + PackFile(entry) + "</script>\n");
            }
        }
    }
}

void RichHtmlReporter::InvalidateConfigurationFiles()
{
    std::unordered_map<string, string> shas;
    std::filesystem::path shaPath(HCXX::EnvironmentUtils::GetSelfExecutableDir() / ".cooddy/self-check.sha1.json");
    if (!std::filesystem::exists(shaPath)) {
        return;  // LCOV_EXCL_LINE
    }
    std::ifstream shaStream(shaPath);
    jsoncpp::parse(shas, shaStream);

    for (const auto& fn : myWorkspace.GetConfigurationFileNames()) {
        std::filesystem::path annotationPath(fn);
        std::ifstream annotationStream(annotationPath);
        std::string str((std::istreambuf_iterator<char>(annotationStream)), std::istreambuf_iterator<char>());
        auto path = annotationPath.string();
        path = StrUtils::Replace(path, "\\", "/");
        string fileName = path.substr(path.find_last_of("/") + 1);
        auto i = shas.find(fileName);
        descriptor.configurations.insert(
            {path, i == shas.end() || i->second != sha1(str) ? str : "Default"});  // LCOV_EXCL_LINE
    }
}
string RichHtmlReporter::PackData(const string& source)
{
    uLong cmpLen = compressBound(source.length());
    unsigned char* pCmp = new unsigned char[cmpLen];
    if (mz_compress2(pCmp, &cmpLen, reinterpret_cast<const unsigned char*>(source.c_str()), source.length(),
                     MZ_UBER_COMPRESSION) != MZ_OK) {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "Can't compress data\n";
        // LCOV_EXCL_STOP
    }

    auto res = base64_encode(reinterpret_cast<const unsigned char*>(pCmp), cmpLen);
    delete[] pCmp;
    return res;
}
string RichHtmlReporter::PackFile(const std::filesystem::directory_entry& entry)
{
    std::ifstream replacementFile(entry.path());
    std::string replacementStr((std::istreambuf_iterator<char>(replacementFile)), std::istreambuf_iterator<char>());
    return PackData(replacementStr);
}
string RichHtmlReporter::PackJsFile(const std::filesystem::directory_entry& entry)
{
    std::ifstream replacementFile(entry.path());
    std::string replacementStr((std::istreambuf_iterator<char>(replacementFile)), std::istreambuf_iterator<char>());
    if (!StrUtils::EndsWith(entry.path().string(), ".min.js")) {
        Jsmin m;
        replacementStr = m.minify(replacementStr.c_str());
    }
    return PackData(replacementStr);
}

string RichHtmlReporter::CreateLocation(const LocationInfo& location, const Workspace& workspace)
{
    const char delimeter = ':';
    std::stringstream s;
    s << workspace.GetRelativePath(location.filename, true) << delimeter << location.line << delimeter
      << location.column << delimeter << location.endLine << delimeter << location.endColumn;
    string result = s.str();
    std::replace(result.begin(), result.end(), '\\', '/');
    return result;
}

ProblemTrace RichHtmlReporter::ConvertProblemTrace(const TracePath& problemTrace)
{
    ProblemTrace trace;
    trace.reserve(problemTrace.size());
    for (const auto& it : problemTrace) {
        ProblemTraceEvent event;
        event.file = GetWorkspace().GetRelativePath(it.filename, true);
        event.column = it.column;
        event.line = it.line;
        event.endLine = it.endLine;
        event.endColumn = it.endColumn;
        event.length = it.length;
        event.desc = it.description;
        event.parentEventId = it.parentId;
        event.id = it.id;
        event.kind = TraceNode::KindToCStr(it.kind);
        auto range = it.scopeRange.IsValid() ? it.scopeRange : it.range;
        auto location = it.tu->GetLocationInfo(range);
        event.snippetPath = CreateLocation(location, GetWorkspace());
        if (descriptor.codeSnippets.find(event.snippetPath) == descriptor.codeSnippets.end()) {
            CodeSnippet snippet{location, it.tu->GetSourceInRange(range)};
            EmbedExpansions(it, snippet);
            descriptor.codeSnippets.insert({event.snippetPath, snippet});
        }
        trace.push_back(event);
    }
    return trace;
}
void RichHtmlReporter::EmbedExpansions(const TraceNode& it, CodeSnippet& snippet)
{
    for (auto& exp : it.tu->GetDescriptions(it.scopeRange)) {
        EmbedExpansion(it, snippet, exp.codeRange, exp.descriptionRange, exp.ToString(*it.tu));
    }
}
void RichHtmlReporter::EmbedExpansion(const TraceNode& it, CodeSnippet& snippet, const SourceRange& codeRange,
                                      const SourceRange& descriptionRange, const string& description)
{
    auto descriptionLocation = it.tu->GetLocationInfo(descriptionRange);
    // LCOV_EXCL_START
    if (descriptionLocation.line == 0 && descriptionLocation.column == 0 && descriptionLocation.endLine == 0 &&
        descriptionLocation.endColumn == 0) {
        return;
    }
    // LCOV_EXCL_STOP
    auto descriptionSnippetLocation = CreateLocation(descriptionLocation, GetWorkspace());
    auto codeLocation = it.tu->GetLocationInfo(codeRange);
    auto codeLocalLocation = CreateLocation(codeLocation, GetWorkspace());
    snippet.highlightedLocations[codeLocalLocation].insert(descriptionSnippetLocation);
    if (descriptor.codeSnippets.find(descriptionSnippetLocation) == descriptor.codeSnippets.end()) {
        CodeSnippet macroSnippet{descriptionLocation, description};
        descriptor.codeSnippets.insert({descriptionSnippetLocation, macroSnippet});
    }
}
void RichHtmlReporter::UpdateDescriptor()
{
    descriptor.taintManUrl = GetWorkspace().GetOptions().settings.taintManUrl;
    descriptor.endTime = GetCurrentTime();
    descriptor.endTimeStamp = std::time(0);
}
void RichHtmlReporter::Flush()
{
    if (flushed) {
        return;
    }
    flushed = true;
    FlushStream(myResultPath);
}
void RichHtmlReporter::FlushStream(const std::filesystem::path& fileName)
{
    UpdateDescriptor();
    if (templateStrHead.empty()) {
        InvalidateTemplateStr();
    }

    CopyCompilationIssues();
    string jsonStr = PackData(jsoncpp::to_string(descriptor, "\n"));

    if (myFileStream.is_open()) {
        myFileStream.close();
    }
    myFileStream.open(fileName, std::ios_base::out | std::ios_base::trunc);
    myFileStream.seekp(std::ios_base::beg);
    myFileStream << templateStrHead << jsonStr << templateStrTail;
    myFileStream.flush();
    myFileStream.close();
}
}  // namespace HCXX
