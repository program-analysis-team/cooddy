/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_LOG_H
#define COODDY_LOG_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>

namespace HCXX {

enum class LogLevel {
    OFF = 0,
    FATAL = 1,
    PARSE_ERROR = 2,
    ERROR = 3,
    WARNING = 4,
    INFO = 5,
    DEBUG = 6,
    TRACE = 7,
};

class Logger {
public:
    Logger(LogLevel level, std::ostream& stream = std::cout) : myStream(&stream), myLevel(level) {}

    static void SetLevel(LogLevel level)
    {
        myInstance.myLevel = level;
    }

    static void SetParseErrorFile(const std::string& parseErrorFile)
    {
        std::filesystem::path resultsPath = parseErrorFile;

        if (std::filesystem::is_directory(resultsPath)) {
            resultsPath.append("cooddy_parse_errors.log");
        }

        myInstance.myFileStream.open(resultsPath);
    }

    static LogLevel GetLevel()
    {
        return myInstance.myLevel;
    }

    static void SetStream(std::ostream& stream)
    {
        myInstance.myStream = &stream;
    }

    static void CloseFileStream()
    {
        if (myInstance.myFileStream.is_open()) {
            myInstance.myFileStream.close();
        }
    }

private:
    void Log(const char* msg, LogLevel level);
    void LogToStream(std::ostream& out, const char* msg, LogLevel level);
    static Logger myInstance;

    friend class Log;
    std::mutex myMutex;
    std::ostream* myStream;
    std::ofstream myFileStream;
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
