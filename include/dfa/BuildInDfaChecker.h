/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_BUILDINDFACHECKER_H_
#define COODDY_ANALYZER_INCLUDE_DFA_BUILDINDFACHECKER_H_

#include <Checker.h>
#include <dfa/DfaChecker.h>
#include <dfa/DfaState.h>
namespace HCXX {

class BuildInDfaChecker : public DfaChecker {
    const std::unordered_set<UnitLanguage> langs{UnitLanguage::C, UnitLanguage::CPP, UnitLanguage::JAVA};

    std::unordered_set<UnitLanguage> GetLangs() override
    {
        return langs;
    }
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_BUILDINDFACHECKER_H_
