/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SDK_INCLUDE_UTILS_UTC_H_
#define COODDY_SDK_INCLUDE_UTILS_UTC_H_
#include <chrono>
#include <ostream>
#include <string>

namespace HCXX {

// using std::chrono::utc_clock;  // C++20
class utc_clock {
private:
    // Pray to gods that system_clock has an epoch of 1970-01-01 because we have no way of knowing 🙏
    using base_clock = std::chrono::system_clock;

public:
    using rep = base_clock::rep;
    using period = base_clock::period;
    using duration = base_clock::duration;
    using time_point = std::chrono::time_point<utc_clock>;

    static constexpr bool is_steady = base_clock::is_steady;
    static time_point now();

    friend std::ostream& operator<<(std::ostream& os, time_point time);
    friend std::string to_string(time_point time);
};

}  // namespace HCXX

#endif  // COODDY_SDK_INCLUDE_UTILS_UTC_H_
