/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef NODE_TYPE_MATCHER_HEADER_FILE
#define NODE_TYPE_MATCHER_HEADER_FILE

#include <ast/Node.h>

#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "ICurrentNodeMatcher.h"

namespace HCXX::Hast::Details {

class NodeTypeMatcher : public ICurrentNodeMatcher {
public:
    explicit NodeTypeMatcher(HCXX::Node::Kind nodeType);

    NodeTypeMatcher(const NodeTypeMatcher&) = default;
    NodeTypeMatcher(NodeTypeMatcher&&) = default;

    NodeTypeMatcher& operator=(const NodeTypeMatcher&) = default;
    NodeTypeMatcher& operator=(NodeTypeMatcher&&) = default;

    ~NodeTypeMatcher() = default;

    virtual bool CheckMatch(const Node& node) const override;

private:
    HCXX::Node::Kind m_nodeType;
};

}  // namespace HCXX::Hast::Details

#endif  //  #ifndef NODE_TYPE_MATCHER_HEADER_FILE
