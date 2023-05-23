/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include <CheckersFactory.h>
#include <workspace/Profile.h>

#include <filesystem>
#include <fstream>

#include "deps/span.hpp"

using namespace HCXX;
namespace fs = std::filesystem;

const fs::path& Profile::GetProfilePath() const
{
    return myPath;
}

void Profile::SetEnabledCheckers(const std::unordered_set<string>& checkerNames)
{
    for (auto& checkerCfg : myCheckersCfg) {
        bool isEnable = checkerNames.find(checkerCfg.first) != checkerNames.end();
        for (auto& inspectionCfg : checkerCfg.second.inspections) {
            inspectionCfg.isEnabled = isEnable;
        }
    }
}

bool Profile::IsCheckerEnabled(const std::string& checkerName) const
{
    auto it = myCheckersCfg.find(checkerName);
    if (it != myCheckersCfg.end()) {
        for (auto& inspectionCfg : it->second.inspections) {
            if (inspectionCfg.isEnabled) {
                return true;
            }
        }
    }
    return false;
}

const Profile::InspectionCfg& Profile::GetInspectionCfg(const std::string& checkerName,
                                                        const std::string& problemKind) const
{
    static InspectionCfg emptyCfg;
    auto it = myCheckersCfg.find(checkerName);
    if (it != myCheckersCfg.end()) {
        for (auto& inspectionCfg : it->second.inspections) {
            if (problemKind.empty() || problemKind == inspectionCfg.id) {
                return inspectionCfg;
            }
        }
    }
    return emptyCfg;
}

const Profile::CheckerCfg& Profile::GetCheckerCfg(const std::string& checkerName) const
{
    static CheckerCfg emptyCfg;
    auto it = myCheckersCfg.find(checkerName);
    return it != myCheckersCfg.end() ? it->second : emptyCfg;
}

void Profile::SetCheckerCfg(const std::string& checkerName, const CheckerCfg& config)
{
    myCheckersCfg.insert_or_assign(checkerName, config);
}

static fs::path FindProfile(std::string_view profileName, const std::vector<std::string>& searchPaths)
{
    fs::path nameAsPath(profileName);
    if (nameAsPath.is_absolute()) {
        return nameAsPath;
    }

    // Current algorithm: search for <name>.profile.json in each directory. <name> must not be a path of any kind.
    if (!nameAsPath.has_parent_path()) {
        for (fs::path candidate : searchPaths) {
            candidate.append(profileName).concat(".profile.json");
            if (fs::exists(candidate)) {
                return candidate;
            }
        }
    }

    // Deprecated algorithm: search for <relative-profile-path> and <relative-profile-path>.json in each directory.
    for (fs::path candidate1 : searchPaths) {
        candidate1 /= nameAsPath;

        fs::path candidate2 = candidate1;
        candidate2.concat(".json");

        for (auto& candidate : {candidate1, candidate2}) {
            if (fs::exists(candidate)) {
                Log(LogLevel::WARNING)
                    << "Profile found using a deprecated search algorithm: " << candidate
                    << ". Check out --help on more information on how to correctly specify a profile path.\n";
                return candidate;
            }
        }
    }
    return {};
}

void Profile::NormalizeConfig()
{
    for (auto& it : myCheckersCfg) {
        if (it.second.inspections.empty()) {
            it.second.inspections.emplace_back(it.second);
        }
        if (it.second.languages.empty()) {
            if (myLanguages.empty()) {
                it.second.languages.insert("c");
                it.second.languages.insert("c++");
            } else {
                it.second.languages = myLanguages;
            }
        }
        for (auto& inspectionCfg : it.second.inspections) {
            if (inspectionCfg.languages.empty()) {
                inspectionCfg.languages = it.second.languages;
            }
            if (inspectionCfg.profileName.empty()) {
                inspectionCfg.profileName = name;
            }
            if (inspectionCfg.description.empty()) {
                inspectionCfg.description =
                    StrLocales::GetStringLocale(inspectionCfg.id, std::vector<std::string>(), true);
            }
            if (inspectionCfg.description.empty()) {
                inspectionCfg.description = StrLocales::GetStringLocale(it.first);
            }
        }
    }
}

void Profile::LoadBase(const fs::path& profilePath)
{
    if (profilePath.empty()) {
        return;
    }
    myPath = profilePath;
    try {
        auto is = std::ifstream(myPath.string(), std::ios_base::ate);
        std::string jsonContent;
        jsonContent.resize(is.tellg());
        is.seekg(0);
        is.read(jsonContent.data(), jsonContent.size());
        jsoncpp::parse(*this, jsonContent, true);
        // LCOV_EXCL_START
    } catch (const std::exception& ex) {
        Log(HCXX::LogLevel::ERROR) << "Failed to read profile '" << myPath.string() << "' Error is " << ex.what()
                                   << "\n";
        // LCOV_EXCL_STOP
    }
    NormalizeConfig();
}

Profile::Profile(std::string_view profileName, const std::vector<std::string>& searchPaths,
                 std::string_view settingsProfile)
{
    CustomSettings settings;
    if (!settingsProfile.empty()) {
        LoadBase(FindProfile(settingsProfile, searchPaths));
    }
    if (profileName.empty()) {
        return;  // LCOV_EXCL_LINE (HCAT-2802)
    }
    std::swap(settings, mySettings);
    const fs::path& profilePath = FindProfile(profileName, searchPaths);
    LoadBase(profilePath);
    if (!profilePath.empty()) {
        Log(LogLevel::DEBUG) << "Profile found: " << profilePath << "\n";
    } else {
        Log(LogLevel::ERROR) << "No profile found: \"" << profileName << "\"\n";
    }
    settings.Merge(mySettings, true);
    std::swap(settings, mySettings);

    for (auto it = mySubProfiles.rbegin(); it != mySubProfiles.rend(); ++it) {
        Profile subProfile(*it, searchPaths);
        mySubProfilePaths.push_back(subProfile.myPath.string());
        for (auto& checkerCfg : subProfile.myCheckersCfg) {
            myCheckersCfg.emplace(checkerCfg.first, checkerCfg.second);
        }
    }
    if (!myModule.empty()) {
        CheckersFactory::GetInstance().AddModule(myModule);
    }
    if (!profilePath.empty()) {
        Log(HCXX::LogLevel::INFO) << "Profile is loaded: '" << myPath << "'\n";
    }
}
