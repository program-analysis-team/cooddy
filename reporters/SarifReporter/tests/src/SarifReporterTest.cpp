/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <reporters/CompositeReporter.h>
#include <utils/StrUtils.h>
#include <utils/TempFile.h>

#include <deps/json.hpp>
#include <filesystem>

#include "CheckerTest.h"
#include "TestBaseClass.h"

using namespace HCXX;
using Json = nlohmann::json;
namespace fs = std::filesystem;

class SarifReporterTest : public TestBaseClass {
public:
    Json Analyze(std::string_view id)
    {
        fs::path outputPath = fs::temp_directory_path();
        outputPath /= id;
        outputPath += ".sarif";
        TempFile output(outputPath);

        Workspace workspace(TEST_SUITE_PATH(id));

        {
            CompositeReporter compositeReporter({"sarif"}, workspace);
            compositeReporter.Init(TestBaseClass::GetParser(), outputPath.string(), 0);
            TestAnalyze(workspace, compositeReporter);
        }  // Result written to file here

        Json j;
        std::ifstream ifs;
        ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        ifs.open(outputPath);
        ifs >> j;

        ValidateBasicStructure(j);

        return j;
    }

    void ValidateBasicStructure(const Json& sarif)
    {
        ASSERT_TRUE(sarif.contains("version"));
        ASSERT_TRUE(sarif.contains("$schema"));

        ASSERT_EQ(sarif["runs"].size(), 1);
        const Json& run = sarif["runs"][0];

        const Json& tool = run["tool"];
        ASSERT_EQ(tool["driver"]["name"], "Cooddy");
        ASSERT_TRUE(tool["driver"].contains("version"));
        ASSERT_TRUE(tool["driver"]["properties"].contains("revisionTag"));

        ASSERT_EQ(run["invocations"].size(), 1);
        const Json& invocation = run["invocations"][0];

        ASSERT_EQ(invocation["executionSuccessful"], true);
        ASSERT_EQ(invocation["exitCode"], 0);
        ASSERT_TRUE(invocation.contains("commandLine"));
        ASSERT_TRUE(invocation.contains("commandLine"));
        ASSERT_TRUE(invocation["executableLocation"].contains("uri"));
        ASSERT_TRUE(invocation.contains("arguments"));
        ASSERT_TRUE(invocation.contains("startTimeUtc"));
        ASSERT_TRUE(invocation.contains("endTimeUtc"));
        ASSERT_TRUE(invocation["properties"].contains("profileUri"));
        ASSERT_TRUE(invocation["properties"].contains("annotationUris"));

        ASSERT_EQ(run["columnKind"], "unicodeCodePoints");

        for (const Json& result : run["results"]) {
            ASSERT_TRUE(result["message"].contains("text"));
            ASSERT_TRUE(result.contains("level"));
            ASSERT_TRUE(result.contains("ruleId"));
            ASSERT_TRUE(result["fingerprints"].contains("cooddy.uid"));

            ASSERT_EQ(result["locations"].size(), 1);
            ValidateLocationObject(result["locations"][0]);

            if (result.contains("codeFlows")) {
                ASSERT_EQ(result["codeFlows"].size(), 1);
                ASSERT_EQ(result["codeFlows"][0]["threadFlows"].size(), 1);
                ASSERT_GE(result["codeFlows"][0]["threadFlows"][0]["locations"].size(), 1);

                for (const Json& location : result["codeFlows"][0]["threadFlows"][0]["locations"]) {
                    ValidateLocationObject(location["location"]);
                }
            }
        }
    }

    void ValidateLocationObject(const Json& location)
    {
        ASSERT_TRUE(location["physicalLocation"]["artifactLocation"].contains("uri"));

        if (location["physicalLocation"].contains("region")) {
            const Json& region = location["physicalLocation"]["region"];

            ASSERT_TRUE(region.contains("startLine"));
            ASSERT_TRUE(region.contains("endLine"));
            ASSERT_TRUE(region.contains("startColumn"));
            ASSERT_TRUE(region.contains("endColumn"));
        }
    }
};

TEST_F(SarifReporterTest, KitchenSink)
{
    Analyze("KitchenSink");
}

TEST_F(SarifReporterTest, CustomAnnotations)
{
    Json sarif = Analyze("CustomAnnotations");
    Json& annotations = sarif["runs"][0]["invocations"][0]["properties"]["annotationUris"];
    std::string uri0 = annotations[0];

    ASSERT_TRUE(StrUtils::EndsWith(uri0, "CustomAnnotations/.cooddy/.annotations.json"));
}
