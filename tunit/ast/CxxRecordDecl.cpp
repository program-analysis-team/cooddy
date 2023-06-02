/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxCtorDecl.h>
#include <ast/CxxRecordDecl.h>

namespace HCXX {

void RecordDecl::Init()
{
    auto recordName = GetName();
    if (recordName.empty()) {
        auto parentRecord = GetParentOfType<RecordDecl>();
        if (parentRecord != nullptr) {
            recordName = parentRecord->GetFQN() + '#';
        }
    }
    if (!recordName.empty()) {
        InitUniqueId(recordName);
    }
}

RecordUniqueId RecordDecl::MakeUniqueId(const std::string& fileName, uint32_t fileOffset, const std::string& recordName)
{
    std::stringstream fullName;
    fullName << StrUtils::GetLastToken(fileName, "\\/") << ':' << fileOffset << '|' << recordName;
    return StrUtils::GetStringId(fullName.str());
}

void RecordDecl::InitUniqueId(const std::string& recordName) const
{
    auto loc = GetTranslationUnit()->GetLocationInfo(GetRange());
    myUniqueId = MakeUniqueId(loc.filename, loc.offset, recordName);
    for (auto& node : myFields) {
        node->SetRecordUniqueId(myUniqueId);
    }
}

void RecordDecl::SetFields(Fields&& fields)
{
    myFields = fields;
    std::sort(myFields.begin(), myFields.end(),
              [](const NodePtr<FieldDecl>& p1, const NodePtr<FieldDecl>& p2) { return p1->GetName() < p2->GetName(); });
}

const FieldDecl* RecordDecl::GetField(std::string_view fieldName) const
{
    auto it =
        std::lower_bound(myFields.begin(), myFields.end(), fieldName,
                         [](const NodePtr<FieldDecl>& p, const std::string_view& key) { return p->GetName() < key; });
    if (it != myFields.end() && (*it)->GetName() == fieldName) {
        return *it;
    }
    return nullptr;
}

const FieldDecl* RecordDecl::GetField(VirtualOffset offset) const
{
    auto subOffset = offset.ExtractSubOffset();
    if (subOffset.first == VirtualOffset::Kind::INDEX && offset.IsSourceObject()) {
        if (subOffset.second < myFields.size()) {
            return myFields[subOffset.second];
        }
    }
    return nullptr;
}

void CxxRecordDecl::SetMethods(Methods&& methods)
{
    myMethods = methods;
    std::sort(myMethods.begin(), myMethods.end(),
              [](const NodePtr<CxxMethodDecl>& p1, const NodePtr<CxxMethodDecl>& p2) {
                  return p1->GetName() < p2->GetName();
              });
}

const CxxMethodDecl* CxxRecordDecl::GetMethod(std::string_view methodName) const
{
    auto it = std::lower_bound(
        myMethods.begin(), myMethods.end(), methodName,
        [](const NodePtr<CxxMethodDecl>& p, const std::string_view& key) { return p->GetName() < key; });

    if (it != myMethods.end() && (*it)->GetName() == methodName) {
        return *it;
    }

    for (auto& baseRecord : myBaseRecords) {
        auto methodDecl = baseRecord->GetMethod(methodName);
        if (methodDecl != nullptr) {
            return methodDecl;
        }
    }

    return nullptr;
}

const CxxCtorDecl* CxxRecordDecl::FindDefaultCtor() const
{
    for (const CxxCtorDecl* ctor : myConstructors) {
        if (ctor->IsDefault()) {
            return ctor;
        }
    }

    return nullptr;
}

ParamVarDecl CxxMethodDecl::myThisDecl("", Type(), "");

}  // namespace HCXX
