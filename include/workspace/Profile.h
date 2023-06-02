/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_PROFILE_H
#define COODDY_PROFILE_H

#include <ProblemsHolder.h>
#include <utils/Configuration.h>

#include <filesystem>
#include <string>
#include <string_view>

namespace HCXX {

struct CustomSettings {
    std::unordered_set<std::string> annotations;
    std::vector<std::string> rhsServers;
    std::string resultManUrl;
    std::string taintManUrl;

    void Merge(const CustomSettings& rhe, bool replaceRhsServers)
    {
        annotations.insert(rhe.annotations.begin(), rhe.annotations.end());
        if (replaceRhsServers && !rhe.rhsServers.empty() || rhsServers.empty()) {
            rhsServers = rhe.rhsServers;
        }
        if (!rhe.resultManUrl.empty()) {
            resultManUrl = rhe.resultManUrl;
        }
        if (!rhe.taintManUrl.empty()) {
            taintManUrl = rhe.taintManUrl;
        }
    }
};

class Profile {
public:
    struct InspectionCfg {
        std::string id;
        std::string profileName;
        std::string description;
        std::unordered_set<std::string> languages;
        Problem::Severity severity = Problem::Severity::NOTIFY;
        bool isEnabled = true;
        bool noTrace = false;

        DECLARE_FIELDS("id", id, "description", description, "severity",
                       MakeTypeSerializer(severity, Problem::SeverityFromStrWithCheck), "enabled", isEnabled,
                       "no_trace", noTrace, "languages", languages)
    };

    struct CheckerCfg : public InspectionCfg {
        std::vector<InspectionCfg> inspections;

        struct Settings {
            /// file name in default folder or full/relative path to config file
            std::string config;
            std::string inplaceJson;

            template <class X>
            // COODDY_SUPPRESS CamelCaseChecker
            void serialize(jsoncpp::Parser<X>& stream)
            {
                auto curIter = stream.getCurIter();
                jsoncpp::fields(*this, stream, "config", config);
                if (config.empty()) {
                    inplaceJson = std::string(--curIter, stream.getCurIter());
                }
            }
        } settings;

        DECLARE_FIELDS("id", id, "description", description, "severity",
                       MakeTypeSerializer(severity, Problem::SeverityFromStrWithCheck), "enabled", isEnabled,
                       "no_trace", noTrace, "settings", settings, "inspections", inspections, "languages", languages)

        bool IsEmpty() const
        {
            return id.empty();
        }
    };

    explicit Profile(std::string_view profileName = "", const std::vector<std::string>& searchPaths = {},
                     std::string_view settingsProfile = "");

    ~Profile() = default;

    bool IsEmpty() const
    {
        return myCheckersCfg.empty();
    }

    const std::filesystem::path& GetProfilePath() const;

    void SetEnabledCheckers(const std::unordered_set<string>& checkerNames);

    bool IsCheckerEnabled(const std::string& checkerName) const;

    const InspectionCfg& GetInspectionCfg(const std::string& checkerName, const std::string& problemKind = {}) const;

    const CheckerCfg& GetCheckerCfg(const std::string& checkerName) const;

    /// Create or replace a CheckerCfg for this checkerName.
    void SetCheckerCfg(const std::string& checkerName, const CheckerCfg& config);

    const std::string& GetTaintOptions() const
    {
        return myTaintOptions;
    }

    const std::string& GetProblemSeverityLevel() const
    {
        return myProblemSeverityLevel;
    }

    std::vector<std::string> GetFiles() const
    {
        std::vector<std::string> res;
        res.reserve(mySubProfilePaths.size() + 1);
        res.push_back(myPath.string());
        for (auto& s : mySubProfilePaths) {
            res.push_back(s);
        }
        return res;
    }

    const CustomSettings& GetSettings() const
    {
        return mySettings;
    }

    DECLARE_FIELDS("name", name, "module", myModule, "annotations", mySettings.annotations, "taint-options",
                   myTaintOptions, "min-problem-severity", myProblemSeverityLevel, "rhs-servers", mySettings.rhsServers,
                   "profiles", mySubProfiles, "checkers", myCheckersCfg, "languages", myLanguages, "result_man_url",
                   mySettings.resultManUrl, "taint_man_url", mySettings.taintManUrl)

    std::string name;

private:
    /// Load base profile, excluding subprofiles.
    void LoadBase(const std::filesystem::path& profilePath);

    /// Replace missing fields in configs with defaults.
    void NormalizeConfig();

    std::filesystem::path myPath;
    std::string myModule;
    std::string myTaintOptions;
    std::string myProblemSeverityLevel;
    std::unordered_set<std::string> myLanguages;
    std::vector<std::string> mySubProfiles;
    std::vector<std::string> mySubProfilePaths;
    std::unordered_map<std::string, CheckerCfg> myCheckersCfg;
    CustomSettings mySettings;
};
}  // namespace HCXX

#endif  // COODDY_PROFILE_H
