/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DFA_FORMATSTRINGKIND_H_
#define COODDY_ANALYZER_INCLUDE_DFA_FORMATSTRINGKIND_H_
#include <utils/EnumMapping.h>

namespace HCXX {

/// Enumeration for the user data for the "FormatString" annotation. Lowercase because it is written in .annotations
DECLARE_ENUM(FormatStringKind, INVALID, Printf, Scanf, Scanfs);

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_FORMATSTRINGKIND_H_
