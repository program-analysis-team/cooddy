/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <EntryFuncConsumer.h>
#include <Parser.h>
#include <ProblemsList.h>
#include <dfa/TaintSettings.h>
#include <reporters/CompositeReporter.h>
#include <reporters/CsvReporter.h>
#include <reporters/JsonReporter.h>
#include <reporters/OutReporter.h>
#include <utils/Memory.h>
#include <utils/TempFile.h>

#include <atomic>
#include <filesystem>
#include <memory>
#include <set>
#include <thread>

#include "CheckerTest.h"
#include "Cli.h"
#include "TestBaseClass.h"

using namespace HCXX;
using namespace std::filesystem;

// The fixture for testing class AnalyzerToolTest
class AnalyzerToolTest : public TestBaseClass {
protected:
    std::filesystem::path testDirectory = TEST_SUITE_PATH("analyzerToolTest");

    auto AnalyzeFile(const std::string& fileName)
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append(fileName).string();
        WorkspaceOptions ops;
        ops.problemSeverityLevel = Problem::Severity::NOTIFY;
        Workspace workspace(std::move(file), {}, ops, "cwe");
        workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(HCXX::TaintSettings{"su"}));
        auto result = std::make_unique<ProblemsList>(workspace);
        TestAnalyze(workspace, *result);
        return result;
    }

    void TaintedSourcesCsvReporterTest(TaintSettings settings, bool hasCsvLines, Problem::Severity severity)
    {
        TempFile result("cooddy_result.csv");
        {
            auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_grouped_report.cpp").string();
            WorkspaceOptions ops;
            ops.problemSeverityLevel = severity;
            Workspace workspace(std::move(file), {}, ops, "cwe");
            workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(settings));
            CsvReporter problemsList(workspace);
            problemsList.Init(TestBaseClass::GetParser(), current_path().string());
            TestAnalyze(workspace, problemsList);
        }
        std::string content = result.GetLine(severity == Problem::Severity::WARNING ? 4 : 2);
        ASSERT_EQ(content.find("5,12,c/c++,NotAnIssue,,NoIssues,foo,Annotated by macro") != std::string::npos,
                  hasCsvLines);
        content = result.GetLine();
        if (severity == Problem::Severity::WARNING) {
            ASSERT_EQ(content.find("10,1,c/c++,NotAnIssue,,IssuesFound,goo3,Annotated by macro") != std::string::npos,
                      hasCsvLines);
        } else {
            ASSERT_EQ(content.find("10,1,c/c++,NotAnIssue,,NoIssues,goo3,Annotated by macro") != std::string::npos,
                      hasCsvLines);
        }
    }
};

TEST_F(AnalyzerToolTest, AstDumpTest)
{
    TempFile astDump("ast-dump.zip");

    std::filesystem::path testDirectory = TEST_SUITE_PATH("AstDumpTest");
    Workspace workspace(testDirectory.append("virtual_methods").string(), {}, {}, "cwe");
    auto analyzer = Analyzer::Create(TestBaseClass::GetParser(), workspace);
    ProblemsList problemsList(workspace);
    TestErrorCheckConsumer consumerWithFlag;
    consumerWithFlag.SetParseFlags(Parser::DUMP_AST);
    analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, consumerWithFlag);
    ASSERT_FALSE(consumerWithFlag.HasToolError());

    // sleep 100ms due to the issue with sequential archive reopen (conflict with AV)
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    Workspace astDumpWorkspace(astDump, {}, {}, "cwe");
    astDumpWorkspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(TaintSettings{"su"}));
    TestAnalyze(astDumpWorkspace, problemsList);
    ASSERT_EQ(problemsList.GetProblems().size(), 1);
}

TEST_F(AnalyzerToolTest, AstDumpPrinterTest)
{
    UniqueId hash;
    TempFile astDump("ast-dump.zip");
    {
        std::filesystem::path testFile = TEST_SUITE_PATH("AstDumpPrinterTest/test.cpp");
        std::ifstream sourceStream(testFile);
        std::string sourceText =
            string((std::istreambuf_iterator<char>(sourceStream)), std::istreambuf_iterator<char>());
        TempFile testFileCopy("test.cpp", sourceText);  // Should be removed before testing AST dump
        hash = UniqueId::CalcMd5Hash(sourceText);

        Workspace workspace(testFileCopy, {}, {}, "cwe");
        auto analyzer = Analyzer::Create(TestBaseClass::GetParser(), workspace);
        ProblemsList problemsList(workspace);
        TestErrorCheckConsumer consumerWithFlag;
        consumerWithFlag.SetParseFlags(Parser::DUMP_AST);
        analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, consumerWithFlag);
    }
    // sleep 100ms due to the issue with sequential archive reopen (conflict with AV)
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    {
        TempFile testFileCopy("test.cpp");
        Workspace astDumpWorkspace(astDump, {}, {}, "cwe");
        ProblemsList problemsList(astDumpWorkspace);
        TestAnalyze(astDumpWorkspace, problemsList);

        auto entry = FileEntriesCache::GetInstance().GetFileEntry(hash.ToString());
        ASSERT_NE(entry, nullptr);
        auto res = entry->fileSource;
        std::ifstream resultStream(TEST_SUITE_PATH("AstDumpPrinterTest/test.txt"));
        std::string resultExpected =
            string((std::istreambuf_iterator<char>(resultStream)), std::istreambuf_iterator<char>());
        ASSERT_EQ(res, resultExpected);
    }
}

TEST_F(AnalyzerToolTest, AstDumpTestGlobals)
{
    TempFile astDump("ast-dump.zip");

    std::filesystem::path testDirectory = TEST_SUITE_PATH("AstDumpTestGlobals");
    Workspace workspace(testDirectory.append("ast_dump_test").string(), {}, {}, "cwe");
    auto analyzer = Analyzer::Create(TestBaseClass::GetParser(), workspace);
    ProblemsList problemsList(workspace);
    TestErrorCheckConsumer consumerWithFlag;
    consumerWithFlag.SetParseFlags(Parser::DUMP_AST);
    analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, consumerWithFlag);
    ASSERT_FALSE(consumerWithFlag.HasToolError());

    struct Consumer : public HCXX::Analyzer::Consumer {
        void OnAnalysisEnd(TranslationUnit& unit) override
        {
            auto crossInfo = unit.GetCrossTUContext();
            auto global = crossInfo->FindGlobalVar("g_arr");
            auto funcDef = crossInfo->FindFunctionDef("get_idx");
            ASSERT_TRUE(global != nullptr && funcDef != nullptr);
            ASSERT_EQ(global->size(), 2);
            ASSERT_EQ(funcDef->size(), 2);
            for (auto& def : *global) {
                ASSERT_TRUE(def.unit != nullptr && StrUtils::EndsWith(def.unit->GetMainFileName(), "boo.c"));
            }
            for (auto& def : *funcDef) {
                ASSERT_TRUE(def.unit != nullptr && StrUtils::EndsWith(def.unit->GetMainFileName(), "boo.c"));
            }
        };
    } consumer;

    Workspace astDumpWorkspace(astDump, {}, {}, "cwe");
    ProblemsList astProblems(workspace);
    analyzer = HCXX::Analyzer::Create(TestBaseClass::GetParser(), workspace);
    analyzer->Analyze(astDumpWorkspace.GetCompilerOptions(), astProblems, consumer);
    ASSERT_EQ(astProblems.GetProblems().size(), 1);
    ASSERT_EQ(astProblems.GetProblems()[0].line, 7);
}

TEST_F(AnalyzerToolTest, AnalyzeWithFatalErrors)
{
    Workspace workspace(TEST_SUITE_PATH("analyzerToolTest/test_fatal_errors.cpp").string(),
                        {"NullPtrDereferenceChecker"});
    auto parser = HCXX::Parser::Create();
    {
        ProblemsList problemsList(workspace);
        TestAnalyze(workspace, problemsList, nullptr, parser.get());
        ASSERT_EQ(problemsList.GetProblems().size(), 0);
        myTestErrorCheckConsumer.ClearToolError();
    }
    {
        ProblemsList problemsList(workspace);
        parser->statistics.maxFatalErrorCount = 20;
        TestAnalyze(workspace, problemsList, nullptr, parser.get());
        ASSERT_EQ(problemsList.GetProblems().size(), 1);
        myTestErrorCheckConsumer.ClearToolError();
    }
}

TEST_F(AnalyzerToolTest, UniqueValueInResultJsonTest)
{
    auto file = testDirectory.append("test_unique.cpp").string();
    Workspace workspace(std::move(file), {}, {.profile = "test"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto result = problemsList.descriptor.problems;
    ASSERT_FALSE(result.empty());

    for (auto& el : result) {
        ASSERT_EQ(1, std::count(result.begin(), result.end(), el));
    }
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest)
{
    auto file = testDirectory.append("test_trace.cpp").string();
    Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto& result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("outString", result[0].functionName);

    auto& el = result[0];
    // We expect only one defect Null dereference
    ASSERT_EQ("Dereferencing of \"myString\" which can be null", el.description);
    ASSERT_EQ(4, el.problemTrace.size());
    // And trace with 4 events
    auto it = el.problemTrace.begin();
    ASSERT_EQ(8, it->line);
    ASSERT_EQ(8, it->endLine);
    ASSERT_EQ(12, it->column);
    ASSERT_EQ(19, it->endColumn);
    ++it;
    ASSERT_EQ(20, it->line);
    ASSERT_EQ(20, it->endLine);
    ASSERT_EQ(28, it->column);
    ASSERT_EQ(44, it->endColumn);
    ++it;
    ASSERT_EQ(21, it->line);
    ASSERT_EQ(21, it->endLine);
    ASSERT_EQ(15, it->column);
    ASSERT_EQ(23, it->endColumn);
    ++it;
    ASSERT_EQ(13, it->line);
    ASSERT_EQ(13, it->endLine);
    ASSERT_EQ(9, it->column);
    ASSERT_EQ(17, it->endColumn);
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest2)
{
    auto file = testDirectory.append("test_trace_2.cpp").string();
    Workspace workspace(std::move(file), {"UninitializedMemoryUsageChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto& result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());

    auto& el = result[0];
    ASSERT_EQ(4, el.problemTrace.size());

    auto it = el.problemTrace.begin();
    ASSERT_EQ(17, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(18, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(6, it->line);
    ASSERT_EQ(9, it->column);
    ++it;
    ASSERT_EQ(19, it->line);
    ASSERT_EQ(12, it->column);
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest3)
{
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_3.cpp").string();
    Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto& result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());

    auto& el = result[0];
    ASSERT_EQ(2, el.problemTrace.size());

    auto it = el.problemTrace.begin();
    ASSERT_EQ(21, it->line);
    ASSERT_EQ(16, it->column);
    ++it;
    ASSERT_EQ(22, it->line);
    ASSERT_EQ(13, it->column);
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest4)
{
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_4.cpp").string();
    Workspace workspace(std::move(file), {"UninitializedMemoryUsageChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto& result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());

    auto& el = result[0];
    ASSERT_EQ(5, el.problemTrace.size());

    auto it = el.problemTrace.begin();
    ASSERT_EQ(29, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(30, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(19, it->line);
    ASSERT_EQ(9, it->column);
    ++it;
    ASSERT_EQ(8, it->line);
    ASSERT_EQ(9, it->column);
    ++it;
    ASSERT_EQ(31, it->line);
    ASSERT_EQ(12, it->column);
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest5)
{
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_5.cpp").string();
    Workspace workspace(std::move(file), {"UninitializedMemoryUsageChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto& result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());

    auto& el = result[0];
    ASSERT_EQ(6, el.problemTrace.size());

    auto it = el.problemTrace.begin();
    ASSERT_EQ(24, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(25, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(9, it->line);
    ASSERT_EQ(9, it->column);
    ++it;
    ASSERT_EQ(26, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(17, it->line);
    ASSERT_EQ(9, it->column);
    ++it;
    ASSERT_EQ(27, it->line);
    ASSERT_EQ(16, it->column);
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest6)
{
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_6.cpp").string();
    Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto& result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());

    auto& el = result[0];
    ASSERT_EQ(3, el.problemTrace.size());

    auto it = el.problemTrace.begin();
    ASSERT_EQ(4, it->line);
    ASSERT_EQ(12, it->column);
    ++it;
    ASSERT_EQ(10, it->line);
    ASSERT_EQ(18, it->column);
    ++it;
    ASSERT_EQ(11, it->line);
    ASSERT_EQ(17, it->column);
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest7)
{
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_7.cpp").string();
    Workspace workspace(std::move(file), {"UninitializedMemoryUsageChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto& result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());
    auto& el = result[0];

    ASSERT_EQ(4, el.problemTrace.size());

    auto it = el.problemTrace.begin();
    ASSERT_EQ(14, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(15, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(9, it->line);
    ASSERT_EQ(5, it->column);
    ++it;
    ASSERT_EQ(17, it->line);
    ASSERT_EQ(12, it->column);
}

TEST_F(AnalyzerToolTest, TraceInResultJsonTest8)
{
    TempFile file("cooddy_result.json");

    auto workspacePath = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_8.cpp");
    Workspace workspace(workspacePath.string(), {"MemoryAndResourceLeakChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string());
    TestAnalyze(workspace, problemsList);

    auto result = problemsList.descriptor.problems;
    ASSERT_EQ(1, result.size());
    auto& problem = result[0];
    ASSERT_EQ(2, problem.problemTrace.size());
    ASSERT_EQ(13, problem.problemTrace[0].line);
    ASSERT_EQ(17, problem.problemTrace[0].column);
    ASSERT_EQ(13, problem.problemTrace[1].line);
    ASSERT_EQ(5, problem.problemTrace[1].column);
}

TEST_F(AnalyzerToolTest, TraceInResultHtml)
{
    TempDir dir("html_traces");
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_html.cpp").string();
        Workspace workspace(std::move(file), {"NullPtrDereferenceChecker", "OutOfBoundsChecker"});
        workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(TaintSettings{"su"}));
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string(), Reporter::HTML_REPORT);
        TestAnalyze(workspace, problemsList);
    }
    ASSERT_FALSE(result.GetLine().empty());
    ASSERT_FALSE(result.GetLine().empty());
    ASSERT_FALSE(result.GetLine().empty());
    ASSERT_FALSE(result.GetLine().empty());
    ASSERT_TRUE(is_directory("html_traces"));
    path p("html_traces");
    using Fp = bool (*)(const path&);
    size_t traceCount = std::count_if(directory_iterator(p), directory_iterator{}, static_cast<Fp>(is_regular_file));
    ASSERT_EQ(traceCount, 3);
}

TEST_F(AnalyzerToolTest, TraceInResultHtml2)
{
    TempDir dir("html_traces");
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_group_html.cpp").string();
        Workspace workspace(std::move(file), {"OutOfBoundsChecker"});
        workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(TaintSettings{"su"}));
        CompositeReporter problemsList({"csv-html"}, workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string(), Reporter::HTML_REPORT);
        TestAnalyze(workspace, problemsList);
    }
    ASSERT_TRUE(is_directory("html_traces"));
    path p("html_traces");
    using Fp = bool (*)(const path&);
    size_t traceCount = std::count_if(directory_iterator(p), directory_iterator{}, static_cast<Fp>(is_regular_file));
    ASSERT_EQ(traceCount, 3);
}

TEST_F(AnalyzerToolTest, TraceJarInResultHtml)
{
    TempDir dir("html_traces");
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("java").append("test_trace_html.jar");
        Workspace workspace(file.string(), {"NullPtrDereferenceChecker"});
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string(), Reporter::HTML_REPORT);
        TestAnalyze(workspace, problemsList, nullptr, &TestBaseClass::GetParser(&file));
    }
    ASSERT_FALSE(result.GetLine().empty());
    ASSERT_TRUE(is_directory("html_traces"));
    path p("html_traces");
    using Fp = bool (*)(const path&);
    size_t traceCount = std::count_if(directory_iterator(p), directory_iterator{}, static_cast<Fp>(is_regular_file));
    ASSERT_EQ(traceCount, 1);
}

TEST_F(AnalyzerToolTest, TraceTwoFilesHtml)
{
    TempDir dir("html_traces");
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace_html2.cpp").string();
        Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"});
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string(), Reporter::HTML_REPORT);
        TestAnalyze(workspace, problemsList);
    }
    ASSERT_FALSE(result.GetLine().empty());
    ASSERT_TRUE(is_directory("html_traces"));
    path p("html_traces");
    using Fp = bool (*)(const path&);
    size_t traceCount = std::count_if(directory_iterator(p), directory_iterator{}, static_cast<Fp>(is_regular_file));
    ASSERT_EQ(traceCount, 1);
}

TEST_F(AnalyzerToolTest, SinkFunctionFieldTest)
{
    TempDir dir("html_traces");
    TempFile jsonReport("cooddy_result.json");
    TempFile csvResult("cooddy_result.csv");

    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("sink_function_test.cpp").string();
        Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"});

        JsonReporter jsonProblemsList(workspace);
        jsonProblemsList.Init(TestBaseClass::GetParser(), current_path().string());
        TestAnalyze(workspace, jsonProblemsList);
        auto jsonResult = jsonReport.GetJson<JsonReporter::ReportDescriptor>();
        ASSERT_FALSE(jsonResult.problems.empty());
        ASSERT_TRUE(jsonResult.problems[0].sinkFunction == "strlen");

        CsvReporter csvProblemsList(workspace);
        csvProblemsList.Init(TestBaseClass::GetParser(), current_path().string(), Reporter::HTML_REPORT);
        TestAnalyze(workspace, csvProblemsList);
    }
    std::string content = csvResult.GetLine(2);
    ASSERT_TRUE(content.find("sink_function_test.cpp,sink_function_test.cpp,foo,5,12,c/"
                             "c++,NullPtrDereferenceChecker,CWE,NULL.DEREF,,,strlen,") == 0);
}

TEST_F(AnalyzerToolTest, DescriptionJsonTest)
{
    TempFile report("cooddy_result.json");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_unique.cpp").string();
    Workspace workspace(std::move(file), {}, WorkspaceOptions{.commandLine = "--log=error", .profile = "test"});
    {
        JsonReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string());
        TestAnalyze(workspace, problemsList);
    }  // JsonReporter destructor runs here

    auto result = report.GetJson<JsonReporter::ReportDescriptor>();

    ASSERT_TRUE(result.profile == "test");
    ASSERT_TRUE(result.commandLine == "--log=error");
    ASSERT_TRUE(!result.gitCommit.empty());
    ASSERT_TRUE(!result.gitVersion.empty());
    ASSERT_TRUE(!result.startTime.empty());
    ASSERT_TRUE(!result.endTime.empty());
}

TEST_F(AnalyzerToolTest, ProblemIdJsonTest)
{
    auto result1 = AnalyzeFile("test_problem_id.c");
    auto result2 = AnalyzeFile("test_problem_id_changed.c");

    auto problems1 = result1->GetProblems();
    auto problems2 = result2->GetProblems();

    const int numProblems = 4;
    ASSERT_EQ(problems1.size(), numProblems);
    ASSERT_EQ(problems2.size(), numProblems);

    for (int i = 0; i < numProblems; ++i) {
        int j = 0;
        while (j < numProblems && problems1[i].uniqueId != problems1[j].uniqueId) {
            j++;
        }
        if (j == numProblems) {
            ASSERT_EQ(problems1[i].uniqueId, problems2[i].uniqueId);
        }
    }
}

TEST_F(AnalyzerToolTest, FileCreatedCsvReporterTest)
{
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_unique.cpp").string();
        Workspace workspace(std::move(file));
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string());
        TestAnalyze(workspace, problemsList);
    }
    ASSERT_FALSE(result.GetLine().empty());
}

TEST_F(AnalyzerToolTest, ProblemSeveirtyOpTestError)
{
    TempFile jsonReport("cooddy_result.json");
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_prob_severity.cpp").string();
    WorkspaceOptions ops;
    ops.problemSeverityLevel = HCXX::Problem::Severity::ERROR;
    Workspace workspace(std::move(file), {}, ops);

    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    TestAnalyze(workspace, problemsList);

    auto jsonResult = jsonReport.GetJson<JsonReporter::ReportDescriptor>();
    ASSERT_GT(jsonResult.problems.size(), 0);
    for (auto& problem : jsonResult.problems) {
        ASSERT_EQ(problem.problemClass.severity, "ERROR");
    }
}

TEST_F(AnalyzerToolTest, ProblemSeveirtyOpTestWarning)
{
    TempFile jsonReport("cooddy_result.json");
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_prob_severity.cpp").string();
    WorkspaceOptions ops;
    ops.problemSeverityLevel =
        Problem::SeverityFromStr(HCXX::StrUtils::BigSnakeCaseToCamelCase(StrUtils::ToUpper("warning")));
    Workspace workspace(std::move(file), {}, ops);

    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    TestAnalyze(workspace, problemsList);

    auto jsonResult = jsonReport.GetJson<JsonReporter::ReportDescriptor>();
    ASSERT_GT(jsonResult.problems.size(), 0);
    bool hasWarning = false;
    for (auto& problem : jsonResult.problems) {
        if (problem.problemClass.severity == "WARNING") {
            hasWarning = true;
        }
    }

    ASSERT_TRUE(hasWarning);
}

TEST_F(AnalyzerToolTest, TaintedSourcesCsvReporterTest)
{
    TaintedSourcesCsvReporterTest(TaintSettings{"s", true}, true, Problem::Severity::ERROR);
}

TEST_F(AnalyzerToolTest, TaintedSourcesCsvReporterTest2)
{
    TaintedSourcesCsvReporterTest(TaintSettings{"s", false}, false, Problem::Severity::ERROR);
}

TEST_F(AnalyzerToolTest, TaintedSourcesCsvReporterTest3)
{
    TaintedSourcesCsvReporterTest(TaintSettings{"s", true}, true, Problem::Severity::WARNING);
}

TEST_F(AnalyzerToolTest, QuotesInCsvReporterTest)
{
    auto path = EnvironmentUtils::GetSelfExecutableDir() / ".cooddy";
    if (!exists(path)) {
        create_directories(path);
    }
    TempFile profile(path / "a.profile",
                     "{\"name\": "
                     "\"\\\"quo\\\"ted\\\"\",\"module\":\"libcwe\",\"annotations\":[],\"checkers\": {"
                     "  \"NullPtrDereferenceChecker\": {\"inspections\" : ["
                     "{\"id\": \"NULL.DEREF\", \"severity\": \"Error\", \"enabled\": true},"
                     "{\"id\": \"NULL.UNTRUSTED.DEREF\", \"severity\": \"Error\", \"enabled\": true}"
                     "]}}}");

    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test,quotes_in_csv.cpp").string();
        Workspace workspace(std::move(file), {}, WorkspaceOptions{}, profile);
        workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(TaintSettings{"s"}));
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string());
        TestAnalyze(workspace, problemsList);
    }
    std::string content = result.GetLine(2);
    ASSERT_TRUE(content.find("\"test,quotes_in_csv.cpp\",\"A<int, "
                             "int>::goo3\",13,17,c/c++,NullPtrDereferenceChecker,\"\"\"quo\"\"ted\"\"\"") !=
                std::string::npos);
}

TEST_F(AnalyzerToolTest, FuncNameInResultCsvReporterTest)
{
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace.cpp").string();
        Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"});
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string());
        TestAnalyze(workspace, problemsList);
    }
    std::string content = result.GetLine(2);
    ASSERT_TRUE(content.find(",outString,13,9,c/c++,NullPtrDereferenceChecker,CWE,NULL.DEREF,getString") !=
                std::string::npos);
}

TEST_F(AnalyzerToolTest, RelativePathProjectCsvReporterTest)
{
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace.cpp").string();
        WorkspaceOptions workspaceOptions;
        workspaceOptions.projectRoot = TEST_SUITE_PATH("analyzerToolTest/projectRoot");
        Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"}, workspaceOptions);
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string());
        TestAnalyze(workspace, problemsList);
    }
    std::string content = result.GetLine(2);
#if defined(WIN32)
    std::string findline = "..\\test_trace.cpp,test_trace.cpp,";
#else
    std::string findline = "../test_trace.cpp,test_trace.cpp,";
#endif
    ASSERT_TRUE(content.find(findline) != std::string::npos);
}

TEST_F(AnalyzerToolTest, ProfileLangTest)
{
    auto path = EnvironmentUtils::GetSelfExecutableDir() / ".cooddy";
    if (!exists(path)) {
        create_directories(path);
    }
    TempFile aFileName(path / "a.profile",
                       "{\"languages\":[\"java\"],\"checkers\":{\"A\":{\"languages\":[\"c\", \"c++\"]},\"B\":{}}}");
    Workspace workspace(std::move((TEST_SUITE_PATH("analyzerToolTest") / "test_trace.cpp").string()), {},
                        WorkspaceOptions{}, aFileName);
    auto langsA = workspace.GetProfile().GetCheckerCfg("A").languages;
    ASSERT_TRUE(langsA.find("c++") != langsA.end());
    auto langsB = workspace.GetProfile().GetCheckerCfg("B").languages;
    ASSERT_TRUE(langsB.find("java") != langsB.end());
}

TEST_F(AnalyzerToolTest, ProfileCheckersOrderTest)
{
    auto test = [this](std::string aProfile, std::string bProfile, std::string cProfile, std::string expected) {
        auto path = EnvironmentUtils::GetSelfExecutableDir() / ".cooddy";
        if (!exists(path)) {
            create_directories(path);
        }
        TempFile aFileName(path / "a.profile", aProfile);
        TempFile bFileName(path / "b.profile", bProfile);
        TempFile cFileName(path / "c.profile", cProfile);
        TempFile result("cooddy_result.csv");
        TempFile jsonReport("cooddy_result.json");
        {
            WorkspaceOptions ops;
            ops.problemSeverityLevel = Problem::Severity::NOTIFY;
            Workspace workspace(std::move((TEST_SUITE_PATH("analyzerToolTest") / "test_trace.cpp").string()),
                                {"NullPtrDereferenceChecker"}, ops, aFileName);
            // COODDY_SUPPRESS RedundantVectorUsageChecker
            std::vector<std::string> reps{"json", "csv-html"};
            CompositeReporter problemsList(reps, workspace);
            problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
            TestAnalyze(workspace, problemsList);
        }
        std::string content = result.GetLine(2);
        ASSERT_TRUE(content.find("NullPtrDereferenceChecker," + expected) != std::string::npos);
        for (auto problem : jsonReport.GetJson<JsonReporter::ReportDescriptor>().problems) {
            ASSERT_EQ(problem.problemClass.profileName, expected);
        }
    };
    test("{\"name\":\"A\",\"profiles\": [\"b.profile\",\"c.profile\"],\"checkers\":{\"NullPtrDereferenceChecker\":{}}}",
         "{\"name\":\"B\",\"checkers\":{\"NullPtrDereferenceChecker\":{}}}",
         "{\"name\":\"C\",\"checkers\":{\"NullPtrDereferenceChecker\":{}}}", "A");
    test("{\"name\":\"A\",\"profiles\": [\"b.profile\",\"c.profile\"]}",
         "{\"name\":\"B\",\"checkers\":{\"NullPtrDereferenceChecker\":{}}}",
         "{\"name\":\"C\",\"checkers\":{\"NullPtrDereferenceChecker\":{}}}", "C");
    test("{\"name\":\"A\",\"profiles\": [\"b.profile\",\"c.profile\"]}",
         "{\"name\":\"B\",\"checkers\":{\"NullPtrDereferenceChecker\":{}}}", "{\"name\":\"C\"}", "B");
}

TEST_F(AnalyzerToolTest, CodeLanguageInResultCsvReporterTest)
{
    TempFile csvReport("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_codeLanguage.cpp").string();
        WorkspaceOptions ops;
        ops.problemSeverityLevel = Problem::Severity::NOTIFY;
        Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"}, ops);
        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
        TestAnalyze(workspace, problemsList);
    }
    std::string content = csvReport.GetLine(2);
    ASSERT_TRUE(content.find("test_codeLanguage.cpp,test_codeLanguage.cpp,main,3,13,c/"
                             "c++,NullPtrDereferenceChecker,CWE,NULL.DEREF") == 0);
}

TEST_F(AnalyzerToolTest, CodeLanguageInResultJsonReporterTest)
{
    TempFile jsonReport("cooddy_result.json");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_codeLanguage.cpp").string();
    Workspace workspace(std::move(file), {"NullPtrDereferenceChecker", "RttiChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    TestAnalyze(workspace, problemsList);

    auto jsonResult = jsonReport.GetJson<JsonReporter::ReportDescriptor>();
    ASSERT_GT(jsonResult.problems.size(), 0);
    for (auto problem : jsonResult.problems) {
        ASSERT_EQ(problem.codeLanguage, "c/c++");
    }
}

TEST_F(AnalyzerToolTest, CodeSnippetInResultJsonReporterTest)
{
    TempFile jsonReport("cooddy_result.json");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_codeLanguage.cpp").string();
    Workspace workspace(std::move(file), {"NullPtrDereferenceChecker"});
    JsonReporter problemsList(workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 3);
    TestAnalyze(workspace, problemsList);

    auto jsonResult = jsonReport.GetJson<JsonReporter::ReportDescriptor>();
    ASSERT_GT(jsonResult.problems.size(), 0);
    for (auto& problem : jsonResult.problems) {
        ASSERT_FALSE(problem.codeSnippet.empty());
    }
}

TEST_F(AnalyzerToolTest, FileCreatedCompositeReporterTest)
{
    TempFile result("cooddy_result.json");
    TempFile resultCsv("cooddy_result.csv");
    TempFile resultHtml("cooddy_result.html");
    TempFile resultXml("cooddy_result.xml");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_macros.cpp").string();
    Workspace workspace(std::move(file));
    std::vector<std::string> reps{"json", "csv-html", "html"};
    CompositeReporter problemsList(reps, workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    TestAnalyze(workspace, problemsList);

    ASSERT_TRUE(result.Exists());
    ASSERT_TRUE(resultCsv.Exists());
    ASSERT_TRUE(resultHtml.Exists());
}

TEST_F(AnalyzerToolTest, RhsServersFromOptionsTest)
{
    TempFile resultHtml("cooddy_result.html");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_html.cpp").string();
    WorkspaceOptions options;
    options.settings.rhsServers = {"DummyServer1", "DummyServer2", "DumyServer3"};
    Workspace workspace(std::move(file), {}, options);
    std::vector<std::string> reps{"html"};
    CompositeReporter problemsList(reps, workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    TestAnalyze(workspace, problemsList);

    ASSERT_TRUE(resultHtml.Exists());
    ASSERT_NE(resultHtml.GetLine(3).find("'DummyServer1','DummyServer2','DumyServer3'"), string::npos);
}

TEST_F(AnalyzerToolTest, RhsServersFromProfileAndOptionsTest)
{
    auto path = EnvironmentUtils::GetSelfExecutableDir() / ".cooddy";
    if (!exists(path)) {
        create_directories(path);
    }
    TempFile profile(path / "a.profile", "{\"rhs-servers\":[\"DummyServer1\"]}");
    TempFile resultHtml("cooddy_result.html");
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_html.cpp").string();
    WorkspaceOptions options;
    options.settings.rhsServers = {"DummyServer1", "DummyServer2", "DumyServer3"};
    Workspace workspace(std::move(file), {}, options, profile);
    std::vector<std::string> reps{"html"};
    CompositeReporter problemsList(reps, workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    TestAnalyze(workspace, problemsList);

    ASSERT_TRUE(resultHtml.Exists());
    ASSERT_NE(resultHtml.GetLine(3).find("'DummyServer1','DummyServer2','DumyServer3'"), string::npos);
}

TEST_F(AnalyzerToolTest, RhsServersFromProfileTest)
{
    auto path = EnvironmentUtils::GetSelfExecutableDir() / ".cooddy";
    if (!exists(path)) {
        create_directories(path);
    }
    TempFile profile(path / "a.profile", "{\"rhs-servers\":[\"DummyServer1\"]}");
    TempFile resultHtml("cooddy_result.html");
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_html.cpp").string();
    Workspace workspace(std::move(file), {}, WorkspaceOptions{}, profile);
    std::vector<std::string> reps{"html"};
    CompositeReporter problemsList(reps, workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    TestAnalyze(workspace, problemsList);

    ASSERT_TRUE(resultHtml.Exists());
    ASSERT_NE(resultHtml.GetLine(3).find("['DummyServer1']"), string::npos);
}

TEST_F(AnalyzerToolTest, GroupingProblemsTest)
{
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_source_id.c").string();

    WorkspaceOptions options;
    options.problemSeverityLevel = Problem::Severity::WARNING;
    Workspace workspace(std::move(file),
                        {"NullPtrDereferenceChecker", "UninitializedMemoryUsageChecker", "BufferMaxSizeChecker"},
                        options);
    ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);

    auto& problems = problemsList.GetProblems();
    ASSERT_EQ(problems.size(), 9);

    std::set<std::string> sourceIds;
    for (auto& problem : problems) {
        if (!problem.sourceId.empty()) {
            sourceIds.emplace(problem.sourceId);
        }
    }
    ASSERT_EQ(sourceIds.size(), 2);
    auto it = sourceIds.begin();
    ASSERT_EQ(*it++, "get_ptr");
    ASSERT_EQ(*it++, "init");
}

/**
 * @brief Test the ability of Analyzer to run analysis asyncronously via Analyzer::AnalyzeAsync.
 *
 * There are no changes in analysis process. The only different code executed in this test is the entry point: Analyze
 * vs AnalyzeAsync. This means that no checks are necessary - this test is just making sure nothing crashed from
 * possible use-after-free bugs.
 */
TEST_F(AnalyzerToolTest, AsyncAnalyzeTest)
{
    struct SignalingProblemsList : public ProblemsList {
        std::atomic<bool>& signal;

        SignalingProblemsList(const Workspace& workspace, std::atomic<bool>& signal)
            : ProblemsList(workspace), signal(signal)
        {}

        ~SignalingProblemsList() override
        {
            signal = true;
        }
    };

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_async.cpp").string();
    Workspace workspace(std::move(file));
    auto analyzer = Analyzer::Create(TestBaseClass::GetParser(), workspace);

    std::atomic<bool> signal = false;

    {
        auto problemsList = std::make_shared<SignalingProblemsList>(workspace, signal);
        analyzer->AnalyzeAsync(workspace.GetCompilerOptions(), problemsList,
                               MakeSharedNoDelete(myTestErrorCheckConsumer));
    }

    while (!signal) {
        std::this_thread::yield();
    }

    // No checks - just making sure analysis didn't crash
}

// COODDY_SUPPRESS LargeStackAllocationChecker
TEST_F(AnalyzerToolTest, OutReporterSmokeTest)
{
    TempDir dir("html_traces");
    TempFile result("res.log");
    TempFile result2("res2.log");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_reporters.cpp").string();
    Workspace workspace(std::move(file));
    std::vector<std::string> reps{"out"};

    // Test by using CompositeReporters
    CompositeReporter problemsList(reps, workspace);
    problemsList.Init(TestBaseClass::GetParser(), current_path().string(), 0);
    testing::internal::CaptureStdout();
    TestAnalyze(workspace, problemsList);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_FALSE(output.empty());

    // Test by explicit reporter without html reporter
    OutReporter problemList2(workspace);
    problemList2.Init(TestBaseClass::GetParser(), "res", 0);
    TestAnalyze(workspace, problemList2);
    ASSERT_TRUE(result.Exists());

    // Test by explicit reporter with html reporter
    OutReporter problemList3(workspace);
    problemList3.Init(TestBaseClass::GetParser(), "res2", Reporter::InitFlags::HTML_REPORT);
    TestAnalyze(workspace, problemList3);
    ASSERT_TRUE(result2.Exists());
}

TEST_F(AnalyzerToolTest, ParseErrorLogTest)
{
    TempFile result("parse_errors.csv");
    TempFile resultHtml("cooddy_result.html");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_parse_error_log.cpp").string();
    Workspace workspace(std::move(file));
    auto parser = Parser::Create();
    parser->statistics.maxFatalErrorCount = 10;
    auto analyzer = Analyzer::Create(*parser, workspace);
    CompositeReporter problemsList({"html"}, workspace);
    problemsList.Init(*parser, current_path().string(), Reporter::HTML_REPORT | Reporter::PARSE_ERRORS_IN_REPORT);
    problemsList.SetParseErrorsPath(".");
    TestErrorCheckConsumer testConsumer;
    analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, testConsumer);
    ASSERT_EQ(parser->statistics.compilationIssues.size(), 1);
    ASSERT_EQ(parser->statistics.compilationIssues.begin()->second.errors.size(), 2);
    ASSERT_TRUE(testConsumer.HasToolError());
    ASSERT_FALSE(result.IsEmpty());
}

TEST_F(AnalyzerToolTest, ParseErrorLogTest2)
{
    auto dirPath = TEST_SUITE_PATH("analyzerToolTest") / "AnalyzerToolTest.ParseErrorLogTest2";
    TempDir dir(dirPath);
    TempFile result(dirPath / "parse_errors.json");

    create_directory(dirPath);
    auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_parse_error_log.cpp").string();
    Workspace workspace(std::move(file));
    auto parser = Parser::Create();
    parser->statistics.maxFatalErrorCount = 10;
    auto analyzer = Analyzer::Create(*parser, workspace);
    OutReporter problemsList(workspace);
    problemsList.Init(*parser, current_path().string(), Reporter::PARSE_ERRORS_IN_REPORT);
    problemsList.SetParseErrorsPath(result);
    TestErrorCheckConsumer testConsumer;
    analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, testConsumer);
    ASSERT_TRUE(testConsumer.HasToolError());
    ASSERT_EQ(parser->statistics.compilationIssues.size(), 1);
    ASSERT_EQ(parser->statistics.compilationIssues.begin()->second.errors.size(), 2);
    ASSERT_FALSE(result.IsEmpty());
}

TEST_F(AnalyzerToolTest, ParseErrorLogTest3)
{
    struct TestReporter : public Reporter {
        TestReporter(Workspace& workspace) : Reporter(workspace, ""){};

        void Flush() override
        {
            for (auto& issue : GetParseInfo().compilationIssues) {
                if (StrUtils::EndsWith(issue.unitFile, "boo.cpp")) {
                    ASSERT_EQ(issue.reason, "compiler not found");
                } else if (StrUtils::EndsWith(issue.unitFile, "foo.cpp")) {
                    ASSERT_EQ(issue.reason, "header file not found");
                } else if (StrUtils::EndsWith(issue.unitFile, "goo.c")) {
                    ASSERT_EQ(issue.reason, "compilation issue");
                } else if (StrUtils::EndsWith(issue.unitFile, "wrong_file.c")) {
                    ASSERT_EQ(issue.reason, "source file not found");
                }
            }
        }
    };

    Workspace workspace(TEST_SUITE_PATH("analyzerToolTest").append("parseErrors").string());
    auto parser = Parser::Create();
    parser->statistics.maxFatalErrorCount = 10;
    auto analyzer = Analyzer::Create(*parser, workspace);
    TestReporter problemsList(workspace);
    problemsList.Init(*parser, ".", Reporter::PARSE_ERRORS_IN_REPORT);
    TestErrorCheckConsumer testConsumer;
    analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, testConsumer);
    ASSERT_EQ(parser->statistics.compilationIssues.size(), 4);
}

TEST_F(AnalyzerToolTest, EntryFuncTest)
{
    TempFile result("call_graph.csv");

    auto file = TEST_SUITE_PATH("analyzerToolTest").append("entryFunc").string();
    Workspace workspace(std::move(file), {}, {}, "cwe");

    ProblemsList problemsList(workspace);
    auto analyzer = HCXX::Analyzer::Create(TestBaseClass::GetParser(), workspace);
    {
        EntryFuncConsumer consumer;
        analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, consumer);
        consumer.StoreCallGraph("call_graph.csv");
        ASSERT_TRUE(result.Exists());
    }
    EntryFuncConsumer consumer;
    ASSERT_FALSE(consumer.SetEntryFunc("goo", "__call_graph.csv"));
    ASSERT_FALSE(consumer.SetEntryFunc("__goo", "call_graph.csv"));
    myTestErrorCheckConsumer.ClearToolError();

    ASSERT_TRUE(consumer.SetEntryFunc("goo", "call_graph.csv"));
    analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, consumer);
    ASSERT_EQ(problemsList.GetProblems().size(), 1);
    {
        EntryFuncConsumer consumerAll;
        ASSERT_TRUE(consumerAll.SetEntryFunc("", "call_graph.csv"));
        ProblemsList problems(workspace);
        analyzer->Analyze(workspace.GetCompilerOptions(), problems, consumerAll);
        ASSERT_EQ(problems.GetProblems().size(), 3);
    }
    {
        EntryFuncConsumer consumerLine;
        ASSERT_TRUE(consumerLine.SetEntryFunc("boo", "call_graph.csv", 5));
        ProblemsList problems(workspace);
        analyzer->Analyze(workspace.GetCompilerOptions(), problems, consumerLine);
        ASSERT_EQ(problems.GetProblems().size(), 1);
    }
}

void PrepareUnknownArgTest(std::vector<const char*> argv)
{
    auto options = ConstructOptions();
    auto args = options.parse(argv.size(), argv.data());

    ReportUnknownArgs(args);
}

TEST_F(AnalyzerToolTest, UnknownArgTest)
{
    PrepareUnknownArgTest({"cooddy.exe", "--scope", "test.cpp", "helloworld"});

    ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError("Unexpected positional argument ignored"));
    myTestErrorCheckConsumer.ClearToolError();
}

TEST_F(AnalyzerToolTest, UnknownArgLongDashTest)
{
    PrepareUnknownArgTest({"cooddy.exe", "--scope", "test.cpp", "–checkers=NullPtrDereferenceChecker"});

    ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError("Unexpected positional argument ignored"));
    ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError("this argument appears to start with a long dash"));
    myTestErrorCheckConsumer.ClearToolError();
}

TEST_F(AnalyzerToolTest, NonExistingReportPathTest)
{
    std::filesystem::remove_all(current_path().string() + "/non");
    {
        string path = current_path().string() + "/non/existing/report/path/";
        TempDir dir(path + "html_traces");
        TempFile result(path + "cooddy_result.csv");
        ASSERT_FALSE(std::filesystem::exists(result));
        ASSERT_FALSE(std::filesystem::exists(dir));
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("test_trace.cpp").string();
        Workspace workspace(std::move(file));
        auto parser = Parser::Create();
        parser->statistics.maxFatalErrorCount = 10;
        auto analyzer = Analyzer::Create(*parser, workspace);
        CompositeReporter problemsList({"csv-html"}, workspace);
        problemsList.Init(*parser, path, Reporter::HTML_REPORT);
        TestErrorCheckConsumer testConsumer;
        analyzer->Analyze(workspace.GetCompilerOptions(), problemsList, testConsumer);
        ASSERT_TRUE(std::filesystem::exists(result));
        ASSERT_TRUE(std::filesystem::exists(dir));
    }
    std::filesystem::remove_all(current_path().string() + "/non");
}

TEST_F(AnalyzerToolTest, SensiInfoVarsTest)
{
    TempFile result("cooddy_result.csv");
    {
        auto file = TEST_SUITE_PATH("analyzerToolTest").append("SensiInfoTest.cpp").string();
        WorkspaceOptions ops;
        Workspace workspace(std::move(file), {}, ops, "cwe");
        workspace.SetConfiguration("SensitiveDataExposureChecker", jsoncpp::to_string(SensitiveDataSettings{true}));

        CsvReporter problemsList(workspace);
        problemsList.Init(TestBaseClass::GetParser(), current_path().string());
        TestAnalyze(workspace, problemsList);
    }
    auto content = result.GetText();
    ASSERT_TRUE(
        content.find(
            "10,5,c/c++,NotAnIssue,,IssuesFound,SENSI_INFO char* pwd,Annotated by macro as sensitive data source") !=
        std::string::npos);
    ASSERT_TRUE(
        content.find(
            "8,5,c/c++,NotAnIssue,,NoIssues,SENSI_INFO char* usr = cc,Annotated by macro as sensitive data source") !=
        std::string::npos);
}
