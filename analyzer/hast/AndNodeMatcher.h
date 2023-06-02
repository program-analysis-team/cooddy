/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef AND_NODE_MATCHER_HEADER_FILE
#define AND_NODE_MATCHER_HEADER_FILE

#include <memory>
#include <vector>

#include "ICurrentNodeMatcher.h"

namespace HCXX::Hast::Details {

class AndNodeMatcher : public ICurrentNodeMatcher {
public:
    explicit AndNodeMatcher(std::vector<std::shared_ptr<ICurrentNodeMatcher>>&& matchers);

    virtual bool CheckMatch(const Node& node) const override;

private:
    std::vector<std::shared_ptr<ICurrentNodeMatcher>> myMatchers;
};

}  // namespace HCXX::Hast::Details

#endif  // #ifndef AND_NODE_MATCHER_HEADER_FILE
