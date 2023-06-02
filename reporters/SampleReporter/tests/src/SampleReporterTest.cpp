/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CheckerTest.h"
#include "TestBaseClass.h"
#include "reporters/CompositeReporter.h"
#include "utils/TempFile.h"

using namespace HCXX;

// The fixture for testing Sample reporter
class SampleReporterTest : public TestBaseClass {};

TEST_F(SampleReporterTest, UnknownReporterOutputTest)
{
    auto file = TEST_SUITE_PATH("SampleReporterTest").append("test_reporter.cpp").string();
    Workspace workspace(file);
    std::vector<std::string> reps{"unknownReport"};

    // Test by using CompositeReporters
    CompositeReporter compositeReporter(reps, workspace);
    compositeReporter.Init(TestBaseClass::GetParser(), "", 0);

    TestAnalyze(workspace, compositeReporter);
    ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError("Can't load reporter lib"));
    myTestErrorCheckConsumer.ClearToolError();
}

TEST_F(SampleReporterTest, SampleReporterOutputTest)
{
    TempFile result("cooddy_result.log");
    auto file = TEST_SUITE_PATH("SampleReporterTest").append("test_reporter.cpp").string();
    Workspace workspace(file);
    std::vector<std::string> reps{"sample"};

    // Test by using CompositeReporters
    CompositeReporter compositeReporter(reps, workspace);
    compositeReporter.Init(TestBaseClass::GetParser(), "cooddy_result.log", 0);

    testing::internal::CaptureStderr();
    TestAnalyze(workspace, compositeReporter);
    std::string output = testing::internal::GetCapturedStderr();
    ASSERT_FALSE(output.empty());
    ASSERT_TRUE(output.find("This is sample reporter") != std::string::npos);
    ASSERT_TRUE(result.IsEmpty());
}
