/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Declaration of the cxx analyzer.
//

#ifndef HCXXANALYZER_ANALYZERIMPL_H
#define HCXXANALYZER_ANALYZERIMPL_H

#include <Analyzer.h>
#include <Checker.h>
#include <CrossTUContext.h>
#include <Parser.h>
#include <TUnitsPackage.h>
#include <utils/ThreadPool.h>

#include <vector>

#include "../dfa/DataFlowAnalyzer.h"
#include "DeclResolverImpl.h"

class AnalyzerImpl : public HCXX::Analyzer {
public:
    explicit AnalyzerImpl(HCXX::Parser& parser, HCXX::Workspace& workspace);
    ~AnalyzerImpl();

    void Analyze(const HCXX::CompilerOptionsList& unitsOptions, HCXX::ProblemsHolder& holder,
                 HCXX::Analyzer::Consumer& consumer) override;

    void AnalyzeAsync(const HCXX::CompilerOptionsList& unitsOptions, std::shared_ptr<HCXX::ProblemsHolder> holder,
                      std::shared_ptr<HCXX::Analyzer::Consumer> consumer) override;

private:
    void AnalyzeImpl(const CompilerOptionsList& unitsOptions, std::shared_ptr<ProblemsHolder> holder,
                     std::shared_ptr<Analyzer::Consumer> consumer, bool waitForCompletion);

    struct TUContext : public Context {
        uint32_t priority = 0;
        std::atomic<uint32_t> parseCount = 0;
        bool parseResult = true;
        bool parseFinished = false;
        bool isLoaded = false;
        std::future<void> future;
    };

    class AnalyzeContext : public DataFlowAnalyzer::TUnitPoolTask, public TUnitHandler {
    public:
        AnalyzeContext(AnalyzerImpl* analyzer, const HCXX::CompilerOptionsList& unitsOptions,
                       std::shared_ptr<HCXX::ProblemsHolder> holder, std::shared_ptr<Analyzer::Consumer> consumer);
        ~AnalyzeContext() = default;

        void Release();

        void WaitForCompletion();

        void StartProcessing(Parser::ParseStages parseStage);

        void ProcessUnit(TranslationUnitPtr& unit);

    private:
        void AnalyzeUnit(TranslationUnitPtr& unit);
        void ParseDefinitions(TranslationUnitPtr& unit);
        void RunAnalysis(TranslationUnitPtr& unit);
        void LoadMetaInfo(IOStream& stream);
        void StoreMetaInfo(IOStream& stream);
        bool LoadAnalysisData(TranslationUnitPtr& unit);
        void LoadCheckersData(TranslationUnitPtr& unit, IOStream& stream);
        bool LoadFileEntriesData(TranslationUnitPtr& unit, DataStream& dataStream);
        void StoreAnalysisData(TranslationUnitPtr& unit, const std::string& streamPath);
        void StoreFileEntriesData(TranslationUnitPtr& unit, DataStream& dataStream);
        void StoreCheckersData(TranslationUnitPtr& unit, IOStream& stream);
        void StoreAnalysisData();
        void OnTaskCompleted() override;
        std::string GetStreamHash(const TranslationUnit& unit, const FileEntry& entry);
        bool ParseAST(TranslationUnit& unit, bool fullParse);

        void TUnitCreated(std::shared_ptr<TranslationUnit> unit) override;
        // LCOV_EXCL_START
        void TUnitInitDecl(TranslationUnit& unit, const Node* declNode) override
        {
            if (declNode == nullptr) {
                ParseAST(unit, false);
            }
        }
        // LCOV_EXCL_STOP
        TUContext& GetTUContext(TranslationUnit& unit)
        {
            return unit.GetContext<TUContext>(myAnalyzer->myCheckers.size());
        }

        void TUnitPackage(TUnitsPackage::Ptr package) override;

        void TUnitIncreasePriority(TranslationUnit& unit) override
        {
            GetTUContext(unit).priority++;
        }

        static constexpr const char* AST_ARCHIVE_FILE = "ast-dump.zip";
        static constexpr const char* PROBLEMS_FILE = "problems.dat";

        AnalyzerImpl* myAnalyzer;
        DeclResolverImpl myResolver;
        std::shared_ptr<Analyzer::Consumer> myConsumer;
        std::shared_ptr<HCXX::ProblemsHolder> myHolder;
        std::shared_ptr<HCXX::TUnitsPackage> myDumpPackage;
        std::vector<TranslationUnitPtr> myUnits;
        std::unique_ptr<HCXX::Parser::Context> myParserContext;
        std::unique_ptr<HCXX::Parser::Context> myDefinitionsContext;
        std::atomic<uint32_t> myUnitsToProcess;
        Parser::ParseStages myParseStage = HCXX::Parser::DEFINITIONS;
        DataFlowAnalyzer::AnalyzeContext myDfaContext;
        std::promise<void> myPromise;
        std::atomic<uint32_t> myRefCount = 2;
        CrossTUContext myCrossTUContext;
        IOStream myMetaStream;
    };

    struct AnalyzeTask : public HCXX::ThreadPool::Task {
        AnalyzeContext& context;
        TranslationUnitPtr& unit;
        uint32_t& priority;

    public:
        AnalyzeTask(AnalyzeContext& context, TranslationUnitPtr& unit, uint32_t& priority)
            : context(context), unit(unit), priority(priority)
        {}
        ~AnalyzeTask() = default;

        void Start() override
        {
            context.ProcessUnit(unit);
            context.ReleaseTask();
        }
        uint32_t GetPriority() override
        {
            return priority;
        }
    };

    void LogStatistics();

    HCXX::Parser& myParser;
    HCXX::Workspace& myWorkspace;
    std::vector<std::unique_ptr<HCXX::Checker>> myCheckers;
    HCXX::ThreadPool myTreadPool;
    std::unique_ptr<DataFlowAnalyzer> myDfaAnalyzer;
    std::atomic<uint64_t> myParseTime = 0;
    std::atomic<uint64_t> myAnalysisTime = 0;
};

#endif  // HCXXANALYZER_ANALYZERIMPL_H
