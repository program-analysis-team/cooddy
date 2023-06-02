/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/ThisExpression.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/DfaChecker.h>

#include <unordered_set>

using namespace HCXX;

class MismatchedMemManagementChecker : public DfaChecker {
    Annotation::Kind myLockResourceKind;
    Annotation::Kind myUnlockResourceKind;
    Annotation::Kind myFreeFuncKind;
    Annotation::Kind myAllocResourceKind;
    Annotation::Kind myAllocDescriptorKind;
    Annotation::Kind myFreeDescriptorKind;
    Annotation::Kind myStackAllocKind = 0;

public:
    MismatchedMemManagementChecker()
    {
        myLockResourceKind = Annotation::GetKind("LockResource");
        myUnlockResourceKind = Annotation::GetKind("UnlockResource");
        myFreeFuncKind = Annotation::GetKind("FreeSink");
        myAllocResourceKind = Annotation::GetKind("AllocSource");
        myAllocDescriptorKind = Annotation::GetKind("AllocDescriptor");
        myFreeDescriptorKind = Annotation::GetKind("FreeDescriptor");
    }

    void InitState(DfaState& state) override {}

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.GetNodeAs<ThisExpression>() != nullptr) {
            return;
        }
        TryAddPath(state, myAllocResourceKind, myFreeFuncKind, "ALLOC_SOURCE");
        TryAddPath(state, myLockResourceKind, myUnlockResourceKind, "RESOURCE_SOURCE");
        TryAddPath(state, myAllocDescriptorKind, myFreeDescriptorKind, "DESCRIPTOR_SOURCE");
    }
    void TryAddPath(DfaState& state, Annotation::Kind initAnnotation, Annotation::Kind freeAnnotation,
                    const std::string& sourceId)
    {
        if (state.HasAnnotation(initAnnotation) && state.HasAnnotation(freeAnnotation) &&
            !IsMatchedMemFunctions(state, initAnnotation, freeAnnotation)) {
            state.AddSuspiciousPath({*this, freeAnnotation, initAnnotation, StrLocales::GetStringLocale(sourceId)});
        }
    }

    void CheckDeclaration(DfaState& state, ProblemsHolder& holder) override
    {
        if (!state.HasAnnotation(myFreeFuncKind)) {
            return;
        }
        auto type = state.GetNode()->GetType();
        if (type.IsPointer()) {
            return;
        }
        if (type.IsArray()) {
            state.AddSuspiciousPath(
                {*this, myFreeFuncKind, myStackAllocKind, StrLocales::GetStringLocale("STACK_VARIABLE")});
            return;
        }

        SuspiciousPath path{*this, myFreeFuncKind, myStackAllocKind, StrLocales::GetStringLocale("STACK_VARIABLE")};
        Annotation sourceAnnotation{myStackAllocKind, Annotation::ArgInfo(),
                                    state.GetFuncState().GetContext().GetInstruction(*state.GetNode()),
                                    VirtualOffset(VirtualOffset::Kind::ADDR_OF)};
        path.sourceAnnotations.emplace_back(sourceAnnotation);
        state.AddSuspiciousPath(std::move(path));
    }

    bool OnReportProblem(ProblemInfo& problem) override
    {
        auto description = StrLocales::GetStringLocale("WRONG_ALLOC_DEALLOC");
        if (problem.trace.front().annotation.GetKind() == myAllocDescriptorKind) {
            problem.kind = "ALLOC.DESCRIPTOR.MISMATCH";
            description = StrLocales::GetStringLocale("WRONG_DESCRIPTOR_ALLOC_FREE");
        } else if (problem.trace.front().annotation.GetKind() == myLockResourceKind) {
            problem.kind = "LOCK.FUNC.MISMATCH";
            description = StrLocales::GetStringLocale("WRONG_LOCK_UNLOCK");
        } else if (problem.trace.front().annotation.GetKind() == myStackAllocKind) {
            description = StrLocales::GetStringLocale("STACK_VARIABLE_ATTEMPT_TO_DELETE");
        }
        problem.replacements.push_back(description);
        return true;
    }

    bool OnSourceExecuted(const SourceExecInfo& sourceInfo) override
    {
        if (sourceInfo.annotation.GetMemoryOffset() == VirtualOffset(VirtualOffset::Kind::ADDR_OF)) {
            sourceInfo.context.AddOffset(sourceInfo.exprId, VirtualOffset(VirtualOffset::Kind::ADDR_OF));
        }
        return true;
    }

private:
    bool IsMatchedMemFunctions(DfaState& state, Annotation::Kind initAnnotation, Annotation::Kind freeAnnotation) const
    {
        auto freeUserData = GetUniqueAnnotationUserData(state, initAnnotation);
        auto allocUserData = GetUniqueAnnotationUserData(state, freeAnnotation);
        return freeUserData.size() == 1 && allocUserData.size() == 1 && *freeUserData.begin() == *allocUserData.begin();
    }

    std::unordered_set<int> GetUniqueAnnotationUserData(DfaState& state, Annotation::Kind annotation) const
    {
        std::unordered_set<int> result;
        for (auto& source : state.GetAnnotationSources(annotation)) {
            result.insert(source.first.GetUserData<AllocUserData>().allocType);
        }
        return result;
    }
};

std::unique_ptr<HCXX::Checker> CreateMismatchedMemManagementChecker()
{
    return std::make_unique<MismatchedMemManagementChecker>();
}
