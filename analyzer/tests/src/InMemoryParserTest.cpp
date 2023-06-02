/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <gtest/gtest.h>

#include "Analyzer.h"
#include "CompilerOptionsList.h"
#include "ProblemsList.h"
#include "ProjectPaths.h"
#include "TestErrorCheckConsumer.h"
#include "utils/EnvironmentUtils.h"

class InMemoryParserTest : public TestErrorCheckConsumer {
public:
    InMemoryParserTest()
    {
        parser = HCXX::Parser::Create();
        header = HCXX::EnvironmentUtils::NormalizePath(std::filesystem::current_path().append("header.h").string());
        cppfile =
            HCXX::EnvironmentUtils::NormalizePath(std::filesystem::current_path().append("in_memory.cpp").string());
        renamedName = "renamed_source.cpp";
        parseFlags = 0;
        workspace = std::make_unique<HCXX::Workspace>(std::string(cppfile), std::unordered_set<std::string>());
        workspace->SetEnabledCheckers({"NullPtrDereferenceChecker"});
        analyzer = HCXX::Analyzer::Create(*parser, *workspace);
    }
    ~InMemoryParserTest() = default;

    void RunTest(uint32_t parseFlags)
    {
        const char* headerContent = "#include <vector>\r\n";
        const char* cppContent =
            "#include <string>\r\n"
            "#include \"header.h\"\r\n"
            "int* getArray() { return 0; };"
            "int main() {\r\n"
            "    auto ptr = getArray();\r\n"
            "    if (*ptr == 1) return 1;\r\n"
            "    return 0;\r\n"
            "}";

        parser->AddInMemoryFile(cppfile, cppContent);
        parser->AddInMemoryFile(header, headerContent);

        this->parseFlags = parseFlags;
        includePath.clear();
        inclusionSource.clear();
        prolog.clear();

        HCXX::ProblemsList problemsList(*workspace);
        analyzer->Analyze(workspace->GetCompilerOptions(), problemsList, *this);

        auto& problems = problemsList.GetProblems();
        if (parseFlags & HCXX::Parser::INCLUDES_ONLY) {
            ASSERT_EQ(problems.size(), 0);
        } else {
            ASSERT_EQ(problems.size(), 1);
            ASSERT_EQ(problems.begin()->line, 5);
            ASSERT_EQ(problems.begin()->filename, renamedName);
        }

        if (parseFlags & HCXX::Parser::CREATE_PREAMBLE) {
            ASSERT_EQ(includePath, header);
            ASSERT_EQ(inclusionSource, cppfile);
            ASSERT_EQ(prolog, "#include <string>\r\n");
        }
    }

    void OnParseEnd(HCXX::TranslationUnit& unit) override
    {
        unit.SetMainFileName(renamedName);
    }

    uint32_t GetParseFlags() override
    {
        return parseFlags;
    };

    void OnIncludeParsed(std::string includePath, std::string inclusionSource, std::string prolog) override
    {
        this->includePath = includePath;
        this->inclusionSource = inclusionSource;
        this->prolog = prolog;
    }

public:
    std::unique_ptr<HCXX::Workspace> workspace;
    std::unique_ptr<HCXX::Parser> parser;
    std::unique_ptr<HCXX::Analyzer> analyzer;

    std::string cppfile;
    std::string header;
    uint32_t parseFlags;

    std::string includePath;
    std::string inclusionSource;
    std::string prolog;
    std::string renamedName;
};

TEST(InMemoryParserTest, TestCase)
{
    InMemoryParserTest test;
    test.RunTest(HCXX::Parser::IN_MEMORY_FILES | HCXX::Parser::CREATE_PREAMBLE | HCXX::Parser::INCLUDES_ONLY);
}

TEST(InMemoryParserTest, TestCase2)
{
    InMemoryParserTest test;
    test.RunTest(HCXX::Parser::IN_MEMORY_FILES | HCXX::Parser::CREATE_PREAMBLE);
    ASSERT_FALSE(test.HasToolError());
}

TEST(InMemoryParserTest, TestCase3)
{
    InMemoryParserTest test;
    test.RunTest(HCXX::Parser::IN_MEMORY_FILES);
    ASSERT_FALSE(test.HasToolError());
}
