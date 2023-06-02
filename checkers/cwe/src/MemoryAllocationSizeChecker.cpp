/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxNewExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/TaintedChecker.h>

using namespace HCXX;

class MemoryAllocationSizeChecker : public TaintedChecker {
    static constexpr uint32_t MAX_ALLOC_SIZE_BITS = 31;
    Annotation::Kind myAllocSizeKind;
    Annotation::Kind myUntrustedAllocSizeKind;

public:
    MemoryAllocationSizeChecker()
    {
        myAllocSizeKind = Annotation::GetKind("AllocSize");
        myUntrustedAllocSizeKind =
            Annotation::RegisterKind("UntrustedAllocSize", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
    }

    void InitState(DfaState& state) override
    {
        if (auto newExpr = state.GetParentAs<CxxNewExpression>();
            newExpr != nullptr && state.GetNode() == newExpr->GetArrayElemCount()) {
            state.Annotate(myUntrustedAllocSizeKind);
        }
        for (auto& anno : state.GetAnnotationSources(myAllocSizeKind)) {
            if (state.GetNode() == anno.second && anno.first.IsSourceRange()) {
                state.Annotate(myUntrustedAllocSizeKind);
            }
        }
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (!state.HasAnnotation(myUntrustedSourceKind) || !state.HasAnnotation(myUntrustedAllocSizeKind)) {
            return;
        }
        Condition sinkCond{Condition::Operation::INT_OVERFLOW};
        sinkCond.intTypeInfo = IntTypeInfo{1, MAX_ALLOC_SIZE_BITS, 0};
        state.AddSuspiciousPath({*this, myUntrustedAllocSizeKind, myUntrustedSourceKind,
                                 StrLocales::GetStringLocale("UNTRUSTED_ALLOCATED_SIZE"), sinkCond});
    }
};

std::unique_ptr<HCXX::Checker> CreateMemoryAllocationSizeChecker()
{
    return std::make_unique<MemoryAllocationSizeChecker>();
}
