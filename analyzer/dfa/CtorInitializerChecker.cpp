/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxCtorInitializerDecl.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class CtorInitializerChecker : public BuildInDfaChecker {
private:
    void InitState(DfaState& state)
    {
        auto initDecl = state.GetNodeAs<CxxCtorInitializerDecl>();
        if (initDecl == nullptr || initDecl->GetInitializer() == nullptr) {
            return;
        }
        auto& initializerState = state.GetFuncState().GetState(initDecl->GetInitializer());
        for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
            Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
            if (Annotation::IsPropagatedByDecl(annotation)) {
                state.CopyAnnotation(annotation, initializerState);
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateCtorInitializerChecker()
{
    return std::make_unique<CtorInitializerChecker>();
}
