/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_PARSER_UTILS_H_
#define COODDY_ANALYZER_SOURCE_PARSER_UTILS_H_
#include <clang/AST/PrettyPrinter.h>

/// Static print policy for clang types, for consistent printing regardless of language mode
const clang::PrintingPolicy CLANG_PRINT_POLICY = []() {
    clang::PrintingPolicy policy(clang::LangOptions{});
    policy.SuppressTagKeyword = true;
    policy.Bool = true;
    policy.UseVoidForZeroParams = false;
    policy.Nullptr = true;
    policy.Restrict = false;
    policy.Alignof = true;
    policy.SplitTemplateClosers = false;
    policy.Half = false;
    policy.MSWChar = false;
    policy.FullyQualifiedName = true;
    policy.PrintCanonicalTypes = true;

    return policy;
}();

#endif  // COODDY_ANALYZER_SOURCE_PARSER_UTILS_H_
