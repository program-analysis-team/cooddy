/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include <ast/BinaryExpression.h>
#include <ast/ImplicitValueInitExpression.h>
#include <ast/InitListExpression.h>
#include <ast/LiteralExpression.h>
#include <ast/MemberExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/TaintedChecker.h>
#include <solver/FunctionBehavior.h>

#include "ast/LoopStatement.h"

using namespace HCXX;

class SensitiveDataExposureChecker : public TaintedChecker {
    Annotation::Kind mySensitiveDataSourceKind;
    Annotation::Kind mySensitiveDataLeakKind;

public:
    SensitiveDataExposureChecker()
    {
        mySensitiveDataSourceKind = Annotation::RegisterKind("SensitiveDataSource", Annotation::PROPAGATED_BY_POINTER);
        mySensitiveDataLeakKind =
            Annotation::RegisterKind("SensitiveDataLeak", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation(mySensitiveDataSourceKind) && state.HasAnnotation(mySensitiveDataLeakKind)) {
            state.AddSuspiciousPath({*this,
                                     mySensitiveDataLeakKind,
                                     mySensitiveDataSourceKind,
                                     StrLocales::GetStringLocale("SENSITIVE_DATA_LEAK"),
                                     {},
                                     CheckPathParams::SINK_CAN_BE_BEFORE_SOURCE});
        }
    }

    void InitState(DfaState& state) override
    {
        auto node = state.GetNode();
        if (node != nullptr && node->GetType().IsSensitiveData()) {
            state.Annotate(mySensitiveDataSourceKind);
        }
    }

    bool OnSourceExecuted(const SourceExecInfo& sourceInfo) override
    {
        sourceInfo.context.AddUntrustedSourceByKind(sourceInfo.exprId, UntrustedSource::SourceKind::SENSITIVE_DATA);
        return true;
    }
};

std::unique_ptr<HCXX::Checker> CreateSensitiveDataExposureChecker()
{
    return std::make_unique<SensitiveDataExposureChecker>();
}
