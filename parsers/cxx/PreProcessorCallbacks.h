/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// class PreProcessorCallbacks adds to TranslationUnit information
// about file entries and macro expansions
//
#ifndef COODDY_ANALYZER_SOURCE_CLANG_FACADE_PREPROCESSORCALLBACKS_H_
#define COODDY_ANALYZER_SOURCE_CLANG_FACADE_PREPROCESSORCALLBACKS_H_

#include <clang/Lex/MacroArgs.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <utils/FileEntry.h>

#include <filesystem>

#include "Convertors.h"
#include "FileSystemProxy.h"

class PreProcessorCallbacks : public clang::PPCallbacks {
    HCXX::TranslationUnit& myUnit;
    const clang::Preprocessor* myPreprocessor;

    void FileChanged(clang::SourceLocation loc, FileChangeReason reason, clang::SrcMgr::CharacteristicKind fileType,
                     clang::FileID prevFID) override
    {
        if (reason != FileChangeReason::EnterFile || fileType != clang::SrcMgr::C_User) {
            return;
        }
        auto& sm = myPreprocessor->getSourceManager();
        auto fileId = sm.getFileID(loc);
        auto* entry = sm.getFileEntryForID(fileId);
        if (entry == nullptr) {
            return;
        }
        std::string filePath = HCXX::EnvironmentUtils::NormalizePath(entry->getName().str());

        auto* fileEntry = HCXX::FileEntriesCache::GetInstance().GetFileEntry(filePath, [&](std::string& result) {
            bool invalid = false;
            result = sm.getBufferData(fileId, &invalid).str();
            if (invalid) {
                return false;  // LCOV_EXCL_LINE
            }
            return true;
        });
        if (fileEntry == nullptr) {
            // LCOV_EXCL_START
            HCXX::Log(HCXX::LogLevel::ERROR) << "Can't read file " << filePath << std::endl;
            return;
            // LCOV_EXCL_STOP
        }
        myUnit.AddFileEntry(HCXX::ConvertLocation(sm, loc), *fileEntry);
    }

    void MacroExpands(const clang::Token& macroNameTok, const clang::MacroDefinition& macroDefinition,
                      clang::SourceRange sourceRange, const clang::MacroArgs* macroArgs) override
    {
        auto& sm = myPreprocessor->getSourceManager();
        std::vector<std::vector<std::string>> args;

        if (macroArgs != nullptr) {
            for (unsigned i = 0, n = macroArgs->getNumMacroArguments(); i < n; ++i) {
                std::string str;
                auto* token = macroArgs->getUnexpArgument(i);

                unsigned argLen = 0;
                std::vector<std::string> arg;

                if (token != nullptr) {
                    argLen = clang::MacroArgs::getArgLength(token);
                    arg.reserve(argLen);
                }

                for (unsigned j = 0; j < argLen; ++j) {
                    auto tokenInfo = token[j].getIdentifierInfo();

                    tokenInfo != nullptr ? arg.emplace_back(tokenInfo->getName())
                                         : arg.emplace_back(myPreprocessor->getSpelling(token[j]));
                }

                if (!arg.empty()) {
                    args.emplace_back(arg);
                }
            }
        }
        HCXX::SourceRange range = HCXX::ConvertSourceRange(sm, myPreprocessor->getLangOpts(), sourceRange);
        auto macroInfo = macroDefinition.getMacroInfo();
        clang::SourceRange clangDeclSourceRange(macroInfo->getDefinitionLoc(), macroInfo->getDefinitionEndLoc());

        myUnit.AddMacroExpansion(macroNameTok.getIdentifierInfo()->getName().data(), range, std::move(args));
    }

    virtual void MacroDefined(const clang::Token& macroNameTok, const clang::MacroDirective* macroDirective)
    {
        auto& sm = myPreprocessor->getSourceManager();
        auto macroName = macroNameTok.getIdentifierInfo()->getName().data();

        clang::SourceRange clangNameSourceRange = macroDirective->getLocation();
        const HCXX::SourceRange nameRange =
            HCXX::ConvertSourceRange(sm, myPreprocessor->getLangOpts(), clangNameSourceRange);

        const clang::MacroInfo* macroInfo = macroDirective->getMacroInfo();
        clang::SourceRange clangDeclSourceRange =
            clang::SourceRange(macroInfo->getDefinitionLoc(), macroInfo->getDefinitionEndLoc());
        const HCXX::SourceRange declRange =
            HCXX::ConvertSourceRange(sm, myPreprocessor->getLangOpts(), clangDeclSourceRange);

        myUnit.AddMacro(std::move(macroName), declRange, nameRange, myUnit.GetLocationInfo(nameRange).filename.empty(),
                        macroInfo->getNumParams());
    }

public:
    PreProcessorCallbacks(HCXX::TranslationUnit& unit, const clang::Preprocessor* preprocessor)
        : myUnit(unit), myPreprocessor(preprocessor)
    {}
    ~PreProcessorCallbacks() = default;
};

#endif  // COODDY_ANALYZER_SOURCE_CLANG_FACADE_PREPROCESSORCALLBACKS_H_
