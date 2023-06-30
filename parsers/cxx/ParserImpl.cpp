/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of the cxx frontend parser based on CLang.
//

#include "ParserImpl.h"

#include <clang/Basic/SourceManagerInternals.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <llvm/Demangle/Demangle.h>
#include <llvm/Support/Host.h>
#include <utils/LocaleUtils.h>
#include <utils/StrUtils.h>

#include <filesystem>
#include <fstream>
#include <string_view>

#include "DiagnosticConsumer.h"
#include "ParserContext.h"

std::unique_ptr<HCXX::Parser> CxxParserCreate(const HCXX::CompilerOptions& globalOptions)
{
    return std::make_unique<ParserImpl>(globalOptions);
}

ParserImpl::ParserImpl(const HCXX::CompilerOptions& globalOptions)
    : myGlobalOptions(globalOptions), myFileSystem(new FileSystemProxy())
{}

std::string ParserImpl::GetQualifiedName(std::string_view mangledName) const
{
    auto p = mangledName.find('/');
    if (p == std::string::npos) {
        p = mangledName.size();
    }
    auto clangMangledName = std::string{mangledName.begin(), mangledName.begin() + p};
    auto qualifiedName = llvm::demangle(clangMangledName);
    if (p = qualifiedName.find('('); p != std::string::npos) {
        qualifiedName.erase(p);
    }
    return qualifiedName;
}

std::string ParserImpl::GetMangledNameWithoutAbi(std::string_view mangledName) const
{
    auto abiOffset = mangledName.find("/32");
    if (abiOffset != std::string::npos) {
        auto fromPos = mangledName.front() == '_' ? 1 : 0;
        return {mangledName.begin() + fromPos, mangledName.begin() + abiOffset - fromPos};
    }
    return {mangledName.begin(), mangledName.end()};
}

void ParserImpl::InitGCCOptions(clang::CompilerInvocation* ci, HCXX::CompilersInfo::CompilerInfo& info)
{
    // ignore intrinsic headers (CLang doesn't support GCC intrinsics)
    ci->getPreprocessorOpts().addMacroDef("_X86INTRIN_H_INCLUDED");
    ci->getPreprocessorOpts().addMacroDef("_XMMINTRIN_H_INCLUDED");
    ci->getPreprocessorOpts().addMacroDef("_EMMINTRIN_H_INCLUDED");
    ci->getPreprocessorOpts().addMacroDef("_TMMINTRIN_H_INCLUDED");
    ci->getPreprocessorOpts().addMacroDef("_SMMINTRIN_H_INCLUDED");

    ci->getPreprocessorOpts().addMacroUndef("__clang__");
    ci->getPreprocessorOpts().addMacroUndef("__SSE2__");

    for (auto& define : info.defines) {
        ci->getPreprocessorOpts().addMacroDef(define);
    }

    if (!ci->getLangOpts()->CPlusPlus) {
        ci->getPreprocessorOpts().addMacroDef("_Float32=float");
        ci->getPreprocessorOpts().addMacroDef("_Float32x=float");
        ci->getPreprocessorOpts().addMacroDef("_Float64=double");
        ci->getPreprocessorOpts().addMacroDef("_Float64x=double");
        ci->getPreprocessorOpts().addMacroDef("_Float128=__float128");
        ci->getPreprocessorOpts().addMacroDef("_Atomic=");
    }
    ci->getLangOpts()->DeclSpecKeyword = 1;
    ci->getPreprocessorOpts().addMacroDef("__null=0");
    ci->getPreprocessorOpts().addMacroDef("__rdtsc=__rdtsc_xxx");
    ci->getPreprocessorOpts().addMacroDef("__m128=long long");
    ci->getPreprocessorOpts().addMacroDef("__m128i=long long");
    ci->getPreprocessorOpts().addMacroDef("__m128d=long long");
    ci->getPreprocessorOpts().addMacroDef("_mm_loadu_si128(x)=(*x)");
    ci->getPreprocessorOpts().addMacroDef("_mm_setzero_si128()=0");
    ci->getPreprocessorOpts().addMacroDef("_mm_movemask_epi8(x)=(x)");
    ci->getPreprocessorOpts().addMacroDef("_mm_storeu_si128(x,y)");
    ci->getPreprocessorOpts().addMacroDef("_mm_cmpeq_epi32(x,y)=(long long)((x)==(y))");
    ci->getPreprocessorOpts().addMacroDef("_mm_xor_si128(x,y)=(long long)((x)^(y))");
    ci->getPreprocessorOpts().addMacroDef("_mm_cmpgt_epi8(x,y)=(long long)((x)>(y))");
}

// LCOV_EXCL_START
void ParserImpl::InitMSVCOptions(clang::CompilerInvocation* ci)
{
    ci->getLangOpts()->DeclSpecKeyword = 1;
    ci->getLangOpts()->MicrosoftExt = 1;
    ci->getLangOpts()->MSVCCompat = 1;
    ci->getPreprocessorOpts().addMacroDef("_WINDOWS");
}
// LCOV_EXCL_STOP

bool ParserImpl::InitPreamble(HCXX::TranslationUnit& unit, clang::CompilerInvocation& ci,
                              clang::IntrusiveRefCntPtr<llvm::vfs::FileSystem>& vfs, HCXX::Parser::Consumer& consumer)
{
    auto result = myFileSystem->getBufferForFile(unit.GetMainFileName());
    if (!result) {
        return false;  // LCOV_EXCL_LINE: HCAT-2778
    }
    std::unique_ptr<llvm::MemoryBuffer> buffer = std::move(*result);
    if (consumer.GetParseFlags() & ParseFlags::INCLUDES_ONLY) {
        auto preamble = std::make_unique<PreambleData>(myFileSystem.get());
        preamble->Build(ci, *buffer.get(), consumer);
        return false;
    }

    std::unique_lock<std::mutex> lock(myMutex);
    auto& preamble = myPreambles[unit.GetMainFileName()];
    if (preamble == nullptr) {
        preamble = std::make_unique<PreambleData>(myFileSystem.get());
    }
    lock.unlock();

    if (!preamble->Apply(ci, vfs, *buffer.get(), consumer)) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "Can't build preamble: " << unit.GetMainFileName() << std::endl;
        return false;
        // LCOV_EXCL_STOP
    }
    buffer.release();
    return true;
}

std::unique_ptr<HCXX::Parser::Context> ParserImpl::CreateContext(HCXX::DeclResolver* declResolver)
{
    return std::make_unique<ParserContext>();
}

void SetCppLangOpts(clang::LangOptions& opts)
{
    opts.Exceptions = 1;
    opts.CXXExceptions = 1;
}

std::string ParserImpl::ComputeTargetTriple(HCXX::CompilerOptions& options)
{
    std::string triple = options.GetOptionValue("--target");
    if (triple.empty()) {
#ifndef _MSC_VER
        triple = llvm::sys::getDefaultTargetTriple();
#else
        triple = "x86_64-w64-windows-gnu";
#endif
    }
    if (options.HasOption("-m16") || options.HasOption("-m32")) {
        triple = llvm::Triple(llvm::Twine(triple)).get32BitArchVariant().str();
    } else if (options.HasOption("-mx32") || options.HasOption("-m64")) {
        triple = llvm::Triple(llvm::Twine(triple)).get64BitArchVariant().str();
    }
    if (HCXX::StrUtils::StartsWith(triple, "armeb")) {
        // this macro is added to be compatible with some gcc compilers
        options.options.emplace_back("-D__AARCH64EB__");
    }
    return triple;
}

std::string ParserImpl::ProcessGccToolchainPath(HCXX::CompilerOptions& options, const string& triple)
{
    std::string gccToolChainPath;
    for (auto i = 0; i < options.options.size(); ++i) {
        if (options[i].rfind("--gcc-toolchain=", 0) == 0) {
            gccToolChainPath = HCXX::StrUtils::Split(options[i], '=').back();
            options.options.erase(options.options.begin() + i);
        }
    }

    if (!gccToolChainPath.empty()) {
        llvm::Triple clangTriple(triple);
        auto toolchainTriple = clangTriple.getArchName().str() + "-" + clangTriple.getOSAndEnvironmentName().str();
        auto compilerBin = std::filesystem::path(options.compiler).filename().string();
        auto compilerPathFromToolChain = gccToolChainPath + "/bin/" + toolchainTriple + "-" + compilerBin;
        if (std::filesystem::exists(compilerPathFromToolChain)) {
            options.compiler = std::move(compilerPathFromToolChain);  // LCOV_EXCL_LINE
        }
    }
    return gccToolChainPath;
}

std::shared_ptr<clang::CompilerInvocation> ParserImpl::CreateCompilerInvocation(HCXX::TranslationUnit& unit,
                                                                                clang::DiagnosticsEngine& diags)
{
    auto options = unit.GetCompilerOptions();
    options = options + myGlobalOptions;

    std::string triple = ComputeTargetTriple(options);
    std::string toolchainPath = ProcessGccToolchainPath(options, triple);

    auto& curCompilerInfo = myCompilerInfo.GetCompilerInfo(options);
    if (HCXX::StrUtils::StartsWith(curCompilerInfo.triple, "arm") ||
        HCXX::StrUtils::StartsWith(curCompilerInfo.triple, "aarch")) {
        triple = curCompilerInfo.triple;  // LCOV_EXCL_LINE
    }
    bool isSoftFloat = !options.HasOption("-mhard-float") && options.GetOptionValue("-mfloat-abi") != "hard";

    ConvertOptions(options);

    std::vector<const char*> clangArr;
    auto& optionsArr = options.options;
    clangArr.reserve(optionsArr.size());
    for (const auto& op : optionsArr) {
        clangArr.push_back(op.c_str());
    }
    clangArr.push_back("-Wno-everything");
    clangArr.push_back("-fdelayed-template-parsing");

    clang::ArrayRef<const char*> argsArr(clangArr.data(), clangArr.size());

    std::shared_ptr<clang::CompilerInvocation> ci(new clang::CompilerInvocation());
    clang::CompilerInvocation::CreateFromArgs(*ci, argsArr, diags);
    ci->getPreprocessorOutputOpts().ShowComments = 1;
    ci->getPreprocessorOutputOpts().ShowLineMarkers = 1;
    ci->getTargetOpts().Triple = std::move(triple);

    unit.SetLanguage(ci->getLangOpts()->CPlusPlus ? HCXX::UnitLanguage::CPP : HCXX::UnitLanguage::C);
    if (unit.GetLanguage() == HCXX::UnitLanguage::CPP) {
        SetCppLangOpts(*ci->getLangOpts());
    }

    // for now we use gcc only
    InitGCCOptions(ci.get(), curCompilerInfo);

    if (!toolchainPath.empty()) {
        ci->getHeaderSearchOpts().Sysroot.clear();
    }
    if (isSoftFloat) {
        ci->getTargetOpts().FeatureMap.insert(std::make_pair("soft-float", true));
    }
    for (auto& includePath : curCompilerInfo.includes) {
        ci->getHeaderSearchOpts().AddPath(includePath, clang::frontend::System, false, true);
    }
    return ci;
}

std::unique_ptr<clang::ASTUnit> ParserImpl::CreateASTUnit(HCXX::TranslationUnit& unit, Consumer& consumer,
                                                          Context* context)
{
    // Configure the diagnostics
    ::DiagnosticConsumer* diagConsumer = new ::DiagnosticConsumer(&unit, &statistics);
    clang::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diags(
        clang::CompilerInstance::createDiagnostics(new clang::DiagnosticOptions, diagConsumer));

    auto ci = CreateCompilerInvocation(unit, *diags);

    std::unique_ptr<clang::ASTUnit> astUnit;
    auto pch = std::make_shared<clang::PCHContainerOperations>();

    if (context == nullptr || (consumer.GetParseFlags() & Parser::IN_MEMORY_FILES)) {
        clang::IntrusiveRefCntPtr<llvm::vfs::FileSystem> vfs(myFileSystem);
        bool initPreamble = consumer.GetParseFlags() & CREATE_PREAMBLE;
        if (initPreamble && !InitPreamble(unit, *ci, vfs, consumer)) {
            return nullptr;
        }
        auto fileManger = new clang::FileManager(clang::FileSystemOptions(), vfs);
        astUnit = clang::ASTUnit::LoadFromCompilerInvocation(std::move(ci), std::move(pch), diags, fileManger);
    } else {
        ci->getFrontendOpts().SkipFunctionBodies = true;
        ParserContext::Action action(*static_cast<ParserContext*>(context));
        astUnit.reset(clang::ASTUnit::LoadFromCompilerInvocationAction(std::move(ci), std::move(pch), diags, &action));
    }

    if (unit.HasParseErrors()) {
        consumer.OnParseError();
    }
    if (diagConsumer->GetFatalErrorCount() > statistics.maxFatalErrorCount) {
        return nullptr;
    }
    return std::move(astUnit);
}

bool ParserImpl::ParseAST(HCXX::TranslationUnit& unit, Consumer& consumer, Context* context)
{
    for (auto& excl : {".S", ".s", "CMakeCCompilerId.c", "CMakeCXXCompilerId.cpp", "CMakeCCompilerABI.c"}) {
        if (HCXX::StrUtils::EndsWith(unit.GetMainFileName(), excl)) {
            ++statistics.skippedCount;
            return false;
        }
    }
    ++statistics.totalParsedCount;
    auto astUnit = CreateASTUnit(unit, consumer, context);
    if (astUnit == nullptr) {
        // LCOV_EXCL_START
        ++statistics.failedCount;
        HCXX::Log(HCXX::LogLevel::ERROR) << "Can't parse AST: " << unit.GetMainFileName() << std::endl;
        return false;
        // LCOV_EXCL_STOP
    }

    if (unit.GetParseErrors().empty()) {
        ++statistics.succeedCount;
    } else {
        ++statistics.partiallyParsedCount;
    }

    HCXX::Log(HCXX::LogLevel::INFO) << "AST has been parsed: " << unit.GetMainFileName() << std::endl;
    clang::ASTContext& astContext = astUnit->getASTContext();
    unit.SetSizeOfPointer(astContext.getTypeInfo(astContext.getIntPtrType().getTypePtr()).Width);
    HCXX::ASTVisitor(astContext, unit, statistics).Traverse();
    HCXX::Log(HCXX::LogLevel::INFO) << "AST has been converted: " << unit.GetMainFileName() << std::endl;
    return true;
}

void ParserImpl::AddInMemoryFile(const std::string& filePath, std::string fileContent)
{
    myFileSystem->AddFile(filePath, std::move(fileContent));
}
