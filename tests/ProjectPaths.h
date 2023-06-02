/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef PROJECT_PATHS_HEADER_FILE
#define PROJECT_PATHS_HEADER_FILE

#include <filesystem>
#include <string_view>

class ProjectPaths {  //  TODO: we should provide less ugly way to get path to the sources we use for testing
public:
    static void Init(std::string_view exePath);

    static std::filesystem::path GetTestCasesPath(std::string_view testFilePath, std::string_view subDirectory);

    static const std::filesystem::path& GetBinaryFolder();
    static const std::filesystem::path& GetProjectRoot();
};

#define TEST_SUITE_PATH(subPath) ProjectPaths::GetTestCasesPath(__FILE__, subPath)

#endif  //  #ifndef PROJECT_PATHS_HEADER_FILE
