/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_WORKSPACE_INCREMENTALSETTINGS_H_
#define COODDY_ANALYZER_INCLUDE_WORKSPACE_INCREMENTALSETTINGS_H_

#include <utils/EnumMapping.h>
#include <utils/Log.h>

namespace HCXX {

struct IncrementalSettings {
    bool readMode = false;
    bool writeMode = false;
    bool reportNewProblems = false;

    void FromCliArg(std::string_view cliArg)
    {
        if (cliArg == "off") {
            return;
        }
        for (auto c : cliArg) {
            switch (c) {
                case 'n':
                    reportNewProblems = true;
                case 'b':
                    readMode = true;
                    break;
                case 'u':
                    writeMode = true;
                    break;
                    // LCOV_EXCL_START
                default:
                    Log(HCXX::LogLevel::ERROR)
                        << "Invalid value \"" << cliArg << "\" for command-line parameter 'incremental'\n";
                    return;
                    // LCOV_EXCL_STOP
            }
        }
    }
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_WORKSPACE_INCREMENTALSETTINGS_H_
