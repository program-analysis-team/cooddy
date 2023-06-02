/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TESTCASEPARAM_H
#define COODDY_TESTCASEPARAM_H

#include "TestErrorCheckConsumer.h"
#include "dfa/TaintSettings.h"
#include "reporters/JsonReporter.h"

struct ProblemDescription {
    std::string description;
    std::string kind;
    std::string reasonId;
    std::string sourceId;
    std::string filePath;
    std::string sinkFunction;
    uint32_t line = -1;
    uint32_t column = -1;
    uint32_t endColumn = -1;
    HCXX::ProblemTrace tracePath;
    std::string severity;
    bool isAllowParseError = false;
    bool isAllowToolError = false;

    template <class X>
    // TODO HCAT-513
    // COODDY_SUPPRESS CamelCaseChecker
    void serialize(jsoncpp::Stream<X>& stream)
    {
        jsoncpp::fields(*this, stream, "kind", kind, "description", description, "line", line, "pos", column, "pos_end",
                        endColumn, "file_path", filePath, "trace", tracePath, "allowed_parse_error", isAllowParseError,
                        "allowed_tool_error", isAllowToolError, "reason", reasonId, "source", sourceId, "sink_function",
                        sinkFunction, "severity", severity);
    }
};

struct TestCaseOptions {
    std::string testId;
    std::string testcasePurpose;
    std::string testcaseDirectory;
    std::string compilerFlags;
    std::string taintOptions;
    std::string profile;
    std::vector<std::string> os;
    bool isPositive = true;
    bool isSkipped = false;
    ProblemDescription problem;
    std::vector<ProblemDescription> problems;

    template <class X>
    // TODO HCAT-513
    // COODDY_SUPPRESS CamelCaseChecker
    void serialize(jsoncpp::Stream<X>& stream)
    {
        jsoncpp::fields(*this, stream, "test_id", testId, "testcase_purpose", testcasePurpose, "testcase_goal",
                        testcasePurpose, "taint_options", taintOptions, "testcase_directory", testcaseDirectory, "kind",
                        problem.kind, "description", problem.description, "line", problem.line, "pos", problem.column,
                        "pos_end", problem.endColumn, "positive", isPositive, "skipped", isSkipped, "compiler_flags",
                        compilerFlags, "file_path", problem.filePath, "os", os, "problems", problems, "trace",
                        problem.tracePath, "allowed_parse_error", problem.isAllowParseError, "allowed_tool_error",
                        problem.isAllowToolError, "reason", problem.reasonId, "source", problem.sourceId,
                        "sink_function", problem.sinkFunction, "severity", problem.severity, "profile", profile);
    }
};

class TestCaseParam : public HCXX::ThreadPool::Task, public TestErrorCheckConsumer {
public:
    TestCaseParam(std::string&& checkerName, std::string&& optionsPath)
        : checkerName(checkerName), optionsPath(optionsPath)
    {}

    std::string checkerName;
    std::string optionsPath;
    TestCaseOptions options;
    std::promise<void> promise;
    std::unique_ptr<HCXX::ProblemsList> problems;
    std::string jsonErrorDescription;
    std::filesystem::path scopeFile;

    void Start() override
    {
        CheckerTest::Start(*this);
        promise.set_value();
    }

    /// Output operator for more informative errors by Google Test
    friend std::ostream& operator<<(std::ostream& os, const TestCaseParam& obj)
    {
        // Smaller output that fits on one line in terminal
        using namespace std::filesystem;
        std::string testfile = relative(obj.optionsPath).string();
        // Use universal newlines for uniform output on all platforms
        std::replace(testfile.begin(), testfile.end(), '\\', '/');

        os << testfile;
        return os;
    }
};
#endif  // COODDY_TESTCASEPARAM_H
