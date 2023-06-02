/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "utils/Log.h"

#include <ctime>
#include <iomanip>
#include <thread>

namespace HCXX {

Logger Logger::myInstance(LogLevel::WARNING);

void Logger::LogToStream(std::ostream& out, const char* msg, LogLevel level)
{
    static const char* levelStr[] = {"OFF", "FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE"};

    auto currentTime = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(currentTime);
    std::tm tm = *std::localtime(&t);
    uint32_t millis = currentTime.time_since_epoch().count() / 1000000 % 1000;

    std::lock_guard<std::mutex> lock(myMutex);
    out << '[' << std::put_time(&tm, "%H:%M:%S.") << millis << ']';
    out << '[' << std::this_thread::get_id() << ']';
    out << '[' << levelStr[static_cast<int>(level)] << "] ";
    out << msg;
    out.flush();
}

void Logger::Log(const char* msg, LogLevel level)
{
    LogToStream(*myStream, msg, level);
}

};  // namespace HCXX
