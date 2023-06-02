/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxRecordDecl.h>
#include <ast/MemberExpression.h>
#include <ast/TypedefDecl.h>
#include <dfa/GlobalChecker.h>

#include <filesystem>
#include <unordered_map>

using namespace HCXX;

class UnusedMemberExpressionChecker : public GlobalChecker {
    Annotation::Kind myWriteKind;

    struct FieldUsage {
        uint64_t rangeBegin : 31;
        uint64_t rangeEnd : 31;
        uint64_t wasUsed : 1;
        uint64_t wasRead : 1;

        void Serialize(IOStream& stream)
        {
            stream.SerializePod(*this);
        }
    };
    using FieldsUsage = std::vector<FieldUsage>;

    struct RecordsUsage : public Context {
        std::unordered_map<std::string, FieldsUsage> fieldsUsage;
        std::unordered_map<const FieldDecl*, FieldUsage*> fieldUsageMap;
    };
    using RecordNames = std::unordered_map<RecordUniqueId, std::string>;

public:
    UnusedMemberExpressionChecker()
    {
        myWriteKind = Annotation::GetKind("Write");
    }

    RecordsUsage& GetRecordsUsage(const TranslationUnit& unit)
    {
        return GetContext<RecordsUsage>(unit);
    }

    std::string GetRecordName(const Node& node)
    {
        auto locInfo = node.GetTranslationUnit()->GetLocationInfo(node.GetRange());
        auto nodeName = node.GetFQN();
        if (nodeName.empty()) {
            auto parentRecord = node.GetParentOfType<RecordDecl>();
            if (parentRecord != nullptr) {
                nodeName = parentRecord->GetFQN() + '#';
            }
        }
        return std::filesystem::path(locInfo.filename).filename().string() + ':' + nodeName;
    }

    void Serialize(const TranslationUnit& unit, IOStream& stream) override
    {
        stream << GetRecordsUsage(unit).fieldsUsage;
    }

    void InitFieldUsage(RecordsUsage& recordsUsage, const FieldDecl& field, RecordNames& recordNames)
    {
        auto& fieldUsage = recordsUsage.fieldUsageMap[&field];
        if (fieldUsage != nullptr) {
            return;
        }
        auto record = Node::Cast<RecordDecl>(field.GetParent());
        if (record == nullptr || record->IsUnion()) {
            return;  // LCOV_EXCL_LINE: HCAT-2779
        }
        auto& recordName = recordNames[record->GetUniqueId()];
        if (recordName.empty()) {
            recordName = GetRecordName(*record);
        }
        size_t idx = field.GetIndex();
        auto& fieldsUsage = recordsUsage.fieldsUsage[recordName];
        if (fieldsUsage.empty()) {
            auto& fields = record->GetFields();
            fieldsUsage.resize(fields.size());
            for (auto i = 0; i < fields.size(); ++i) {
                auto nameRange = fields[i]->GetDeclName()->GetRange();
                fieldsUsage[i].rangeBegin = nameRange.begin;
                fieldsUsage[i].rangeEnd = nameRange.end;
            }
        }
        if (idx < fieldsUsage.size()) {
            fieldUsage = &fieldsUsage[idx];
            fieldUsage->wasUsed = 1;
        }
    }

    bool CheckUnit(const TranslationUnit& unit, ProblemsHolder& holder) override
    {
        RecordNames recordNames;
        for (auto& node : unit.GetNodes()) {
            auto typedefDecl = Node::Cast<TypedefDecl>(node);
            if (typedefDecl != nullptr) {
                auto record = Node::Cast<RecordDecl>(typedefDecl->GetType().GetDeclaration());
                if (record != nullptr && record->GetName().empty()) {
                    recordNames[record->GetUniqueId()] = GetRecordName(*typedefDecl);
                }
            }
        }
        auto& recordsUsage = GetRecordsUsage(unit);
        for (auto& node : unit.GetNodes()) {
            auto memberExpr = Node::Cast<MemberExpression>(node);
            auto fieldDecl = memberExpr != nullptr ? Node::Cast<FieldDecl>(memberExpr->GetValue()) : nullptr;
            if (fieldDecl != nullptr) {
                InitFieldUsage(recordsUsage, *fieldDecl, recordNames);
            }
        }
        return true;
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        auto memberExpr = state.GetNodeAs<MemberExpression>();
        auto fieldDecl = memberExpr != nullptr ? Node::Cast<FieldDecl>(memberExpr->GetValue()) : nullptr;

        if (fieldDecl != nullptr && (!state.HasAnnotation(myWriteKind) || fieldDecl->GetType().IsReference())) {
            auto& fieldsUsage = GetRecordsUsage(*fieldDecl->GetTranslationUnit()).fieldUsageMap;
            auto fieldUsage = fieldsUsage.find(fieldDecl);
            if (fieldUsage != fieldsUsage.end() && fieldUsage->second != nullptr) {
                fieldUsage->second->wasRead = true;
            }
        }
    }

    void PostCheck(std::vector<TranslationUnitPtr>& units, ProblemsHolder& holder) override
    {
        std::unordered_map<std::string, std::pair<const TranslationUnit*, FieldsUsage>> globalUsages;
        for (auto& unit : units) {
            for (auto& recordUsage : GetContext<RecordsUsage>(*unit).fieldsUsage) {
                auto& usage = recordUsage.second;
                auto& globalUsage = globalUsages[recordUsage.first];
                globalUsage.second.resize(usage.size());

                bool wasRead = false;
                for (uint64_t i = 0; i < usage.size(); ++i) {
                    globalUsage.second[i].wasRead |= usage[i].wasRead;
                    globalUsage.second[i].wasUsed |= usage[i].wasUsed;
                    globalUsage.second[i].rangeBegin = usage[i].rangeBegin;
                    globalUsage.second[i].rangeEnd = usage[i].rangeEnd;
                    wasRead |= globalUsage.second[i].wasRead;
                }
                if (wasRead) {
                    globalUsage.first = unit.get();
                }
            }
        }
        for (auto& it : globalUsages) {
            for (auto& fieldUsage : it.second.second) {
                auto tu = it.second.first;
                if (tu == nullptr || fieldUsage.wasRead) {
                    continue;
                }
                SourceRange range = {uint32_t(fieldUsage.rangeBegin), uint32_t(fieldUsage.rangeEnd)};
                Problem info;
                info.sourceId += it.first + ':' + tu->GetSourceInRange(range);
                info.isSourceIdUnique = true;
                info.reasonCode = (fieldUsage.wasUsed) ? "Write only" : "No usage";
                holder.RegisterProblem(*this, *tu, range, std::move(info));
            }
        }
    }
};

std::unique_ptr<HCXX::Checker> CreateUnusedMemberExpressionChecker()
{
    return std::make_unique<UnusedMemberExpressionChecker>();
}
