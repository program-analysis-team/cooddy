/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "AndNodeMatcher.h"

namespace HCXX::Hast::Details {

AndNodeMatcher::AndNodeMatcher(std::vector<std::shared_ptr<ICurrentNodeMatcher>>&& matchers)
    : ICurrentNodeMatcher(), myMatchers(std::move(matchers))
{}

bool AndNodeMatcher::CheckMatch(const Node& node) const
{
    for (const auto& ptr : myMatchers) {
        if (!ptr->CheckMatch(node)) {
            return false;
        }
    }

    return true;
}

}  // namespace HCXX::Hast::Details
