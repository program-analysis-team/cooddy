/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <TranslationUnit.h>

#include "utils/HashImpl.h"

namespace HCXX {

void CommentManager::AddComment(const std::string_view& commentText, const LocationInfo& locationInfo)
{
    static constexpr std::string_view suppressBase = "COODDY_SUPPRESS";

    // create suppress
    if (commentText.find(suppressBase) != std::string::npos) {
        CommentManager::SuppressInfo suppressLocation{locationInfo.line + 1, locationInfo.filename};
        std::string_view comment;

        if (commentText.find("//") == 0) {
            comment = commentText.substr(2);
        } else if (commentText.find("/*") == 0) {
            comment = commentText.substr(2, commentText.size() - 4);
        }

        auto splitComment = StrUtils::Split(StrUtils::TrimWhitespace(comment), ' ');
        if (splitComment.size() > 1 && splitComment.front() == suppressBase) {
            for (std::size_t i = 1; i < splitComment.size(); ++i) {
                std::string_view key = *(mySuppressesKeyStorage.insert(std::string{splitComment[i]}).first);
                mySuppresses[key].emplace(suppressLocation);
            }
        } else {
            mySuppresses[""].emplace(suppressLocation);
        }
    }
}

bool CommentManager::CheckSuppress(const std::string& checkerName, const LocationInfo& locinfo) const
{
    auto checkSuppress = [this](const std::string& checkerId, const LocationInfo& locinfo) {
        auto suppressesForChecker = this->mySuppresses.find(checkerId);

        if (suppressesForChecker != this->mySuppresses.end()) {
            return suppressesForChecker->second.find({locinfo.line, locinfo.filename}) !=
                   suppressesForChecker->second.end();
        }

        return false;
    };

    // check suppresses by checkers's name or for any checker (empty ID)
    return checkSuppress(checkerName, locinfo) || checkSuppress("", locinfo);
}

std::size_t CommentManager::SuppressInfoHash::operator()(const SuppressInfo& suppressInfo) const
{
    return HCXX::CalcHash(suppressInfo.line, suppressInfo.fileName);
}

void CommentManager::Clear()
{
    myComments = Comments();
}

}  // namespace HCXX
