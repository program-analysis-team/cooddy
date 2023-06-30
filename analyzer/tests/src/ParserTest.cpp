/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CrossTUContext.h>
#include <HastMatchingEngine.h>
#include <TranslationUnit.h>

#include <filesystem>
#include <fstream>
#include <memory>

#include "CheckerTest.h"
#include "TestBaseClass.h"

// The fixture for testing class ParserFixtureTest
class ParserFixtureTest : public TestBaseClass {};

void ParseDefinitions(HCXX::CrossTUContext& crossContext, const char* filePath)
{
    auto path = TEST_SUITE_PATH(filePath).string();
    HCXX::TranslationUnit tu(HCXX::CompilerOptions{{std::move(path)}});
    tu.SetCrossTUContext(crossContext);
    HCXX::Parser::VirtualFunctionsConsumer consumer;
    auto context = TestBaseClass::GetParser().CreateDefinitionsContext(consumer);
    TestBaseClass::GetParser().ParseDefinitions(tu, context.get());
}

TEST_F(ParserFixtureTest, DefinitionsParserTest)
{
    HCXX::CrossTUContext crossContext;
    ParseDefinitions(crossContext, "ParserTest/func_definitions.c");

    ASSERT_TRUE(crossContext.FindFunctionDef("first_func") != nullptr);
    ASSERT_TRUE(crossContext.FindFunctionDef("func_with_multiline_params_with_definition") != nullptr);
    ASSERT_TRUE(crossContext.FindFunctionDef("some_func") != nullptr);
    ASSERT_TRUE(crossContext.FindFunctionDef("tail_func") != nullptr);
    myTestErrorCheckConsumer.ClearToolError();
}

TEST_F(ParserFixtureTest, DefinitionsParserCppTest)
{
    HCXX::CrossTUContext crossContext;
    ParseDefinitions(crossContext, "ParserTest/func_cpp_definitions.cpp");

    ASSERT_TRUE(crossContext.FindFunctionDef("A::private_foo_a") != nullptr);
    ASSERT_TRUE(crossContext.FindFunctionDef("A::public_foo_a") != nullptr);
    ASSERT_TRUE(crossContext.FindFunctionDef("B::public_foo_b") != nullptr);
    ASSERT_TRUE(crossContext.FindFunctionDef("B::private_foo_b") != nullptr);
    ASSERT_TRUE(crossContext.FindFunctionDef("C::D::foo_d") != nullptr);
    myTestErrorCheckConsumer.ClearToolError();
}

struct ParserTestParameters {
    explicit ParserTestParameters(const std::filesystem::path& path) : m_jsonFile(path) {}
    ~ParserTestParameters() = default;

    const std::filesystem::path m_jsonFile;

    friend std::ostream& operator<<(std::ostream& os, const ParserTestParameters& rha)
    {
        os << rha.m_jsonFile.filename().generic_u8string();
        return os;
    }
};

class ParserTest : public ParserFixtureTest, public testing::WithParamInterface<ParserTestParameters> {
public:
    static std::vector<ParserTestParameters> EnumerateTestCases()
    {
        std::vector<ParserTestParameters> result;
        std::filesystem::path path(TEST_SUITE_PATH("ParserTest"));
        ::HCXX::Test::FileSystem::ForEachFileInDirectory(
            path, ".json", [&result](const std::filesystem::path& entry) { result.emplace_back(entry); });

        return result;
    }

protected:
    struct ParserTestCaseOptions {
        struct Match {
            uint32_t line = 0;
            uint32_t column = 0;

            template <class X>
            // TODO HCAT-513
            // COODDY_SUPPRESS CamelCaseChecker
            void serialize(jsoncpp::Stream<X>& stream)
            {
                jsoncpp::fields(*this, stream, "line", line, "column", column);
            }

            friend bool operator<(const Match& lha, const Match& rha)
            {
                if (lha.line < rha.line) {
                    return true;
                }

                if (lha.line > rha.line) {
                    return false;
                }

                return (lha.column > rha.column);
            }

            friend bool operator==(const Match& lha, const Match& rha)
            {
                return (lha.line == rha.line) && (lha.column == rha.column);
            }

            friend std::ostream& operator<<(std::ostream& os, const Match& rha)
            {
                os << "line=" << rha.line << ", column=" << rha.column;
                return os;
            }
        };

        //  Actually, test case ignores the description.
        //  We require description in the json file to make the test case goal clear for humans
        std::string description;
        //  HAST expression which should be matched
        std::string expression;
        std::vector<Match> matches;
        //  Name of cpp-file which is checked.
        //  The parameter is optional. By default, the same name as json-file is used.
        std::string cppFileName;
        bool allowToolError = false;

        template <class X>
        // TODO HCAT-513
        // COODDY_SUPPRESS CamelCaseChecker
        void serialize(jsoncpp::Stream<X>& stream)
        {
            jsoncpp::fields(*this, stream, "description", description, "expression", expression, "matches", matches,
                            "file_path", cppFileName, "allowed_tool_error", allowToolError);
        }
    };

    ParserTestCaseOptions GetTestCaseOption(const ParserTestParameters& parameters)
    {
        ParserTestCaseOptions testOptions;
        const std::string& jsonFileName = parameters.m_jsonFile.generic_u8string();
        std::ifstream fileStream(jsonFileName);
        jsoncpp::parse(testOptions, fileStream);
        return testOptions;
    }

    std::string GetCppFileName(const ParserTestParameters& parameters, const ParserTestCaseOptions& testOptions)
    {
        //  At the moment we ignore parameter testOptions
        //  May be later we add possibility to override filename in the json
        std::filesystem::path cppPath(parameters.m_jsonFile);
        if (testOptions.cppFileName.empty()) {
            cppPath.replace_extension(".cpp");
        } else {
            cppPath.replace_filename(testOptions.cppFileName);
        }
        return cppPath.generic_u8string();
    }
};

TEST_P(ParserTest, TestCase)
{
    ParserTestCaseOptions testOptions(GetTestCaseOption(GetParam()));

    const std::string cppFileName(GetCppFileName(GetParam(), testOptions));
    auto path = std::filesystem::path(cppFileName);
    ASSERT_TRUE(std::filesystem::exists(path));

    HCXX::CompilerOptions parseOptions;
    parseOptions.options.emplace_back(cppFileName);
    HCXX::TranslationUnit tu(parseOptions);
    HCXX::Parser* parser = &TestBaseClass::GetParser(&path);
    auto context = parser->CreateContext();
    parser->ParseAST(tu, context.get());

    ::HCXX::HastMatchingEngine engine;
    std::string errorMsg;
    auto expressionPtr = engine.BuildExpression(testOptions.expression, HCXX::SupportedLanguages(), &errorMsg);
    ASSERT_TRUE(expressionPtr.get());
    ASSERT_TRUE(errorMsg.empty());

    engine.MatchAll(tu);

    std::set<ParserTestCaseOptions::Match> expectedMatches;
    for (const auto& item : testOptions.matches) {
        bool inserted = expectedMatches.emplace(item).second;
        ASSERT_TRUE(inserted);  //    Matched nodes in the json file must be unique
    }

    std::set<ParserTestCaseOptions::Match> actualMatches;
    for (const auto& item : expressionPtr->GetMatchedResult()) {
        ASSERT_FALSE(item->IsSystem());  //   HAST doesn't check build in nodes
        auto location = tu.GetLocationInfo(item->GetRange());
        actualMatches.emplace(ParserTestCaseOptions::Match{location.line, location.column});
    }

    ASSERT_EQ(actualMatches.size(), expectedMatches.size());
    for (auto& match : actualMatches) {
        auto it = expectedMatches.find(match);
        if (it == expectedMatches.end()) {
            //  We know that test fails but we want to error clear to user
            //  Using simple 'ASSERT_NE(it, expectedMatches.end());' cause unclear error text if test fails.
            //  For example,
            //  'Expected: (it) != (expectedMatches.end()), actual: 8-byte object
            //      <D8-F4 F9-01 00-00 00-00> vs 8-byte object <D8-F4 F9-01 00-00 00-00>'
            ASSERT_FALSE(expectedMatches.empty());
            ASSERT_EQ(*expectedMatches.begin(), match);
        }
        ASSERT_NE(it, expectedMatches.end());
        expectedMatches.erase(it);
    }
    if (testOptions.allowToolError) {
        myTestErrorCheckConsumer.ClearToolError();
    }
}

TEST(ParserFailTest, IgnoreTUTest)
{
    HCXX::CompilerOptions parseOptions;
    parseOptions.options.emplace_back("main.S");
    HCXX::TranslationUnit tu(parseOptions);
    auto res = TestBaseClass::GetParser().ParseAST(tu);
    ASSERT_FALSE(res);
}

TEST(ParserTest, MacroDescriptions)
{
    auto path = TEST_SUITE_PATH("ParserTest/macro_descriptions.cpp").string();
    HCXX::CrossTUContext context;
    HCXX::TranslationUnit tu(HCXX::CompilerOptions{{std::move(path)}});
    tu.SetCrossTUContext(context);
    TestBaseClass::GetParser().ParseAST(tu);
    for (auto& child : tu.GetChildren()) {
        if (child->IsKindOf(HCXX::Node::Kind::FUNCTION_DECL)) {
            auto desc = tu.GetCodeDescriptions(child->GetRange());
            ASSERT_EQ(desc.size(), 13);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(ParserTestSuite, ParserTest, testing::ValuesIn(ParserTest::EnumerateTestCases()));
