/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <MangledNameFinder.h>
#include <Parser.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "ProjectPaths.h"
#include "TestBaseClass.h"
#include "dfa/TaintSettings.h"

using namespace HCXX;

struct MangledNameFinderTest : public TestBaseClass {};

auto Mangle(std::string_view filename, std::string_view functionName)
{
    auto path = TEST_SUITE_PATH("MangledNameFinder") / filename;
    Workspace workspace(path.string());
    CompilerOptions opts;
    opts.options.emplace_back(path.string());
    ProblemsHolder holder(workspace);
    auto parser = HCXX::Parser::Create();
    auto analyzer = Analyzer::Create(*parser, workspace);
    MangledNameFinder mangler(functionName, *parser);
    analyzer->Analyze(workspace.GetCompilerOptions(), holder, mangler);
    return std::pair(mangler.GetResult(), mangler.FormatResult());
}

auto ContainsPair = [](auto& data, auto&& signature, auto&& mangledName) {
    return std::find(data.begin(), data.end(), std::pair<std::string, std::string>{signature, mangledName}) !=
           data.end();
};

TEST_F(MangledNameFinderTest, Basic)
{
    auto pairs = Mangle("source.cpp", "f").first;
    ASSERT_EQ(pairs.size(), 9);

    ASSERT_TRUE(ContainsPair(pairs, "void f()", "_Z1fv"));
    ASSERT_TRUE(ContainsPair(pairs, "int f(int)", "_Z1fi"));
    ASSERT_TRUE(ContainsPair(pairs, "int f(double)", "_Z1fd"));
    ASSERT_TRUE(ContainsPair(pairs, "int& f(Struct<int*, double**>&&)", "_Z1fO6StructIPiPPdE"));
    ASSERT_TRUE(ContainsPair(pairs, "int** f(Class<Struct<int*&, double>>)", "_Z1f5ClassI6StructIRPidEvE"));
    ASSERT_TRUE(ContainsPair(pairs, "void f(const volatile int* const**&)", "_Z1fRPPKPVKi"));

    ASSERT_TRUE(ContainsPair(pairs, "void ns::f()", "_ZN2ns1fEv"));
    ASSERT_TRUE(ContainsPair(pairs, "int ns::f(int)", "_ZN2ns1fEi"));
    ASSERT_TRUE(ContainsPair(pairs, "int ns::f(double)", "_ZN2ns1fEd"));

    ASSERT_FALSE(ContainsPair(pairs, "void xf()", "_Z2xfv"));
    ASSERT_FALSE(ContainsPair(pairs, "void fx()", "_Z2xfv"));
}

TEST_F(MangledNameFinderTest, Qualified)
{
    auto pairs = Mangle("source.cpp", "ns::f").first;
    ASSERT_EQ(pairs.size(), 3);

    ASSERT_FALSE(ContainsPair(pairs, "void f()", "_Z1fv"));
    ASSERT_FALSE(ContainsPair(pairs, "int f(int)", "_Z1fi"));
    ASSERT_FALSE(ContainsPair(pairs, "int f(double)", "_Z1fd"));
    ASSERT_FALSE(ContainsPair(pairs, "int& f(Struct<int*, double**>&&)", "_Z1fO6StructIPiPPdE"));
    ASSERT_FALSE(ContainsPair(pairs, "int** f(Class<Struct<int*&, double>>)", "_Z1f5ClassI6StructIRPidEvE"));
    ASSERT_FALSE(ContainsPair(pairs, "void f(const volatile int* const**&)", "_Z1fRPPKPVKi"));

    ASSERT_TRUE(ContainsPair(pairs, "void ns::f()", "_ZN2ns1fEv"));
    ASSERT_TRUE(ContainsPair(pairs, "int ns::f(int)", "_ZN2ns1fEi"));
    ASSERT_TRUE(ContainsPair(pairs, "int ns::f(double)", "_ZN2ns1fEd"));

    ASSERT_FALSE(ContainsPair(pairs, "void xf()", "_Z2xfv"));
    ASSERT_FALSE(ContainsPair(pairs, "void fx()", "_Z2xfv"));
}

TEST_F(MangledNameFinderTest, FullyQualified)
{
    auto pairs = Mangle("source.cpp", "::f").first;
    ASSERT_EQ(pairs.size(), 6);

    ASSERT_TRUE(ContainsPair(pairs, "void f()", "_Z1fv"));
    ASSERT_TRUE(ContainsPair(pairs, "int f(int)", "_Z1fi"));
    ASSERT_TRUE(ContainsPair(pairs, "int f(double)", "_Z1fd"));
    ASSERT_TRUE(ContainsPair(pairs, "int& f(Struct<int*, double**>&&)", "_Z1fO6StructIPiPPdE"));
    ASSERT_TRUE(ContainsPair(pairs, "int** f(Class<Struct<int*&, double>>)", "_Z1f5ClassI6StructIRPidEvE"));
    ASSERT_TRUE(ContainsPair(pairs, "void f(const volatile int* const**&)", "_Z1fRPPKPVKi"));

    ASSERT_FALSE(ContainsPair(pairs, "void ns::f()", "_ZN2ns1fEv"));
    ASSERT_FALSE(ContainsPair(pairs, "int ns::f(int)", "_ZN2ns1fEi"));
    ASSERT_FALSE(ContainsPair(pairs, "int ns::f(double)", "_ZN2ns1fEd"));

    ASSERT_FALSE(ContainsPair(pairs, "void xf()", "_Z2xfv"));
    ASSERT_FALSE(ContainsPair(pairs, "void fx()", "_Z2xfv"));
}

TEST_F(MangledNameFinderTest, FormattedOutput)
{
    std::string str = Mangle("source.cpp", "::f").second;
    auto lines = StrUtils::Split(str, '\n');
    ASSERT_EQ(lines.size(), 1 + 1 + 6);  // message + table header + 6 hits

    ASSERT_NE(lines[1].find("SIGNATURE"), lines[0].npos);
    ASSERT_NE(lines[1].find("MANGLED NAME"), lines[0].npos);

    for (int i = 1; i < lines.size(); ++i) {
        auto& line = lines[i];
        ASSERT_NE(line.find("  "), line.npos);  // Columns separated by 2 spaces
    }
}

TEST_F(MangledNameFinderTest, FormattedOutputEmpty)
{
    std::string str = Mangle("source.cpp", "something_else").second;
    ASSERT_EQ(str, "Found no signatures matching \"something_else\".\n");
}
