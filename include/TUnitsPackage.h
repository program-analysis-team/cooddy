/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_TUNITSPACKAGE_H_
#define COODDY_ANALYZER_INCLUDE_TUNITSPACKAGE_H_

#include <TranslationUnit.h>

#include <string>

namespace HCXX {

class CompilerOptionsList;

class TUnitsPackage {
public:
    using EntryId = uint32_t;
    using Ptr = std::shared_ptr<TUnitsPackage>;

    static Ptr Create(const CompilerOptionsList& compilerOptions);

    static Ptr CreateEmptyASTContainer(const std::string& archivePath);

    static Ptr CreateArchive(const std::string& archivePath);

    virtual ~TUnitsPackage() = default;

    virtual uint32_t GetEntriesCount() = 0;

    virtual std::string GetEntryName(EntryId entry) = 0;

    virtual std::shared_ptr<TranslationUnit> GetTUnit(EntryId entry) = 0;

    virtual void AddTUnit(std::shared_ptr<TranslationUnit>& unit){};  // LCOV_EXCL_LINE

    virtual Ptr GetPackage(EntryId entry) = 0;

    virtual void InitTUnit(TranslationUnit& unit, EntryId entry) {}

    virtual bool GetMetaInfo(IOStream& stream)
    {
        return false;
    }
    virtual void AddMetaInfo(IOStream& stream){};  // LCOV_EXCL_LINE
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_TUNITSPACKAGE_H_
