/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of SSR-1.1.2: Assign a new value to the variable pointing to a resource handle
// or descriptor immediately after the resource is freed
//

#include <ast/ArraySubscriptExpression.h>
#include <ast/CallExpression.h>
#include <ast/RefExpression.h>
#include <ast/UnaryExpression.h>
#include <ast/VarDecl.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class UseAfterFreeChecker : public DfaChecker {
    Annotation::Kind myDerefKind;
    Annotation::Kind myFreeSourceKind;
    Annotation::Kind myUseResourceKind;

public:
    UseAfterFreeChecker()
    {
        myFreeSourceKind = Annotation::GetKind("FreeSource");
        myUseResourceKind = Annotation::RegisterKind("UseResource", Annotation::PROPAGATED_BY_DECL);
        myDerefKind = Annotation::GetKind("Deref");
    }

    void InitState(DfaState& state) override
    {
        if (state.HasAnnotation(myDerefKind)) {
            state.CloneAnnotation(myDerefKind, myUseResourceKind, USE_AFTER_FREE_KIND);
            return;
        }

        AnnotateAddrOfArrayElement(state);

        if (state.HasAnnotation(myFreeSourceKind)) {
            return;
        }

        auto callExpr = state.GetParentAs<CallExpression>();
        auto callFunc = (callExpr == nullptr ? nullptr : callExpr->GetFunction());
        if (callFunc == nullptr || !callFunc->IsSystem()) {
            return;
        }
        auto argPos = callExpr->FindArgument(*state.GetNode());
        Type argType = argPos ? callExpr->GetArgumentType(*argPos) : Type();
        if (argType.IsPointer() && !argType.IsReference()) {
            state.Annotate(myUseResourceKind);
        }
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        // Prevent double free report on not suitable node
        if (state.GetNodeAs<VarDecl>() != nullptr) {
            return;
        }

        if (state.HasAnnotation(myFreeSourceKind) && state.HasAnnotation(myUseResourceKind) &&
            !state.HasSameAnnotationSource(myFreeSourceKind, myUseResourceKind)) {
            SuspiciousPath path{*this, myUseResourceKind, myFreeSourceKind,
                                StrLocales::GetStringLocale("USE_AFTER_FREE")};
            state.AddSuspiciousPath(std::move(path));
        }
    };

    void ExitState(DfaState& state) override
    {
        // Double Free
        for (auto& it : state.GetAnnotationSources(myFreeSourceKind)) {
            if (it.first.GetUserData<FreeUserData>().isFromAliasState) {
                continue;
            }
            state.CloneAnnotation(myFreeSourceKind, myUseResourceKind, DOUBLE_FREE_KIND);
        }
    }

    bool OnReportProblem(ProblemInfo& problem) override
    {
        std::string description = StrLocales::GetStringLocale("USE_AFTER_FREE");
        auto checkerKind = problem.trace.back().annotation.GetUserData<CheckerKind>();

        if (checkerKind == DOUBLE_FREE_KIND) {
            if (problem.trace.size() == 1) {
                // this case is the result of wrong behavior of Annotation::PROPAGATED_BY_ALIAS flag
                // the real fix should be implemented via HCAT-2533
                return false;  // LCOV_EXCL_LINE
            }
            description = StrLocales::GetStringLocale("DOUBLE_FREE");
            problem.kind = "MEM.DOUBLE.FREE";
            problem.trace.front().description = StrLocales::GetStringLocale("FREE");
        }

        problem.replacements.push_back(description);
        return true;
    }

private:
    enum CheckerKind { USE_AFTER_FREE_KIND, DOUBLE_FREE_KIND };

    void AnnotateAddrOfArrayElement(DfaState& state) const
    {
        auto arraySubscription = state.GetNodeAs<ArraySubscriptExpression>();
        if (arraySubscription != nullptr && arraySubscription->GetBase() != nullptr) {
            auto parentUnary = state.GetParentAs<UnaryExpression>();
            if (parentUnary && parentUnary->GetOperation() == UnaryExpression::Operation::ADDR_OF) {
                state.GetFuncState()
                    .GetState(arraySubscription->GetBase())
                    .Annotate(myUseResourceKind, USE_AFTER_FREE_KIND);
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateUseAfterFreeChecker()
{
    return std::make_unique<UseAfterFreeChecker>();
}
