/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_COMPILEROPTIONS_H_
#define COODDY_ANALYZER_INCLUDE_COMPILEROPTIONS_H_
#include <utils/Log.h>

#include <algorithm>
#include <string>
#include <vector>

namespace HCXX {
struct CompilerOptions {
    std::vector<std::string> options;
    std::string compiler;
    std::string directory;

    std::string GetFilePath() const
    {
        return options.empty() ? "" : options[0];
    };
    std::string& operator[](int index)
    {
        return options[index];
    }

    const std::string& operator[](int index) const
    {
        return options[index];
    }

    bool operator!=(const CompilerOptions& other) const
    {
        return options != other.options;
    }

    bool RemoveOption(const std::string& option)
    {
        bool wasRemoved = false;
        auto it = std::find(options.begin(), options.end(), option);
        while (it != options.end()) {
            options.erase(it);
            it = std::find(options.begin(), options.end(), option);
            wasRemoved = true;
        }
        return wasRemoved;
    }

    bool HasOption(const std::string& option) const
    {
        return std::find(options.begin(), options.end(), option) != options.end();
    }

    std::string GetOptionValue(const std::string& option) const
    {
        auto s = option.size();
        for (auto i = 0; i < options.size(); ++i) {
            auto& op = options[i];
            if (op.compare(0, s, option) == 0) {
                return op[s] == '=' ? op.substr(s + 1) : (++i != options.size() ? options[i] : "");
            }
        }
        return "";
    }

    CompilerOptions operator+(const CompilerOptions& right) const
    {
        CompilerOptions result = *this;
        if (!right.options.empty()) {
            result.options.insert(result.options.end(), right.options.begin(), right.options.end());
        }
        // Considering right-hand CompilerOptions with higher priority
        if (result.compiler.empty() || !right.compiler.empty()) {
            result.compiler = right.compiler;
        }
        return result;
    }

    uint64_t GetMemUsage() const
    {
        uint64_t result = sizeof(CompilerOptions) + options.capacity() * sizeof(std::string);
        for (auto& it : options) {
            result += it.capacity();
        }
        return result;
    }
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_COMPILEROPTIONS_H_
