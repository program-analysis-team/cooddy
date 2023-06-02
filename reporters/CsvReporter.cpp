/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#include "reporters/CsvReporter.h"

#include <filesystem>
#include <sstream>

#include "utils/CsvUtils.h"

using namespace HCXX;

namespace HCXX {

CsvReporter::CsvReporter(const Workspace& workspace) : Reporter(workspace, ".csv") {}

void CsvReporter::Init(Parser& parser, const std::string& resultPath, uint32_t initFlags)
{
    Reporter::Init(parser, resultPath, initFlags);
    CsvUtils::WriteRow(myFileStream, {"Path", "File", "Function", "Line", "Column", "Language", "Checker", "Profile",
                                      "Problem Type", "Source", "Reason", "Sink Function", "UniqueId", "Trace"});
}

std::string CsvReporter::ConvertProblemTrace(const TracePath& problemTrace)
{
    std::stringstream trace;
    bool skip = true;
    for (const auto& it : problemTrace) {
        if (it.kind == TraceNode::Kind::DEFAULT) {
            if (skip) {
                skip = false;
            } else {
                trace << "\n";
            }
            trace << it.filename << ":" << it.line << ":" << it.column;
        }
    }
    return trace.str();
}

void CsvReporter::RegisterProblemImpl(const Problem& problem)
{
    auto traceReport = Reporter::AddProblemTrace(problem);
    if (!traceReport.empty()) {
        std::filesystem::path tracePath(traceReport);
        traceReport =
            "=HYPERLINK(\"" + tracePath.parent_path().filename().string() + "/" + tracePath.filename().string() + "\")";
    } else {
        traceReport = ConvertProblemTrace(problem.trace);
    }
    CsvUtils::WriteRow(myFileStream,
                       {GetWorkspace().GetRelativePath(problem.filename, true),
                        std::filesystem::path(problem.filename).filename().string(), problem.functionName,
                        std::to_string(problem.line), std::to_string(problem.column), problem.codeLanguage,
                        problem.checkerName, problem.profileName, problem.kind, problem.sourceId, problem.reasonCode,
                        problem.sinkFunction, problem.uniqueId.ToString(), std::move(traceReport)});
}

}  // namespace HCXX
