/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <dfa/DfaChecker.h>

using namespace HCXX;

class StringSizeChecker : public DfaChecker {
    Annotation::Kind myAllocKind;
    Annotation::Kind myOutputStringKind;

public:
    StringSizeChecker()
    {
        myAllocKind = Annotation::GetKind("AllocSource");
        myOutputStringKind = Annotation::RegisterKind(
            "OutputString", Annotation::PROPAGATED_BY_DECL | Annotation::ANNOTATE_CALL_INSTRUCTION);
    }

    void CheckGlobalVar(DfaState& state, ProblemsHolder& holder) override
    {
        CheckState(state, holder);
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (!state.HasAnnotation(myOutputStringKind)) {
            return;
        }
        if (auto vDecl = state.GetNodeAs<VarDecl>(); vDecl != nullptr) {
            if (vDecl->IsVAArg()) {
                auto idx = state.GetFuncState().GetContext().GetFunction()->GetParams().size();
                state.GetFuncState().GetContext().AddAnnotation(idx + 1, myOutputStringKind);
                return;
            }
        }
        if (!state.HasAnnotation(myAllocKind) &&
            !(state.GetNode()->GetType().IsConstantArray() && state.GetNodeAs<VarDecl>() != nullptr)) {
            return;
        }
        state.AddSuspiciousPath({*this, myOutputStringKind, 0, StrLocales::GetStringLocale("MEMORY_BUFFER")});
    }

    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        if (int64_t allowedSize = sinkInfo.annotation.GetUserData<int64_t>(); allowedSize != 0) {
            auto& solverCtx = sinkInfo.context;
            auto argPos = sinkInfo.annotation.GetArgPos() - 1;
            auto sizeExprId = solverCtx.GetExprId(ExprLocation::CALL_ARG_SIZE, argPos);
            if (allowedSize != -1) {
                ExprId allowedSizeExprId = solverCtx.GetExprId(ExprLocation::CONSTANT, allowedSize);
                solverCtx.AddCondition(sinkInfo.exprId, sizeExprId, Condition::Operation::LT, allowedSizeExprId);
            } else {
                ExprId sizeLimitExprId = solverCtx.GetExprId(ExprLocation::CALL_ARG, argPos + 1);
                solverCtx.AddCondition(sinkInfo.exprId, sizeExprId, Condition::Operation::LT, sizeLimitExprId);
            }
        }
        return true;
    }
};

std::unique_ptr<HCXX::Checker> CreateStringSizeChecker()
{
    return std::make_unique<StringSizeChecker>();
}
