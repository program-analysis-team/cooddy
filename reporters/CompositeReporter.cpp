/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "reporters/CompositeReporter.h"

#include "reporters/CsvReporter.h"
#include "reporters/JsonReporter.h"
#include "reporters/OutReporter.h"
#include "reporters/RichHtmlReporter.h"
#include "utils/SharedObjectUtils.h"

namespace HCXX {
CompositeReporter::CompositeReporter(const std::vector<std::string>& parsedReporters, const Workspace& workspace)
    : Reporter(workspace, ""), myThreadPool(1)
{
    for (auto& repArg : parsedReporters) {
        std::shared_ptr<Reporter> reporter;
        if (StrUtils::StartsWith(repArg, "html")) {
            reporter = std::make_shared<RichHtmlReporter>(workspace);
        } else if (StrUtils::StartsWith(repArg, "json")) {
            reporter = std::make_shared<JsonReporter>(workspace);
        } else if (repArg == "csv" || repArg == "csv-html") {
            reporter = std::make_shared<CsvReporter>(workspace);
        } else if (repArg == "out") {
            reporter = std::make_shared<OutReporter>(workspace);
        } else {
            reporter = LoadCustomReporter(workspace, std::move(repArg));
        }
        if (reporter == nullptr) {
            reporter = std::make_shared<OutReporter>(workspace);
        }
        myReporters.push_back(reporter);
    }
}

CompositeReporter::~CompositeReporter()
{
    myThreadPool.WaitAll();
}

void CompositeReporter::Flush()
{
    myThreadPool.WaitAll();
    Reporter::Flush();
    for (auto& rep : myReporters) {
        rep->Flush();
    }
}

void CompositeReporter::Init(Parser& parser, const std::string& resultPath, uint32_t initFlags)
{
    Reporter::Init(parser, resultPath, initFlags);
    for (auto& rep : myReporters) {
        rep->Init(parser, resultPath, initFlags);
    }
}

void CompositeReporter::RegisterProblemImpl(const Problem& problem)
{
    myThreadPool.AddTask(std::make_shared<RegisterProblemTask>(*this, std::move(const_cast<Problem&>(problem))));
}

std::shared_ptr<Reporter> CompositeReporter::LoadCustomReporter(const Workspace& workspace, const std::string& arg)
{
    std::string reporterClass = arg + "Reporter";
    reporterClass[0] = toupper(reporterClass[0]);
    std::string reporterLib = "lib" + reporterClass;
    std::filesystem::path modulePath = EnvironmentUtils::GetSelfExecutableDir() / reporterLib;
    auto dllReporters = SharedObjectUtils::LoadSharedObject(modulePath.string());
    if (dllReporters == nullptr) {
        Log(LogLevel::ERROR) << "Can't load reporter lib: " << modulePath.string() << "\n";
        return nullptr;
    }
    using TCreateReporterFunc = std::shared_ptr<HCXX::Reporter> (*)(const HCXX::Workspace& workspace);
    auto createReporterFunc = SharedObjectUtils::GetSymbol<TCreateReporterFunc>(dllReporters, "CreateReporter");
    if (createReporterFunc == nullptr) {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "Can't find 'CreateReporter' function in " << modulePath.string() << "\n";
        return nullptr;
        // LCOV_EXCL_STOP
    }
    auto reporter = createReporterFunc(workspace);
    if (reporter == nullptr) {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "Can't create reporter " << reporterClass << "\n";
        return nullptr;
        // LCOV_EXCL_STOP
    } else {
        Log(LogLevel::INFO) << "Selected reporter is " << reporterClass << "\n";
        return reporter;
    }
}

void CompositeReporter::CallRegisterProblemImpl(const Problem& problem)
{
    Reporter::AddProblemTrace(problem);
    for (auto& rep : myReporters) {
        rep->RegisterProblemImpl(problem);
    }
}

}  // namespace HCXX
