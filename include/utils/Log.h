/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_LOG_H
#define COODDY_LOG_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>

namespace HCXX {

enum class LogLevel {
    OFF,
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE,
};

class Logger {
public:
    Logger(LogLevel level, std::ostream& stream = std::cout) : myStream(&stream), myLevel(level) {}

    static void SetLevel(LogLevel level)
    {
        myInstance.myLevel = level;
    }

    static LogLevel GetLevel()
    {
        return myInstance.myLevel;
    }

    static void SetStream(std::ostream& stream)
    {
        myInstance.myStream = &stream;
    }

private:
    void Log(const char* msg, LogLevel level);
    void LogToStream(std::ostream& out, const char* msg, LogLevel level);
    static Logger myInstance;

    friend class Log;
    std::mutex myMutex;
    std::ostream* myStream;
    LogLevel myLevel;
};

class Log {
public:
    explicit Log(LogLevel level) : myLevel(level) {}

    ~Log()
    {
        if (myLevel <= Logger::myInstance.myLevel) {
            Logger::myInstance.Log(myStream.str().c_str(), myLevel);
        }
    }

    template <typename T>
    Log& operator<<(const T& s)
    {
        if (myLevel <= Logger::myInstance.myLevel) {
            myStream << s;
        }
        return *this;
    }

    Log& operator<<(std::ostream& (*var)(std::ostream&))
    {
        if (myLevel <= Logger::myInstance.myLevel) {
            var(myStream);
        }
        return *this;
    }

private:
    std::stringstream myStream;
    LogLevel myLevel;
};

}  // namespace HCXX
#endif  // COODDY_LOG_H
