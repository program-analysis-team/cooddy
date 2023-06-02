/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_TIMER_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_TIMER_H_

#include <atomic>
#include <chrono>

namespace HCXX {

class Timer {
public:
    Timer()
    {
        myStartTime = std::chrono::high_resolution_clock::now();
    }
    ~Timer() = default;

    void Flush(std::atomic<uint64_t>& timer)
    {
        auto curTime = std::chrono::high_resolution_clock::now();
        timer += (curTime - myStartTime).count();
        myStartTime = curTime;
    }

    uint64_t Duration()
    {
        return (std::chrono::high_resolution_clock::now() - myStartTime).count();
    }

    static double Seconds(uint64_t duration)
    {
        return duration / 1e9;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> myStartTime;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_TIMER_H_
