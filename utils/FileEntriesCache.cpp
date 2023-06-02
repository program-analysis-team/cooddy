/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <deps/md5.h>
#include <utils/EnvironmentUtils.h>
#include <utils/FileEntry.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace HCXX {

FileEntriesCache& FileEntriesCache::GetInstance()
{
    static FileEntriesCache instance;
    return instance;
}

UniqueId UniqueId::CalcMd5Hash(const std::string& source)
{
    MD5_CTX md5ctx;
    MD5_Init(&md5ctx);
    MD5_Update(&md5ctx, source.data(), source.size());

    UniqueId result;
    MD5_Final(result.Data(), &md5ctx);
    return result;
}

FileEntry* FileEntriesCache::GetFileEntry(const std::string& sourcePath,
                                          const std::function<bool(std::string&)>& sourceProvider)
{
    std::unique_lock<std::mutex> lock(myMutex);
    auto it = myEntries.find(sourcePath);
    if (it != myEntries.end()) {
        return &it->second;
    }
    std::string source;
    if (sourceProvider != nullptr) {
        if (!sourceProvider(source)) {
            return nullptr;  // LCOV_EXCL_LINE
        }
    } else {
        std::ifstream fileStream(sourcePath, std::ios_base::ate | std::ios_base::binary);
        if (!fileStream.good()) {
            return nullptr;  // LCOV_EXCL_LINE
        }
        size_t fileSize = fileStream.tellg();
        if (fileSize >= UINT32_MAX) {
            return nullptr;  // LCOV_EXCL_LINE
        }
        source.resize(fileSize);
        fileStream.seekg(0);
        fileStream.read(&source[0], source.size());
    }
    auto& entry = myEntries[sourcePath];
    entry.fileSize = source.size();
    entry.hash = UniqueId::CalcMd5Hash(source);
    entry.fileSource = std::move(source);
    entry.filePath = EnvironmentUtils::GetCanonicalPath(sourcePath);

    for (size_t p = 0, newp = 0; newp != std::string::npos; p = newp + 1) {
        newp = entry.fileSource.find('\n', p);
        entry.lineOffsets.emplace_back(p);
    }
    return &entry;
}

void FileEntriesCache::RemoveEntry(const std::string& sourcePath)
{
    auto path = EnvironmentUtils::GetCanonicalPath(sourcePath);

    std::unique_lock<std::mutex> lock(myMutex);
    for (auto it = myEntries.begin(); it != myEntries.end();) {
        if (it->second.filePath == path) {
            auto tmp = it++;
            myEntries.erase(tmp);
        } else {
            ++it;
        }
    }
    myEntries.erase(sourcePath);
}

void FileEntriesCache::Serialize(IOStream& stream)
{
    std::unique_lock<std::mutex> lock(myMutex);
    if (stream.IsOut()) {
        stream.Add<uint32_t>(myEntries.size());
        for (auto& it : myEntries) {
            FileEntry& entry = it.second;
            stream.AddString(entry.hash.ToString().c_str());
            stream.AddString(std::filesystem::path(entry.filePath).filename().string().c_str());
            stream << entry.fileSize << entry.lineOffsets;
        }
    } else {
        auto numEntries = stream.Get<uint32_t>();
        for (auto i = 0; i < numEntries; ++i) {
            std::string hash = stream.GetString();
            FileEntry& entry = myEntries[hash];
            entry.filePath = stream.GetString();
            stream << entry.fileSize << entry.lineOffsets;
            entry.isLoaded = true;
            entry.fileSource = std::string(entry.fileSize, ' ');
            entry.isFakeSource = true;
            for (auto lineOffset : entry.lineOffsets) {
                if (lineOffset != 0) {
                    entry.fileSource[lineOffset - 1] = '\n';
                }
            }
            for (auto j = 0; j < hash.size(); j += 2) {
                entry.hash.Data()[j / 2] = std::strtol(hash.substr(j, 2).c_str(), NULL, 16);
            }
        }
    }
}

}  // namespace HCXX
