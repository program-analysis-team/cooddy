/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CheckerTest.h"

#include <algorithm>
#include <filesystem>
#include <memory>

#include "Analyzer.h"
#include "CheckersFactory.h"
#include "CompilerOptionsList.h"
#include "Parser.h"
#include "ProblemsList.h"
#include "TestBaseClass.h"
#include "TestCaseParam.h"
#include "dfa/TaintSettings.h"
#include "utils/LocaleUtils.h"
#include "utils/StrUtils.h"
#include "workspace/CompileCommand.h"

HCXX::ThreadPool CheckerTest::myThreadPool;

#if defined(_WIN32)
const std::string CheckerTest::CURRENT_OS = "Windows";
#elif defined(__linux__)
const std::string CheckerTest::CURRENT_OS = "Linux";
#else
const std::string CheckerTest::CURRENT_OS = "Undefined";
#endif

void PrintTo(const TestCaseParamPtr& obj, std::ostream* os)
{
    if (obj == nullptr) {
        *os << "<nullptr>";
    } else {
        *os << (*obj);
    }
}

std::vector<TestCaseParamPtr> CheckerTest::GetTestCases(const char* testFile, const char* checkerClass)
{
#ifndef _MSC_VER
    std::string filter = ::testing::GTEST_FLAG(filter);
    if (filter != "*" && filter.find(checkerClass) == std::string::npos) {
        return std::vector<TestCaseParamPtr>();
    }
#endif
    std::vector<TestCaseParamPtr> result;
    std::filesystem::path checkerSuit(ProjectPaths::GetTestCasesPath(testFile, checkerClass));

    ::HCXX::Test::FileSystem::ForEachFileInDirectory(
        checkerSuit, ".json", [&result, checkerClass](const std::filesystem::path& entry) {
            auto param = std::make_shared<TestCaseParam>(checkerClass, entry.generic_u8string());
            myThreadPool.AddTask(param);
            result.emplace_back(param);
        });

    return result;
}

bool CheckerTest::IsRunTestForCurrentOS(const std::vector<std::string>& os)
{
    return os.empty() || find(os.begin(), os.end(), CURRENT_OS) != os.end();
}

/// compare description taking into account some variable part of description marked as {*} in test case
bool CompareDescription(const std::string& testDesc, const std::string& problemDesc)
{
    auto p = testDesc.find("{*}");
    if (p == std::string::npos) {
        return testDesc == problemDesc;
    }
    if (testDesc.compare(0, p, problemDesc, 0, p) != 0) {
        return false;
    }
    auto tailLen = testDesc.size() - p - 3 /*size of "{*}*/;
    return testDesc.compare(testDesc.size() - tailLen, std::string::npos, problemDesc, problemDesc.size() - tailLen,
                            std::string::npos) == 0;
}

bool IsProblemMatchesTestcase(const HCXX::Problem& problem, const ProblemDescription& problemDesc,
                              std::filesystem::path& cppName)
{
    if (!problemDesc.filePath.empty()) {
        if (std::filesystem::path(problemDesc.filePath).filename() !=
            std::filesystem::path(problem.filename).filename()) {
            return false;
        }
    } else {
        if (std::filesystem::path(problem.filename).filename() != cppName) {
            return false;
        }
    }
    return (problem.line == problemDesc.line && problem.column == problemDesc.column &&
            CompareDescription(problemDesc.description, problem.description) && problem.kind == problemDesc.kind);
}

// COODDY_SUPPRESS FunctionLengthChecker
void CheckerTest::Start(TestCaseParam& param)
{
    TestErrorCheckConsumer::SetAsCurrent(&param);

    const std::string& optionsPath = param.optionsPath;
    if (optionsPath.find(HCXX::Workspace::GetWorkspaceFolderName()) != std::string::npos ||
        HCXX::StrUtils::EndsWith(std::string(optionsPath), COMPILE_COMMAND_DB)) {
        // ignore jsons from project profile
        return;
    }

    TestCaseOptions& testOptions = param.options;
    try {
        std::ifstream fileStream(optionsPath);
        jsoncpp::parse(testOptions, fileStream);
    } catch (jsoncpp::ParserError& error) {
        param.jsonErrorDescription = "JSON parse error: " + std::string(error.what());
        return;
    }

    if (!IsRunTestForCurrentOS(testOptions.os)) {
        return;
    }

    vector<string> extraCompilerFlags;
    if (!testOptions.compilerFlags.empty()) {
        std::istringstream iss(testOptions.compilerFlags);
        extraCompilerFlags = {std::istream_iterator<string>{iss}, std::istream_iterator<string>{}};
    }

    std::filesystem::path scopePath = optionsPath;
    auto& parser = TestBaseClass::GetParser(&scopePath);

    if (testOptions.testcaseDirectory.empty() && testOptions.problem.filePath.empty()) {
        scopePath.replace_extension("cpp");
        if (!std::filesystem::exists(scopePath)) {
            scopePath.replace_extension("c");
        }
        if (!std::filesystem::exists(scopePath)) {
            scopePath.replace_extension(scopePath.parent_path().filename());
        }
        param.scopeFile = scopePath.filename().string();
    } else if (!testOptions.problem.filePath.empty() && testOptions.testcaseDirectory.empty()) {
        scopePath.replace_filename(testOptions.problem.filePath);
    } else {
        scopePath = scopePath.parent_path().append(testOptions.testcaseDirectory);
    }

    if (!scopePath.empty() && !std::filesystem::exists(scopePath)) {
        param.jsonErrorDescription = "Scope path for " + optionsPath + " doesn't exist";
        return;
    }

    HCXX::WorkspaceOptions options;
    options.problemSeverityLevel = HCXX::Problem::Severity::NOTIFY;
    HCXX::Workspace workspace(scopePath.generic_u8string(), {param.checkerName}, options, testOptions.profile);
    auto taintOp = param.options.taintOptions.empty() ? "su" : param.options.taintOptions;
    workspace.SetConfiguration("#UntrustedSourceChecker", jsoncpp::to_string(HCXX::TaintSettings{taintOp}));

    std::unique_ptr<HCXX::Analyzer> analyzer = HCXX::Analyzer::Create(parser, workspace);

    HCXX::CompilerOptionsList compilerOptions = workspace.GetCompilerOptions();
    compilerOptions.AddExtraOptions(extraCompilerFlags);

    param.problems = std::make_unique<HCXX::ProblemsList>(workspace);
    analyzer->Analyze(compilerOptions, *param.problems, param);
}

void CheckTraceAssert(const HCXX::Problem& problem, const ProblemDescription& testCaseOption)
{
    if (!testCaseOption.tracePath.empty()) {
        ASSERT_EQ(testCaseOption.tracePath.size(), problem.trace.size());
    }
    for (size_t i = 0; i < problem.trace.size(); i++) {
        auto& resultTraceElem = problem.trace[i];
        ASSERT_FALSE(resultTraceElem.description.empty());
        if (!testCaseOption.tracePath.empty()) {
            auto& testCaseTraceElem = testCaseOption.tracePath[i];
            if (!CompareDescription(testCaseTraceElem.desc, resultTraceElem.description)) {
                ASSERT_EQ(testCaseTraceElem.desc, resultTraceElem.description);
            }
            ASSERT_EQ(testCaseTraceElem.line, resultTraceElem.line);
            ASSERT_EQ(testCaseTraceElem.column, resultTraceElem.column);
            ASSERT_EQ(std::filesystem::path(testCaseTraceElem.file).filename(),
                      std::filesystem::path(resultTraceElem.filename).filename());
        }
    }
}

void CheckProblem(const ProblemDescription& testCaseProblem, HCXX::Problem& problem, std::filesystem::path& cppName,
                  TestCaseOptions& testOptions)
{
    // if there are no problem matches test case then show to user comparison with 1-st one.
    if (!testCaseProblem.filePath.empty()) {
        ASSERT_EQ(std::filesystem::path(testCaseProblem.filePath).filename(),
                  std::filesystem::path(problem.filename).filename());
    } else {
        ASSERT_EQ(std::filesystem::path(problem.filename).filename(), cppName);
    }
    if (testOptions.isSkipped) {
        return;
    }
    ASSERT_EQ(problem.line, testCaseProblem.line);
    ASSERT_EQ(problem.column, testCaseProblem.column);
    ASSERT_NE(problem.endColumn, UINT32_MAX);
    if (testCaseProblem.endColumn != -1) {
        ASSERT_EQ(problem.endColumn, testCaseProblem.endColumn);
    }
    if (!CompareDescription(testCaseProblem.description, problem.description)) {
        ASSERT_EQ(problem.description, testCaseProblem.description);
    }
    ASSERT_EQ(problem.kind, testCaseProblem.kind);
    ASSERT_TRUE(!problem.checkerName.empty());

    if (!testCaseProblem.reasonId.empty()) {
        ASSERT_EQ(problem.reasonCode, testCaseProblem.reasonId);
    }
    if (!testCaseProblem.sourceId.empty()) {
        ASSERT_TRUE(HCXX::StrUtils::MatchQualifiedName(problem.sourceId, testCaseProblem.sourceId));
    }
    if (!testCaseProblem.sinkFunction.empty()) {
        ASSERT_EQ(problem.sinkFunction, testCaseProblem.sinkFunction);
    }
    if (!testCaseProblem.severity.empty()) {
        ASSERT_EQ(HCXX::Problem::SeverityToCStr(problem.severity), testCaseProblem.severity);
    }

    CheckTraceAssert(problem, testCaseProblem);
}

void CheckTestCases(std::vector<HCXX::Problem>& problems, TestCaseOptions& testOptions, std::filesystem::path& cppName)
{
    for (auto& testCaseProblem : testOptions.problems) {
        if (testCaseProblem.description.empty()) {
            testCaseProblem.description = testOptions.problem.description;
        }
        if (testCaseProblem.kind.empty()) {
            testCaseProblem.kind = testOptions.problem.kind;
        }

        if (testCaseProblem.tracePath.empty()) {
            testCaseProblem.tracePath = testOptions.problem.tracePath;
        }

        ASSERT_TRUE(!problems.empty());

        auto it = std::find_if(problems.begin(), problems.end(), [&](const HCXX::Problem& p) {
            return IsProblemMatchesTestcase(p, testCaseProblem, cppName);
        });

        if (it != problems.end()) {
            CheckProblem(testCaseProblem, *it, cppName, testOptions);
            problems.erase(it);
            continue;
        }
        CheckProblem(testCaseProblem, problems[0], cppName, testOptions);
    }
}

void AnalyzeTestResults(TestCaseParam& param)
{
    TestCaseOptions& testOptions = param.options;
    HCXX::ProblemsList& problemsList = *param.problems;

    auto problems = problemsList.GetProblems();
    problems.erase(std::remove_if(problems.begin(), problems.end(),
                                  [](const HCXX::Problem& p) { return p.checkerName == "NotAnIssue"; }),
                   problems.end());

    if (testOptions.isSkipped) {
        testOptions.isPositive = !testOptions.isPositive;
    }
    if (!testOptions.isPositive) {
        ASSERT_EQ(problems.size(), 0);
        return;
    }

    if (testOptions.problems.empty()) {
        // if problems list is empty, add description from testOptions
        testOptions.problems.emplace_back(testOptions.problem);
    }
    ASSERT_EQ(problems.size(), testOptions.problems.size());
    CheckTestCases(problems, testOptions, param.scopeFile);
}

void CheckerTest::Test(TestCaseParam& param)
{
    if (!param.jsonErrorDescription.empty()) {
        std::cout << param.optionsPath << std::endl << param.jsonErrorDescription << std::endl;
        FAIL();
    }
    param.promise.get_future().wait();
    std::cout << param.optionsPath << std::endl;

    bool hasToolError = !param.options.problem.isAllowToolError && param.HasToolError();
    param.ClearToolError();

    if (hasToolError) {
        FAIL() << "Caught tool errors";
    }
    ASSERT_FALSE(hasToolError);

    if (!param.options.problem.isAllowParseError) {
        ASSERT_FALSE(param.HasParseError());
    }
    if (param.problems != nullptr) {
        AnalyzeTestResults(param);
    }
}
