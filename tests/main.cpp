/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "Checker.h"
#include "CheckerTest.h"
#include "ProjectPaths.h"
#include "TestStream.h"

THREAD_LOCAL TestErrorCheckConsumer* TestErrorCheckConsumer::myCurrentTest = nullptr;

extern void InitTests();
TestStream g_testStream;

int main(int argc, char** argv)
{
    if (!HCXX::StrLocales::LoadLocale("EN")) {
        return 1;
    }
    HCXX::Logger::SetStream(g_testStream);
    ProjectPaths::Init(argv[0]);
    std::cout << argv[0] << std::endl;
    InitTests();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
