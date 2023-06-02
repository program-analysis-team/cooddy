/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_PARSER_COMPILERINCLUDECASH_H_
#define COODDY_ANALYZER_SOURCE_PARSER_COMPILERINCLUDECASH_H_
#include <CompilerOptions.h>

#include <filesystem>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace HCXX {
class CompilersInfo {
public:
    CompilersInfo();
    struct CompilerInfo {
        std::string triple;
        std::vector<std::string> includes;
        std::vector<std::string> defines;
    };

    CompilerInfo& GetCompilerInfo(CompilerOptions& compilerOptions);

private:
    CompilerInfo CollectNewCompilerInfo(const std::string& compiler, std::string&& extraOptions);
    bool IsCorrectCompilerBinary(const std::string& binary);
    bool ShouldAddCompilerMacro(const std::string& define);
    std::string GetBinaryPath(const std::string& binaryName) const;

    std::unordered_map<std::string, CompilerInfo> myCompilers;
    std::mutex myMutex;
    const std::string myDefaultCCompilerName;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_SOURCE_PARSER_COMPILERINCLUDECASH_H_
