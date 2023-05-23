/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_TOOL_TAINTMACROCLIDEFINITION_H_
#define COODDY_ANALYZER_TOOL_TAINTMACROCLIDEFINITION_H_

#include <string>
#include <string_view>

/// Produce a CLI option for GCC which defines specified macro as an AST annotation "__cooddy_security_risk".
inline std::string TaintMacroCliDefinition(std::string_view taintMacro, std::string_view annotation)
{
    std::string result;
    result += "-D";
    result += taintMacro;
    result += "=__attribute__((annotate(\"";
    result += annotation;
    result += "\")))";
    return result;
}

#endif  // COODDY_ANALYZER_TOOL_TAINTMACROCLIDEFINITION_H_
