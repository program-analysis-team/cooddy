/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_MACRO_CODEEXPANSIONMANAGER_H_
#define COODDY_ANALYZER_INCLUDE_MACRO_CODEEXPANSIONMANAGER_H_

#include <macro/Macro.h>

#include <unordered_map>

namespace HCXX {
class TranslationUnit;

class CodeDescriptionManager {
public:
    struct CodeDescription {
        enum CodeDescriptionType { NONE, MACRO, ENUM };
        SourceRange codeRange;         // source range where macro/vardecl is expanded
        SourceRange descriptionRange;  // source range where macro/vardecl is declared
        CodeDescriptionType type;
        int64_t value;

        bool operator<(const CodeDescription& e) const
        {
            return codeRange.end < e.codeRange.end;
        }
        bool operator<(Location loc) const
        {
            return codeRange.end < loc;
        }
        std::string ToString(const TranslationUnit& tu) const;
    };
    void InitDescriptionRanges(TranslationUnit& tu);

    SourceRange GetMacroDeclRangeByExpansion(Location loc) const;

    SourceRange GetRecordDeclRangeByMember(SourceRange sourceRange) const;

    bool IsMacroExpansionRange(SourceRange sourceRange) const;

    const std::vector<CodeDescription> GetDescriptions(const SourceRange& loc) const;

private:
    std::vector<CodeDescription> myDescriptions;  // sorted list of macro/vardecl expansion
    std::vector<SourceRange> myMacroRanges;
    std::vector<SourceRange> myRecordRanges;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_MACRO_CODEEXPANSIONMANAGER_H_
