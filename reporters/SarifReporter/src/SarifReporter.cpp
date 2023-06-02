/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "SarifReporter.h"

#include <utils/PathUtils.h>
#include <utils/StrUtils.h>
#include <utils/Utc.h>

#include <deps/json.hpp>
using Json = nlohmann::ordered_json;

using namespace HCXX;

// Helpers =============================================================================================================
namespace {

/// Static report skeleton.
const Json SCAFFOLDING = Json::parse(R"(
{
    "$schema": "https://raw.githubusercontent.com/oasis-tcs/sarif-spec/master/Schemata/sarif-schema-2.1.0.json",
    "version": "2.1.0",
    "runs": [{
        "tool": {
            "driver": {
                "name": "Cooddy",
                "version": ")" GIT_VERSION R"(",
                "properties": {
                    "revisionTag": ")" GIT_COMMIT R"("
                }
            }
        },
        "invocations": [{
            "executionSuccessful": true,
            "exitCode": 0
        }],
        "columnKind": "unicodeCodePoints",
        "results": []
    }]
}
)");

const Json TRACE_SCAFFOLDING = Json::parse(R"(
[{
    "threadFlows": [{
        "locations": []
    }]
}]
)");

std::string_view SarifLevel(Problem::Severity severity)
{
    switch (severity) {
        case Problem::Severity::ERROR:
            return "error";
        case Problem::Severity::WARNING:
            return "warning";
        case Problem::Severity::NOTIFY:
            return "note";
        // LCOV_EXCL_START
        case Problem::Severity::UNDEF:
            return "none";
        default:
            assert(false);  // One of the possible severities was not handled
            __builtin_unreachable();
            // LCOV_EXCL_STOP
    }
}

bool HasRegion(const LocationInfoBase& location)
{
    return location.line != location.endLine || location.column != location.endColumn;
}

Json SarifLocationBase(std::string_view uri, const LocationInfoBase& location)
{
    Json result;

    result["artifactLocation"]["uri"] = uri;

    if (HasRegion(location)) {
        result["region"] = {
            {"startLine", location.line},
            {"endLine", location.endLine},
            {"startColumn", location.column},
            {"endColumn", location.endColumn},
        };
    }

    Json trueResult;
    trueResult["physicalLocation"] = std::move(result);
    return trueResult;
}

Json SarifLocation(const Problem& problem)
{
    return SarifLocationBase(PathUtils::FileUri(problem.filename), problem);
}

Json SarifLocation(const TraceNode& tracePoint)
{
    Json result = SarifLocationBase(PathUtils::FileUri(tracePoint.filename), tracePoint);
    result["physicalLocation"]["region"]["message"]["text"] = tracePoint.description;

    return result;
}

}  // namespace

// Reporter implementation =============================================================================================
namespace HCXX {

SarifReporter::SarifReporter(const Workspace& workspace)
    : Reporter(workspace, ".sarif"), report(SCAFFOLDING), problems(report["runs"][0]["results"])
{
    Json& invocation = report["runs"][0]["invocations"][0];
    invocation["commandLine"] = StrUtils::Join(EnvironmentUtils::Argv(), ' ');
    invocation["executableLocation"]["uri"] = PathUtils::FileUri(EnvironmentUtils::GetSelfExecutablePath());

    invocation["arguments"] = nullptr;
    if (EnvironmentUtils::Argv().size() != 0) {
        invocation["arguments"] = EnvironmentUtils::Argv().subspan(1);  // LCOV_EXCL_LINE: Argv() is broken on test runs
    }

    Json& properties = invocation["properties"];
    properties["profileUri"] = PathUtils::FileUri(workspace.GetProfile().GetProfilePath());
    for (auto& path : myWorkspace.GetConfigurationFileNames()) {
        if (StrUtils::EndsWith(path, "annotations.json")) {
            properties["annotationUris"].push_back(PathUtils::FileUri(path));
        }
    }

    invocation["startTimeUtc"] = to_string(utc_clock::now());
}

SarifReporter::~SarifReporter()
{
    Json& invocation = report["runs"][0]["invocations"][0];
    invocation["endTimeUtc"] = to_string(utc_clock::now());

    myFileStream << std::setfill('\t') << std::setw(1) << report;
}

void SarifReporter::RegisterProblemImpl(const Problem& problem)
{
    Json result;
    result["message"]["text"] = problem.description;
    result["level"] = SarifLevel(problem.severity);
    result["ruleId"] = problem.kind;
    result["fingerprints"]["cooddy.uid"] = problem.uniqueId.ToString();
    result["locations"].push_back(SarifLocation(problem));

    // Trace -----------------------------------------------------------------------------------------------------------
    // A trace only makes sense when the problem has a region and is not is TU-general like RttiChecker
    if (HasRegion(problem)) {
        result["codeFlows"] = TRACE_SCAFFOLDING;
        Json& trace = result["codeFlows"][0]["threadFlows"][0]["locations"];
        for (auto& tracePoint : problem.trace) {
            Json tp;
            tp["location"] = SarifLocation(tracePoint);

            trace.push_back(std::move(tp));
        }
    }

    problems.push_back(std::move(result));
}

}  // namespace HCXX

extern "C" std::shared_ptr<HCXX::Reporter> CreateReporter(const HCXX::Workspace& workspace)
{
    return std::make_shared<HCXX::SarifReporter>(workspace);
}
