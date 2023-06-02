/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_WORKSPACE_EXCLUSIONS_H_
#define COODDY_ANALYZER_INCLUDE_WORKSPACE_EXCLUSIONS_H_

#include <ProblemsHolder.h>

#include <string>
#include <unordered_set>

namespace HCXX {

class ExclusionManager {
public:
    ExclusionManager(const std::string& exclusionsFile,
                     const Problem::Severity& reportedSeverity = Problem::Severity::ERROR);
    // LCOV_EXCL_START
    ~ExclusionManager() = default;
    // LCOV_EXCL_STOP

    bool IsProblemExcluded(const Problem& problem) const;

private:
    std::vector<std::string> myPathExclusions;
    Problem::Severity myProblemSeverityLevel;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_WORKSPACE_EXCLUSIONS_H_
