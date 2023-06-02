/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CheckersFactory.h>

DECLARE_CHECKER_BEGIN(Sample)
DECLARE_CHECKER(SampleAstChecker)
DECLARE_CHECKER(SampleDfaChecker)
DECLARE_CHECKER_END()
