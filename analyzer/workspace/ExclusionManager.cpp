/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <utils/Log.h>
#include <utils/json-cpp.h>
#include <workspace/ExclusionManager.h>

#include <fstream>

namespace HCXX {

ExclusionManager::ExclusionManager(const std::string& exclusionsFile, const Problem::Severity& reportedSeverity)
    : myProblemSeverityLevel(reportedSeverity)
{
    if (!exclusionsFile.empty()) {
        myPathExclusions = {"/third_party/", "/open_source/"};
        std::ifstream exclusionsStream(exclusionsFile);
        for (std::string sLine; std::getline(exclusionsStream, sLine);) {
            myPathExclusions.emplace_back(sLine);
        }
    }
}

bool ExclusionManager::IsProblemExcluded(const Problem& problem) const
{
    for (auto& it : myPathExclusions) {
        if (problem.filename.find(it) != std::string::npos) {
            return true;  // LCOV_EXCL_LINE
        }
    }
    if (myProblemSeverityLevel < problem.severity) {
        return true;
    }

    return false;
}

}  // namespace HCXX
