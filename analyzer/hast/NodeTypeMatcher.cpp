/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "NodeTypeMatcher.h"

namespace HCXX::Hast::Details {

NodeTypeMatcher::NodeTypeMatcher(HCXX::Node::Kind nodeType) : m_nodeType(nodeType) {}

bool NodeTypeMatcher::CheckMatch(const Node& node) const
{
    return node.IsKindOf(m_nodeType);
}

}  // namespace HCXX::Hast::Details
