/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_JAVA_PARSER_JAVAPARSERIMPL_H_
#define COODDY_ANALYZER_SOURCE_JAVA_PARSER_JAVAPARSERIMPL_H_

#include <TUnitsPackage.h>
#include <ast/CxxRecordDecl.h>

#include "../include/Parser.h"

class JavaParserImpl : public HCXX::Parser {
public:
    explicit JavaParserImpl();

    std::unique_ptr<Context> CreateContext(HCXX::DeclResolver* declResolver) override;

    bool ParseAST(HCXX::TranslationUnit& unit, Consumer& consumer, Context* context) override;

    bool ParseAST(HCXX::TranslationUnit& unit, HCXX::DeclResolver* resolver, HCXX::CxxRecordDecl& mainClass,
                  bool isSystem);

    void AddInMemoryFile(const std::string& filePath, std::string fileContent) override;

private:
    HCXX::TUnitsPackage::Ptr mySystemPackage;
};

#endif  // COODDY_ANALYZER_SOURCE_JAVA_PARSER_JAVAPARSERIMPL_H_
