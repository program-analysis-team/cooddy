/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CodeDescriptionManager.h>
#include <TranslationUnit.h>
#include <ast/VarDecl.h>
#include <macro/MacroManager.h>

#include <vector>

#include "ast/CallExpression.h"
#include "ast/CxxMemberCallExpression.h"
#include "ast/CxxRecordDecl.h"
#include "ast/EnumConstantDecl.h"

namespace HCXX {
void CodeDescriptionManager::InitDescriptionRanges(TranslationUnit& tu)
{
    for (auto node : tu.GetNodes()) {
        auto decl = node->GetType().GetPointedDeclaration();
        if (decl != nullptr && decl->GetRange().IsValid()) {
            auto varDecl = HCXX::Node::Cast<VarDecl>(node);
            if (varDecl != nullptr) {
                myDescriptions.emplace_back(CodeDescription{varDecl->GetTypeSpecSourceRange(), decl->GetRange(),
                                                            CodeDescription::CodeDescriptionType::NONE});
            }
            if (node->IsKindOf(Node::Kind::PARAM_VAR_DECL)) {
                myDescriptions.emplace_back(
                    CodeDescription{node->GetRange(), decl->GetRange(), CodeDescription::CodeDescriptionType::NONE});
            }
        }
        auto ref = HCXX::Node::Cast<RefExpression>(node);
        if (ref != nullptr) {
            auto refDecl = ref->GetDeclaration();
            if (refDecl != nullptr) {
                if (refDecl->GetType().IsConstant() || refDecl->GetType().IsConstantArray()) {
                    myDescriptions.emplace_back(CodeDescription{ref->GetRange(), refDecl->GetRange(),
                                                                CodeDescription::CodeDescriptionType::NONE});
                }
                auto enumDecl = HCXX::Node::Cast<EnumConstantDecl>(refDecl);
                if (enumDecl != nullptr) {
                    myDescriptions.emplace_back(CodeDescription{node->GetRange(), node->GetRange(),
                                                                CodeDescription::CodeDescriptionType::ENUM,
                                                                enumDecl->GetValue()});
                }
            }
        }
        auto member = HCXX::Node::Cast<MemberExpression>(node);
        if (member != nullptr && member->GetParentOfType<MemberExpression>() == nullptr) {
            auto memberDecl = member->GetValue();
            if (memberDecl != nullptr && memberDecl->GetRange().IsValid() &&
                (memberDecl->GetType().IsArray() || memberDecl->GetType().IsConstant())) {
                myDescriptions.emplace_back(CodeDescription{member->GetRange(), memberDecl->GetRange(),
                                                            CodeDescription::CodeDescriptionType::NONE});
            }
        }
        auto recordDecl = HCXX::Node::Cast<RecordDecl>(node);
        if (recordDecl != nullptr) {
            myRecordRanges.emplace_back(recordDecl->GetRange());
        }
    }

    for (auto [name, macro] : tu.GetMacros()) {
        for (auto expansion : macro.macroExpansions) {
            myDescriptions.emplace_back(CodeDescription{expansion.GetRange(), macro.declSourceRange,
                                                        CodeDescription::CodeDescriptionType::MACRO});
            myMacroRanges.emplace_back(expansion.GetRange());
        }
    }
    std::stable_sort(myDescriptions.begin(), myDescriptions.end());
    std::stable_sort(myMacroRanges.begin(), myMacroRanges.end());
    std::stable_sort(myRecordRanges.begin(), myRecordRanges.end());
}

bool CodeDescriptionManager::IsMacroExpansionRange(SourceRange sourceRange) const
{
    if (!sourceRange.IsValid()) {
        return false;
    }
    auto it = std::lower_bound(myMacroRanges.begin(), myMacroRanges.end(), sourceRange);
    return it != myMacroRanges.end() && *it == sourceRange;
}
std::string CodeDescriptionManager::CodeDescription::ToString(const TranslationUnit& tu) const
{
    switch (type) {
        case ENUM:
            return "Enum " + tu.GetSourceInRange(descriptionRange) + " value is " + std::to_string(value);
        case MACRO:
            return "#define " + tu.GetSourceInRange(descriptionRange);
        default:
            return tu.GetSourceInRange(descriptionRange);
    }
}

const std::vector<CodeDescriptionManager::CodeDescription> CodeDescriptionManager::GetDescriptions(
    const SourceRange& loc) const
{
    std::vector<CodeDescriptionManager::CodeDescription> result;
    auto b = std::lower_bound(myDescriptions.begin(), myDescriptions.end(), loc.begin);
    if (b == myDescriptions.end()) {
        return result;
    }
    for (auto it = b; it != myDescriptions.end() && it->codeRange.end <= loc.end; ++it) {
        result.push_back(*it);
    }
    return result;
}

SourceRange CodeDescriptionManager::GetRecordDeclRangeByMember(SourceRange sourceRange) const
{
    auto b = std::lower_bound(myRecordRanges.begin(), myRecordRanges.end(), sourceRange,
                              [](const SourceRange& i, const SourceRange& value) {
                                  return i.begin > value.begin || (i.begin == value.begin && i.end < value.end);
                              });
    if (b == myRecordRanges.end() || b->end <= sourceRange.end) {
        return {};
    }
    return *b;
}

};  // namespace HCXX
