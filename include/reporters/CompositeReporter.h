/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_REPORTERS_COMPOSITEREPORTER_H_
#define COODDY_ANALYZER_INCLUDE_REPORTERS_COMPOSITEREPORTER_H_
#include <utils/ThreadPool.h>

#include <string>
#include <vector>

#include "Parser.h"
#include "Reporter.h"

namespace HCXX {
class CompositeReporter : public Reporter {
public:
    CompositeReporter(const std::vector<std::string>& parsedReporters, const Workspace& workspace);
    ~CompositeReporter();

    void RegisterProblemImpl(const Problem& problem) override;
    void Init(Parser& parser, const std::string& resultPath, uint32_t initFlags) override;
    void Flush() override;

private:
    std::shared_ptr<Reporter> LoadCustomReporter(const Workspace& workspace, const std::string& arg);

    void CallRegisterProblemImpl(const Problem& problem);

    struct RegisterProblemTask : public HCXX::ThreadPool::Task {
        RegisterProblemTask(CompositeReporter& owner, Problem&& problem) : owner(owner), problem(std::move(problem)) {}
        ~RegisterProblemTask() = default;

        void Start() override
        {
            owner.CallRegisterProblemImpl(problem);
        }
        CompositeReporter& owner;
        Problem problem;
    };

    std::vector<std::shared_ptr<Reporter>> myReporters;
    ThreadPool myThreadPool;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_REPORTERS_COMPOSITEREPORTER_H_
