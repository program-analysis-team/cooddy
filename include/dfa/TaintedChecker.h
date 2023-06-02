/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_TAINTEDCHECKER_H_
#define COODDY_ANALYZER_INCLUDE_DFA_TAINTEDCHECKER_H_

#include <dfa/DfaChecker.h>

namespace HCXX {

class TaintedChecker : public DfaChecker {
protected:
    Annotation::Kind myUntrustedSourceKind;

    TaintedChecker()
    {
        myUntrustedSourceKind = Annotation::GetKind("UntrustedSource");
    }

    void CheckDeclaration(DfaState& state, ProblemsHolder& holder) override
    {
        // Check state to pick up untrusted source annotations from function parameters.
        if (state.GetNodeAs<ParamVarDecl>() != nullptr) {
            CheckState(state, holder);
        }
    }
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_TAINTEDCHECKER_H_
