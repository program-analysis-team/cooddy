/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_ENGINE_ANALYSISSTATISTICS_H_
#define COODDY_ANALYZER_SOURCE_ENGINE_ANALYSISSTATISTICS_H_

#include <utils/Log.h>

#include <iomanip>
#include <map>

class AnalysisStatistics {
public:
    struct Stat {
        const char* statName = nullptr;
        const CheckerStat* statData = nullptr;
    };
    using CheckerStatMap = std::map<uint64_t, Stat>;

    void AddCheckerStat(const char* statName, const CheckerStat& stat)
    {
        myMaxCheckerName = std::max(myMaxCheckerName, strlen(statName));
        myTotalStat.analysisTime += stat.analysisTime;
        myTotalStat.checkPathTime += stat.checkPathTime;
        myTotalStat.problemsFound += stat.problemsFound;
        myTotalStat.timeoutsCount += stat.timeoutsCount;
        myTotalStat.checkedPaths += stat.checkedPaths;
        myStatistics[stat.analysisTime + stat.checkPathTime] = {statName, &stat};
    }

    void ResetAnalysisTime()
    {
        myTotalStat.analysisTime = 0;
    }

    void PrintStatIfNotNull(std::stringstream& strm, const uint64_t val, const std::string& header)
    {
        if (val > 0) {
            strm << "\t\t* " << std::setw(30) << std::left << header << ": " << std::fixed << std::setw(6) << std::right
                 << val << std::endl;
        }
    }

    void LogStatistics(uint64_t runTime, uint64_t undefFuncCounter, HCXX::Parser::ParserStatistics& parserStatistics,
                       std::string_view manUrl, uint32_t topCount = 10)
    {
        Log(LogLevel::INFO) << "Total analysis time: " << std::fixed << std::setprecision(2) << std::setw(6)
                            << Timer::Seconds(runTime) << std::endl;
        uint64_t totalTime = myTotalStat.analysisTime + myTotalStat.checkPathTime;
        myStatistics[totalTime] = {"#Total", &myTotalStat};
        auto it = myStatistics.rbegin();
        for (uint32_t i = topCount + 1; it != myStatistics.rend() && it->first != 0 && i != 0; ++it, --i) {
            auto& stat = *it->second.statData;
            Log(LogLevel::INFO) << std::left << std::setw(myMaxCheckerName) << it->second.statName << ": " << std::right
                                << std::fixed << std::setprecision(2) << std::setw(6) << Timer::Seconds(it->first)
                                << " (analysis: " << std::setw(6) << Timer::Seconds(stat.analysisTime)
                                << ", solver: " << std::setw(6) << Timer::Seconds(stat.checkPathTime) << "("
                                << stat.checkedPaths << "), timeouts: " << stat.timeoutsCount
                                << ", problems: " << stat.problemsFound << ")\n";
        }
        std::stringstream stat;
        stat << std::endl
             << std::setw(120) << std::setfill('*') << '*' << std::endl
             << std::setfill(' ') << "Analysis statistic:\n";
        PrintStatIfNotNull(stat, parserStatistics.totalParsedCount, "Total files number");
        PrintStatIfNotNull(stat, parserStatistics.succeedCount, "Succeeded to compile");
        PrintStatIfNotNull(stat, parserStatistics.failedCount, "Failed to parse");
        PrintStatIfNotNull(stat, parserStatistics.partiallyParsedCount, "Partially parsed");
        PrintStatIfNotNull(stat, undefFuncCounter, "Undefined func num");
        PrintStatIfNotNull(stat, parserStatistics.failedCFGCount, "CFG failure Count");
        PrintStatIfNotNull(stat, GetTotalStat().timeoutsCount, "Total z3 timeouts num");
        PrintStatIfNotNull(stat, parserStatistics.unknownOptionsSet.size(), "Unknown compiler options num");
        if (!parserStatistics.unknownOptionsSet.empty()) {
            stat << "\t options are: "
                 << std::accumulate(parserStatistics.unknownOptionsSet.begin(),  // LCOV_EXCL_LINE
                                    parserStatistics.unknownOptionsSet.end(),    // LCOV_EXCL_LINE
                                    std::string(), [](const std::string& a, const std::string& b) -> std::string {
                                        return a + (a.length() > 0 ? ", " : "") + b;
                                    });
        }
        if (!manUrl.empty()) {
            stat << std::endl << "You can take a look here " << manUrl << " for additional info on this counters.";
        }
        stat << std::endl << std::setw(120) << std::setfill('*') << '*' << std::endl << std::setfill(' ') << std::endl;
        Log(LogLevel::INFO) << stat.str();
    }

    const CheckerStat& GetTotalStat() const
    {
        return myTotalStat;
    }

private:
    CheckerStatMap myStatistics;
    CheckerStat myTotalStat;
    size_t myMaxCheckerName = 0;
};

#endif  // COODDY_ANALYZER_SOURCE_ENGINE_ANALYSISSTATISTICS_H_
