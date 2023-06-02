/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "Convertors.h"

namespace HCXX {

Location ConvertLocation(clang::SourceManager& sm, const clang::SourceLocation& location)
{
    return sm.getExpansionLoc(location).getRawEncoding();
}

SourceRange ConvertSourceRange(clang::SourceManager& sm, const clang::LangOptions& langOptions,
                               const clang::SourceRange& sourceRange)
{
    if (sm.isMacroArgExpansion(sourceRange.getEnd())) {
        if (sm.isMacroArgExpansion(sourceRange.getBegin())) {
            auto macroBegin = sm.getTopMacroCallerLoc(sourceRange.getBegin());
            auto macroEnd = sm.getTopMacroCallerLoc(sourceRange.getEnd());
            if (!macroBegin.isMacroID() && !macroEnd.isMacroID()) {
                auto clangLastTokenEnd = clang::Lexer::getLocForEndOfToken(macroEnd, 0, sm, langOptions);
                return {macroBegin.getRawEncoding(), clangLastTokenEnd.getRawEncoding()};
            }
        } else {
            auto macroEnd = sm.getTopMacroCallerLoc(sourceRange.getEnd());
            if (!macroEnd.isMacroID()) {
                auto clangLastTokenEnd = clang::Lexer::getLocForEndOfToken(macroEnd, 0, sm, langOptions);
                return {macroEnd.getRawEncoding(), clangLastTokenEnd.getRawEncoding()};
            }
        }
    }

    auto charSourceRange = sm.getExpansionRange(sourceRange);
    HCXX::Location hcxxBeginLoc = charSourceRange.getBegin().getRawEncoding();

    auto sourceEnd = sourceRange.getEnd();
    if (sourceRange.getBegin().isMacroID() || sourceRange.getEnd().isMacroID()) {
        // LCOV_EXCL_START
        sourceEnd = charSourceRange.getEnd();
        // LCOV_EXCL_STOP
    }

    auto clangLastTokenEnd = clang::Lexer::getLocForEndOfToken(sourceEnd, 0, sm, langOptions);
    return {hcxxBeginLoc, clangLastTokenEnd.getRawEncoding()};
}

}  // namespace HCXX
