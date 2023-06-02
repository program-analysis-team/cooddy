/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Declaration of the cxx frontend parser based on CLang.
//

#ifndef COODDY_PARSERIMPL_H
#define COODDY_PARSERIMPL_H

#include <Parser.h>
#include <clang/Frontend/ASTUnit.h>

#include <string>
#include <string_view>
#include <unordered_map>

#include "ASTVisitor.h"
#include "CompilersInfo.h"
#include "FileSystemProxy.h"
#include "llvm/IR/LLVMContext.h"

class ParserImpl : public HCXX::Parser {
    HCXX::CompilerOptions myGlobalOptions;
    HCXX::CompilersInfo myCompilerInfo;
    clang::IntrusiveRefCntPtr<FileSystemProxy> myFileSystem;
    std::unordered_map<std::string, std::unique_ptr<PreambleData>> myPreambles;
    std::mutex myMutex;

    void InitGCCOptions(clang::CompilerInvocation* ci, HCXX::CompilersInfo::CompilerInfo& info);

    void InitMSVCOptions(clang::CompilerInvocation* ci);

    bool InitPreamble(HCXX::TranslationUnit& unit, clang::CompilerInvocation& ci,
                      clang::IntrusiveRefCntPtr<llvm::vfs::FileSystem>& vfs, HCXX::Parser::Consumer& consumer);

    std::string ComputeTargetTriple(HCXX::CompilerOptions& options);

    std::string ProcessGccToolchainPath(HCXX::CompilerOptions& options, const string& triple);

public:
    explicit ParserImpl(const HCXX::CompilerOptions& globalOptions);

    std::string GetQualifiedName(std::string_view mangledName) const override;

    std::string GetMangledNameWithoutAbi(std::string_view mangledName) const override;

    void ConvertOptions(HCXX::CompilerOptions& options) override;

    std::unique_ptr<Context> CreateContext(HCXX::DeclResolver* declResolver) override;

    bool ParseAST(HCXX::TranslationUnit& unit, Consumer& consumer, Context* context) override;

    std::unique_ptr<Context> CreateDefinitionsContext(VirtualFunctionsConsumer& consumer) override;

    bool ParseDefinitions(HCXX::TranslationUnit& unit, Context* context) override;

    void ParseUsages(HCXX::TranslationUnit& unit) override;

    void AddInMemoryFile(const std::string& filePath, std::string fileContent) override;

    std::unique_ptr<clang::ASTUnit> CreateASTUnit(HCXX::TranslationUnit& unit, Consumer& consumer, Context* context);

    std::shared_ptr<clang::CompilerInvocation> CreateCompilerInvocation(HCXX::TranslationUnit& unit,
                                                                        clang::DiagnosticsEngine& diags);
};

#endif  // COODDY_PARSERIMPL_H
