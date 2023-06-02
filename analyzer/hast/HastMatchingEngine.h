/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef HASTMATCHER_HEADER_FILE
#define HASTMATCHER_HEADER_FILE

#include <functional>
#include <memory>
#include <set>
#include <string_view>

#include "SupportedLanguages.h"

namespace HCXX {

class Node;
class TranslationUnit;

class HastExpression {
public:
    virtual ~HastExpression() = default;

    virtual const std::set<const Node*>& GetMatchedResult() const = 0;
};

using HastExpressionPtr = std::unique_ptr<HastExpression, std::function<void(HastExpression*)>>;

class HastMatchingEngine {
public:
    HastExpressionPtr BuildExpression(const std::string_view expression, SupportedLanguages languages,
                                      std::string* errorMsg);

    void DeleteExpression(const HastExpression*);

    void MatchAll(const TranslationUnit& node);

public:
    HastMatchingEngine();
    ~HastMatchingEngine();

private:
    class Impl;

    std::unique_ptr<Impl> myImpl;
};

}  // namespace HCXX

#endif  // #ifndef HASTMATCHER_HEADER_FILE
