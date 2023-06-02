/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_BIT_H_
#define COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_BIT_H_
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

/**
 * @brief Count bits up to and including the highest set bit.
 *
 * Replace with std::bit_width under C++20.
 */
constexpr int BitWidth(uint64_t val)
{
    return val == 0 ? 0 : (64 - __builtin_clzll(val));
}

/**
 * @brief Count consecutive zero bits in a value, starting from the rightmost (lowest).
 *
 * Replace with std::countr_zero under C++20.
 */
template <typename T>
constexpr int CountRZero(T val)
{
    static_assert(std::is_integral_v<T>);

    if constexpr (std::is_same_v<T, uint64_t>) {
        // __builtin_ffsll will overflow with uint64
        int64_t alt = 0;
        std::memcpy(&alt, &val, 8);

        return CountRZero(alt);
    }

    return val == 0 ? sizeof(T) * 8 : __builtin_ffsll(val) - 1;
}

#endif  // COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_BIT_H_
