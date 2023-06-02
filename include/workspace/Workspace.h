/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Class Workspace provides interfaces for access to project specific options:
// compile commands, exclusions, checkers' options, functions' resources
//

#ifndef COODDY_ANALYZER_INCLUDE_WORKSPACE_H_
#define COODDY_ANALYZER_INCLUDE_WORKSPACE_H_

#include <Checker.h>
#include <CompilerOptionsList.h>
#include <dfa/Annotation.h>
#include <utils/DataStream.h>
#include <workspace/ExclusionManager.h>
#include <workspace/GenAnnoSettings.h>
#include <workspace/IncrementalSettings.h>
#include <workspace/Profile.h>

#include <filesystem>
#include <istream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "utils/EnvironmentUtils.h"

namespace HCXX {

class CompilerOptionsList;
class Parser;
class Node;
class FunctionDecl;

struct WorkspaceOptions {
    static constexpr std::string_view GEN_ANNOTATIONS_DEFAULT_PATH = ".generated_annotations.json";

    bool ignoreSuppress = false;
    Problem::Severity problemSeverityLevel = Problem::Severity::NOTIFY;
    HCXX::GenAnnoSettings genAnnoSettings = GenAnnoSettings::Default();
    std::filesystem::path genAnnoPath = GEN_ANNOTATIONS_DEFAULT_PATH;
    std::filesystem::path projectRoot;
    IncrementalSettings incrementalSettings;
    std::string commandLine;
    std::string profile;
    CustomSettings settings;
    CompilerOptionsList::Replacements compileOptionReplacements;
};

class Workspace {
public:
    struct FunctionInfo {
        std::string displayName;
        FunctionAnnotation annotations;
    };
    Workspace(std::filesystem::path rootDir = EnvironmentUtils::GetSelfExecutableDir(),
              const std::unordered_set<string>& checkers = {}, const WorkspaceOptions& options = {},
              std::string_view profileName = {}, const std::filesystem::path& workingDir = ".");
    // LCOV_EXCL_START
    ~Workspace() = default;
    // LCOV_EXCL_STOP

    static void Init(std::string_view defaultProfile = {});

    CompilerOptionsList GetCompilerOptions() const;

    const ExclusionManager& GetExclusionManager() const
    {
        return *myExclusionManager;
    }

    const Profile& GetProfile() const
    {
        return *myProfile;
    }

    Checker::Config GetConfiguration(std::string_view checkerName);

    /**
     * @brief Create or replace a configuration for this checker with 'data' as the JSON config.
     *
     * Existing configuration will be completely removed.
     */
    void SetConfiguration(std::string_view checkerName, std::string_view data);

    bool GetAnalysisData(const std::string& sourceHash, DataStream& dataStream) const;

    void StoreAnalysisData(const std::string& sourceHash, DataStream& dataStream) const;

    void ClearUnusedAnalysisData(const std::unordered_set<std::string>& sourceHashes);

    bool GetAnnotation(const std::string& function, FunctionAnnotation& annotation, UnitLanguage lang);

    void GenerateFunctionAnnotations(std::vector<const DfaFunctionContext*>& globalFuncs);

    void SetEnabledCheckers(const std::unordered_set<string>& checkerNames);

    static const char* GetWorkspaceFolderName()
    {
        return WORKSPACE_ROOT;
    }

    const WorkspaceOptions& GetOptions() const
    {
        return myOptions;
    }

    const std::filesystem::path& GetWorkspacePath() const
    {
        return myWorkspaceRoot;
    }

    std::string GetProjectRoot() const;
    std::string GetRelativePath(const std::string& path, bool toProjectRoot = false) const;

    //  The parameters
    //  -   relativePath - relative(from the workspace root) path to the configuration file
    //  -   readOnly specifies if user is going to use file for reading or writing
    std::string GetFilePath(const std::filesystem::path& relativePath, bool readOnly = true) const;

    std::vector<std::string> GetConfigurationFileNames() const
    {
        std::vector<std::string> res;
        res.reserve(myConfigurationFileNames.size() + myDefaultWorkspace->myConfigurationFileNames.size());
        for (auto& a : myConfigurationFileNames) {
            res.push_back(a);
        }
        for (auto& a : myDefaultWorkspace->myConfigurationFileNames) {
            res.push_back(a);
        }
        return res;
    }

private:
    static std::unique_ptr<Workspace> myDefaultWorkspace;

    void LoadCustomAnnotations();

    void LoadSettings();

    void SaveAnnotations(const std::unordered_map<string, FunctionInfo>& annoToExport,
                         const std::filesystem::path& path);
    bool IsAnnotationShouldBeGenerated(const DfaFunctionContext* funcContext);

    void LoadAnnotations(const std::string& annotationsPath, UnitLanguage lang);

    bool IsDefault()
    {
        return this == myDefaultWorkspace.get();
    }

    void GetInspectionsProfile(std::string_view profileName);

    static constexpr const char* EXCLUSIONS_FILE_NAME = ".cooddy-ignore";
    static constexpr const char* ANNOTATIONS_FILE_NAME = ".annotations.json";
    static constexpr const char* ANNOTATIONS_JAVA_FILE_NAME = ".java-annotations.json";
    static constexpr const char* WORKSPACE_ROOT = ".cooddy";
    static constexpr const char* DEFAULT_PROFILE = "default";
    static constexpr const char* SETTINGS_PROFILE = "settings";
    static constexpr const char* STORAGE_FOLDER = "storage/";

    std::filesystem::path myRootDir;
    std::filesystem::path myWorkingDir;
    std::vector<std::string> myConfigurationFileNames;
    std::vector<std::unordered_map<std::string, FunctionInfo>> myAnnotations;
    std::filesystem::path myWorkspaceRoot;
    std::unique_ptr<Profile> myProfile;
    std::unique_ptr<ExclusionManager> myExclusionManager;
    WorkspaceOptions myOptions;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_WORKSPACE_H_
