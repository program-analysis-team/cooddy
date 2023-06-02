/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_DFA_DATAFLOWANALYZER_H_
#define COODDY_ANALYZER_SOURCE_DFA_DATAFLOWANALYZER_H_

#include <Analyzer.h>

#include "FunctionContext.h"

using namespace HCXX;
using FunctionContextPtr = std::shared_ptr<FunctionContext>;
using CalleeList = std::vector<std::pair<const FunctionDecl*, Instruction>>;

class DataFlowAnalyzer {
public:
    static constexpr uint32_t MAX_TRAVERSED_CHILDREN_COUNT = 1024;

    Annotation::Kind noReturnKind;
    Annotation::Kind aliasKind;
    Annotation::Kind taintPropagationKind;
    Annotation::Kind writeKind;
    Annotation::Kind conditionKind;
    Annotation::Kind resultKind;
    Annotation::Kind arrayIndexKind;
    Annotation::Kind untrustedSourceKind;
    Annotation::Kind allocKind;
    Annotation::Kind allocSizeKind;
    Annotation::Kind taintInputKind;
    Annotation::Kind initNullKind;
    Annotation::Kind createObjectKind;
    Annotation::Kind changedKind;

    std::atomic<uint64_t> analysisTime = 0;
    std::atomic<uint64_t> executionTime = 0;
    std::atomic<uint64_t> functionsCount = 0;
    std::atomic<uint64_t> functionsMemorySize = 0;
    std::atomic<uint64_t> pendingFunctionsCount = 0;
    std::atomic<uint64_t> pendingMemorySize = 0;
    std::atomic<uint64_t> undefinedFunctionsCount = 0;

    explicit DataFlowAnalyzer(Parser& parser, Workspace& workspace,
                              std::vector<std::vector<HCXX::DfaChecker*>>&& checkers)
        : myParser(parser), myWorkspace(workspace), myCheckers(std::move(checkers))
    {
        noReturnKind = Annotation::GetKind("NoReturn");
        aliasKind = Annotation::GetKind("Alias");
        taintPropagationKind = Annotation::GetKind("TaintPropagation");
        writeKind = Annotation::GetKind("Write");
        conditionKind = Annotation::RegisterKind("Condition", Annotation::NOT_PROPAGATED_FROM_PARAMETER);
        resultKind = Annotation::RegisterKind("Result", Annotation::NOT_PROPAGATED_FROM_PARAMETER);
        arrayIndexKind = Annotation::RegisterKind("ArrayIndex", 0);
        untrustedSourceKind = Annotation::GetKind("UntrustedSource");
        allocKind = Annotation::RegisterKind("AllocSource", 0);
        allocSizeKind = Annotation::RegisterKind("AllocSize", 0);
        taintInputKind = Annotation::GetKind("TaintInput");
        initNullKind = Annotation::GetKind("InitNull");
        createObjectKind = Annotation::GetKind("CreateObject");
        changedKind = Annotation::GetKind("Changed");
    }
    ~DataFlowAnalyzer() = default;

    struct TUnitPoolTask : public PoolTask {
        TUnitPoolTask(ThreadPool& pool) : PoolTask(pool){};
        virtual void TUnitIncreasePriority(TranslationUnit& unit) = 0;
    };

    class AnalyzeContext : public Parser::VirtualFunctionsConsumer {
    public:
        AnalyzeContext(DataFlowAnalyzer& analyzer, Analyzer::Consumer& consumer, TUnitPoolTask& poolTask,
                       ProblemsHolder& holder)
            : myAnalyzer(analyzer), myConsumer(consumer), myPoolTask(poolTask), myHolder(holder)
        {}
        ~AnalyzeContext() = default;

        void CheckUnit(TranslationUnitPtr& unit);

        void PostCheck();

        void CollectDependencies(TranslationUnitPtr& unit, std::unordered_map<const FileEntry*, uint32_t>& entries);

        void LoadAnalysisData(TranslationUnitPtr& unit, DataStream& stream);
        void StoreAnalysisData(TranslationUnitPtr& unit, DataStream& stream);
        void OnTaskCompleted();

        void ResolveVirtualFunctions(TranslationUnitPtr& unit);

        void SerializeRecords(IOStream& stream);

    private:
        using FunctionMap = std::unordered_map<std::string, FunctionContextPtr>;
        using FunctionList = std::vector<FunctionContextPtr>;
        using StrToId = std::unordered_map<std::string, uint16_t>;
        using IdToStr = std::unordered_map<uint16_t, std::string>;

        void StoreMappingData(FunctionList& funcList, DataStream& stream, StrToId& funcMapping);

        FunctionContextPtr RegisterFunction(const std::string& signature);

        FunctionContextPtr ResolveFunction(const std::string& signature, FunctionMap& unitFunctions, bool isSystem);

        void ResolveCallees(FunctionMap& unitFunctions, FunctionContext& funcCtx, const CalleeList& callees);

        void ResolveCallees(FunctionMap& unitFunctions, FunctionContext& funcCtx, DataStream& stream, IdToStr& mapping,
                            UnitLanguage lang);

        void CheckFunction(FunctionContext& funcCtx, const std::function<void()>& resolveCallees);

        bool IsFunctionDefined(const FunctionDecl& funcDecl);

        void SetFunctionFlags(FunctionContext& funcCtx, const FunctionDecl& funcDecl, bool usedInTU = true);

        void ResolveVirtualFunction(RecordsTree::Node& node, const std::string& name, const std::string& signature);

        void ResolveVirtualFunctions(const CxxRecordDecl& recordDecl);

        Handle OnClassDeclaration(RecordUniqueId classId, const std::vector<RecordUniqueId>& baseClasses) override
        {
            auto node = myRecordsTree.AddRecord(classId, baseClasses);
            return node ? node.value() : nullptr;
        }
        void OnVirtualFunction(Handle classHandle, const std::string& name, const std::string& signature) override
        {
            ResolveVirtualFunction(*static_cast<RecordsTree::Node*>(classHandle), name, signature);
        }

        DataFlowAnalyzer& myAnalyzer;
        TUnitPoolTask& myPoolTask;
        ProblemsHolder& myHolder;
        FunctionMap myGlobalFunctions;
        RecordsTree myRecordsTree;
        std::unordered_map<TranslationUnit*, FunctionList> myUnitFunctions;
        std::mutex myMutex;
        uint32_t myCurRevisionId = 0;
        Analyzer::Consumer& myConsumer;
    };

    void Analyze(const FunctionContext& context, const HCXX::Cfg& cfg, HCXX::ProblemsHolder& holder);

    void LogMemoryStatistics();

    Workspace& GetWorkspace() const
    {
        return myWorkspace;
    }
    Parser& GetParser() const
    {
        return myParser;
    }

private:
    using AnalyzedSet = std::unordered_set<const Node*>;

    void AnalyzeState(HCXX::DfaState& state, HCXX::ProblemsHolder& holder, AnalyzedSet& analyzedSet);

    void CheckState(DfaState& state, HCXX::ProblemsHolder& holder, const std::vector<DfaChecker*>& checkers);

    Workspace& myWorkspace;
    Parser& myParser;
    std::vector<std::vector<HCXX::DfaChecker*>> myCheckers;
};

#endif  // COODDY_ANALYZER_SOURCE_DFA_DATAFLOWANALYZER_H_
