/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CheckersFactory.h>
#include <CompilerOptionsList.h>
#include <deps/miniz.h>
#include <utils/EnvironmentUtils.h>
#include <utils/Log.h>
#include <utils/PathUtils.h>
#include <workspace/Workspace.h>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace HCXX {

std::unique_ptr<Workspace> Workspace::myDefaultWorkspace;

Workspace::Workspace(fs::path rootDir, const std::unordered_set<string>& checkers, const WorkspaceOptions& options,
                     std::string_view profileName, const fs::path& workingDir)
    : myRootDir(std::move(rootDir)), myOptions(options), myWorkingDir(fs::canonical(workingDir))
{
    if (!PathUtils::IsAbsolute(myRootDir)) {
        myRootDir = fs::canonical(myWorkingDir / myRootDir);
    }
    myAnnotations.resize(UnitLanguage::MAX_LANG);

    myWorkspaceRoot = myRootDir;
    if (!fs::is_directory(myWorkspaceRoot)) {
        myWorkspaceRoot = myWorkspaceRoot.parent_path();
    }
    myWorkspaceRoot.append(WORKSPACE_ROOT);

    LoadAnnotations(GetFilePath(ANNOTATIONS_FILE_NAME), UnitLanguage::C);
    LoadAnnotations(GetFilePath(ANNOTATIONS_JAVA_FILE_NAME), UnitLanguage::JAVA);

    GetInspectionsProfile(profileName);

    if (!checkers.empty()) {
        myProfile->SetEnabledCheckers(checkers);
    }
    if (myOptions.problemSeverityLevel == Problem::Severity::NOTIFY &&
        !GetProfile().GetProblemSeverityLevel().empty()) {
        // if severity level is not defined in options, then use value from profile
        myOptions.problemSeverityLevel =
            Problem::SeverityFromCaseInsensitiveString(GetProfile().GetProblemSeverityLevel());
    }
    if (myDefaultWorkspace != nullptr) {
        myExclusionManager =
            std::make_unique<ExclusionManager>(GetFilePath(EXCLUSIONS_FILE_NAME), myOptions.problemSeverityLevel);
        auto settings = myDefaultWorkspace->GetProfile().GetSettings();
        settings.Merge(myProfile->GetSettings(), true);
        myOptions.settings.Merge(settings, false);
    }
}

void Workspace::Init(std::string_view profileName)
{
    CheckersFactory::GetInstance().Init();
    myDefaultWorkspace = std::make_unique<Workspace>(EnvironmentUtils::GetSelfExecutableDir(),
                                                     std::unordered_set<string>{}, WorkspaceOptions{}, profileName);
}

void Workspace::LoadCustomAnnotations()
{
    auto& customAnnotations = myProfile->GetSettings().annotations;

    // todo: support custom annotations for Java
    for (const auto& annotation : customAnnotations) {
        fs::path annotationPath(annotation);
        fs::path path;
        if (annotationPath.has_parent_path() && PathUtils::IsRelative(annotationPath)) {
            path = myProfile->GetProfilePath().parent_path().append(annotation);
        } else {
            path = annotation;
        }
        LoadAnnotations(fs::exists(path) ? path.string() : GetFilePath(annotation), UnitLanguage::C);
    }
}

std::string Workspace::GetFilePath(const fs::path& relativePath, bool readOnly) const
{
    auto path = relativePath;
    if (!path.empty()) {
        path = fs::path(myWorkspaceRoot);
        path /= relativePath;
        if (!readOnly) {
            fs::create_directories(path.parent_path());
        } else if (!fs::exists(path)) {
            HCXX::Log(HCXX::LogLevel::DEBUG) << "File does not exists: '" << path.string() << "'" << std::endl;
            return std::string();
        }
    }
    return path.string();
}

CompilerOptionsList Workspace::GetCompilerOptions() const
{
    CompilerOptionsList result;
    fs::path scopePath(myRootDir);

    if (!result.ParseScopeFile(scopePath, myOptions.compileOptionReplacements)) {
        result.AddOptions({std::vector<string>(1, scopePath.string()), ""});
    }
    return result;
}

Checker::Config Workspace::GetConfiguration(std::string_view checkerName)
{
    auto& settings = myProfile->GetCheckerCfg(std::string(checkerName)).settings;

    if (!settings.config.empty()) {
        std::string configPath = GetFilePath(settings.config + ".json");
        if (configPath.empty() && myDefaultWorkspace != nullptr) {
            configPath = myDefaultWorkspace->GetFilePath(settings.config + ".json");
        }
        if (!configPath.empty()) {
            return std::make_unique<std::ifstream>(configPath);
        }
    } else if (!settings.inplaceJson.empty()) {
        auto result = std::make_unique<std::stringstream>();
        *result << settings.inplaceJson;
        result->seekg(0);
        return result;
    }

    return nullptr;
}

void Workspace::SetConfiguration(const std::string_view checkerName, std::string_view data)
{
    auto checkerNameString = std::string{checkerName};
    myProfile->SetCheckerCfg(checkerNameString, data);
}

void Workspace::GetInspectionsProfile(std::string_view profileName)
{
    if (profileName.empty() && myDefaultWorkspace != nullptr) {
        myProfile = std::make_unique<Profile>(myDefaultWorkspace->GetProfile());
        return;
    }

    std::vector<std::string> searchPaths{myWorkingDir.string(), myWorkspaceRoot.string()};
    if (myDefaultWorkspace != nullptr) {
        searchPaths.emplace_back(myDefaultWorkspace->myWorkspaceRoot.string());
    }
    myProfile = std::make_unique<Profile>(profileName, searchPaths, SETTINGS_PROFILE);

    for (auto& f : myProfile->GetFiles()) {
        myConfigurationFileNames.push_back(f);
    }
    LoadCustomAnnotations();
}

bool Workspace::GetAnalysisData(const std::string& sourceHash, DataStream& dataStream) const
{
    std::string dataPath = GetFilePath(STORAGE_FOLDER + sourceHash, false);
    if (dataPath.empty()) {
        return false;  // LCOV_EXCL_LINE: HCAT-2779
    }
    std::ifstream fileStream(dataPath, std::ios_base::ate | std::ios_base::binary);
    if (!fileStream.good()) {
        return false;
    }
    std::vector<char> fileBuff(fileStream.tellg());
    fileStream.seekg(0);
    fileStream.read(&fileBuff[0], fileBuff.size());

    size_t outLen = 0;
    auto outBuff = tinfl_decompress_mem_to_heap(fileBuff.data(), fileBuff.size(), &outLen, 0);
    if (outBuff == nullptr) {
        return false;
    }
    // COODDY_SUPPRESS
    dataStream.AddBuffer(reinterpret_cast<uint8_t*>(outBuff), outLen);
    free(outBuff);
    return true;
}

void Workspace::StoreAnalysisData(const std::string& sourceHash, DataStream& dataStream) const
{
    std::string dataPath = GetFilePath(STORAGE_FOLDER + sourceHash, false);
    if (dataPath.empty()) {
        // LCOV_EXCL_START
        return;
        // LCOV_EXCL_STOP
    }
    std::ofstream fileStream(dataPath, std::ios_base::binary);
    if (fileStream.good()) {
        size_t outLen = 0;
        auto outBuff = tdefl_compress_mem_to_heap(dataStream.GetData<char>(), dataStream.Count(), &outLen,
                                                  TDEFL_DEFAULT_MAX_PROBES);
        if (outBuff != nullptr) {
            // COODDY_SUPPRESS
            fileStream.write(reinterpret_cast<char*>(outBuff), outLen);
            free(outBuff);
        }
    }
}

void Workspace::ClearUnusedAnalysisData(const std::unordered_set<std::string>& sourceHashes)
{
    std::string dataPath = GetFilePath(STORAGE_FOLDER, false);
    for (const auto& entry : fs::directory_iterator(dataPath)) {
        if (sourceHashes.find(entry.path().filename().string()) == sourceHashes.end()) {
            fs::remove(entry.path());
        }
    }
}
std::string Workspace::GetProjectRoot() const
{
    if (myOptions.projectRoot.empty()) {
        myWorkspaceRoot.parent_path().string();
    }

    if (!myOptions.projectRoot.is_absolute()) {
        return (myWorkingDir / myOptions.projectRoot).string();
    }

    return myOptions.projectRoot.string();  // LCOV_EXCL_LINE
}
std::string Workspace::GetRelativePath(const std::string& path, bool toProjectRoot) const
{
    fs::path filePath(path);
    std::error_code ec;
    auto relativeTo =
        toProjectRoot && !myOptions.projectRoot.empty() ? myOptions.projectRoot : myWorkspaceRoot.parent_path();
    auto relativePath = fs::relative(filePath, relativeTo, ec);
    if (ec || relativePath.empty()) {
        relativePath = path;  // LCOV_EXCL_LINE
    }
    return relativePath.string();
}

};  // namespace HCXX
