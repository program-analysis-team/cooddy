/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CLANG_FACADE_CONVERTORS_H_
#define COODDY_ANALYZER_SOURCE_CLANG_FACADE_CONVERTORS_H_

#include <ast/Node.h>
#include <clang/AST/RecordLayout.h>
#include <clang/Lex/Lexer.h>

namespace HCXX {

Location ConvertLocation(clang::SourceManager& sm, const clang::SourceLocation& location);

SourceRange ConvertSourceRange(clang::SourceManager& sm, const clang::LangOptions& langOptions,
                               const clang::SourceRange& sourceRange);

}  // namespace HCXX

#endif
