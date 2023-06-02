/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_COMPILEROPTIONSLIST_H_
#define COODDY_ANALYZER_INCLUDE_COMPILEROPTIONSLIST_H_

#include <TranslationUnit.h>

#include <filesystem>
#include <string>
#include <vector>

namespace HCXX {

class CompilerOptionsList {
public:
    using Replacements = std::vector<std::pair<std::string, std::string>>;

    bool ParseScopeFile(std::filesystem::path scopePath, const Replacements& replacements);

    void AddOptions(const CompilerOptions& options)
    {
        myOptions.emplace_back(options);
    }

    void AddExtraOptions(const std::vector<std::string>& extraOptions)
    {
        for (auto& it : myOptions) {
            it.options.insert(it.options.end(), extraOptions.begin(), extraOptions.end());
        }
    }

    const std::vector<CompilerOptions>& Get() const
    {
        return myOptions;
    }

private:
    void ParseResponseFile(const std::string& curDir, const std::string& rspFile, CompilerOptions& options);

    void ParseCommandLine(const std::string& curDir, CompilerOptions& options, std::string& command);

    /**
     * @brief Sort TU units by compilation target and bitness.
     *
     * Sorting TUs solves a rare problem when:
     * 1. A source file 'A' is compiled into several TUs with different target/bitness;
     * 2. Another TU 'B' links to a TU from this file (which means it will pick the first one in this list);
     * 3. 'A' and 'B' have different target/bitness, which leads to problems.
     *
     * After sorting, TUs with the same target/bitness will all be at the top, which ensures that Cooddy will only
     * analyze TUs which belong together.
     *
     * Note: this fix assumes that if 'A' and 'B' are supposed to be linked, they have the same set of target/bitness
     * variations (e.g. both have variations x32/x64, or arm64/x64). If 'A' has x32/x64 and 'B' only has x32, this
     * can still lead to bugs, but this situation does not happen in real-life usage.
     *
     * See also hcpp_ts/OutOfBoundsChecker/compile_options_fp.json test.
     */
    void SortTUnitsByOptions();

    std::vector<CompilerOptions> myOptions;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_COMPILEROPTIONSLIST_H_
