/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef ATTRIBUTE_MATCHER_HEADER_FILE
#define ATTRIBUTE_MATCHER_HEADER_FILE

#include <optional>

#include "ICurrentNodeMatcher.h"

namespace HCXX::Hast::Details {

class AttributeMatcher : public ICurrentNodeMatcher {
public:
    static std::unique_ptr<ICurrentNodeMatcher> Create(std::string_view attrName, std::string_view attrValue);

public:
    AttributeMatcher(std::string_view attrName, std::string_view attrValue);

    virtual bool CheckMatch(const Node& node) const override;

private:
    std::string m_attrName;
    std::string m_attrValue;
};

}  // namespace HCXX::Hast::Details

#endif  // #ifndef ATTRIBUTE_MATCHER_HEADER_FILE
