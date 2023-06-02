/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_TEMPFILE_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_TEMPFILE_H_

#include <utils/json-cpp.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>

namespace HCXX {
class TempFile {
public:
    explicit TempFile(const std::filesystem::path& path) : myFileName(path.string()) {}
    TempFile() = delete;
    TempFile(const std::filesystem::path& path, const std::string& body) : TempFile(path)
    {
        std::ofstream s(myFileName, std::ios_base::ate | std::ios_base::binary);
        s << body;
    }
    ~TempFile()
    {
        if (myStream.is_open()) {
            myStream.close();
        }
        remove(myFileName.c_str());
    }

    operator std::filesystem::path() const
    {
        return myFileName;
    }

    operator std::string() const
    {
        return myFileName;
    }
    operator std::string_view() const
    {
        return myFileName;
    }
    bool IsEmpty()
    {
        if (!Exists()) {
            return true;  // LCOV_EXCL_LINE: HCAT-2777
        }
        myStream.seekg(0, std::ios::end);
        return myStream.tellg() == 0;
    }
    bool Exists()
    {
        if (!myStream.is_open()) {
            myStream.open(myFileName);
        }
        return myStream.is_open();
    }
    std::string GetLine(int n = 1)
    {
        if (!myStream.is_open()) {
            myStream.open(myFileName);
        }
        std::string result;
        for (int i = 0; i < n; i++) {
            if (!myStream.good()) {
                return "";
            }
            std::getline(myStream, result);
        }
        return result;
    }
    template <typename T>
    T GetJson()
    {
        if (!myStream.is_open()) {
            myStream.open(myFileName);
        }
        T result;
        jsoncpp::parse<T>(result, myStream);
        return result;
    }

private:
    std::ifstream myStream;
    std::string myFileName;
};
class TempDir {
public:
    explicit TempDir(const std::filesystem::path& path) : myDirName(path.string()) {}
    TempDir() = delete;
    ~TempDir()
    {
        std::filesystem::remove_all(myDirName);
    }

private:
    std::string myDirName;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_TEMPFILE_H_
