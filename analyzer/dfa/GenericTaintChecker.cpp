/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <dfa/DfaChecker.h>
#include <dfa/TaintedChecker.h>
#include <utils/Configuration.h>
using namespace HCXX;

class GenericTaintChecker : public TaintedChecker {
    struct TaintDefinition {
        std::string sinkKindName;
        std::string sourceKindName;
        std::string filterKindName;
        std::string profileName;
        Annotation::Kind sinkKind = 0;
        Annotation::Kind sourceKind = 0;
        Annotation::Kind filterKind = 0;

        DECLARE_FIELDS("sink", sinkKindName, "source", sourceKindName, "filter", filterKindName, "profile", profileName)
    };
    std::vector<TaintDefinition> myTaints;

public:
    bool Initialize(const Checker::Config& config) override
    {
        bool result = LoadConfiguration(myTaints, config, GetName());
        for (auto& it : myTaints) {
            it.sinkKind = Annotation::RegisterKind(it.sinkKindName.c_str(),
                                                   Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
            it.sourceKind = Annotation::RegisterKind(it.sourceKindName.c_str(), Annotation::PROPAGATED_BY_POINTER);
            if (!it.filterKindName.empty()) {
                it.filterKind = Annotation::RegisterKind(it.filterKindName.c_str(), Annotation::PROPAGATED_BY_POINTER);
            }
        }

        return result;
    }

    void InitState(DfaState& state) override
    {
        for (auto& taint : myTaints) {
            if (taint.filterKind != 0 && state.HasAnnotation(taint.filterKind)) {
                state.ClearAnnotation(taint.sinkKind);
            }
        }
    }

    bool CheckSource(DfaState& state, const TaintDefinition& taint)
    {
        if (state.HasAnnotation(taint.sourceKind)) {
            return true;
        }
        if (taint.sourceKind != myUntrustedSourceKind) {
            return false;  // LCOV_EXCL_LINE: HCAT-2777
        }
        auto varDecl = state.GetNodeAs<VarDecl>();
        return varDecl != nullptr && varDecl->GetType().IsConstantArray();
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        for (auto& taint : myTaints) {
            if (state.HasAnnotation(taint.sinkKind) && CheckSource(state, taint)) {
                SuspiciousPath suspPath(*this, taint.sinkKind,
                                        state.HasAnnotation(taint.sourceKind) ? taint.sourceKind : 0);
                suspPath.problemKind = taint.profileName;
                suspPath.sinkCondition = Condition{Condition::Operation::EQ, UNDEFINED_VALUE};
                state.AddSuspiciousPath(std::move(suspPath));
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateGenericTaintChecker()
{
    return std::make_unique<GenericTaintChecker>();
}
