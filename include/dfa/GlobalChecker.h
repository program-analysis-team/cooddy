/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_GLOBALCHECKER_H_
#define COODDY_ANALYZER_INCLUDE_DFA_GLOBALCHECKER_H_

#include <dfa/DfaChecker.h>

namespace HCXX {

class GlobalChecker : public DfaChecker {
    Kind GetKind() override
    {
        return Kind::GLOBAL_CHECKER;
    }

public:
    // This method allows to load/store checker's data in persistent storage
    virtual void Serialize(const TranslationUnit& unit, IOStream& stream) = 0;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_GLOBALCHECKER_H_
