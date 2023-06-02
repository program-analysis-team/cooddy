/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <TranslationUnit.h>
#include <utils/EnvironmentUtils.h>
#include <utils/LocaleUtils.h>

#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

namespace HCXX {

TranslationUnit::TranslationUnit(const CompilerOptions& compilerOptions)
    : AstManager(this), myCompilerOptions(compilerOptions)
{
    myTU = this;
    myRefCount = 1;
    myIndexPos = INT_MAX;
    mySourceRange = {0, UINT_MAX};
    myMainFileName =
        !myCompilerOptions.options.empty() ? EnvironmentUtils::NormalizePath(myCompilerOptions[0]) : std::string();
    myMainFileEntry = nullptr;
    myLanguage = UnitLanguage::CPP;
}

TranslationUnit::~TranslationUnit()
{
    Log(LogLevel::DEBUG) << "TranslationUnit destructor: " << myMainFileName << std::endl;
}

void TranslationUnit::InitIndices()
{
    AstManager::Init();
    CodeDescriptionManager::InitDescriptionRanges(*this);
    CfgManager::Init();
    std::sort(myParseErrors.begin(), myParseErrors.end(), [](auto& e1, auto& e2) { return e1 < e2; });
}

void TranslationUnit::ClearUnusedData()
{
    MacroManager::Clear();
    CommentManager::Clear();
    AstManager::Clear();
    CfgManager::Clear();
}

void TranslationUnit::AddFileEntry(Location fileLocation, FileEntry& entry)
{
    const auto& it = myFileEntries.emplace(fileLocation, entry).first->second;
    if (it.filePath == myMainFileName) {
        myMainFileEntry = &it;
    }
}

LocationInfo TranslationUnit::GetLocationInfo(const SourceRange& range) const
{
    if (range.end == UINT_MAX || range.end < range.begin) {
        // it is translation unit location
        // LCOV_EXCL_START
        return {0, 0, 0, 0, myMainFileName, 0};
        // LCOV_EXCL_STOP
    }

    auto loc = range.begin;
    auto it = myFileEntries.upper_bound(loc);

    if (myFileEntries.size() && it != myFileEntries.begin()) {
        uint32_t fileOffset = loc - (--it)->first;
        if (fileOffset < it->second.fileSize) {
            auto& offsets = it->second.lineOffsets;
            auto line =
                static_cast<uint32_t>(upper_bound(offsets.begin(), offsets.end(), fileOffset) - offsets.begin());
            auto column = line ? fileOffset - offsets[line - 1] + 1 : 0;
            auto endLine = static_cast<uint32_t>(upper_bound(offsets.begin(), offsets.end(), range.end - it->first) -
                                                 offsets.begin());
            auto endColumn = endLine ? range.end - it->first - offsets[endLine - 1] + 1 : 0;

            const auto& path = &it->second == myMainFileEntry ? myMainFileName : it->second.filePath;
            return {line, column, endLine, endColumn, path, range.end - range.begin, fileOffset};
        }
    }

    // LCOV_EXCL_START
    static std::string emptyString;
    return {0, 0, 0, 0, emptyString, 0};
    // LCOV_EXCL_STOP
}

void TranslationUnit::SetMainFileName(std::string mainFileName)
{
    myMainFileName = std::move(mainFileName);
}

std::vector<SourceRange> TranslationUnit::GetFileEntriesRanges() const
{
    std::vector<SourceRange> ranges;
    ranges.reserve(myFileEntries.size());
    for (auto& i : myFileEntries) {
        ranges.push_back({i.first, i.first + i.second.fileSize});
    }
    return ranges;
}

std::string TranslationUnit::GetSourceInRange(const SourceRange& range) const
{
    auto rangeBegin = range.begin;

    auto it = myFileEntries.upper_bound(rangeBegin);

    if (myFileEntries.size() && it != myFileEntries.begin()) {
        auto rangeEnd = range.end;
        const FileEntry& fileEntry = (--it)->second;
        auto fileBegin = it->first;
        auto fileEnd = it->first + fileEntry.fileSize;

        if (rangeBegin >= fileBegin && rangeBegin < fileEnd && rangeEnd <= fileEnd) {
            return fileEntry.fileSource.substr(rangeBegin - fileBegin, rangeEnd - rangeBegin);
        }
    }
    // LCOV_EXCL_START
    return "";
    // LCOV_EXCL_STOP
}

SourceRange TranslationUnit::GetLineRangeByLocation(Location location) const
{
    if (myFileEntries.empty()) {
        // LCOV_EXCL_START
        return {0, 0};
        // LCOV_EXCL_STOP
    }

    auto it = myFileEntries.upper_bound(location);
    if (it != myFileEntries.begin()) {
        uint32_t fileOffset = location - (--it)->first;
        if (fileOffset < it->second.fileSize) {
            auto& lineOffsets = it->second.lineOffsets;
            auto lineIt = upper_bound(lineOffsets.begin(), lineOffsets.end(), fileOffset);
            if (lineIt == lineOffsets.end()) {
                // LCOV_EXCL_START
                return {it->first + *(lineOffsets.end() - 1), it->first + it->second.fileSize - 1};
                // LCOV_EXCL_STOP
            }

            return {*(lineIt - 1) + it->first, *(lineIt) + it->first - 1};
        }
    }
    // LCOV_EXCL_START
    return {0, 0};
    // LCOV_EXCL_STOP
}

uint32_t TranslationUnit::GetLinesOfCodes(const Node& codePart) const
{
    auto codeNodes = codePart.GetChildren(true);
    std::unordered_set<uint32_t> linesOfCode;
    for (auto& node : codeNodes) {
        auto nodeLocInfo = node->GetTranslationUnit()->GetLocationInfo(node->GetRange());
        linesOfCode.insert(nodeLocInfo.line);
        if (node->IsKindOf(Node::Kind::LITERAL_EXPRESSION)) {
            for (uint32_t codeLine = nodeLocInfo.line + 1; codeLine <= nodeLocInfo.endLine; codeLine++) {
                linesOfCode.insert(codeLine);
            }
        }
        if (node->IsKindOf(Node::Kind::COMPOUND_STATEMENT)) {
            linesOfCode.insert(nodeLocInfo.endLine);
        }
    }
    return linesOfCode.size();
}

// LCOV_EXCL_START
void TranslationUnit::PrintRangeToLogger(const SourceRange& range, const string& comment) const
{
    auto loc = GetLocationInfo(range);
    Log(HCXX::LogLevel::ERROR) << comment << " " << loc.filename << ":" << loc.line << ":" << loc.column << " - "
                               << loc.endLine << ":" << loc.endColumn << std::endl;
}
// LCOV_EXCL_STOP

void TranslationUnit::SerializeUnit(IOStream& stream)
{
    if (stream.IsOut()) {
        stream.Add<uint32_t>(myFileEntries.size());
        for (auto& it : myFileEntries) {
            stream.AddString(it.second.hash.ToString().c_str());
            stream.Add<uint32_t>(it.first);
        }
    } else {
        auto numEntries = stream.Get<uint32_t>();
        for (auto i = 0; i < numEntries; ++i) {
            std::string fileHash = stream.GetString();
            auto fileLocation = stream.Get<uint32_t>();
            auto fileEntry = FileEntriesCache::GetInstance().GetFileEntry(fileHash);
            if (fileEntry != nullptr) {
                myFileEntries.emplace(fileLocation, *fileEntry);
            }
        }
    }

    AstManager::Serialize(stream);
    CfgManager::Serialize(stream);

    if (stream.IsOut() || !stream.IsEOF()) {
        stream << mySizeOfPointer;
    }
    if (stream.IsOut()) {
        auto dirHash = UniqueId::CalcMd5Hash(myCompilerOptions.directory).ToString();
        stream << dirHash;
    } else if (!stream.IsEOF()) {
        stream << myCompilerOptions.directory;
    }
    if (stream.IsOut() || !stream.IsEOF()) {
        stream << myParseErrors;
    }
}

bool TranslationUnit::IsIfdefInRange(const SourceRange& range) const
{
    std::string source = GetSourceInRange(range);
    source.erase(std::remove_if(source.begin(), source.end(), ::isspace), source.end());
    return source.find("#if") != std::string::npos;
}

}  // namespace HCXX
