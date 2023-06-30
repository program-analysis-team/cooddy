/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/BinaryExpression.h>
#include <ast/ImplicitValueInitExpression.h>
#include <ast/InitListExpression.h>
#include <ast/LiteralExpression.h>
#include <ast/MemberExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/AllocSourceChecker.h>
#include <dfa/TaintedChecker.h>
#include <solver/FunctionBehavior.h>
#include <utils/Configuration.h>

#include "SourceRangeHash.h"
#include "ast/CxxRecordDecl.h"
#include "ast/LoopStatement.h"
#include "dfa/SensitiveDataSettings.h"

using namespace HCXX;
class SensitiveDataExposureChecker : public TaintedChecker {
    Annotation::Kind mySensitiveDataSourceKind;
    Annotation::Kind mySensitiveDataLeakKind;
    struct SensitiveData {
        uint32_t locationBegin : 31;
        uint32_t isRecordSensitive : 1;
        uint32_t locationEnd : 31;
        uint32_t isAnnotatedByConfig : 1;
    };

    struct SensitiveDataManager : public Context {
        std::unordered_map<SourceRange, bool, SourceRangeHash> sensiInfos;
    };

public:
    SensitiveDataExposureChecker()
    {
        mySensitiveDataSourceKind = Annotation::RegisterKind("SensitiveDataSource", Annotation::PROPAGATED_BY_POINTER);
        mySensitiveDataLeakKind =
            Annotation::RegisterKind("SensitiveDataLeak", Annotation::PROPAGATED_BY_DECL | Annotation::UNTRUSTED_SINK);
    }
    bool Initialize(const Checker::Config& config) override
    {
        if (config == nullptr) {
            // No config found, use default values
            return true;
        }
        return LoadConfiguration(mySettings, config, GetName());
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        if (state.HasAnnotation(mySensitiveDataSourceKind)) {
            if (state.HasAnnotation(mySensitiveDataLeakKind)) {
                state.AddSuspiciousPath({*this,
                                         mySensitiveDataLeakKind,
                                         mySensitiveDataSourceKind,
                                         StrLocales::GetStringLocale("SENSITIVE_DATA_LEAK"),
                                         {},
                                         CheckPathParams::SINK_CAN_BE_BEFORE_SOURCE});
            }
        }
    }
    bool CheckUnit(const TranslationUnit& unit, ProblemsHolder& holder) override
    {
        GetContext<SensitiveDataManager>(unit);  // to avoid raise conditions on InitState
        return true;
    }
    void InitState(DfaState& state) override
    {
        auto node = state.GetNode();
        if (node == nullptr) {
            return;  // LCOV_EXCL_LINE
        }
        auto& context = GetContext<SensitiveDataManager>(*node->GetTranslationUnit());
        auto& infos = context.sensiInfos;
        if (state.HasAnnotation(mySensitiveDataSourceKind) && node != nullptr) {
            for (auto [n, v] : state.GetAnnotationSources(mySensitiveDataSourceKind)) {
                if (n.IsSourceRange() && !state.GetNode()->GetType().IsSensitiveData()) {
                    state.ClearAnnotation(mySensitiveDataSourceKind);
                    state.Annotate(mySensitiveDataSourceKind, SensitiveData{0, false, 0, true});
                    break;
                }
            }
        } else if (auto member = Node::Cast<MemberExpression>(node); member != nullptr) {
            if (auto decl = member->GetValue(); decl != nullptr) {
                auto record = decl->GetParentOfType<RecordDecl>();
                bool recordSensitive = record != nullptr && record->IsSensitive();
                auto fieldDecl = Node::Cast<FieldDecl>(decl);
                bool fieldSensitive = fieldDecl != nullptr && fieldDecl->GetType().IsSensitiveData();
                if (recordSensitive || fieldSensitive) {
                    SensitiveData data{decl->GetRange().begin, recordSensitive, decl->GetRange().end, false};
                    state.Annotate(mySensitiveDataSourceKind, data);
                    infos[decl->GetRange()] = false;
                }
            }
        } else if (node != nullptr && node->GetType().IsSensitiveData()) {
            SensitiveData data{0, false, 0, false};
            if (auto ref = Node::Cast<RefExpression>(node); ref != nullptr) {
                if (auto decl = ref->GetDeclaration(); decl != nullptr) {
                    data = {decl->GetRange().begin, false, decl->GetRange().end, false};
                    infos[decl->GetRange()] = false;
                }
            }
            state.Annotate(mySensitiveDataSourceKind, data);
        }
    }
    bool OnSourceExecuted(const SourceExecInfo& sourceInfo) override
    {
        sourceInfo.context.AddUntrustedSourceByKind(sourceInfo.exprId, UntrustedSource::SourceKind::SENSITIVE_DATA);
        return true;
    }
    bool OnReportProblem(ProblemInfo& problemInfo) override
    {
        auto& traceNode = problemInfo.trace.front();
        SensitiveData sensitiveData = traceNode.annotation.GetUserData<SensitiveData>();
        if (sensitiveData.locationEnd != 0 && sensitiveData.locationBegin != 0) {
            traceNode.range = SourceRange{sensitiveData.locationBegin, sensitiveData.locationEnd};
            auto location = traceNode.tu->GetLocationInfo(traceNode.range);
            auto scopeRange = traceNode.tu->GetRecordDeclRangeByMember(traceNode.range);
            traceNode.scopeRange = scopeRange.IsValid() ? scopeRange : traceNode.range;
            traceNode.line = location.line;
            traceNode.column = location.column;
            traceNode.endLine = location.endLine;
            traceNode.endColumn = location.endColumn;
            traceNode.filename = location.filename;
        }
        if (sensitiveData.isAnnotatedByConfig) {
            traceNode.description = StrLocales::GetStringLocale("SENSITIVE_ANNOTATED_BY_CONFIG");
        } else {
            traceNode.description = StrLocales::GetStringLocale(
                sensitiveData.isRecordSensitive ? "SENSITIVE_ANNOTATED_RECORD" : "SENSITIVE_ANNOTATED_BY_MACRO");
        }
        auto& context = GetContext<SensitiveDataManager>(*traceNode.tu->GetTranslationUnit());
        auto& infos = context.sensiInfos;
        infos[traceNode.range] = true;
        return true;
    }

    /// When --sensi-info-summary option is specified, report information via detects on whether issues were found for
    /// annotated variables.
    void PostCheck(std::vector<TranslationUnitPtr>& units, ProblemsHolder& holder) override
    {
        if (!mySettings.sensiInfoSummary) {
            return;
        }

        for (auto& unit : units) {
            auto& context = GetContext<SensitiveDataManager>(*unit);
            for (auto& [info, hasIssues] : context.sensiInfos) {
                Problem problem;
                problem.checkerName = "NotAnIssue";
                problem.kind = hasIssues ? "IssuesFound" : "NoIssues";
                problem.scopeRange = unit->GetRecordDeclRangeByMember(info);
                problem.sourceId = unit->GetSourceInRange(info);
                problem.reasonCode = StrLocales::GetStringLocale("ANNOTATED_BY_MACRO_REASON");
                problem.description =
                    StrLocales::GetStringLocale("VARIABLE_ANNOTATED_BY_MACRO", {unit->GetSourceInRange(info)});
                problem.severity = Problem::Severity::ERROR;
                holder.RegisterProblem(*this, *unit, info, std::move(problem));
            }
        }
    }

private:
    SensitiveDataSettings mySettings;
};

std::unique_ptr<HCXX::Checker> CreateSensitiveDataExposureChecker()
{
    return std::make_unique<SensitiveDataExposureChecker>();
}
