/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Parser.h>

#include <filesystem>
#include <string_view>
#include <vector>

#include "ProjectPaths.h"
#include "TestBaseClass.h"
#include "dfa/TaintSettings.h"

using namespace HCXX;

struct SignatureGenTest : public TestBaseClass {};

void CheckDeclSignature(const std::vector<Node*>& nodes, std::string_view name, std::string_view expectedSignature,
                        unsigned int flags)
{
    auto declIt = std::find_if(nodes.begin(), nodes.end(), [&](auto* node) {
        auto* decl = Node::Cast<FunctionDecl>(node);

        if (decl != nullptr && decl->GetName() == name) {
            return true;
        }

        return false;
    });

    ASSERT_NE(declIt, nodes.end());

    auto actualSignature = Node::Cast<FunctionDecl>(*declIt)->GetSignature(flags);

    ASSERT_EQ(expectedSignature, actualSignature);
}

TEST_F(SignatureGenTest, SignatureGenTest)
{
    auto path = TEST_SUITE_PATH("SignatureGen") / "source.cpp";
    ASSERT_TRUE(std::filesystem::exists(path));

    CompilerOptions parseOptions;
    parseOptions.options.emplace_back(path.string());
    TranslationUnit tu(parseOptions);
    auto& parser = TestBaseClass::GetParser(&path);
    auto context = parser.CreateContext();
    parser.ParseAST(tu, context.get());

    auto& nodes = tu.GetNodes();

    auto flags = FunctionDecl::SFF_WITH_PARAM_NAMES | FunctionDecl::SFF_WITH_RETURN_TYPE;
    CheckDeclSignature(nodes, "f1", "void f1()", flags);
    CheckDeclSignature(nodes, "f2", "int f2(int x)", flags);
    CheckDeclSignature(nodes, "f3", "int f3(double)", flags);
    CheckDeclSignature(nodes, "f4", "int& f4(Struct<int*, double**>&& x)", flags);
    CheckDeclSignature(nodes, "f5", "int** f5(Class<Struct<int*&, double>> x)", flags);
    CheckDeclSignature(nodes, "f6", "void f6(const volatile int* const**&)", flags);
    CheckDeclSignature(nodes, "f7", "void f7(const volatile int)", flags);

    flags = FunctionDecl::SFF_WITH_RETURN_TYPE;
    CheckDeclSignature(nodes, "f1", "void f1()", flags);
    CheckDeclSignature(nodes, "f2", "int f2(int)", flags);
    CheckDeclSignature(nodes, "f3", "int f3(double)", flags);
    CheckDeclSignature(nodes, "f4", "int& f4(Struct<int*, double**>&&)", flags);
    CheckDeclSignature(nodes, "f5", "int** f5(Class<Struct<int*&, double>>)", flags);
    CheckDeclSignature(nodes, "f6", "void f6(const volatile int* const**&)", flags);
    CheckDeclSignature(nodes, "f7", "void f7(const volatile int)", flags);

    flags = FunctionDecl::SFF_WITH_PARAM_NAMES;
    CheckDeclSignature(nodes, "f1", "f1()", flags);
    CheckDeclSignature(nodes, "f2", "f2(int x)", flags);
    CheckDeclSignature(nodes, "f3", "f3(double)", flags);
    CheckDeclSignature(nodes, "f4", "f4(Struct<int*, double**>&& x)", flags);
    CheckDeclSignature(nodes, "f5", "f5(Class<Struct<int*&, double>> x)", flags);
    CheckDeclSignature(nodes, "f6", "f6(const volatile int* const**&)", flags);
    CheckDeclSignature(nodes, "f7", "f7(const volatile int)", flags);

    flags = FunctionDecl::SFF_NONE;
    CheckDeclSignature(nodes, "f1", "f1()", flags);
    CheckDeclSignature(nodes, "f2", "f2(int)", flags);
    CheckDeclSignature(nodes, "f3", "f3(double)", flags);
    CheckDeclSignature(nodes, "f4", "f4(Struct<int*, double**>&&)", flags);
    CheckDeclSignature(nodes, "f5", "f5(Class<Struct<int*&, double>>)", flags);
    CheckDeclSignature(nodes, "f6", "f6(const volatile int* const**&)", flags);
    CheckDeclSignature(nodes, "f7", "f7(const volatile int)", flags);

    flags = FunctionDecl::SFF_WITH_PARAM_NAMES | FunctionDecl::SFF_WITH_RETURN_TYPE;
    CheckDeclSignature(nodes, "f8", "void ns::f8()", flags);
    CheckDeclSignature(nodes, "f9", "void X::f9()", flags);
    CheckDeclSignature(nodes, "f10", "void X::f10()", flags);
    CheckDeclSignature(nodes, "f11", "void X::f11()", flags);
    CheckDeclSignature(nodes, "f12", "int f12()", flags);
    CheckDeclSignature(nodes, "f13", "Struct<int, double> f13()", flags);
}
