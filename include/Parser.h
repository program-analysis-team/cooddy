/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Declaration of a frontend parser interface.
//

#ifndef HCXXANALYZER_TRANSLATIONUNIT_H
#define HCXXANALYZER_TRANSLATIONUNIT_H

#include <Checker.h>
#include <TranslationUnit.h>
#include <ast/CxxRecordDecl.h>
#include <ast/Node.h>
#include <utils/json-cpp.h>

#include <memory>
#include <set>
#include <vector>

namespace HCXX {
class DeclResolver;

class Parser {
public:
    enum ParseFlags { INCLUDES_ONLY = 1, CREATE_PREAMBLE = 2, DUMP_AST = 4, IN_MEMORY_FILES = 8 };
    enum ParseStages { DEFINITIONS, USAGES, AST };

    // LCOV_EXCL_START
    struct Consumer {
        virtual uint32_t GetParseFlags()
        {
            return myParseFlags;
        }
        void SetParseFlags(uint32_t parseFlags)
        {
            myParseFlags = parseFlags;
        }
        virtual void OnIncludeParsed(std::string includePath, std::string inclusionSource, std::string prolog) {}
        virtual void OnParseError() {}

        virtual ~Consumer() = default;

    private:
        uint32_t myParseFlags = 0;
    };

    struct VirtualFunctionsConsumer {
        using Handle = void*;
        virtual Handle OnClassDeclaration(RecordUniqueId classId, const std::vector<RecordUniqueId>& baseClasses)
        {
            return nullptr;
        }
        virtual void OnVirtualFunction(Handle classHandle, const std::string& name, const std::string& signature){};
    };

    using Context = HCXX::Context;

    virtual std::unique_ptr<Context> CreateDefinitionsContext(VirtualFunctionsConsumer& consumer)
    {
        return nullptr;
    }
    virtual bool ParseDefinitions(HCXX::TranslationUnit& unit, Context* context = nullptr)
    {
        return false;
    }
    virtual void ParseUsages(HCXX::TranslationUnit& unit){};

    virtual std::unique_ptr<Context> CreateContext(DeclResolver* declResolver = nullptr) = 0;

    virtual bool ParseAST(HCXX::TranslationUnit& unit, Consumer& consumer, Context* context = nullptr) = 0;
    bool ParseAST(HCXX::TranslationUnit& unit, Context* context = nullptr)
    {
        Consumer consumer;
        return ParseAST(unit, consumer, context);
    }

    virtual void AddInMemoryFile(const std::string& filePath, std::string fileContent) {}

    /// Convert frontend compiler options to the Parser internal format
    virtual void ConvertOptions(CompilerOptions& options) {}

    /// Returns qualified name converted from mangled name
    virtual std::string GetQualifiedName(std::string_view mangledName) const
    {
        return std::string{mangledName.begin(), mangledName.end()};
    }

    /// Returns mangled symbol name without abi info
    virtual std::string GetMangledNameWithoutAbi(std::string_view mangledName) const
    {
        return std::string{mangledName.begin(), mangledName.end()};
    }

    virtual ~Parser() = default;

    /// Create Parser instance based on 'lang' option
    /// If 'lang' option is not specified, CxxParser will be created
    static std::unique_ptr<Parser> Create(const CompilerOptions& globalOptions = CompilerOptions(),
                                          const std::string& scopePath = "");
    // LCOV_EXCL_STOP
    struct ParserStatistics {
        struct CompilationIssue {
            std::string tu;
            std::string file;
            std::string message;
            std::string severity;
            uint32_t line;
            uint32_t column;

            DECLARE_FIELDS("tu", tu, "file", file, "message", message, "line", line, "column", column, "severity",
                           severity);
        };

        std::vector<CompilationIssue> compilationIssues;
        uint32_t maxFatalErrorCount = 0;
        std::atomic<uint64_t> totalParsedCount = 0;
        std::atomic<uint64_t> succeedCount = 0;
        std::atomic<uint64_t> skippedCount = 0;
        std::atomic<uint64_t> failedCount = 0;
        std::atomic<uint64_t> partiallyParsedCount = 0;
        std::atomic<uint64_t> failedCFGCount = 0;
        std::set<std::string> unknownOptionsSet;
        std::mutex mutex;
    };

    ParserStatistics statistics;
};

};  // namespace HCXX

#endif  // HCXXANALYZER_TRANSLATIONUNIT_H
