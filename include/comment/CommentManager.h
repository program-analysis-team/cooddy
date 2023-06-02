/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_COMMENTMANAGER_H
#define COODDY_COMMENTMANAGER_H

#include <comment/Comment.h>

#include <regex>
#include <unordered_set>

namespace HCXX {

class CommentManager {
public:
    using Comments = std::vector<Comment>;

    struct SuppressInfo {
        uint32_t line;
        const std::string& fileName;

        // LCOV_EXCL_START
        bool operator==(const SuppressInfo& other) const
        {
            return line == other.line && fileName == other.fileName;
        }
        // LCOV_EXCL_STOP
    };

    virtual void AddComment(const std::string_view& commentText, const LocationInfo& locationInfo);

    bool CheckSuppress(const std::string& checkerName, const LocationInfo& locinfo) const;

    const Comments& GetComments() const
    {
        return myComments;
    }

protected:
    CommentManager() : myComments(Comments()) {}
    virtual ~CommentManager() = default;

    void Clear();

private:
    struct SuppressInfoHash {
        std::size_t operator()(const SuppressInfo& suppressInfo) const;
    };

    Comments myComments;

    // key is checker code, name or "" for any checker
    std::unordered_map<std::string_view, std::unordered_set<SuppressInfo, SuppressInfoHash>> mySuppresses;
    std::unordered_set<std::string> mySuppressesKeyStorage;  // Remove in C++20 by replacing map key with string
};

}  // namespace HCXX

#endif  // COODDY_COMMENTMANAGER_H
