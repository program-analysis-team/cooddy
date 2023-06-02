/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <gtest/gtest.h>

#include "CheckerTest.h"
#include "Parser.h"
#include "TestBaseClass.h"

class SimpleAstParserTest : public TestBaseClass, public testing::WithParamInterface<std::string> {
public:
    static std::vector<std::string> EnumerateTestCases()
    {
        auto path(TEST_SUITE_PATH("SimpleChecker"));

        std::vector<std::string> result;
        ::HCXX::Test::FileSystem::ForEachFileInDirectory(path, ".cpp", [&result](const std::filesystem::path& entry) {
            if (entry.parent_path().string().find("proxy") == std::string::npos) {
                result.emplace_back(entry.generic_u8string());
            }
        });
        return result;
    }
};

TEST_P(SimpleAstParserTest, TestCase)
{
    HCXX::CompilerOptions parseOptions;
    const std::string& analyzingFileName = GetParam();

    parseOptions.options.emplace_back(analyzingFileName);

    TestBaseClass::GetParser().ParseAST(*std::make_unique<HCXX::TranslationUnit>(parseOptions));

    ASSERT_TRUE("The test is successful if the current line is reached. Earlier we cored dump in parseAST" != nullptr);
}

INSTANTIATE_TEST_SUITE_P(SimpleAstParserTestSuite, SimpleAstParserTest,
                         testing::ValuesIn(SimpleAstParserTest::EnumerateTestCases()));
