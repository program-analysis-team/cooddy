/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/ArraySubscriptExpression.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class SampleDfaChecker : public DfaChecker {
    // annotation declaration
    Annotation::Kind myAccessKind;

public:
    // init class members
    SampleDfaChecker()
    {
        myAccessKind = Annotation::RegisterKind("Access", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
    }

    // set annotations on states
    void InitState(DfaState& state) override
    {
        auto arrayExpression = state.GetNodeAs<ArraySubscriptExpression>();
        if (arrayExpression != nullptr && arrayExpression->GetIdx() != nullptr) {
            state.Annotate(myAccessKind, arrayExpression->GetSizeOfType());
        }
    }

    // check paths
    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation(myAccessKind)) {
            state.AddSuspiciousPath({*this, myAccessKind, 0, "", {}, CheckPathParams::USE_MEMORY_ADDRESSES});
        }
    }

    // modify solver expr for sink point
    bool OnSinkExecuted(const SinkExecInfo& sinkInfo) override
    {
        auto& solverCtx = sinkInfo.context;
        auto sizeOfArrayEl = static_cast<int32_t>(sinkInfo.annotation.GetUserData());

        auto idxExpr = solverCtx.GetExprId(ExprLocation::OPERAND, 1);
        solverCtx.ApplyOperation(idxExpr, SolverContext::Operation::MUL,
                                 solverCtx.GetExprId(ExprLocation::CONSTANT, sizeOfArrayEl));

        auto baseExpr = solverCtx.GetExprId(ExprLocation::OPERAND, 0);
        solverCtx.ApplyOperation(sinkInfo.exprId, SolverContext::Operation::ASSIGN, baseExpr);
        solverCtx.ApplyOperation(sinkInfo.exprId, SolverContext::Operation::ADD, idxExpr);

        Condition condition{Condition::Operation::ACCESS, sizeOfArrayEl};
        return solverCtx.AddCondition(sinkInfo.exprId, condition);
    }
};

std::unique_ptr<Checker> CreateSampleDfaChecker()
{
    return std::make_unique<SampleDfaChecker>();
}
