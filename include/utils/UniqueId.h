/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_UNIQUEID_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_UNIQUEID_H_

#include <string>

namespace HCXX {

struct UniqueId {
    uint64_t lHash = 0;
    uint64_t hHash = 0;

    bool IsValid() const
    {
        return lHash != 0 || hHash != 0;
    }

    bool operator==(const UniqueId& p) const
    {
        return p.lHash == lHash && p.hHash == hHash;
    }

    bool operator!=(const UniqueId& p) const
    {
        return !(*this == p);
    }

    struct HashFunction {
        size_t operator()(const UniqueId& p) const
        {
            return p.lHash ^ p.hHash;
        }
    };

    uint8_t* Data() const
    {
        // COODDY_SUPPRESS
        return (uint8_t*)(this);
    }

    std::string ToString() const;

    static UniqueId CalcMd5Hash(const std::string& content);
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_UNIQUEID_H_
