/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef ICURRENT_MODE_MATCHER_HEADER_FILE
#define ICURRENT_MODE_MATCHER_HEADER_FILE

#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace HCXX {

class Node;

namespace Hast::Details {

class ICurrentNodeMatcher {
public:
    static std::shared_ptr<ICurrentNodeMatcher> Create(std::string_view str, std::string* errorMsg,
                                                       std::string_view::size_type& usedCharsAmount);

    virtual bool CheckMatch(const Node& node) const = 0;

    virtual ~ICurrentNodeMatcher() = default;
};

}  // namespace Hast::Details
}  // namespace HCXX

#endif  //   #ifndef ICURRENT_MODE_MATCHER_HEADER_FILE
