/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "ProjectPaths.h"

#include <iostream>

std::filesystem::path& GetProjectRootImpl()
{
    static std::filesystem::path projectRoot;
    return projectRoot;
}

std::filesystem::path& GetBinaryFolderImpl()
{
    static std::filesystem::path binaryFolder;
    return binaryFolder;
}

void ProjectPaths::Init(std::string_view exePath)
{
    GetBinaryFolderImpl() = std::filesystem::canonical(exePath).remove_filename();
    GetProjectRootImpl() = GetBinaryFolder().parent_path().parent_path().parent_path();
}

std::filesystem::path ProjectPaths::GetTestCasesPath(std::string_view testFilePath, std::string_view subDirectory)
{
    std::filesystem::path tmp(testFilePath);
    tmp = tmp.parent_path().parent_path();
    if (tmp.filename() != "tests") {
        tmp /= "tests";
    }
    return tmp.append("suite").append(subDirectory);
}

const std::filesystem::path& ProjectPaths::GetProjectRoot()
{
    return GetProjectRootImpl();
}

const std::filesystem::path& ProjectPaths::GetBinaryFolder()
{
    return GetBinaryFolderImpl();
}
