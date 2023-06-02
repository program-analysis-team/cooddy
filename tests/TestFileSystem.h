/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_TESTFILESSYSTEM_HEADER_FILE
#define COODDY_TESTFILESSYSTEM_HEADER_FILE

#include <filesystem>

namespace HCXX::Test::FileSystem {

template <typename Action>
void ForEachFileInDirectory(const std::filesystem::path& path, const char* extention, Action action)
{
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.path().extension() == extention) {
            action(entry.path());
            // COODDY_SUPPRESS IndentChecker
        } else if (entry.is_directory()) {
            ForEachFileInDirectory(entry.path(), extention, action);
        }
    }
}

}  // namespace HCXX::Test::FileSystem

#endif  //  #ifndef COODDY_TESTFILESSYSTEM_HEADER_FILE
