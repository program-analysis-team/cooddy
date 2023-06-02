/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SARIFREPORTER_H
#define COODDY_SARIFREPORTER_H

#include <deps/json.hpp>

#include "reporters/Reporter.h"

namespace HCXX {
class SarifReporter : public Reporter {
public:
    explicit SarifReporter(const Workspace& workspace);
    void RegisterProblemImpl(const Problem& problem) override;

    virtual ~SarifReporter();

private:
    nlohmann::ordered_json report;
    nlohmann::ordered_json& problems;
};
}  // namespace HCXX

#endif  // COODDY_SARIFREPORTER_H
