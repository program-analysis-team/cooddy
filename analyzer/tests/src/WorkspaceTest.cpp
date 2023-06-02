/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <CompilerOptionsList.h>
#include <Parser.h>
#include <ProblemsList.h>
#include <dfa/TaintSettings.h>
#include <utils/TempFile.h>

#include <deps/HTTPRequest.hpp>
#include <fstream>

#include "CheckerTest.h"
#include "TestBaseClass.h"

using namespace HCXX;

// The fixture for testing class WorkspaceTest
class WorkspaceTest : public TestBaseClass {
public:
    WorkspaceTest()
    {
        commonWorkspaceTestPath = std::filesystem::canonical(TEST_SUITE_PATH("WorkspaceTest"));
    }

protected:
    std::filesystem::path commonWorkspaceTestPath;
};

TEST_F(WorkspaceTest, AnnotationTest)
{
    // Copy pregenerated .annotations to rootPath
    std::filesystem::path curTestDir = commonWorkspaceTestPath / "workspace";
    auto workspacePath = curTestDir / ".cooddy";
    if (std::filesystem::exists(workspacePath)) {
        std::filesystem::remove_all(workspacePath);
    }
    TempDir dir(workspacePath);

    std::filesystem::create_directory(workspacePath);

    std::filesystem::copy(curTestDir / ".annotations.json", workspacePath.append(".annotations.json"),
                          std::filesystem::copy_options::overwrite_existing);

    WorkspaceOptions workspaceOptions;
    workspaceOptions.genAnnoSettings = GenAnnoSettings::FromCliArg("all");
    workspaceOptions.genAnnoPath = "./x/.generated_annotations_x.json";
    Workspace workspace((curTestDir / "annotations.cpp").string(), {}, workspaceOptions);

    auto parser = HCXX::Parser::Create();
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList, nullptr, parser.get());

    FunctionAnnotation annotation;
    auto filePath = curTestDir / ".cooddy/x/.generated_annotations_x.json";
    ASSERT_TRUE(FileHasString("_Z20test_gen_annotationsv", filePath));

    ASSERT_FALSE(workspace.GetAnnotation("unknown_function", annotation, UnitLanguage::C));

    ASSERT_TRUE(workspace.GetAnnotation("java.util.Scanner.nextLine", annotation, UnitLanguage::JAVA));
    ASSERT_EQ(annotation.size(), 1);
    ASSERT_EQ(annotation[0][0].GetKind(), Annotation::GetKind("UntrustedSource"));

    ASSERT_TRUE(workspace.GetAnnotation("_Test_annotations", annotation, UnitLanguage::C));
    ASSERT_EQ(annotation.size(), 7);
    ASSERT_EQ(annotation[2][0].GetKind(), Annotation::GetKind("Write"));

    auto subOffsets = annotation[2][0].GetMemoryOffset();
    subOffsets.ExtractSubOffset();

    auto subOffset = subOffsets.ExtractSubOffset();
    ASSERT_EQ(subOffset.first, VirtualOffset::Kind::INDEX);
    ASSERT_EQ(subOffset.second, 123);

    subOffset = subOffsets.ExtractSubOffset();
    ASSERT_EQ(subOffset.first, VirtualOffset::Kind::DEREF);
    ASSERT_EQ(annotation[2][0].GetMemoryOffset().ToString(), "[1][123]*");
    ASSERT_TRUE(annotation[2][0].GetMemoryOffset() == VirtualOffset("[1]&[123]*"));
    ASSERT_EQ(annotation[3][0].GetMemoryOffset().ToString(), "o");
    ASSERT_TRUE(annotation[3][0].GetMemoryOffset() == VirtualOffset("o"));
    ASSERT_TRUE(annotation[4][0].GetMemoryOffset().ToString() == "&");
}

TEST_F(WorkspaceTest, RelativeProfileTest)
{
    std::filesystem::path curTestPath(commonWorkspaceTestPath / "relative_profile" / "src");
    FunctionAnnotation annotation;
    Workspace workspace(".", {}, {}, "../profile/relative_profile.profile.json", curTestPath);
    ASSERT_TRUE(workspace.GetAnnotation("MyAlloc", annotation, UnitLanguage::C));
    ASSERT_TRUE(workspace.GetAnnotation("MyFree", annotation, UnitLanguage::C));
}

TEST_F(WorkspaceTest, CompileCommandsInputFileTest)
{
    Workspace workspace((commonWorkspaceTestPath / "workspace/compile_commands.json").string());
    ASSERT_TRUE(!workspace.GetCompilerOptions().Get().empty());
}

TEST_F(WorkspaceTest, StripCompileOptionsTest)
{
    Workspace workspace((commonWorkspaceTestPath / "workspace/compile_commands.json").string());
    auto optionsList = workspace.GetCompilerOptions().Get();
    ASSERT_TRUE(!optionsList.empty());
    auto& options = optionsList[0];
    TestBaseClass::GetParser().ConvertOptions(options);
    ASSERT_FALSE(options.HasOption("--param"));
    ASSERT_FALSE(options.HasOption("param_option=42"));
    ASSERT_FALSE(options.HasOption("-o"));
    ASSERT_FALSE(options.HasOption("objectfile.o"));
    ASSERT_FALSE(options.HasOption("-fPIC"));
    ASSERT_FALSE(options.HasOption("-c"));
    ASSERT_FALSE(options.HasOption("foo.cpp"));
    ASSERT_TRUE(options.HasOption("-std=gnu++17"));
}

TEST_F(WorkspaceTest, StripCompileOptionsTest_2)
{
    Workspace workspace((commonWorkspaceTestPath / "workspace_2/compile_commands.json").string());
    auto optionsList = workspace.GetCompilerOptions().Get();
    ASSERT_TRUE(!optionsList.empty());
    auto& options = optionsList[0];
    TestBaseClass::GetParser().ConvertOptions(options);
    ASSERT_FALSE(options.HasOption("--param"));
    ASSERT_FALSE(options.HasOption("param_option=42"));
    ASSERT_FALSE(options.HasOption("-o"));
    ASSERT_FALSE(options.HasOption("objectfile.o"));
    ASSERT_FALSE(options.HasOption("-fPIC"));
    ASSERT_FALSE(options.HasOption("-c"));
    ASSERT_FALSE(options.HasOption("-msse2"));
    ASSERT_FALSE(options.HasOption("-s"));
    ASSERT_FALSE(options.HasOption("foo.cpp"));
    ASSERT_TRUE(options.HasOption("-isysroot"));
    ASSERT_TRUE(options.HasOption("/opt/RTOS/207.7.0/arm64le_4.4_ek_preempt_pro/sdk"));
    options = optionsList[1];
    TestBaseClass::GetParser().ConvertOptions(options);
    ASSERT_TRUE(options.HasOption("-isysroot"));
    ASSERT_TRUE(options.HasOption("/opt/RTOS/207.7.0/arm64le_4.4_ek_preempt_pro/sdk"));
}

TEST_F(WorkspaceTest, StripCompileOptionsTest_3)
{
    Workspace workspace((commonWorkspaceTestPath / "workspace_2/compile_commands.json").string());
    auto optionsList = workspace.GetCompilerOptions().Get();
    ASSERT_TRUE(!optionsList.empty());
    auto& options = optionsList[2];
    TestBaseClass::GetParser().ConvertOptions(options);
    ASSERT_TRUE(options.HasOption("-DTEST"));
}

TEST_F(WorkspaceTest, IncludeFileOptionTest)
{
    Workspace workspace((commonWorkspaceTestPath / "workspace_3/compile_commands.json").string());
    auto optionsList = workspace.GetCompilerOptions().Get();
    ASSERT_TRUE(!optionsList.empty());
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);

    bool foundNPD = false;
    bool foundZero = false;
    bool foundMismatch = false;
    for (auto& problem : problemsList.GetProblems()) {
        if (problem.checkerName == "NullPtrDereferenceChecker") {
            foundNPD = true;
        } else if (problem.checkerName == "DivisionByZeroChecker") {
            foundZero = true;
        } else if (problem.checkerName == "MismatchedMemManagementChecker") {
            foundMismatch = true;
        }
    }

    ASSERT_TRUE(foundZero);
    ASSERT_TRUE(foundNPD);
    ASSERT_TRUE(foundMismatch);
}

TEST_F(WorkspaceTest, CDBAnnotationsTest)
{
    // Copy pregenerated .annotations to rootPath
    auto workspacePath = commonWorkspaceTestPath / "workspace_5/.cooddy";
    TempDir dir(workspacePath);
    if (std::filesystem::exists(workspacePath)) {
        std::filesystem::remove_all(workspacePath);
    }
    std::filesystem::create_directory(workspacePath);

    std::filesystem::copy(commonWorkspaceTestPath / "workspace/default.profile.json",
                          workspacePath.append("default.profile.json"),
                          std::filesystem::copy_options::overwrite_existing);

    WorkspaceOptions workspaceOptions;
    workspaceOptions.genAnnoSettings = GenAnnoSettings::FromCliArg("all");
    Workspace workspace((commonWorkspaceTestPath / "workspace_5").string(), {}, workspaceOptions);

    auto parser = HCXX::Parser::Create();

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList, nullptr, parser.get());

    FunctionAnnotation annotation;
    auto filePath = commonWorkspaceTestPath / "workspace_5/.cooddy/.generated_annotations.json";
    ASSERT_TRUE(FileHasString("_Z4testv", filePath));
    ASSERT_FALSE(workspace.GetAnnotation("unknown_function", annotation, UnitLanguage::C));
}

TEST_F(WorkspaceTest, SeveralAnnotationsTest)
{
    // Copy pregenerated .annotations to rootPath
    auto workspacePath = commonWorkspaceTestPath / "workspace" / ".cooddy";
    if (std::filesystem::exists(workspacePath)) {
        std::filesystem::remove_all(workspacePath);
    }
    TempDir dir(workspacePath);
    std::filesystem::create_directory(workspacePath);
    auto curTestPath = commonWorkspaceTestPath / "workspace";

    std::filesystem::copy(curTestPath / ".annotations.json", workspacePath / ".annotations.json",
                          std::filesystem::copy_options::overwrite_existing);
    std::filesystem::copy(curTestPath / ".annotations-test.json", workspacePath / ".annotations-test.json",
                          std::filesystem::copy_options::overwrite_existing);
    std::filesystem::copy(curTestPath / "default.profile.json", workspacePath / "default.profile.json",
                          std::filesystem::copy_options::overwrite_existing);

    WorkspaceOptions workspaceOptions;
    workspaceOptions.genAnnoSettings = GenAnnoSettings::FromCliArg("all");
    Workspace workspace((curTestPath / "annotations.cpp").string(), {}, workspaceOptions, "default");

    auto parser = HCXX::Parser::Create();
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList, nullptr, parser.get());

    FunctionAnnotation annotation;
    auto filePath = workspacePath / ".generated_annotations.json";
    ASSERT_TRUE(FileHasString("_Z20test_gen_annotationsv", filePath));

    ASSERT_FALSE(workspace.GetAnnotation("unknown_function", annotation, UnitLanguage::C));

    ASSERT_TRUE(workspace.GetAnnotation("_Test_annotations", annotation, UnitLanguage::C));
    ASSERT_EQ(annotation.size(), 8);
    ASSERT_EQ(annotation[2][0].GetKind(), Annotation::GetKind("Write"));
    ASSERT_EQ(annotation.size(), 8);

    auto& annotations = workspace.GetProfile().GetSettings().annotations;
    ASSERT_TRUE(annotations.find(".annotations-test.json") != annotations.end());
}

// COODDY_SUPPRESS LargeStackAllocationChecker
TEST_F(WorkspaceTest, StoreAnalysisDataTest)
{
    auto workspacePath = commonWorkspaceTestPath / "workspace_4";
    auto fooCopyPath = workspacePath / "foo_copy.cpp";
    auto booCopyPath = workspacePath / "boo_copy.cpp";
    TempDir dir(workspacePath / ".cooddy");
    TempFile fooCopyTmp(workspacePath / "foo_copy.cpp");
    TempFile booCopyTmp(workspacePath / "boo_copy.cpp");

    std::filesystem::remove_all(workspacePath / ".cooddy/storage");
    std::filesystem::remove(fooCopyPath);
    std::filesystem::remove(booCopyPath);
    std::filesystem::copy_file(workspacePath / "foo.cpp", fooCopyPath);
    std::filesystem::copy_file(workspacePath / "boo.cpp", booCopyPath);

    auto astParser = HCXX::Parser::Create();
    WorkspaceOptions workspaceOptions;
    workspaceOptions.incrementalSettings.FromCliArg("bu");
    workspaceOptions.problemSeverityLevel = HCXX::Problem::Severity::NOTIFY;
    Workspace workspace(workspacePath.string(), {"NullPtrDereferenceChecker", "UnusedMemberExpressionChecker"},
                        workspaceOptions);
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList, nullptr, astParser.get());
    // should report one problem about unused fieldOne field
    ASSERT_EQ(problemsList.GetProblems().size(), 1);
    ASSERT_EQ(problemsList.GetProblems()[0].sourceId, "header.h:MyStruct:fieldOne");

    // add null deref problem to foo_copy.cpp and remove problem about fieldOne
    std::ofstream fooStream(fooCopyPath.string(), std::ios_base::app);
    ASSERT_TRUE(fooStream.good());
    fooStream << "int foo(){ MyStruct a; int b = a.fieldOne; int* p = 0; return *p; }\n";
    fooStream.close();
    FileEntriesCache::GetInstance().RemoveEntry(fooCopyPath.string());

    // run analysis on modified source
    HCXX::ProblemsList problemsList2(workspace);
    TestAnalyze(workspace, problemsList2, nullptr, astParser.get());
    ASSERT_EQ(problemsList2.GetProblems().size(), 1);
    ASSERT_EQ(problemsList2.GetProblems()[0].checkerName, "NullPtrDereferenceChecker");

    // add null deref and unused fieldTwo problems to boo_copy.cpp
    std::ofstream booStream(booCopyPath.string());
    ASSERT_TRUE(booStream.good());
    booStream << "int* get_ptr(){ return nullptr; }\n";
    booStream.close();
    FileEntriesCache::GetInstance().RemoveEntry(booCopyPath.string());

    // run analysis on modified source
    HCXX::ProblemsList problemsList3(workspace);
    TestAnalyze(workspace, problemsList3, nullptr, astParser.get());
    // Should report three problems
    ASSERT_EQ(problemsList3.GetProblems().size(), 3);

    // run analysis once again to check stored problems
    HCXX::ProblemsList problemsList4(workspace);
    TestAnalyze(workspace, problemsList4, nullptr, astParser.get());
    // run analysis once again with added macro definition
    auto options = workspace.GetCompilerOptions();
    options.AddExtraOptions({"-DFOO_DEF"});

    HCXX::ProblemsList problemsList5(workspace);
    TestAnalyze(workspace, problemsList5, &options, astParser.get());

    // Should report two problems
    ASSERT_EQ(problemsList5.GetProblems().size(), 2);
}

TEST_F(WorkspaceTest, IncrementalModeNewProblems)
{
    auto workspacePath = commonWorkspaceTestPath / "workspace_9";
    auto fooCopyPath = workspacePath / "foo_copy.cpp";

    std::filesystem::remove_all(workspacePath / ".cooddy/storage");
    std::filesystem::remove(fooCopyPath);
    std::filesystem::copy_file(workspacePath / "foo.cpp", fooCopyPath);

    TempDir dir(workspacePath / ".cooddy");
    TempFile fooCopyTmp(workspacePath / "foo_copy.cpp");

    auto astParser = HCXX::Parser::Create();
    WorkspaceOptions workspaceOptions;
    workspaceOptions.incrementalSettings.FromCliArg("nu");
    Workspace workspace(workspacePath.string(), {"OutOfBoundsChecker"}, workspaceOptions);
    workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(HCXX::TaintSettings{"su"}));

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList, nullptr, astParser.get());
    ASSERT_EQ(problemsList.GetProblems().size(), 3);

    // apply changes
    std::filesystem::remove(fooCopyPath);
    std::filesystem::copy_file(workspacePath / "foo_changed.cpp", fooCopyPath);
    FileEntriesCache::GetInstance().RemoveEntry(fooCopyPath.string());

    // run analysis on modified source
    HCXX::ProblemsList problemsList2(workspace);
    TestAnalyze(workspace, problemsList2, nullptr, astParser.get());
    ASSERT_EQ(problemsList2.GetProblems().size(), 1);

    auto problem = problemsList2.GetProblems()[0];
    ASSERT_EQ(problem.checkerName, "OutOfBoundsChecker");
    ASSERT_EQ(problem.functionName, "boo1");
    ASSERT_EQ(problem.sourceId, "some");
    ASSERT_EQ(problem.line, 8);
    ASSERT_EQ(problem.column, 5);
}

TEST_F(WorkspaceTest, IncrementalModeOff)
{
    IncrementalSettings settings;
    settings.FromCliArg("off");
    ASSERT_FALSE(settings.readMode);
    ASSERT_FALSE(settings.writeMode);
    ASSERT_FALSE(settings.reportNewProblems);
}

TEST_F(WorkspaceTest, CFileWithCppCodeTest)
{
    Workspace workspace((commonWorkspaceTestPath / "workspace_8/compile_commands.json").string());

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);
    int foundNPD = 0;

    for (auto& problem : problemsList.GetProblems()) {
        if (problem.checkerName == "NullPtrDereferenceChecker") {
            foundNPD += 1;
        }
    }
    ASSERT_FALSE(myTestErrorCheckConsumer.HasParseError());
    ASSERT_EQ(foundNPD, 4);
}

TEST_F(WorkspaceTest, AnalyzeFolderWithCompileOptionTest)
{
    auto path = commonWorkspaceTestPath / "workspace_10";
    Workspace workspace(path.string(), {"OutOfBoundsChecker"});
    auto options = workspace.GetCompilerOptions();
    options.AddExtraOptions({{"-I" + (path / "include").string()}});

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList, &options);

    ASSERT_FALSE(myTestErrorCheckConsumer.HasParseError());
    ASSERT_EQ(1, problemsList.GetProblems().size());
}

TEST_F(WorkspaceTest, SettingsProfileTest)
{
    auto path = commonWorkspaceTestPath / "workspace_11";
    Workspace workspace(path.string(), {}, {}, "user");

    auto& annotations = workspace.GetProfile().GetSettings().annotations;
    ASSERT_TRUE(annotations.find(".some_annotations_1.json") != annotations.end());
    ASSERT_TRUE(annotations.find(".some_annotations_2.json") != annotations.end());

    auto& rhsServers = workspace.GetProfile().GetSettings().rhsServers;
    ASSERT_EQ(rhsServers.size(), 1);
    ASSERT_TRUE(rhsServers[0] == "http://some_url");
}

TEST_F(WorkspaceTest, RelativeScopePathTest)
{
    Workspace workspace("main.cpp", {}, {}, "default.profile.json", commonWorkspaceTestPath / "empty");
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);
}

#ifdef _WIN32
TEST_F(WorkspaceTest, UncScopePathTest)
{
    // See https://learn.microsoft.com/en-us/dotnet/standard/io/file-path-formats#unc-paths
    std::string path = (commonWorkspaceTestPath / "empty" / "main.cpp").string();
    path = "\\\\localhost\\" + StrUtils::Replace(path, ":", "$");

    Workspace workspace(path, {}, {}, "default.profile.json", commonWorkspaceTestPath / "empty");
    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);
}
#endif  // _WIN32

TEST_F(WorkspaceTest, CompileOptionsReplacements)
{
    auto path = commonWorkspaceTestPath / "workspace_12";
    WorkspaceOptions workspaceOptions;
    workspaceOptions.compileOptionReplacements = {
        {"\"wrong_compiler\"", "gcc -x c++"}, {"wrong_include", "include"}, {"'<wrong_define>'", "<header.h>"}};
    Workspace workspace(path.string(), {"OutOfBoundsChecker"}, workspaceOptions);

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);

    ASSERT_FALSE(myTestErrorCheckConsumer.HasParseError());
}
