/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CompilerOptions.h>
#include <Parser.h>

using namespace HCXX;

extern std::unique_ptr<Parser> CxxParserCreate(const CompilerOptions& globalOptions);
extern std::unique_ptr<Parser> JavaParserCreate();

std::unique_ptr<Parser> HCXX::Parser::Create(const CompilerOptions& globalOptions, const std::string& scopePath)
{
    if (globalOptions.HasOption("-x=java") || StrUtils::EndsWith(scopePath, ".java") ||
        StrUtils::EndsWith(scopePath, ".jar")) {
        return JavaParserCreate();
    }
    return CxxParserCreate(globalOptions);
}
