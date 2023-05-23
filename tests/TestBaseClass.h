/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_TESTBASECLASS_H
#define COODDY_TESTBASECLASS_H

#include <gtest/gtest.h>

#include "../cooddy/TaintMacroCliDefinition.h"
#include "CompilerOptionsList.h"
#include "TestErrorCheckConsumer.h"

class TestBaseClass : public ::testing::Test {
public:
    static HCXX::Parser& GetParser(const std::filesystem::path* jsonPath = nullptr)
    {
        if (jsonPath != nullptr && jsonPath->parent_path().filename() == "java") {
            static auto parser = HCXX::Parser::Create(HCXX::CompilerOptions{{"-x=java"}});
            return *parser;
        }
        static auto parser = HCXX::Parser::Create(
            HCXX::CompilerOptions{{TaintMacroCliDefinition("EXTER_ATTACK", "__cooddy_security_risk"),
                                   TaintMacroCliDefinition("SENSI_INFO", "__cooddy_security_sde")},
                                  ""});
        return *parser;
    }

protected:
    TestErrorCheckConsumer myTestErrorCheckConsumer;

    void TearDown() override
    {
        if (myTestErrorCheckConsumer.HasToolError()) {
            FAIL() << "Caught tool errors";
        }
    }

    void TestAnalyze(HCXX::Workspace& workspace, HCXX::ProblemsHolder& holder,
                     HCXX::CompilerOptionsList* compilerOptionsList = nullptr, HCXX::Parser* parser = nullptr)
    {
        if (parser == nullptr) {
            parser = &GetParser();
        }
        auto analyzer = HCXX::Analyzer::Create(*parser, workspace);
        if (compilerOptionsList == nullptr) {
            analyzer->Analyze(workspace.GetCompilerOptions(), holder, myTestErrorCheckConsumer);
        } else {
            analyzer->Analyze(*compilerOptionsList, holder, myTestErrorCheckConsumer);
        }
    }

    bool FileHasString(const char* funcName, const std::filesystem::path& filePath)
    {
        std::ifstream resultStream(filePath.string());
        std::string resultFile =
            string((std::istreambuf_iterator<char>(resultStream)), std::istreambuf_iterator<char>());
        return resultFile.find(funcName) != std::string::npos;
    }
};

#endif  // COODDY_TESTBASECLASS_H
