/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "SampleReporter.h"

namespace HCXX {

SampleReporter::SampleReporter(const Workspace& workspace) : Reporter(workspace, ".log") {}

void SampleReporter::Init(Parser& parser, const std::string& resultPath, uint32_t initFlags) {}

void SampleReporter::RegisterProblemImpl(const Problem& problem)
{
    std::cerr << "This is sample reporter\n"
              << "Problem: " << problem.description << " in " << problem.filename << " on " << problem.line << "\n";
}

}  // namespace HCXX

extern "C" std::shared_ptr<HCXX::Reporter> CreateReporter(const HCXX::Workspace& workspace)
{
    return std::make_shared<HCXX::SampleReporter>(workspace);
}
