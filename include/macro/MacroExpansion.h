/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_MACROINFO_H_
#define COODDY_ANALYZER_INCLUDE_AST_MACROINFO_H_

#include <ast/Node.h>

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace HCXX {

class MacroExpansion {
public:
    MacroExpansion(const SourceRange& range, const std::vector<std::vector<std::string>>&& args)
        : myRange(range), myArguments(std::move(args))
    {}

    ~MacroExpansion() = default;

    const SourceRange& GetRange() const
    {
        return myRange;
    }

    std::size_t GetArgumentsNum() const
    {
        return myArguments.size();
    }

    // LCOV_EXCL_START
    std::string GetArgument(std::size_t pos) const
    {
        return pos < myArguments.size()
                   ? std::accumulate(myArguments[pos].begin(), myArguments[pos].end(), std::string())
                   : "";
    }

    const std::vector<std::string>& GetTokensList(std::size_t argIdx) const
    {
        // TODO check out-of-range
        return myArguments[argIdx];
    }
    // LCOV_EXCL_STOP

    bool IsTokenInArgument(const std::string& token, std::size_t argIdx) const
    {
        if (argIdx >= myArguments.size()) {
            return false;  // LCOV_EXCL_LINE: HCAT-2777
        }

        return std::find(myArguments[argIdx].begin(), myArguments[argIdx].end(), token) != myArguments[argIdx].end();
    }

private:
    SourceRange myRange;
    std::vector<std::vector<std::string>> myArguments;
};

}  // namespace HCXX
#endif  // COODDY_ANALYZER_INCLUDE_AST_MACROINFO_H_
