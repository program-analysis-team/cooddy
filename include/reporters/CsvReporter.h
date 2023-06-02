/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#ifndef COODDY_ANALYZER_INCLUDE_CSVREPORTER_H_
#define COODDY_ANALYZER_INCLUDE_CSVREPORTER_H_

#include <string>

#include "Reporter.h"

namespace HCXX {
class CsvReporter : public Reporter {
public:
    explicit CsvReporter(const Workspace& workspace);

    void Init(Parser& parser, const std::string& resultPath, uint32_t initFlags = 0) override;

    std::string ConvertProblemTrace(const TracePath& problemTrace);

    void RegisterProblemImpl(const Problem& problem) override;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_CSVREPORTER_H_
