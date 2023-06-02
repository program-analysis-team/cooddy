/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#ifndef COODDY_ANALYZER_INCLUDE_OUTREPORTER_H_
#define COODDY_ANALYZER_INCLUDE_OUTREPORTER_H_

#include "Reporter.h"

namespace HCXX {
class OutReporter : public Reporter {
public:
    explicit OutReporter(const Workspace& workspace);

    void Init(Parser& parser, const std::string& resultPath, uint32_t initFlags = 0) override;

    template <class TLog>
    void LogProblem(TLog& log, const Problem& problem);

    void RegisterProblemImpl(const Problem& problem) override;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_OUTREPORTER_H_
