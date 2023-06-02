/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <solver/SolverContext.h>
#include <utils/StrUtils.h>

#include <array>
#include <cctype>
#include <string>

using namespace HCXX;

static constexpr std::array<std::string_view, 7> g_SIGNS = {"none", "==", "!=", ">", "<=", ">=", "<"};

void Condition::FromString(std::string_view str)
{
    for (size_t i = 0; i < g_SIGNS.size(); ++i) {
        std::string_view sign = g_SIGNS[i];
        if (StrUtils::StartsWith(str, sign) && std::isdigit(str[sign.size()])) {
            operation = static_cast<Operation>(i);
            operand = StrUtils::ToInteger<int32_t>(str.substr(sign.size()));
            break;
        }
    }
}
// LCOV_EXCL_START
std::string Condition::ToString() const
{
    return std::string{g_SIGNS[static_cast<size_t>(operation)]} + std::to_string(operand);
}
// LCOV_EXCL_STOP
