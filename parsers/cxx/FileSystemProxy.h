/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of llvm::vfs::ProxyFileSystem to support in-memory source files.
//
#ifndef COODDY_ANALYZER_SOURCE_CLANG_FACADE_FILESYSTEMPROXY_H_
#define COODDY_ANALYZER_SOURCE_CLANG_FACADE_FILESYSTEMPROXY_H_

#include <llvm/Support/VirtualFileSystem.h>
#include <utils/EnvironmentUtils.h>
#include <utils/FileEntry.h>

#include "PreambleData.h"

class FileSystemProxy : public llvm::vfs::ProxyFileSystem {
    std::mutex myMutex;

public:
    explicit FileSystemProxy() : llvm::vfs::ProxyFileSystem(llvm::vfs::createPhysicalFileSystem().release()) {}
    ~FileSystemProxy() = default;

    void AddFile(const std::string& filePath, std::string&& fileContent)
    {
        std::string normalizedPath = HCXX::EnvironmentUtils::NormalizePath(filePath);
        std::unique_lock<std::mutex> lock(myMutex);
        auto entry = HCXX::FileEntriesCache::GetInstance().GetFileEntry(normalizedPath, [&](std::string& content) {
            content = fileContent;
            return true;
        });
        if (entry != nullptr) {
            const_cast<HCXX::FileEntry*>(entry)->fileSource = std::move(fileContent);
        }
    }

    static llvm::vfs::Status GetStatus(const HCXX::FileEntry& entry)
    {
        return llvm::vfs::Status(entry.filePath, llvm::sys::fs::UniqueID(0, uintptr_t(&entry)), {}, 0, 0,
                                 entry.fileSource.size(), {}, {});
    }

    llvm::ErrorOr<llvm::vfs::Status> status(const llvm::Twine& path) override
    {
        std::string normalizedPath = HCXX::EnvironmentUtils::NormalizePath(path.str());
        auto p = normalizedPath.find_last_of('.');
        if (p != std::string::npos && normalizedPath.compare(p + 1, 3, "jar") == 0) {
            // LCOV_EXCL_START
            // check that path contains '.jar' and it's not the final file in the archive
            return llvm::vfs::Status(path.str(), {}, {}, 0, 0, 0, llvm::sys::fs::file_type::directory_file, {});
            // LCOV_EXCL_STOP
        }
        std::unique_lock<std::mutex> lock(myMutex);
        auto entry =
            HCXX::FileEntriesCache::GetInstance().GetFileEntry(normalizedPath, [](std::string&) { return false; });
        if (entry != nullptr) {
            return GetStatus(*entry);
        }
        lock.unlock();
        return getUnderlyingFS().status(path);
    }

    llvm::ErrorOr<std::unique_ptr<llvm::vfs::File>> openFileForRead(const llvm::Twine& inPath) override
    {
        std::string normalizedPath = HCXX::EnvironmentUtils::NormalizePath(inPath.str());
        std::unique_lock<std::mutex> lock(myMutex);
        auto entry = HCXX::FileEntriesCache::GetInstance().GetFileEntry(normalizedPath);
        if (entry != nullptr) {
            return std::make_unique<File>(GetStatus(*entry), entry->fileSource);
        }
        lock.unlock();
        return getUnderlyingFS().openFileForRead(inPath);
    }

protected:
    class File : public llvm::vfs::File {
    public:
        File(const llvm::vfs::Status status, std::string content)
            : myStatus(std::move(status)), myContent(std::move(content))
        {}
        ~File() {}
        // LCOV_EXCL_START
        std::error_code close() override
        {
            return std::error_code();
        }
        // LCOV_EXCL_STOP

        llvm::ErrorOr<llvm::vfs::Status> status() override
        {
            return myStatus;
        }

        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> getBuffer(const llvm::Twine& name, int64_t fileSize,
                                                                     bool requiresNullTerminator,
                                                                     bool /*isVolatile*/) override
        {
            return llvm::MemoryBuffer::getMemBufferCopy(myContent, "");
        }

    private:
        llvm::vfs::Status myStatus;
        std::string myContent;
    };
};

#endif  // COODDY_ANALYZER_SOURCE_CLANG_FACADE_FILESYSTEMPROXY_H_
