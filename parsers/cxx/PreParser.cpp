/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CrossTUContext.h>
#include <clang/Frontend/FrontendActions.h>

#include "ParserImpl.h"

using namespace HCXX;
using namespace std;

namespace HCXX {
extern bool CanDefineSizeForType(const clang::Type& clangType, clang::TagDecl* tagDecl);
extern std::string MakeQualifiedName(const clang::FunctionDecl& func);
};  // namespace HCXX

struct PreParserFileSystem : public FileSystemProxy {
    PreParserFileSystem(const FileEntry& entry) : myEntry(entry) {}

    llvm::ErrorOr<llvm::vfs::Status> status(const llvm::Twine& path) override
    {
        return getUnderlyingFS().status(path);
    }

    llvm::ErrorOr<std::unique_ptr<llvm::vfs::File>> openFileForRead(const llvm::Twine& path) override
    {
        if (EnvironmentUtils::NormalizePath(path.str()) == myEntry.filePath) {
            return std::make_unique<File>(GetStatus(myEntry), myEntry.fileSource);
        }
        return getUnderlyingFS().openFileForRead(path);
    }

private:
    const FileEntry& myEntry;
};

class ClangPreParser : public clang::RecursiveASTVisitor<ClangPreParser> {
public:
    ClangPreParser(ParserImpl& owner, Parser::VirtualFunctionsConsumer& consumer, TranslationUnit& unit)
        : myOwner(owner),
          myConsumer(consumer),
          myUnit(unit),
          myCrossContext(*unit.GetCrossTUContext()),
          myOptions(unit.GetCompilerOptions())
    {
        myOwner.ConvertOptions(myOptions);
    }

    ~ClangPreParser()
    {
        struct EmptyConsumer : public clang::DiagnosticConsumer {
            void HandleDiagnostic(clang::DiagnosticsEngine::Level diagLevel, const clang::Diagnostic& info) override {}
        };
        auto diags = clang::CompilerInstance::createDiagnostics(new clang::DiagnosticOptions, new EmptyConsumer);
        auto ci = myOwner.CreateCompilerInvocation(myUnit, *diags);
        auto fileManger = new clang::FileManager(clang::FileSystemOptions(), new PreParserFileSystem(myEntry));
        auto pch = std::make_shared<clang::PCHContainerOperations>();
        ci->getFrontendOpts().SkipFunctionBodies = true;
        auto astUnit = clang::ASTUnit::LoadFromCompilerInvocation(std::move(ci), std::move(pch), diags, fileManger);

        if (astUnit != nullptr) {
            TraverseAST(astUnit->getASTContext());
        }
    }

    bool CanAddUnit(TranslationUnit& unit)
    {
        auto options = unit.GetCompilerOptions();
        if (myOptions.directory != options.directory) {
            return false;
        }
        myOwner.ConvertOptions(options);
        if (options.options.size() != myOptions.options.size()) {
            return false;
        }
        for (auto i = 1; i < options.options.size(); ++i) {
            if (options.options[i] != myOptions.options[i]) {
                return false;
            }
        }
        return true;
    }

    void AddFileEntry(TranslationUnit& unit, const FileEntry& entry)
    {
        auto entryPath = EnvironmentUtils::NormalizePath(entry.filePath);
        if (myEntry.filePath.empty()) {
            myEntry.filePath = entryPath;
            myEntry.fileSource = entry.fileSource;
        } else {
            myEntry.fileSource += "\n#include \"" + entryPath + "\"";
        }
        myUnitsMap.emplace(entryPath, &unit);
        myUnitsMap.emplace(unit.GetMainFileName(), &unit);
    }

    bool shouldVisitTemplateInstantiations() const
    {
        return true;
    }

    bool VisitVarDecl(clang::VarDecl* varDecl)
    {
        if (varDecl == nullptr || varDecl->isLocalVarDecl()) {
            return true;
        }
        auto typeSourceInfo = varDecl->getTypeSourceInfo();
        CrossTUContext::VarInfo varInfo;
        auto type = varDecl->getType().getCanonicalType().getTypePtr();
        if (typeSourceInfo != nullptr && typeSourceInfo->getTypeLoc().getAs<clang::ConstantArrayTypeLoc>() &&
            type != nullptr && CanDefineSizeForType(*type, nullptr)) {
            varInfo.sizeOfType = varDecl->getASTContext().getTypeInfo(type).Width;
        }
        varInfo.unit = varDecl->hasExternalStorage() ? nullptr : GetCurUnit(varDecl);

        myCrossContext.AddGlobalVar(varDecl->getNameAsString(), std::move(varInfo));
        return true;
    }

    bool VisitFunctionDecl(clang::FunctionDecl* funcDecl)
    {
        if (funcDecl == nullptr || !funcDecl->isThisDeclarationADefinition()) {
            return true;
        }
        if (GetCurUnit(funcDecl) != nullptr) {
            auto fqName = MakeQualifiedName(*funcDecl);
            if (auto funcName = funcDecl->getNameAsString(); funcName != fqName) {
                myCrossContext.AddFunctionDef(std::move(funcName), myCurUnit);
            }
            myCrossContext.AddFunctionDef(std::move(fqName), myCurUnit);
        }
        return true;
    }

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* recordDecl)
    {
        if (recordDecl == nullptr || !recordDecl->isThisDeclarationADefinition() || recordDecl->isEmpty()) {
            return true;
        }
        auto& sm = recordDecl->getASTContext().getSourceManager();
        auto entry = sm.getFileEntryForID(sm.getFileID(recordDecl->getBeginLoc()));
        if (sm.isInSystemHeader(recordDecl->getBeginLoc()) || entry == nullptr) {
            return true;
        }
        auto recordId = RecordDecl::MakeUniqueId(entry->getName().str(), sm.getFileOffset(recordDecl->getBeginLoc()),
                                                 recordDecl->getNameAsString());
        myRecordIds.emplace(recordDecl, recordId);

        std::vector<RecordUniqueId> baseRecords;
        for (auto base : ASTVisitor::GetBaseRecords(*recordDecl)) {
            if (auto baseId = myRecordIds[base]; baseId != 0) {
                baseRecords.push_back(baseId);
            }
        }
        auto handle = myConsumer.OnClassDeclaration(recordId, baseRecords);
        if (handle == nullptr) {
            return true;  // LCOV_EXCL_LINE: HCAT-2778
        }
        for (auto method : recordDecl->methods()) {
            if (method->isVirtual()) {
                auto signature = clang::ASTNameGenerator(recordDecl->getASTContext()).getName(method);
                myConsumer.OnVirtualFunction(handle, method->getNameAsString(), signature);
            }
        }
        return true;
    }

private:
    TranslationUnit* GetCurUnit(clang::Decl* decl)
    {
        auto& sm = decl->getASTContext().getSourceManager();
        auto fileId = sm.getFileID(decl->getLocation());
        if (fileId == myCurFileId) {
            return myCurUnit;
        }
        myCurUnit = nullptr;
        myCurFileId = fileId;
        if (auto entry = sm.getFileEntryForID(fileId); entry != nullptr) {
            auto unit = myUnitsMap.find(entry->getName().str());
            myCurUnit = unit != myUnitsMap.end() ? unit->second : nullptr;
        }
        return myCurUnit;
    }

private:
    ParserImpl& myOwner;
    TranslationUnit& myUnit;
    CrossTUContext& myCrossContext;
    CompilerOptions myOptions;
    FileEntry myEntry;
    Parser::VirtualFunctionsConsumer& myConsumer;
    std::unordered_map<clang::CXXRecordDecl*, RecordUniqueId> myRecordIds;
    clang::FileID myCurFileId;
    TranslationUnit* myCurUnit = nullptr;
    std::unordered_map<std::string, TranslationUnit*> myUnitsMap;
};

struct PreParseContext : public HCXX::Context {
    PreParseContext(Parser::VirtualFunctionsConsumer& consumer) : myConsumer(consumer) {}

    void Parse(ParserImpl& owner, TranslationUnit& unit, const FileEntry& entry)
    {
        std::unique_lock<std::mutex> lock(myMutex);
        std::unique_ptr<ClangPreParser> parser;
        bool canAddUnit = myParser == nullptr || myParser->CanAddUnit(unit);
        if (++myBatchCounter == UNITS_IN_BATCH || !canAddUnit) {
            std::swap(parser, myParser);  // LCOV_EXCL_LINE
            myBatchCounter = 0;           // LCOV_EXCL_LINE
        }
        if (myParser == nullptr) {
            myParser = std::make_unique<ClangPreParser>(owner, myConsumer, unit);
        }
        myParser->AddFileEntry(unit, entry);
        lock.unlock();
    }

private:
    Parser::VirtualFunctionsConsumer& myConsumer;
    std::unique_ptr<ClangPreParser> myParser;
    uint32_t myBatchCounter = 0;
    static const uint32_t UNITS_IN_BATCH = 16;
    std::mutex myMutex;
};

std::unique_ptr<Context> ParserImpl::CreateDefinitionsContext(VirtualFunctionsConsumer& consumer)
{
    return std::make_unique<PreParseContext>(consumer);
}

bool ParserImpl::ParseDefinitions(TranslationUnit& unit, Context* context)
{
    auto parseContext = static_cast<PreParseContext*>(context);
    if (parseContext == nullptr) {
        return true;
    }
    auto fileEntry = FileEntriesCache::GetInstance().GetFileEntry(unit.GetMainFileName());
    if (fileEntry != nullptr) {
        parseContext->Parse(*this, unit, *fileEntry);
    }
    return true;
}

void ParserImpl::ParseUsages(HCXX::TranslationUnit& unit)
{
    auto fileEntry = FileEntriesCache::GetInstance().GetFileEntry(unit.GetMainFileName());
    if (fileEntry == nullptr) {
        return;
    }
    std::string token;
    for (auto c : fileEntry->fileSource) {
        if (isdigit(c) || isalpha(c) || c == '~' || c == '_') {
            token += c;
        } else if (!token.empty()) {
            unit.GetCrossTUContext()->AddFunctionUsage(token, &unit);
            token.clear();
        }
    }
}
