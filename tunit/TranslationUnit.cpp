/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CrossTUContext.h>
#include <TranslationUnit.h>
#include <utils/EnvironmentUtils.h>
#include <utils/LocaleUtils.h>

#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

namespace HCXX {

std::atomic<uint64_t> TranslationUnit::memUsage = 0;

TranslationUnit::TranslationUnit(const CompilerOptions& compilerOptions)
    : AstManager(this), myCompilerOptions(compilerOptions)
{
    myTU = this;
    myRefCount = 1;
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
    MacroManager::Init();
    CfgManager::Init();
    InitDeclarations();
    std::sort(myParseErrors.begin(), myParseErrors.end(), [](auto& e1, auto& e2) { return e1 < e2; });
}

void TranslationUnit::ClearUnusedData()
{
    MacroManager::Clear();
    CommentManager::Clear();
    AstManager::Clear();
    CfgManager::Clear();
    myChildren = std::vector<NodePtr<Node>>();
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

EntryOffset TranslationUnit::GetEntryOffsetByLoc(Location loc) const
{
    auto it = myFileEntries.upper_bound(loc);
    if (it-- != myFileEntries.begin()) {
        return loc - it->first + it->second.entryOffset;
    }
    return 0;
}

Location TranslationUnit::GetLocByEntryOffset(EntryOffset offset) const
{
    for (auto& [loc, entry] : myFileEntries) {
        if (offset >= entry.entryOffset && offset < entry.entryOffset + entry.fileSize) {
            return loc + offset - entry.entryOffset;
        }
    }
    return 0;  // LCOV_EXCL_LINE
}

Location TranslationUnit::AddMacroDeclaration(const std::vector<SourceRange>& macroDecls)
{
    if (myCrossContext == nullptr) {
        return 0;
    }
    auto declLoc = macroDecls.front().begin;
    auto declOffset = GetEntryOffsetByLoc(declLoc);
    if (myCrossContext->FindDeclaration(declOffset) != nullptr) {
        return declLoc;
    }
    CrossTUContext::Declaration decl{declOffset, macroDecls.front().end - declLoc, 1};
    for (auto i = 1; i < macroDecls.size(); ++i) {
        auto offset = GetEntryOffsetByLoc(macroDecls[i].begin);
        decl.nestedDecls.emplace_back(offset, macroDecls[i].end - macroDecls[i].begin);
    }
    memUsage += sizeof(CrossTUContext::Declaration) + decl.nestedDecls.capacity() * sizeof(uint64_t);
    myCrossContext->AddDeclaration(std::move(decl));
    return declLoc;
}

void TranslationUnit::InitDeclarations()
{
    const FunctionDecl* curFunc = nullptr;
    for (auto node : GetNodes()) {
        if (auto func = Node::Cast<FunctionDecl>(node); func != nullptr && func->GetBody() != nullptr) {
            curFunc = func;
        }
        if (curFunc != nullptr && node->GetRange().end < curFunc->GetRange().end) {
            if (auto decl = node->GetType().GetPointedDeclaration(); decl != nullptr && decl->GetRange().IsValid()) {
                auto varDecl = HCXX::Node::Cast<VarDecl>(node);
                if (varDecl != nullptr || node->IsKindOf(Node::Kind::PARAM_VAR_DECL)) {
                    myLocToRecord.emplace_back(node->GetRange().begin, decl->GetRange().begin);
                }
            }
        }
        if (myCrossContext != nullptr && node->IsKindOf(Node::Kind::RECORD_DECL)) {
            auto range = node->GetRange();
            if (myCrossContext->AddDeclaration(
                    CrossTUContext::Declaration{GetEntryOffsetByLoc(range.begin), range.end - range.begin, 0})) {
                memUsage += sizeof(CrossTUContext::Declaration);
            }
        }
    }
    std::stable_sort(myLocToRecord.begin(), myLocToRecord.end());
    myLocToRecord.shrink_to_fit();

    memUsage += myLocToRecord.capacity() * sizeof(std::pair<Location, Location>);
    memUsage += GetExpansions().capacity() * sizeof(std::pair<Location, Location>);
}

void TranslationUnit::CollectDescriptions(SourceRange sourceRange, std::vector<CodeDescription>& result,
                                          const LocToDeclArray& locs, CodeDescription::Type type) const
{
    auto entry = myFileEntries.upper_bound(sourceRange.begin);
    if (entry-- == myFileEntries.begin()) {
        return;
    }
    auto it = std::lower_bound(locs.begin(), locs.end(), std::make_pair(sourceRange.begin, 0U));
    for (; it != locs.end() && it->first < sourceRange.end; ++it) {
        auto entryOffset = GetEntryOffsetByLoc(it->second);
        auto offsetInFile = it->first - entry->first;
        auto decl = myCrossContext->FindDeclaration(entryOffset);
        if (decl == nullptr || offsetInFile >= entry->second.fileSize) {
            continue;
        }
        auto codeSize = 0;
        auto source = entry->second.fileSource.c_str() + offsetInFile;
        while (std::isalpha(source[codeSize]) || std::isdigit(source[codeSize]) || source[codeSize] == '_') {
            codeSize++;
        }
        SourceRange codeRange{it->first, it->first + codeSize};
        result.emplace_back(CodeDescription{codeRange, SourceRange{it->second, it->second + decl->declSize}, type});
        for (auto [offset, size] : decl->nestedDecls) {
            auto declBegin = GetLocByEntryOffset(offset);
            result.emplace_back(CodeDescription{codeRange, SourceRange{declBegin, declBegin + size}, type});
        }
    }
}

std::vector<TranslationUnit::CodeDescription> TranslationUnit::GetCodeDescriptions(SourceRange sourceRange) const
{
    std::vector<TranslationUnit::CodeDescription> result;
    CollectDescriptions(sourceRange, result, myLocToRecord, CodeDescription::RECORD);
    CollectDescriptions(sourceRange, result, GetExpansions(), CodeDescription::MACRO);
    return result;
}

SourceRange TranslationUnit::GetRecordDeclRangeByMember(SourceRange sourceRange) const
{
    auto entryOffset = GetEntryOffsetByLoc(sourceRange.begin);
    if (entryOffset != 0) {
        auto decl = myCrossContext->FindInclusiveDeclaration(entryOffset);
        if (decl != nullptr && !decl->isMacro) {
            auto recordBegin = sourceRange.begin - entryOffset + decl->declOffset;
            return SourceRange{recordBegin, recordBegin + decl->declSize};
        }
    }
    return SourceRange{};
}

}  // namespace HCXX
