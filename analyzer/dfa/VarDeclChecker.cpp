/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxConstructExpression.h>
#include <ast/VarDecl.h>
#include <dfa/BuildInDfaChecker.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class VarDeclChecker : public BuildInDfaChecker {
private:
    void InitState(DfaState& state)
    {
        auto varDecl = state.GetNodeAs<VarDecl>();
        if (varDecl == nullptr) {
            return;
        }

        if (auto initializer = varDecl->GetInitializer(); initializer != nullptr) {
            auto& initializerState = state.GetFuncState().GetState(initializer->GetInnerNode());
            for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
                Annotation::Kind annotation = static_cast<Annotation::Kind>(i);
                if (Annotation::IsPropagatedByDecl(annotation)) {
                    for (auto& it : state.GetAnnotationSources(annotation)) {
                        initializerState.GetSubState(it.first.GetMemoryOffset()).Annotate(it.first, *it.second);
                    }
                }
            }
        }

        state.ClearSubAnnotations();

        varDecl->SetCfgBlock(state.GetFuncState().GetCfgBlock().GetPos());
    }

    void ExitState(DfaState& state) override
    {
        if (state.GetNodeAs<VarDecl>() != nullptr) {
            for (int i = 0; i < Annotation::GetAnnotationsCount(); i++) {
                state.ClearAnnotation(i);
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateVarDeclChecker()
{
    return std::make_unique<VarDeclChecker>();
}
