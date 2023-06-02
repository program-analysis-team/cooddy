/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CHECKERTEST_H
#define COODDY_CHECKERTEST_H

#include <gtest/gtest.h>

#include "CheckersFactory.h"
#include "ProjectPaths.h"
#include "TestFileSystem.h"
#include "utils/ThreadPool.h"

class TestCaseParam;
using TestCaseParamPtr = std::shared_ptr<TestCaseParam>;

/// Custom output function for more informative errors by Google Test
void PrintTo(const TestCaseParamPtr& obj, std::ostream* os);

class CheckerTest : public testing::TestWithParam<TestCaseParamPtr> {
    static HCXX::ThreadPool myThreadPool;

    static const std::string CURRENT_OS;

    static bool IsRunTestForCurrentOS(const std::vector<std::string>& os);

public:
    static std::vector<TestCaseParamPtr> GetTestCases(const char* testFile, const char* checkerClass);

    static void Start(TestCaseParam& param);

    void Test(TestCaseParam& param);
};

#define DECLARE_CHECKER_TEST(TestFile, CheckerId, CheckerClass)     \
    struct CheckerClass##Test : public CheckerTest {};              \
    TEST_P(CheckerClass##Test, AnalyzeAsExpected)                   \
    {                                                               \
        Test(*GetParam());                                          \
    }                                                               \
    INSTANTIATE_TEST_SUITE_P(GeneralAndSpecial, CheckerClass##Test, \
                             testing::ValuesIn(CheckerTest::GetTestCases(TestFile, #CheckerId)));

#define DECLARE_TEST(CheckerId, CheckerClass)                                               \
    extern std::unique_ptr<HCXX::Checker> Create##CheckerClass();                           \
    void register##CheckerClass()                                                           \
    {                                                                                       \
        HCXX::CheckersFactory::GetInstance().Register(#CheckerClass, Create##CheckerClass); \
    }                                                                                       \
    DECLARE_CHECKER_TEST(__FILE__, CheckerId, CheckerClass)

#undef DECLARE_CHECKER_BEGIN
#define DECLARE_CHECKER_BEGIN(scope)

#undef DECLARE_CHECKER_END
#define DECLARE_CHECKER_END()

#undef DECLARE_CHECKER
#define DECLARE_CHECKER(CheckerClass) DECLARE_CHECKER_TEST(__FILE__, CheckerClass, CheckerClass)

/// This macro should be used for custom testing of existing checkers (cwe, unusedvars) to avoid test names duplication
#define DECLARE_CHECKER_EX(CheckerClass) DECLARE_CHECKER_TEST(__FILE__, CheckerClass, CheckerClass##Ex)

#endif  // COODDY_CHECKERTEST_H
