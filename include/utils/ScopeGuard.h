/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_SCOPEGUARD_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_SCOPEGUARD_H_

#include <utility>

namespace HCXX {

/// Utility class for calling executing code on scope end, whether normally or by exception.
template <typename F>
class ScopeGuard {
public:
    explicit ScopeGuard(F&& closure) : myClosure(std::forward<F>(closure)) {}
    ScopeGuard(const ScopeGuard&) = delete;

    ~ScopeGuard()
    {
        myClosure();
    }

private:
    F myClosure;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_SCOPEGUARD_H_
