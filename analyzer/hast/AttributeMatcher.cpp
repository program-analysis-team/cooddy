/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "AttributeMatcher.h"

#include <ast/Node.h>

namespace HCXX::Hast::Details {

std::unique_ptr<ICurrentNodeMatcher> AttributeMatcher::Create(std::string_view attrName, std::string_view attrValue)
{
    return std::make_unique<AttributeMatcher>(attrName, attrValue);
}

AttributeMatcher::AttributeMatcher(std::string_view attrName, std::string_view attrValue)
    : ICurrentNodeMatcher(), m_attrName(attrName), m_attrValue(attrValue)
{}

bool AttributeMatcher::CheckMatch(const Node& node) const
{
    std::optional<std::string> currentNodeAttrValue = node.GetAttribute(m_attrName);
    if (!currentNodeAttrValue.has_value()) {
        return false;
    }

    if (currentNodeAttrValue.value() == m_attrValue) {
        return true;
    }

    return false;
}

}  // namespace HCXX::Hast::Details
