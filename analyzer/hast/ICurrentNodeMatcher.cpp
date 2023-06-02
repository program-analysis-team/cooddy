/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "ICurrentNodeMatcher.h"

#include <ast/Node.h>
#include <utils/StrUtils.h>

#include <algorithm>
#include <optional>
#include <regex>
#include <string_view>

#include "AndNodeMatcher.h"
#include "AttributeMatcher.h"
#include "CurrentNodeMatcherCompiler.h"
#include "NodeTypeMatcher.h"

namespace HCXX::Hast::Details {

std::unique_ptr<ICurrentNodeMatcher> CreateNodeTypeMatcher(std::string_view nodeType, std::string* errorMsg)
{
    std::optional<HCXX::Node::Kind> kind(HCXX::Node::KindFromStrWithCheck(nodeType));

    if (!kind.has_value()) {
        if (errorMsg != nullptr) {
            errorMsg->clear();
            *errorMsg += "Node type '";
            *errorMsg += nodeType;
            *errorMsg += "' isn't supported";
        }
        return std::unique_ptr<ICurrentNodeMatcher>();
    }

    return std::make_unique<NodeTypeMatcher>(kind.value());
}

static const CurrentNodeMatcherCompiler g_COMPILER;

//  Node description may look like
//  -   NodeType
//  -   [@AttributeName='AttributeValue']
//  -   [@AttributeName_1='AttributeValue_1' AND ... @AttributeName_N='AttributeValue_N']
//  -   NodeType[@AttributeName='AttributeValue']
//  -   NodeType[@AttributeName_1='AttributeValue_1' AND ... @AttributeName_N='AttributeValue_N']
std::shared_ptr<ICurrentNodeMatcher> ICurrentNodeMatcher::Create(std::string_view str, std::string* errorMsg,
                                                                 std::string_view::size_type& usedCharsAmount)
{
    auto compileRes = g_COMPILER.Compile(str);
    if (!compileRes.has_value()) {
        if (errorMsg != nullptr) {
            *errorMsg = "XPath step format is incorrect or not supported: '";
            *errorMsg += str;
            *errorMsg += "'";
        }
        return std::shared_ptr<ICurrentNodeMatcher>();
    }

    usedCharsAmount = compileRes.value().numUsedChars;

    std::vector<std::shared_ptr<ICurrentNodeMatcher>> matchers;

    std::string nodeType = compileRes.value().nodeType;
    if (!nodeType.empty()) {
        auto ptr = CreateNodeTypeMatcher(nodeType, errorMsg);
        // LCOV_EXCL_START
        if (!ptr) {
            return std::shared_ptr<ICurrentNodeMatcher>();
        }
        // LCOV_EXCL_STOP
        matchers.emplace_back(std::move(ptr));
    }

    {
        const auto& attrsVec = compileRes.value().attributes;
        for (const auto& item : attrsVec) {
            auto ptr = AttributeMatcher::Create(item.first, item.second);

            // LCOV_EXCL_START
            if (!ptr) {
                return std::shared_ptr<ICurrentNodeMatcher>();
            }
            // LCOV_EXCL_STOP
            matchers.emplace_back(std::move(ptr));
        }
    }

    if (matchers.empty()) {
        // LCOV_EXCL_START
        return std::shared_ptr<ICurrentNodeMatcher>();
        // LCOV_EXCL_STOP
    }

    if (matchers.size() == 1) {
        std::shared_ptr<ICurrentNodeMatcher> ptr;
        std::swap(ptr, matchers[0]);
        return ptr;
    }

    return std::make_shared<AndNodeMatcher>(std::move(matchers));
}

}  //  namespace HCXX::Hast::Details
