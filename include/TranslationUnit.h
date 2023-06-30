/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TRANSLATIONUNIT_H
#define COODDY_TRANSLATIONUNIT_H

#include <CompilerOptions.h>
#include <ast/AstManager.h>
#include <cfg/CfgManager.h>
#include <comment/CommentManager.h>
#include <macro/MacroManager.h>
#include <utils/FileEntry.h>
#include <utils/Log.h>
#include <utils/StrUtils.h>

#include <algorithm>
#include <map>
#include <memory>
#include <unordered_map>

#include "SourceCodePrinterManager.h"

namespace HCXX {

class DeclResolver;
class CrossTUContext;

struct LocationInfoBase {
    LocationInfoBase(uint32_t line, uint32_t column, uint32_t endLine, uint32_t endColumn, uint32_t length,
                     uint32_t offset = 0)
        : line(line), column(column), endLine(endLine), endColumn(endColumn), length(length), offset(offset)
    {}
    LocationInfoBase(){};
    bool IsInitialized()
    {
        return line != UINT32_MAX && column != UINT32_MAX;
    }

    ~LocationInfoBase() = default;

    uint32_t line = UINT32_MAX;
    uint32_t column = UINT32_MAX;
    uint32_t endLine = 0;
    uint32_t endColumn = 0;
    uint32_t length = 0;
    uint32_t offset = 0;
};

struct LocationInfo : public LocationInfoBase {
public:
    LocationInfo(uint32_t line, uint32_t column, uint32_t endLine, uint32_t endColumn, const std::string& fileName,
                 uint32_t length, uint32_t offset = 0)
        : LocationInfoBase(line, column, endLine, endColumn, length, offset), filename(fileName)
    {}

    ~LocationInfo() = default;
    const std::string& filename;
};

struct Context {
    virtual ~Context() {}
};

enum UnitLanguage { C, CPP, JAVA, MAX_LANG };

class TranslationUnit : public CompoundNode,
                        public MacroManager,
                        public CommentManager,
                        public AstManager,
                        public CfgManager,
                        public SourceCodePrinterManager {
public:
    using FileEntriesMap = std::map<Location, FileEntry&>;
    static std::atomic<uint64_t> memUsage;

    explicit TranslationUnit(const CompilerOptions& compilerOptions);
    TranslationUnit(const TranslationUnit&) = delete;

    ~TranslationUnit();

    DECLARE_KIND(CompoundNode, Node::Kind::TRANSLATION_UNIT);

    virtual LocationInfo GetLocationInfo(const SourceRange& range) const;

    FileEntriesMap& GetFileEntries()
    {
        return myFileEntries;
    }

    const FileEntriesMap& GetFileEntries() const
    {
        return myFileEntries;
    }

    const CompilerOptions& GetCompilerOptions() const
    {
        return myCompilerOptions;
    }

    const std::string& GetMainFileName() const
    {
        return this->myMainFileName;
    };

    void SetMainFileName(std::string mainFileName);

    virtual void AddFileEntry(Location fileLocation, FileEntry& entry);

    std::vector<SourceRange> GetFileEntriesRanges() const;

    /* Init internal structures for nodes after all addNode() calls */
    virtual void InitIndices();

    /* Clear data which is not useful after ast analysis */
    void ClearUnusedData();

    std::string GetSourceInRange(const SourceRange& range) const;

    bool IsIfdefInRange(const SourceRange& range) const;

    SourceRange GetLineRangeByLocation(Location location) const;

    void SetLanguage(UnitLanguage language)
    {
        myLanguage = language;
    }

    UnitLanguage GetLanguage() const
    {
        return myLanguage;
    }

    void SetSizeOfPointer(uint32_t sizeOfPointer)
    {
        mySizeOfPointer = sizeOfPointer;
    }

    uint32_t GetSizeOfPointer() const
    {
        return mySizeOfPointer;
    }

    uint32_t GetSizeOfType(const Type& type) const
    {
        return (type.IsPointer() ? mySizeOfPointer : type.GetSizeInBits());
    }

    void SerializeUnit(IOStream& stream);

    void PrintRangeToLogger(const SourceRange& range, const string& comment) const;

    uint32_t GetLinesOfCodes(const Node& codePart) const;

    void AllocateContexts(uint32_t contextsCount)
    {
        myContexts.resize(contextsCount);
    }

    template <class TContext>
    TContext& GetContext(uint32_t contextIndex) const
    {
        auto& result = myContexts[contextIndex];
        if (result == nullptr) {
            result = std::make_unique<TContext>();
        }
        return static_cast<TContext&>(*result);
    }

    void AddParseError(Location location)
    {
        myParseErrors.emplace_back(location);
    }

    auto& GetParseErrors() const
    {
        return myParseErrors;
    }

    bool HasParseErrors() const
    {
        return !myParseErrors.empty();
    }

    void SetResolver(DeclResolver* resolver)
    {
        myResolver = resolver;
    }

    DeclResolver* GetResolver() const
    {
        // COODDY_SUPPRESS ReturnMemberAddressChecker
        return myResolver;
    }

    void SetCrossTUContext(CrossTUContext& crossContext)
    {
        myCrossContext = &crossContext;
    }

    CrossTUContext* GetCrossTUContext() const
    {
        // COODDY_SUPPRESS ReturnMemberAddressChecker
        return myCrossContext;
    }

    EntryOffset GetEntryOffsetByLoc(Location loc) const;

    Location GetLocByEntryOffset(EntryOffset offset) const;

    SourceRange GetRecordDeclRangeByMember(SourceRange sourceRange) const;

    struct CodeDescription {
        SourceRange codeRange;         // source range where macro/vardecl is expanded
        SourceRange descriptionRange;  // source range where macro/vardecl is declared
        enum Type { RECORD, MACRO } type;
    };
    std::vector<CodeDescription> GetCodeDescriptions(SourceRange sourceRange) const;

private:
    void InitDeclarations();
    Location AddMacroDeclaration(const std::vector<SourceRange>& macroDecls) override;
    void CollectDescriptions(SourceRange sourceRange, std::vector<CodeDescription>& result, const LocToDeclArray& locs,
                             CodeDescription::Type type) const;

    CompilerOptions myCompilerOptions;
    std::string myMainFileName;
    FileEntriesMap myFileEntries;
    const FileEntry* myMainFileEntry;
    uint32_t mySizeOfPointer = sizeof(uintptr_t) * 8;
    mutable std::vector<std::unique_ptr<Context>> myContexts;
    std::vector<Location> myParseErrors;
    UnitLanguage myLanguage;
    DeclResolver* myResolver = nullptr;
    CrossTUContext* myCrossContext = nullptr;
    LocToDeclArray myLocToRecord;
};

using TranslationUnitPtr = std::shared_ptr<TranslationUnit>;

inline uint32_t Node::GetSizeOfType() const
{
    return myTU->GetSizeOfType(GetType()) / 8;
}

};  // namespace HCXX

#endif  // COODDY_TRANSLATIONUNIT_H
