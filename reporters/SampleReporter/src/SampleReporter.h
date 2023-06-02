/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SAMPLEREPORTER_H
#define COODDY_SAMPLEREPORTER_H

#include "reporters/Reporter.h"

namespace HCXX {
class SampleReporter : public Reporter {
public:
    explicit SampleReporter(const Workspace& workspace);

    void Init(Parser& parser, const std::string& resultPath, uint32_t initFlags = 0) override;

    void RegisterProblemImpl(const Problem& problem) override;
};
}  // namespace HCXX

#endif  // COODDY_SAMPLEREPORTER_H
