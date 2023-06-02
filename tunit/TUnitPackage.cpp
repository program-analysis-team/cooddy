/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <CompilerOptionsList.h>
#include <TUnitsPackage.h>
#include <deps/miniz.h>
#include <utils/EnvironmentUtils.h>

namespace HCXX {

static bool IsSourceFile(const std::filesystem::path& ext)
{
    return ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".c++" || ext == ".cxx" || ext == ".java";
}

static bool IsArchiveFile(const std::filesystem::path& ext)
{
    return ext == ".zip" || ext == ".jar";
}

class ArchivePackage : public TUnitsPackage {
public:
    struct ZipReader {
        mz_zip_archive zipArchive;
        bool opened = true;

        explicit ZipReader(const ArchivePackage& package)
        {
            mz_zip_zero_struct(&zipArchive);
            opened = mz_zip_reader_init_file_v2(&zipArchive, package.myArchivePath.c_str(),
                                                MZ_DEFAULT_LEVEL | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY, 0, 0);
        }
        ~ZipReader()
        {
            if (opened) {
                mz_zip_reader_end(&zipArchive);
            }
        }
        bool GetData(EntryId entry, IOStream& stream, uint32_t size)
        {
            return mz_zip_reader_extract_to_mem(&zipArchive, entry, stream.GetData(size), size, MZ_DEFAULT_LEVEL);
        }
    };

    explicit ArchivePackage(const std::filesystem::path& archivePath) : myArchivePath(archivePath.string()) {}

    bool Open()
    {
        ZipReader zipReader(*this);
        if (!zipReader.opened) {
            // LCOV_EXCL_START
            Log(LogLevel::ERROR) << "Can't open archive file '" << myArchivePath.c_str() << "' ("
                                 << mz_zip_get_last_error(&zipReader.zipArchive) << ")\n";
            return false;
            // LCOV_EXCL_STOP
        }
        myEntries.resize(mz_zip_reader_get_num_files(&zipReader.zipArchive));
        for (auto i = 0; i < myEntries.size(); ++i) {
            mz_zip_archive_file_stat stat;
            if (mz_zip_reader_file_stat(&zipReader.zipArchive, i, &stat)) {
                myEntries[i] = {stat.m_filename, stat.m_uncomp_size};
            }
        }
        return true;
    }

    uint32_t GetEntriesCount() override
    {
        return myEntries.size();
    }

    std::string GetEntryName(EntryId entry) override
    {
        return myEntries[entry].first;
    }

    std::shared_ptr<TranslationUnit> GetTUnit(uint32_t entry) override
    {
        std::string entryPath = EnvironmentUtils::NormalizePath(myArchivePath + "/" + myEntries[entry].first);
        if (!IsSourceFile(std::filesystem::path(entryPath).extension())) {
            return nullptr;  // LCOV_EXCL_LINE
        }
        return std::make_shared<TranslationUnit>(CompilerOptions{{std::move(entryPath)}});
    }

    void InitTUnit(TranslationUnit& unit, EntryId entry) override
    {
        FileEntriesCache::GetInstance().GetFileEntry(unit.GetMainFileName(), [&](std::string& source) {
            IOStream stream(false);
            ZipReader(*this).GetData(entry, stream, myEntries[entry].second);
            source.assign(stream.GetData<const char>(), stream.Count());
            return true;
        });
    }

    std::shared_ptr<TUnitsPackage> GetPackage(uint32_t entry) override
    {
        return nullptr;
    }

protected:
    std::string myArchivePath;
    std::vector<std::pair<std::string, uint32_t>> myEntries;
};

class ASTContainer : public ArchivePackage {
public:
    explicit ASTContainer(ArchivePackage&& base, uint32_t entriesPos)
        : ArchivePackage(std::move(base)), myEntriesPos(entriesPos)
    {}

    std::shared_ptr<TranslationUnit> GetTUnit(uint32_t entry) override
    {
        std::string entryPath(myEntries[entry].first);
        auto fileEntry = FileEntriesCache::GetInstance().GetFileEntry(entryPath);
        if (fileEntry != nullptr) {
            entryPath = fileEntry->filePath;
        }
        if (!IsSourceFile(std::filesystem::path(entryPath).extension())) {
            return nullptr;  // LCOV_EXCL_LINE
        }
        return std::make_shared<TranslationUnit>(CompilerOptions{{std::move(entryPath)}});
    }

    void InitTUnit(TranslationUnit& unit, EntryId entry) override
    {
        if (!unit.IsInited()) {
            IOStream stream(false);
            ZipReader(*this).GetData(entry, stream, myEntries[entry].second);
            unit.SerializeUnit(stream);
        }
    }

    bool GetMetaInfo(IOStream& stream) override
    {
        ZipReader(*this).GetData(myEntriesPos, stream, myEntries[myEntriesPos].second);
        return true;
    }

private:
    uint32_t myEntriesPos;
};

class ASTEmptyContainer : public ArchivePackage {
public:
    explicit ASTEmptyContainer(ArchivePackage&& base) : ArchivePackage(std::move(base))
    {
        std::remove(myArchivePath.c_str());
        mz_zip_zero_struct(&myZipArchive);
        myOpened = mz_zip_writer_init_file_v2(&myZipArchive, myArchivePath.c_str(), 0,
                                              MZ_DEFAULT_LEVEL | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY);
        if (!myOpened) {
            Log(LogLevel::ERROR) << "Can't open ast dump file '" << myArchivePath << "'\n";  // LCOV_EXCL_LINE
        }
    }
    ~ASTEmptyContainer()
    {
        mz_zip_writer_finalize_archive(&myZipArchive);
        mz_zip_writer_end(&myZipArchive);
    }

    void AddTUnit(std::shared_ptr<TranslationUnit>& unit) override
    {
        IOStream stream(true);
        unit->SerializeUnit(stream);

        auto& entries = unit->GetFileEntries();
        if (!entries.empty()) {
            std::unique_lock<std::mutex> lock(myMutex);
            WriteStream(entries.begin()->second.hash.ToString().c_str(), stream);
            myUnits.emplace_back(unit);
        } else {
            Log(LogLevel::ERROR) << "Can't dump ast for " << unit->GetMainFileName() << std::endl;  // LCOV_EXCL_LINE
        }
    }

    uint32_t GetEntriesCount() override
    {
        return myUnits.size();
    }

    std::shared_ptr<TranslationUnit> GetTUnit(uint32_t entry) override
    {
        return myUnits[entry];
    }

    void AddMetaInfo(IOStream& stream) override
    {
        WriteStream("entries.dat", stream);
    }

private:
    void WriteStream(const char* name, IOStream& stream)
    {
        if (myOpened) {
            mz_zip_writer_add_mem_ex(&myZipArchive, name, stream.GetData(), stream.Count(), nullptr, 0,
                                     MZ_DEFAULT_LEVEL, 0, 0);
        }
    }

    std::mutex myMutex;
    mz_zip_archive myZipArchive;
    std::vector<std::shared_ptr<TranslationUnit>> myUnits;
    bool myOpened = false;
};

// If HCAT-2761 is fixed, remove these coverage comments
// LCOV_EXCL_START
class JARContainer : public ArchivePackage {
public:
    explicit JARContainer(ArchivePackage&& base) : ArchivePackage(std::move(base))
    {
        static std::atomic<uint32_t> instCounter = 0;
        myTmpFolder = myArchivePath;
        myTmpFolder = myTmpFolder.replace_filename("." + myTmpFolder.stem().string() + std::to_string(++instCounter));
        std::string commandLine = "java -jar " + EnvironmentUtils::GetInstalledJarPath("cfr.jar") + " " +
                                  myArchivePath + " --silent true --outputdir " + myTmpFolder.string();
        int result = std::system(commandLine.c_str());
        if (result != 0) {
            Log(LogLevel::ERROR) << "Can't start: " << commandLine << std::endl;  // LCOV_EXCL_LINE
        }
    }
    ~JARContainer()
    {
        std::filesystem::remove_all(myTmpFolder);
    }

    std::shared_ptr<TranslationUnit> GetTUnit(uint32_t entry) override
    {
        std::string entryPath = EnvironmentUtils::NormalizePath(myArchivePath + "/" + myEntries[entry].first);
        if (!StrUtils::EndsWith(entryPath, ".class") || entryPath.find('$') != std::string::npos) {
            return nullptr;  // LCOV_EXCL_LINE
        }
        return std::make_shared<TranslationUnit>(CompilerOptions{{std::move(entryPath)}});
    }

    void InitTUnit(TranslationUnit& unit, EntryId entry) override
    {
        FileEntriesCache::GetInstance().GetFileEntry(unit.GetMainFileName(), [&](std::string& source) {
            auto javaFilePath = myTmpFolder / myEntries[entry].first;
            javaFilePath.replace_extension(".java");
            std::ifstream fileStream(javaFilePath.string(), std::ios_base::ate | std::ios_base::binary);
            size_t fileSize = fileStream.tellg();
            if (!fileStream.good() || fileSize >= UINT32_MAX) {
                return false;
            }
            source.resize(fileSize);
            fileStream.seekg(0);
            fileStream.read(&source[0], source.size());
            return true;
        });
    }

private:
    std::filesystem::path myTmpFolder;
};
// LCOV_EXCL_STOP

static std::shared_ptr<TUnitsPackage> CreateArchivePackage(const std::filesystem::path& path)
{
    if (!IsArchiveFile(path.extension())) {
        return nullptr;
    }
    ArchivePackage package(path);
    if (!package.Open()) {
        return nullptr;  // LCOV_EXCL_LINE
    }
    auto count = package.GetEntriesCount();
    for (int i = count - 1; i >= 0; i--) {
        auto entryName = package.GetEntryName(i);
        if (entryName == "entries.dat") {
            return std::make_shared<ASTContainer>(std::move(package), i);
        }
        if (StrUtils::EndsWith(entryName, ".class")) {
            // If HCAT-2761 is fixed, remove this coverage comment
            return std::make_shared<JARContainer>(std::move(package));  // LCOV_EXCL_LINE
        }
    }
    return std::make_shared<ArchivePackage>(std::move(package));
}

class FolderPackage : public TUnitsPackage {
public:
    explicit FolderPackage(const std::filesystem::path& folderPath, const CompilerOptions& compilerOptions)
        : myCompilerOptions(compilerOptions)
    {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_directory() || IsSourceFile(entry.path().extension()) ||
                IsArchiveFile(entry.path().extension())) {
                myEntries.emplace_back(entry);
            }
        }
    }
    ~FolderPackage() = default;

    uint32_t GetEntriesCount() override
    {
        return myEntries.size();
    }

    std::string GetEntryName(EntryId entry) override
    {
        return myEntries[entry].path().filename().string();
    }

    std::shared_ptr<TranslationUnit> GetTUnit(uint32_t entry) override
    {
        if (myEntries[entry].is_directory()) {
            return nullptr;  // LCOV_EXCL_LINE
        }
        auto compilerOptions = CompilerOptions{{myEntries[entry].path().string()}} + myCompilerOptions;
        return std::make_shared<TranslationUnit>(compilerOptions);
    }

    std::shared_ptr<TUnitsPackage> GetPackage(uint32_t entry) override
    {
        auto& entryPath = myEntries[entry].path();
        if (myEntries[entry].is_directory()) {
            return std::make_shared<FolderPackage>(entryPath, myCompilerOptions);  // LCOV_EXCL_LINE
        }
        return CreateArchivePackage(entryPath);
    }

private:
    std::vector<std::filesystem::directory_entry> myEntries;
    const CompilerOptions& myCompilerOptions;
};

class CompilerDBPackage : public TUnitsPackage {
public:
    explicit CompilerDBPackage(const CompilerOptionsList& compilerOptions) : myCompilerOptions(compilerOptions) {}
    ~CompilerDBPackage() = default;

    uint32_t GetEntriesCount() override
    {
        return myCompilerOptions.Get().size();
    }

    std::string GetEntryName(EntryId entry) override
    {
        return myCompilerOptions.Get()[entry].GetFilePath();
    }

    std::shared_ptr<TranslationUnit> GetTUnit(uint32_t entry) override
    {
        return std::make_shared<TranslationUnit>(myCompilerOptions.Get()[entry]);
    }

    std::shared_ptr<TUnitsPackage> GetPackage(uint32_t entry) override
    {
        auto& compilerOptions = myCompilerOptions.Get()[entry];
        std::filesystem::path entryPath = compilerOptions.GetFilePath();
        if (std::filesystem::is_directory(entryPath)) {
            return std::make_shared<FolderPackage>(entryPath, compilerOptions);
        }
        return CreateArchivePackage(entryPath);
    }

private:
    const CompilerOptionsList& myCompilerOptions;
};

TUnitsPackage::Ptr TUnitsPackage::Create(const CompilerOptionsList& compilerOptions)
{
    return std::make_shared<CompilerDBPackage>(compilerOptions);
}

TUnitsPackage::Ptr TUnitsPackage::CreateEmptyASTContainer(const std::string& archivePath)
{
    return std::make_shared<ASTEmptyContainer>(ArchivePackage(archivePath));
}

TUnitsPackage::Ptr TUnitsPackage::CreateArchive(const std::string& archivePath)
{
    return CreateArchivePackage(archivePath);
}

};  // namespace HCXX
