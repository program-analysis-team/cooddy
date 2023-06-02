/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef CURRENTNODEMATCHERCOMPILER_HEADER_FILE
#define CURRENTNODEMATCHERCOMPILER_HEADER_FILE

#include <optional>
#include <regex>
#include <string>

namespace HCXX::Hast::Details {

class CurrentNodeMatcherCompiler {
public:
    struct CompileResult {
        std::string nodeType;
        std::vector<std::pair<std::string, std::string>> attributes;
        std::string_view::size_type numUsedChars = 0;
    };

public:
    CurrentNodeMatcherCompiler();
    std::optional<CompileResult> Compile(std::string_view expression) const;

private:
    bool ProcessNonFirstAttributes(std::string_view str,
                                   std::vector<std::pair<std::string, std::string>>& attributes) const;

private:
    std::regex myRegex;
    std::regex myNonFirstAttributesRegex;
};

}  // namespace HCXX::Hast::Details

#endif  // #ifndef CURRENTNODEMATCHERCOMPILER_HEADER_FILE
