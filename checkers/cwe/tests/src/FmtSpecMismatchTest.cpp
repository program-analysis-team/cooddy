/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Analyzer.h>
#include <Parser.h>
#include <ProblemsList.h>
#include <utils/Log.h>

#include <string>
#include <string_view>

#include "ProjectPaths.h"
#include "TestBaseClass.h"

using namespace HCXX;

struct FmtSpecMismatchTest : public TestBaseClass {};

TEST_F(FmtSpecMismatchTest, BadAnnotationsTest)
{
    Workspace workspace(TEST_SUITE_PATH("FmtSpecMismatch/foo.cpp").string());

    HCXX::ProblemsList problemsList(workspace);
    TestAnalyze(workspace, problemsList);

    ASSERT_TRUE(myTestErrorCheckConsumer.ContainsToolError(
        "contains a \"FormatStringArgs\" annotation in an invalid position"));
    myTestErrorCheckConsumer.ClearToolError();
}
