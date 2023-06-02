/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_HTMLSEPARATEREPORTER_H
#define COODDY_HTMLSEPARATEREPORTER_H
#include <ProblemsHolder.h>

#include <filesystem>
#include <fstream>

#include "workspace/Workspace.h"
namespace HCXX {
class RichHTMLSeparateReporter {
public:
    // Create html report for problem trace and return path to the report
    virtual std::string AddProblem(const Problem& problem) = 0;

    virtual ~RichHTMLSeparateReporter() = default;

    static std::unique_ptr<RichHTMLSeparateReporter> Create(const Workspace& workspace, const std::string& reportPath);
};
}  // namespace HCXX
#endif  // COODDY_HTMLSEPARATEREPORTER_H
