/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#include "reporters/OutReporter.h"

namespace HCXX {

OutReporter::OutReporter(const Workspace& workspace) : Reporter(workspace, ".log") {}

void OutReporter::Init(Parser& parser, const std::string& resultPath, uint32_t initFlags)
{
    if (!resultPath.empty() && resultPath != std::filesystem::current_path().string()) {
        Reporter::Init(parser, resultPath, initFlags);
    } else {
        myParser = &parser;
    }
}

template <class TLog>
void OutReporter::LogProblem(TLog& log, const Problem& problem)
{
    log << "Problem found: " << (problem.kind.empty() ? problem.checkerName : problem.kind) << ":"
        << problem.description << " \n";
    log << "\tFile: " << problem.filename << ":" << problem.line << ":" << problem.column;

    if (problem.trace.size() > 1) {
        log << ", Trace(" << problem.solverDuration << "):";
        auto traceReport = Reporter::AddProblemTrace(problem);
        if (!traceReport.empty()) {
            log << "\n\t\tReport: " << traceReport;
        } else {
            auto it = problem.trace.rbegin();
            while (++it != problem.trace.rend()) {
                if (it->kind == TraceNode::Kind::DEFAULT) {
                    log << "\n\t\tFile: " << it->filename << ":" << it->line << ":" << it->column;
                }
            }
        }
    }
    log << std::endl;
}

void OutReporter::RegisterProblemImpl(const Problem& problem)
{
    if (myFileStream.is_open()) {
        LogProblem(myFileStream, problem);
    } else {
        LogLevel level = LogLevel::ERROR;
        if (problem.severity == Problem::Severity::WARNING) {
            level = LogLevel::WARNING;  // LCOV_EXCL_LINE (HCAT-2802)
        } else if (problem.severity == Problem::Severity::NOTIFY) {
            level = LogLevel::INFO;
        }
        Log log(level);
        LogProblem(log, problem);
    }
}

}  // namespace HCXX
