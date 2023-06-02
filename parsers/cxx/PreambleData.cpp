/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "PreambleData.h"

#include <utils/EnvironmentUtils.h>

#include "DiagnosticConsumer.h"

bool PreambleData::Apply(clang::CompilerInvocation& ci, clang::IntrusiveRefCntPtr<llvm::vfs::FileSystem>& vfs,
                         llvm::MemoryBuffer& memBuffer, HCXX::Parser::Consumer& consumer)
{
    auto bounds = clang::ComputePreambleBounds(*ci.getLangOpts(), memBuffer, 0);

    if (myPreamble == nullptr || !myPreamble->CanReuse(ci, memBuffer, bounds, *myFileSystem)) {
        if (!Build(ci, memBuffer, consumer)) {
            return false;  // LCOV_EXCL_LINE: HCAT-2778
        }
    }
    myPreamble->OverridePreamble(ci, vfs, &memBuffer);
    return true;
}

class HeadersCallback : public clang::PPCallbacks {
public:
    HeadersCallback(const clang::SourceManager& sm, HCXX::Parser::Consumer* consumer) : mySM(sm), myConsumer(consumer)
    {}
    ~HeadersCallback() = default;

    void InclusionDirective(clang::SourceLocation hashLoc, const clang::Token& includeTok, llvm::StringRef fileName,
                            bool isAngled, clang::CharSourceRange /*filenameRange*/, const clang::FileEntry* file,
                            llvm::StringRef /*searchPath*/, llvm::StringRef /*relativePath*/,
                            const clang::Module* /*imported*/, clang::SrcMgr::CharacteristicKind fileKind) override
    {
        if (myConsumer == nullptr || file == nullptr || fileKind != clang::SrcMgr::C_User) {
            return;
        }
        clang::FileID fileID = mySM.getFileID(hashLoc);
        auto* includingFileEntry = mySM.getFileEntryForID(fileID);
        if (includingFileEntry != nullptr) {
            if (myCurFileID != fileID) {
                myCurFileID = fileID;
                myCurBuffer = mySM.getBufferOrFake(fileID).getBuffer();
            }
            std::string prolog = myCurBuffer.take_front(mySM.getFileOffset(hashLoc)).str();
            myConsumer->OnIncludeParsed(HCXX::EnvironmentUtils::NormalizePath(file->getName().str()),
                                        HCXX::EnvironmentUtils::NormalizePath(includingFileEntry->getName().str()),
                                        std::move(prolog));
        }
    }

private:
    const clang::SourceManager& mySM;
    HCXX::Parser::Consumer* myConsumer;
    clang::FileID myCurFileID;
    clang::StringRef myCurBuffer;
};

class PreambleCallbacks : public clang::PreambleCallbacks {
public:
    explicit PreambleCallbacks(HCXX::Parser::Consumer* consumer) : myConsumer(consumer) {}
    ~PreambleCallbacks() = default;

    void BeforeExecute(clang::CompilerInstance& compilerInstance) override
    {
        mySM = &compilerInstance.getSourceManager();
    }

    std::unique_ptr<clang::PPCallbacks> createPPCallbacks() override
    {
        return std::make_unique<HeadersCallback>(*mySM, myConsumer);
    }

private:
    HCXX::Parser::Consumer* myConsumer;
    const clang::SourceManager* mySM = nullptr;
};

bool PreambleData::Build(clang::CompilerInvocation& ci, llvm::MemoryBuffer& memBuffer, HCXX::Parser::Consumer& consumer)
{
    DiagnosticConsumer diagsConsumer(nullptr);
    llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diagsEngine =
        clang::CompilerInstance::createDiagnostics(&ci.getDiagnosticOpts(), &diagsConsumer, false);

    ci.getFrontendOpts().SkipFunctionBodies = true;
    ci.getPreprocessorOpts().WriteCommentListToPCH = false;

    if (consumer.GetParseFlags() & HCXX::Parser::INCLUDES_ONLY) {
        ci.getPreprocessorOpts().SingleFileParseMode = true;
    }

    auto bounds = clang::ComputePreambleBounds(*ci.getLangOpts(), memBuffer, 0);

    PreambleCallbacks preambleCallbacks(&consumer);
    clang::IntrusiveRefCntPtr<llvm::vfs::FileSystem> vfs(myFileSystem);
    auto result =
        clang::PrecompiledPreamble::Build(ci, &memBuffer, bounds, *diagsEngine, vfs,
                                          std::make_shared<clang::PCHContainerOperations>(), true, preambleCallbacks);
    ci.getFrontendOpts().SkipFunctionBodies = false;
    myPreamble = result ? std::make_unique<clang::PrecompiledPreamble>(std::move(*result)) : nullptr;
    return !!result;
}
