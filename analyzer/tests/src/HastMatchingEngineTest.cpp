/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <HastMatchingEngine.h>

#include "TestBaseClass.h"

// The fixture for testing class HastMatchingEngineTest
class HastMatchingEngineTest : public TestBaseClass {};

TEST_F(HastMatchingEngineTest, DeletionOfIncorrectExpression)
{
    ::HCXX::HastMatchingEngine engine;
    ::HCXX::HastExpression* expresssion = nullptr;
    engine.DeleteExpression(expresssion);
    ++expresssion;
    engine.DeleteExpression(expresssion);
}
