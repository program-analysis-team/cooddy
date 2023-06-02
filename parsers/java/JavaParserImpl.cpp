/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "JavaParserImpl.h"

#include <pthread.h>
#include <utils/EnvironmentUtils.h>
#include <utils/Log.h>

#include <mutex>

#include "ASTConverter.h"
#include "JavaLexer.h"
#include "JavaParser.h"
#include "JavaResolver.h"
#include "ParseErrorListener.h"

struct JavaParserContext : public HCXX::Parser::Context, public HCXX::TUnitHandler {
    HCXX::DeclResolver* resolver = nullptr;
    JavaParserImpl* parser = nullptr;

    void TUnitInitDecl(HCXX::TranslationUnit& unit, const HCXX::Node* declNode) override
    {
        if (declNode == nullptr) {
            ASTConverter::CreateMainClass(unit);
        } else {
            unit.GetResolver()->InitTUnitData();
            auto classDecl = const_cast<HCXX::Node*>(declNode);
            parser->ParseAST(unit, resolver, *HCXX::Node::Cast<HCXX::CxxRecordDecl>(classDecl), true);
        }
    }
};

std::unique_ptr<HCXX::Parser> JavaParserCreate()
{
    return std::make_unique<JavaParserImpl>();
}

JavaParserImpl::JavaParserImpl()
{
    mySystemPackage = HCXX::TUnitsPackage::CreateArchive(HCXX::EnvironmentUtils::GetInstalledJarPath("jdk.jar"));
}

bool JavaParserImpl::ParseAST(HCXX::TranslationUnit& unit, Consumer& consumer, Context* context)
{
    auto& mainClass = ASTConverter::CreateMainClass(unit);
    auto javaContext = static_cast<JavaParserContext*>(context);
    if (javaContext == nullptr) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "Java parser context is null\n";
        return false;
        // LCOV_EXCL_STOP
    }
    return ParseAST(unit, javaContext->resolver, mainClass, false);
}

bool JavaParserImpl::ParseAST(HCXX::TranslationUnit& unit, HCXX::DeclResolver* resolver, HCXX::CxxRecordDecl& mainClass,
                              bool isSystem)
{
    auto& filePath = unit.GetMainFileName();
    auto fileEntry = HCXX::FileEntriesCache::GetInstance().GetFileEntry(filePath);
    if (fileEntry == nullptr) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "File entry is empty: " << filePath << "\n";
        return false;
        // LCOV_EXCL_STOP
    }
    unit.AddFileEntry(1, *fileEntry);

    antlr4::ANTLRInputStream input(fileEntry->fileSource.c_str(), fileEntry->fileSource.size());
    JavaLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    JavaParser parser(&tokens);

    HCXX::ParseErrorListener listener(filePath);
    parser.removeErrorListeners();
    parser.addErrorListener(&listener);

    auto unitContext = parser.compilationUnit();
    JavaResolver unitResolver(unitContext->packageDeclaration(), resolver);
    unitResolver.AddImportDecl("java.lang");

    ASTConverter converter(unit, mainClass, *unitContext, parser, unitResolver, isSystem);
    converter.Convert();

    bool hasErrors = parser.getNumberOfSyntaxErrors() > 0;
    if (hasErrors) {
        unit.AddParseError(0);  // LCOV_EXCL_LINE
    }
    unit.SetLanguage(HCXX::UnitLanguage::JAVA);
    unit.InitIndices();
    return !hasErrors;
}

void JavaParserImpl::AddInMemoryFile(const std::string& filePath, std::string fileContent) {}  // LCOV_EXCL_LINE

std::unique_ptr<HCXX::Context> JavaParserImpl::CreateContext(HCXX::DeclResolver* declResolver)
{
    auto result = make_unique<JavaParserContext>();
    result->resolver = declResolver;
    result->parser = this;
    if (declResolver != nullptr && mySystemPackage != nullptr) {
        declResolver->AddPackage(mySystemPackage, *result);
    }
    return std::move(result);
}
