/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_COMMENTCALLBACK_H
#define COODDY_COMMENTCALLBACK_H

#include <comment/CommentManager.h>

#include "Convertors.h"
#include "PreProcessorCallbacks.h"

class CommentCallback : public clang::CommentHandler {
public:
    explicit CommentCallback(HCXX::TranslationUnit& unit) : myUnit(unit) {}
    ~CommentCallback() = default;

    // The handler shall return true if it has pushed any tokens
    // to be read using e.g. EnterToken or EnterTokenStream.
    bool HandleComment(clang::Preprocessor& preprocessor, clang::SourceRange commentRange) override
    {
        auto& sm = preprocessor.getSourceManager();

        bool invalid = false;
        auto comStart = sm.getCharacterData(commentRange.getBegin(), &invalid);

        if (invalid) {
            return false;  // LCOV_EXCL_LINE: HCAT-2778
        }

        HCXX::SourceRange range = HCXX::ConvertSourceRange(sm, preprocessor.getLangOpts(), commentRange);
        std::string_view commentText(comStart, range.end - range.begin);
        myUnit.AddComment(commentText, myUnit.GetLocationInfo(range));

        return false;
    }

private:
    HCXX::TranslationUnit& myUnit;
};

#endif  // COODDY_COMMENTCALLBACK_H
