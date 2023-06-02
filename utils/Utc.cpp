/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "utils/Utc.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std::chrono_literals;
namespace ch = std::chrono;

namespace HCXX {

utc_clock::time_point utc_clock::now()
{
    // See https://en.wikipedia.org/wiki/Leap_second
    time_point time{base_clock::now().time_since_epoch()};
    return time - 37s;
}

std::ostream& operator<<(std::ostream& os, utc_clock::time_point time)
{
    utc_clock::base_clock::time_point base_clock_time{time.time_since_epoch()};
    std::time_t time_t_time = ch::system_clock::to_time_t(base_clock_time);

    os << std::put_time(std::gmtime(&time_t_time), "%Y-%m-%dT%H:%M:%S.")
       << (ch::duration_cast<ch::milliseconds>(base_clock_time.time_since_epoch()).count() % 1000) << "Z";
    return os;
}

std::string to_string(utc_clock::time_point time)
{
    std::ostringstream os;
    os << time;
    return os.str();
}

}  // namespace HCXX
