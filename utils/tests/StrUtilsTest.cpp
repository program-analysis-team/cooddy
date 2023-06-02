/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <cstddef>
#include <string_view>
#include <vector>

#include "TestBaseClass.h"

using namespace HCXX::StrUtils;
using namespace HCXX::StrLocales;

// The fixture for testing class StrUtilsTests
class StrUtilsTest : public TestBaseClass {};

TEST_F(StrUtilsTest, BaseA)
{
    const char* str1 = "SomeCamelCase";
    auto str2 = CamelCaseToBigSnakeCase(str1);
    ASSERT_EQ(str2, "SOME_CAMEL_CASE");
    auto str3 = BigSnakeCaseToCamelCase(str2);
    ASSERT_EQ(str1, str3);
}

TEST_F(StrUtilsTest, Abbreviation)
{
    const char* str1 = "ItIsTU";
    auto str2 = CamelCaseToBigSnakeCase(str1);
    ASSERT_EQ(str2, "IT_IS_TU");
    auto str3 = BigSnakeCaseToCamelCase(str2);
    ASSERT_EQ(str3, "ItIsTu");  //  It isn't the same as initial string by design
}

TEST_F(StrUtilsTest, AandZ)
{
    const char* str1 = "AaaaaAaaaZzzzzZzzz";
    auto str2 = CamelCaseToBigSnakeCase(str1);
    ASSERT_EQ(str2, "AAAAA_AAAA_ZZZZZ_ZZZZ");
    auto str3 = BigSnakeCaseToCamelCase(str2);
    ASSERT_EQ(str1, str3);
}

TEST_F(StrUtilsTest, WithNumber)
{
    const char* str1 = "Bbbb12Caz13";
    auto str2 = CamelCaseToBigSnakeCase(str1);
    ASSERT_EQ(str2, "BBBB12_CAZ13");
    auto str3 = BigSnakeCaseToCamelCase(str2);
    ASSERT_EQ(str1, str3);
}

TEST_F(StrUtilsTest, HardcodedStrings)
{
    auto str1 = GetStringLocale("STATIC_ANALYZER_OF_CPP_SOURCE_CODE");
    ASSERT_EQ(str1, "Static analyzer of C++ source code");
    auto str2 = GetStringLocale("ERROR_SIGNAL", {"2", "INT"});
    ASSERT_EQ(str2, "Error: signal 2 (INT)");
}

TEST_F(StrUtilsTest, LocaleTest)
{
    ASSERT_TRUE(LoadLocale("CN"));
    HCXX::Log(HCXX::LogLevel::WARNING) << "TEST_STRING : '" << GetStringLocale("TEST_STRING") << "' = '"
                                       << "中文文本"
                                       << "'\n";
    ASSERT_EQ(GetStringLocale("TEST_STRING"), "中文文本");
    ASSERT_EQ(GetStringLocale("NULL_POINTER"), "Null pointer");
    ASSERT_TRUE(LoadLocale("EN"));
}

TEST_F(StrUtilsTest, JoinBasic)
{
    using namespace std;
    string actual = Join({"1", "2", "3"}, ", ");
    string expected = "1, 2, 3";

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitBasic)
{
    using namespace std;
    vector<string_view> actual = Split("1,2,3", ',');
    vector<string_view> expected = {"1", "2", "3"};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitSkip1)
{
    using namespace std;
    vector<string_view> actual = Split("1,2,,3", ',');
    vector<string_view> expected = {"1", "2", "3"};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitSkip2)
{
    using namespace std;
    vector<string_view> actual = Split("1,2,3,", ',');
    vector<string_view> expected = {"1", "2", "3"};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitSkip3)
{
    using namespace std;
    vector<string_view> actual = Split(",1,2,3", ',');
    vector<string_view> expected = {"1", "2", "3"};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitSkip4)
{
    using namespace std;
    vector<string_view> actual = Split(",,,,", ',');
    vector<string_view> expected = {};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitNoSkip1)
{
    using namespace std;
    vector<string_view> actual = Split("1,2,,3", ',', false);
    vector<string_view> expected = {"1", "2", "", "3"};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitNoSkip2)
{
    using namespace std;
    vector<string_view> actual = Split("1,2,3,", ',', false);
    vector<string_view> expected = {"1", "2", "3", ""};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitNoSkip3)
{
    using namespace std;
    vector<string_view> actual = Split(",,,,", ',', false);
    vector<string_view> expected = {"", "", "", "", ""};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitEmpty)
{
    using namespace std;
    vector<string_view> actual = Split("", ' ');
    vector<string_view> expected = {};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, SplitEmptyNoSkip)
{
    using namespace std;
    vector<string_view> actual = Split("", ' ', false);
    vector<string_view> expected = {""};

    ASSERT_EQ(expected, actual);
}

TEST_F(StrUtilsTest, ToIntegerBasic)
{
    ASSERT_EQ(ToInteger<int>("0"), 0);
    ASSERT_EQ(ToInteger<int>("1"), 1);
    ASSERT_EQ(ToInteger<int>("-1"), -1);
    ASSERT_EQ(ToInteger<int>("123"), 123);
    ASSERT_EQ(ToInteger<int>("2147483647"), 2147483647);
    ASSERT_EQ(ToInteger<int>("-2147483648"), -2147483648);
}

TEST_F(StrUtilsTest, ToIntegerError)
{
    ASSERT_THROW(ToInteger<int>("hello"), std::invalid_argument);
    ASSERT_THROW(ToInteger<int>("69 lol"), std::invalid_argument);
    ASSERT_THROW(ToInteger<int>("+1"), std::invalid_argument);  // Plus sign not recognised
    ASSERT_THROW(ToInteger<int>(" 1"), std::invalid_argument);  // Spaces in front not skipped

    ASSERT_THROW(ToInteger<int>("9999999999"), std::out_of_range);
    ASSERT_THROW(ToInteger<int>("21474836470"), std::out_of_range);
}

TEST_F(StrUtilsTest, TrimWhitespace)
{
    ASSERT_EQ(TrimWhitespace("hello"), "hello");
    ASSERT_EQ(TrimWhitespace("hello "), "hello");
    ASSERT_EQ(TrimWhitespace(" hello"), "hello");
    ASSERT_EQ(TrimWhitespace("  hello"), "hello");
    ASSERT_EQ(TrimWhitespace(" hello "), "hello");
    ASSERT_EQ(TrimWhitespace("\thello"), "hello");
    ASSERT_EQ(TrimWhitespace(" \thello"), "hello");
    ASSERT_EQ(TrimWhitespace(""), "");
    ASSERT_EQ(TrimWhitespace(" "), "");
    ASSERT_EQ(TrimWhitespace("  "), "");
}

TEST_F(StrUtilsTest, StringId)
{
    std::size_t id1 = GetStringId("StrUtilsTest.StringId");
    std::size_t id2 = GetStringId("StrUtilsTest.StringId");

    ASSERT_EQ(id1, id2);

    const std::string& str = GetStringById(id1);

    ASSERT_EQ(str, "StrUtilsTest.StringId");
}

TEST_F(StrUtilsTest, Replace)
{
    ASSERT_EQ(Replace("abcdef", "bc", "xy"), "axydef");
    ASSERT_EQ(Replace("abcdef", "", "?"), "abcdef");
    ASSERT_EQ(Replace("1,2,3,4,5,6,7,8,9,10,11,12", ",", ";"), "1;2;3;4;5;6;7;8;9;10;11;12");
    ASSERT_EQ(Replace("remove spaces from string", " ", ""), "removespacesfromstring");
    ASSERT_EQ(Replace("string", "string", "stringstring"), "stringstring");
    ASSERT_EQ(Replace("string", "string", ""), "");
    ASSERT_EQ(Replace("", "a", "b"), "");
}

TEST_F(StrUtilsTest, ReplacePlaceholders)
{
    ASSERT_EQ(ReplacePlaceholders("basic %1%", {"example"}), "basic example");
    ASSERT_EQ(ReplacePlaceholders("%1% %2%", {"basic", "example"}), "basic example");
    ASSERT_EQ(ReplacePlaceholders("%1%", {"test"}), "test");
    ASSERT_EQ(ReplacePlaceholders("%1% %1%", {"test"}), "test test");
    ASSERT_EQ(ReplacePlaceholders("%1%1%1%", {"test"}), "test1test");
    ASSERT_EQ(ReplacePlaceholders("%0%", {"test"}), "");
    ASSERT_EQ(ReplacePlaceholders("%3%, %2%, %4%, %1%, %5%", {"1", "2", "3", "4"}), "3, 2, 4, 1, ");
}

TEST_F(StrUtilsTest, FindMatchingBrace)
{
    ASSERT_EQ(FindMatchingBrace("0{ 1{ 2{ 3{ 3} 2} 1} 0}", 0, '{', '}'), 22);
    ASSERT_EQ(FindMatchingBrace("0{ 1{ 2{ 3{ 3} 2} 1} 0}", 4, '{', '}'), 19);

    ASSERT_EQ(FindMatchingBrace("0{ 1{ 2{ 3{ 3} 2} 1}", 0, '{', '}'), std::string_view::npos);
}

TEST_F(StrUtilsTest, MatchQualifiedNameBasic)
{
    ASSERT_TRUE(MatchQualifiedName("foo", "foo"));
    ASSERT_TRUE(MatchQualifiedName("foo<int>", "foo<int>"));
    ASSERT_TRUE(MatchQualifiedName("scope::foo", "scope::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope::foo", "scope2::scope::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<int>::foo", "scope2::scope<int>::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<vector<int>>::foo", "scope2::scope<vector<int>>::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<std::vector<int>>::foo", "scope2::scope<std::vector<int>>::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<std::vector<int>, double>::foo",
                                   "scope2::scope<std::vector<int>, double>::foo"));

    ASSERT_FALSE(MatchQualifiedName("xfoo", "foo"));
    ASSERT_FALSE(MatchQualifiedName("foox", "foo"));
    ASSERT_FALSE(MatchQualifiedName("scope::xfoo", "scope::foo"));
    ASSERT_FALSE(MatchQualifiedName("scope::foox", "scope::foo"));
    ASSERT_FALSE(MatchQualifiedName("scope::foo", "scope"));
}

TEST_F(StrUtilsTest, MatchQualifiedNameOperator)
{
    ASSERT_TRUE(MatchQualifiedName("operator<", "operator<"));
    ASSERT_TRUE(MatchQualifiedName("scope::operator<", "scope::operator<"));
    ASSERT_TRUE(MatchQualifiedName("scope::operator<<", "scope::operator<<"));
    ASSERT_TRUE(MatchQualifiedName("scope::operator<<int>", "scope::operator<<int>"));
    ASSERT_TRUE(MatchQualifiedName("scope::operator<<<int>", "scope::operator<<<int>"));
    ASSERT_TRUE(MatchQualifiedName("scope::operator<<<std::vector<int>>", "scope::operator<<<std::vector<int>>"));
    ASSERT_TRUE(MatchQualifiedName("scope::operator<<<std::vector<int>, double>",
                                   "scope::operator<<<std::vector<int>, double>"));
}

TEST_F(StrUtilsTest, MatchQualifiedNameMissingTemplate)
{
    ASSERT_TRUE(MatchQualifiedName("foo<int>", "foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<int>::foo", "scope2::scope::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<int>::foo<int>", "scope2::scope::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<int>::foo<int>", "scope2::scope<int>::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<int>::foo<int>", "scope2::scope::foo<int>"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<vector<int>>::foo", "scope2::scope::foo"));
    ASSERT_TRUE(MatchQualifiedName("scope2::scope<std::vector<int>>::foo", "scope2::scope::foo"));
}

TEST_F(StrUtilsTest, MatchQualifiedNamePartial)
{
    ASSERT_TRUE(MatchPartiallyQualifiedName("foo", "foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("foo<int>", "foo<int>"));

    ASSERT_TRUE(MatchPartiallyQualifiedName("scope::foo", "scope::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("scope::foo", "foo"));

    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope::foo", "scope2::scope::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope::foo", "scope::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope::foo", "foo"));

    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope<int>::foo", "scope2::scope<int>::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope<int>::foo", "scope<int>::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope<int>::foo", "foo"));

    ASSERT_TRUE(
        MatchPartiallyQualifiedName("scope2::scope<std::vector<int>>::foo", "scope2::scope<std::vector<int>>::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope<std::vector<int>>::foo", "scope<std::vector<int>>::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("scope2::scope<std::vector<int>>::foo", "foo"));

    ASSERT_FALSE(MatchPartiallyQualifiedName("xfoo", "foo"));
    ASSERT_FALSE(MatchPartiallyQualifiedName("foox", "foo"));
    ASSERT_FALSE(MatchPartiallyQualifiedName("scope::xfoo", "scope::foo"));
    ASSERT_FALSE(MatchPartiallyQualifiedName("scope::foox", "scope::foo"));

    ASSERT_TRUE(MatchPartiallyQualifiedName("x::y::x::foo", "x::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("x::foo::x::foo", "x::foo"));
    ASSERT_TRUE(MatchPartiallyQualifiedName("x::foo::x::foo<int>", "x::foo<int>"));
    ASSERT_FALSE(MatchPartiallyQualifiedName("x::y::x::foo", "x"));
    ASSERT_FALSE(MatchPartiallyQualifiedName("x::y::foo", "x::foo"));
}
