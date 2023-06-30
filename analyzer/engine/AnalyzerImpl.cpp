/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of the cxx analyzer.
//

#include "AnalyzerImpl.h"

#include <CheckersFactory.h>
#include <CompilerOptionsList.h>
#include <deps/md5.h>
#include <deps/miniz.h>
#include <dfa/GlobalChecker.h>
#include <utils/Memory.h>
#include <utils/ThreadPool.h>

#include "AnalysisStatistics.h"
using namespace HCXX;

std::unique_ptr<Analyzer> Analyzer::Create(Parser& parser, Workspace& workspace)
{
    return std::make_unique<AnalyzerImpl>(parser, workspace);
}

AnalyzerImpl::AnalyzerImpl(Parser& parser, Workspace& workspace) : myParser(parser), myWorkspace(workspace)
{
    HCXX::CheckersFactory::GetInstance().Create(workspace.GetProfile(), myCheckers);

    std::vector<std::vector<DfaChecker*>> dfaCheckers(UnitLanguage::MAX_LANG);
    for (auto& checker : myCheckers) {
        if (checker->Initialize(workspace.GetConfiguration(checker->GetName()))) {
            if (checker->GetKind() != Checker::Kind::AST_CHECKER) {
                for (auto lang : checker->GetLangs()) {
                    dfaCheckers[lang].emplace_back(static_cast<DfaChecker*>(checker.get()));
                }
            }
        } else {
            // LCOV_EXCL_START
            HCXX::Log(HCXX::LogLevel::ERROR) << "Can't initialize checker: " << checker->GetName() << std::endl;
            // LCOV_EXCL_STOP
        }
    }

    myDfaAnalyzer = std::make_unique<DataFlowAnalyzer>(myParser, myWorkspace, std::move(dfaCheckers));
}

AnalyzerImpl::~AnalyzerImpl()
{
    myTreadPool.WaitAll();
    LogStatistics();
}

/** Show counters like:
 * Counters:
 *      Parse error +
 *      Undefined functions
 *      z3 timeout num
 *      Not analyzed files (fatal errors)
 *      Unknown options lists
 *      CFG construction ?
 *
 *    If duration more then a second, then show specific info for each checker, total timeout and IR creating statisics
 */
void AnalyzerImpl::LogStatistics()
{
    AnalysisStatistics stat;
    for (auto& checker : myCheckers) {
        stat.AddCheckerStat(checker->GetName().c_str(), *checker);
    }
    stat.ResetAnalysisTime();

    CheckerStat clangStat;
    clangStat.analysisTime += myParseTime;
    stat.AddCheckerStat("#CLangParser", clangStat);

    CheckerStat astStat;
    astStat.analysisTime += myAnalysisTime;
    stat.AddCheckerStat("#AstEngine", astStat);

    CheckerStat dfaStat;
    dfaStat.analysisTime += myDfaAnalyzer->analysisTime;
    dfaStat.checkPathTime += myDfaAnalyzer->executionTime;
    stat.AddCheckerStat("#DfaEngine", dfaStat);

    stat.LogStatistics(myTreadPool.GetRunTime(), myDfaAnalyzer->undefinedFunctionsCount, myParser.statistics,
                       myWorkspace.GetOptions().settings.resultManUrl);
}

void AnalyzerImpl::AnalyzeImpl(const CompilerOptionsList& unitsOptions, std::shared_ptr<ProblemsHolder> holder,
                               std::shared_ptr<Analyzer::Consumer> consumer, bool waitForCompletion)
{
    auto analyzeContext = new AnalyzeContext(this, unitsOptions, holder, consumer);
    analyzeContext->StartProcessing(Parser::ParseStages::DEFINITIONS);
    analyzeContext->ReleaseTask();
    if (waitForCompletion) {
        analyzeContext->WaitForCompletion();
    }
    analyzeContext->Release();
}

void AnalyzerImpl::Analyze(const CompilerOptionsList& unitsOptions, ProblemsHolder& holder,
                           Analyzer::Consumer& consumer)
{
    AnalyzeImpl(unitsOptions, MakeSharedNoDelete(holder), MakeSharedNoDelete(consumer), true);
}

void AnalyzerImpl::AnalyzeAsync(const CompilerOptionsList& unitsOptions, std::shared_ptr<ProblemsHolder> holder,
                                std::shared_ptr<Analyzer::Consumer> consumer)
{
    if (holder == nullptr) {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "AnalyzeAsync: argument 'holder' is nullptr\n";
        return;
        // LCOV_EXCL_STOP
    }

    if (consumer == nullptr) {
        // Use default consumer
        // LCOV_EXCL_START
        consumer = std::make_shared<HCXX::Analyzer::Consumer>();
        // LCOV_EXCL_STOP
    }

    AnalyzeImpl(unitsOptions, holder, consumer, false);
}

AnalyzerImpl::AnalyzeContext::AnalyzeContext(AnalyzerImpl* analyzer, const CompilerOptionsList& unitsOptions,
                                             std::shared_ptr<HCXX::ProblemsHolder> holder,
                                             std::shared_ptr<Analyzer::Consumer> consumer)
    : TUnitPoolTask(analyzer->myTreadPool),
      myDfaContext(*analyzer->myDfaAnalyzer, *consumer, *this, *holder),
      myAnalyzer(analyzer),
      myConsumer(consumer),
      myHolder(holder),
      myMetaStream(false)
{
    myParserContext = myAnalyzer->myParser.CreateContext(&myResolver);
    myResolver.AddPackage(TUnitsPackage::Create(unitsOptions), *this);

    if (myUnits.size() > 1) {
        myDefinitionsContext = myAnalyzer->myParser.CreateDefinitionsContext(myDfaContext);
    }
    if (myConsumer != nullptr && (myConsumer->GetParseFlags() & Parser::DUMP_AST)) {
        myDumpPackage = TUnitsPackage::CreateEmptyASTContainer(AST_ARCHIVE_FILE);
    }
    if (myMetaStream.Count() != 0) {
        LoadMetaInfo(myMetaStream);
    }
}

void AnalyzerImpl::AnalyzeContext::TUnitCreated(std::shared_ptr<TranslationUnit> unit)
{
    unit->SetCrossTUContext(myCrossTUContext);
    unit->AllocateContexts(myAnalyzer->myCheckers.size() + 1);
    myUnits.emplace_back(unit);
    GetTUContext(*unit);
}

void AnalyzerImpl::AnalyzeContext::TUnitPackage(TUnitsPackage::Ptr package)
{
    if (package->GetMetaInfo(myMetaStream)) {
        FileEntriesCache::GetInstance().Serialize(myMetaStream);
    }
}

// LCOV_EXCL_START
bool AnalyzerImpl::AnalyzeContext::ParseAST(TranslationUnit& unit, bool fullParse)
{
    auto& tuContext = GetTUContext(unit);
    if (++tuContext.parseCount == 1) {
        unit.GetResolver()->InitTUnitData();
        if (unit.IsInited()) {
            return true;  // unit is already parsed
        }
        auto parseCall = [&]() {
            tuContext.parseResult = myAnalyzer->myParser.ParseAST(unit, *myConsumer, myParserContext.get());
            tuContext.parseFinished = true;
        };
        if (fullParse) {
            parseCall();
        } else {
            tuContext.future = std::async(parseCall);
        }
    }
    if (!tuContext.parseFinished && fullParse) {
        while (!tuContext.future.valid()) {
            std::this_thread::yield();
        }
        tuContext.future.wait();
    }
    return tuContext.parseResult;
}
// LCOV_EXCL_STOP

void AnalyzerImpl::AnalyzeContext::Release()
{
    if (--myRefCount == 0) {
        // COODDY_SUPPRESS
        delete this;
    }
}

void AnalyzerImpl::AnalyzeContext::OnTaskCompleted()
{
    if (myDumpPackage != nullptr) {
        IOStream metaStream(true);
        FileEntriesCache::GetInstance().Serialize(metaStream);
        StoreMetaInfo(metaStream);
        myDumpPackage->AddMetaInfo(metaStream);
        myDumpPackage.reset();
    }
    StoreAnalysisData();
    CALL_CHECKERS(myAnalyzer->myCheckers, checker->PostCheck(myUnits, *myHolder), analysis);
    myDfaContext.OnTaskCompleted();
    myHolder->Flush();
    myHolder.reset();
    myParserContext.reset();
    myUnits.clear();
    myPromise.set_value();
    Release();
}

void AnalyzerImpl::AnalyzeContext::WaitForCompletion()
{
    if (!myUnits.empty()) {
        myPromise.get_future().wait();
    }
}

void AnalyzerImpl::AnalyzeContext::StartProcessing(Parser::ParseStages parseStage)
{
    myParseStage = parseStage;
    myUnitsToProcess = myUnits.size();
    for (auto& unit : myUnits) {
        AddTask(std::make_shared<AnalyzeTask>(*this, unit, GetTUContext(*unit).priority));
    }
}

void LogMemUsage(HCXX::Log& log, const char* name, uint64_t size, uint64_t count = 0)
{
    log << ", " << name << '=' << (size / (1024 * 1024)) << "Mb";
    if (count != 0) {
        log << '(' << count << ')';  // LCOV_EXCL_LINE
    }
}

void AnalyzerImpl::AnalyzeContext::LogUnitsInfo()
{
    uint64_t optionsSize = 0;
    for (auto& unit : myUnits) {
        optionsSize += unit->GetCompilerOptions().GetMemUsage();
    }
    HCXX::Log log(HCXX::LogLevel::INFO);
    log << "Project info: units=" << myUnits.size();
    LogMemUsage(log, "code", FileEntriesCache::GetInstance().GetMemUsage());
    LogMemUsage(log, "options", optionsSize);
    LogMemUsage(log, "globals", myCrossTUContext.GetMemUsage());
    log << "\n";
}

void AnalyzerImpl::AnalyzeContext::ProcessUnit(TranslationUnitPtr& unit)
{
    switch (myParseStage) {
        case Parser::ParseStages::DEFINITIONS:
            if (myConsumer->ShouldParseUnit(*unit, myParseStage) && myMetaStream.Count() == 0) {
                ParseDefinitions(unit);
            }
            if (--myUnitsToProcess == 0) {
                myDefinitionsContext.reset();
                StartProcessing(HCXX::Parser::USAGES);
            }
            break;
        case Parser::ParseStages::USAGES:
            if (myConsumer->ShouldParseUnit(*unit, myParseStage)) {
                myAnalyzer->myParser.ParseUsages(*unit);
            }
            if (--myUnitsToProcess == 0) {
                myCrossTUContext.InitUniqueIds();
                LogUnitsInfo();
                StartProcessing(HCXX::Parser::AST);
            }
            break;
        case Parser::ParseStages::AST:
            if (myConsumer->ShouldParseUnit(*unit, myParseStage)) {
                AnalyzeUnit(unit);
            }
            if (--myUnitsToProcess == 0) {
                myDfaContext.PostCheck();
            }
            break;
            // LCOV_EXCL_START: HCAT-2777
        default:
            break;
            // LCOV_EXCL_STOP
    }
}

void AnalyzerImpl::AnalyzeContext::AnalyzeUnit(TranslationUnitPtr& unit)
{
    HCXX::Log(HCXX::LogLevel::INFO) << "Analysis started: " << unit->GetMainFileName() << std::endl;
    myConsumer->OnAnalysisBegin(*unit);
    if (!GetTUContext(*unit).isLoaded) {
        Timer timer;
        bool parseResult = ParseAST(*unit, true);
        timer.Flush(myAnalyzer->myParseTime);
        if (parseResult) {
            RunAnalysis(unit);
        }
        timer.Flush(myAnalyzer->myAnalysisTime);

        if (myDumpPackage == nullptr) {
            if (myUnits.size() == 1) {
                myDfaContext.ResolveVirtualFunctions(unit);
            }
            myDfaContext.CheckUnit(unit);
        }
    } else {
        HCXX::Log(HCXX::LogLevel::INFO) << "Analysis data loaded:" << unit->GetMainFileName() << std::endl;
    }
    myConsumer->OnAnalysisEnd(*unit);
    HCXX::Log(HCXX::LogLevel::INFO) << "Analysis finished: " << unit->GetMainFileName() << std::endl;
    myAnalyzer->myDfaAnalyzer->LogAnalysisInfo();
    unit->ClearUnusedData();
}

bool AnalyzerImpl::AnalyzeContext::LoadFileEntriesData(TranslationUnitPtr& unit, DataStream& dataStream)
{
    std::filesystem::path rootPath = myAnalyzer->myWorkspace.GetWorkspacePath().parent_path();

    uint32_t entriesCount = dataStream.Get<uint32_t>();
    std::vector<std::pair<Location, FileEntry*>> fileEntries;
    for (uint32_t i = 0; i < entriesCount; ++i) {
        std::filesystem::path entryPath = dataStream.GetString();
        if (entryPath.is_relative()) {
            entryPath = rootPath / entryPath;
        }
        UniqueId entryHash = dataStream.Get<UniqueId>();
        Location location = dataStream.Get<uint32_t>();
        auto fileEntry = FileEntriesCache::GetInstance().GetFileEntry(entryPath.string());
        if (fileEntry == nullptr || fileEntry->hash != entryHash) {
            // LCOV_EXCL_START
            return false;
            // LCOV_EXCL_STOP
        }
        if (location != 0) {
            fileEntries.emplace_back(location, fileEntry);
        }
    }
    for (auto& entry : fileEntries) {
        unit->AddFileEntry(entry.first, *entry.second);
    }
    return true;
}

void AnalyzerImpl::AnalyzeContext::StoreFileEntriesData(TranslationUnitPtr& unit, DataStream& dataStream)
{
    std::filesystem::path rootPath = myAnalyzer->myWorkspace.GetWorkspacePath().parent_path();

    std::unordered_map<const FileEntry*, uint32_t> dependencies;
    for (auto& it : unit->GetFileEntries()) {
        dependencies.emplace(&it.second, it.first);
    }
    myDfaContext.CollectDependencies(unit, dependencies);

    dataStream.Add<uint32_t>(dependencies.size());
    for (auto& it : dependencies) {
        std::error_code ec;
        std::filesystem::path entryPath(it.first->filePath);
        auto relativePath = std::filesystem::relative(entryPath, rootPath, ec);
        if (!ec) {
            entryPath = relativePath;
        }
        dataStream.AddString(entryPath.string().c_str());
        dataStream.Add<UniqueId>(it.first->hash);
        dataStream.Add<uint32_t>(it.second);
    }
}

void AnalyzerImpl::AnalyzeContext::StoreCheckersData(TranslationUnitPtr& unit, IOStream& stream)
{
    for (auto& checker : myAnalyzer->myCheckers) {
        if (checker->GetKind() == Checker::Kind::GLOBAL_CHECKER) {
            stream.AddString(checker->GetName().c_str());
            static_cast<GlobalChecker*>(checker.get())->Serialize(*unit, stream);
        }
    }
    stream.AddString("");
}

void AnalyzerImpl::AnalyzeContext::LoadCheckersData(TranslationUnitPtr& unit, IOStream& stream)
{
    std::string checkName = stream.GetString();
    while (!checkName.empty()) {
        for (auto& checker : myAnalyzer->myCheckers) {
            if (checker->GetName() == checkName) {
                static_cast<GlobalChecker*>(checker.get())->Serialize(*unit, stream);
                break;
            }
        }
        checkName = stream.GetString();
    }
}

bool AnalyzerImpl::AnalyzeContext::LoadAnalysisData(TranslationUnitPtr& unit)
{
    if (!myAnalyzer->myWorkspace.GetOptions().incrementalSettings.readMode) {
        return false;
    }
    auto fileEntry = FileEntriesCache::GetInstance().GetFileEntry(unit->GetMainFileName());
    if (fileEntry == nullptr) {
        return false;  // LCOV_EXCL_LINE: HCAT-2778
    }
    IOStream dataStream(false);
    if (!myAnalyzer->myWorkspace.GetAnalysisData(GetStreamHash(*unit, *fileEntry), dataStream)) {
        return false;
    }
    if (!LoadFileEntriesData(unit, dataStream)) {
        return false;
    }
    LoadCheckersData(unit, dataStream);
    myDfaContext.LoadAnalysisData(unit, dataStream);
    return true;
}

void AnalyzerImpl::AnalyzeContext::StoreAnalysisData(TranslationUnitPtr& unit, const std::string& streamPath)
{
    IOStream dataStream(true);
    StoreFileEntriesData(unit, dataStream);
    StoreCheckersData(unit, dataStream);
    myDfaContext.StoreAnalysisData(unit, dataStream);
    myAnalyzer->myWorkspace.StoreAnalysisData(streamPath, dataStream);
}

void PrintSource(TranslationUnit& unit)
{
    for (auto& node : unit.GetNodes()) {
        auto funcDecl = Node::Cast<FunctionDecl>(node);
        if (funcDecl == nullptr || funcDecl->GetBody() == nullptr || funcDecl->GetMangledName().empty()) {
            continue;
        }
        auto range = funcDecl->GetRange();
        FileEntry* entry;
        Location entryOffset = 0;
        auto it = unit.GetFileEntries().upper_bound(range.begin);
        if (it != unit.GetFileEntries().begin()) {
            entry = &(--it)->second;
            entryOffset = it->first;
        } else {
            // LCOV_EXCL_START
            entry = &it->second;
            // LCOV_EXCL_STOP
        }

        if (entry != nullptr && entry->isFakeSource) {
            node->Print(entry->fileSource, entryOffset);
        }
    }
}
void AnalyzerImpl::AnalyzeContext::RunAnalysis(TranslationUnitPtr& unit)
{
    if (myDumpPackage != nullptr) {
        myDumpPackage->AddTUnit(unit);
        return;
    }
    if (myConsumer != nullptr) {
        myConsumer->OnParseEnd(*unit);
        if (!myConsumer->ShouldAnalyzeUnit(*unit)) {
            return;
        }
    }
    PrintSource(*unit);
    std::vector<Checker*> checkers;
    for (auto& checker : myAnalyzer->myCheckers) {
        if (checker->CanProcessLang(unit->GetLanguage()) && !checker->CheckUnit(*unit, *myHolder)) {
            checkers.emplace_back(checker.get());
        }
    }
    for (auto& node : unit->GetNodes()) {
        CALL_CHECKERS(checkers, checker->CheckNode(*node, *myHolder), analysis);
    }
}

std::string AnalyzerImpl::AnalyzeContext::GetStreamHash(const TranslationUnit& unit, const FileEntry& entry)
{
    MD5_CTX md5ctx;
    MD5_Init(&md5ctx);
    MD5_Update(&md5ctx, GIT_COMMIT, std::strlen(GIT_COMMIT));
    MD5_Update(&md5ctx, entry.hash.Data(), sizeof(entry.hash));

    for (auto& option : unit.GetCompilerOptions().options) {
        if (option.find("-D") == 0) {
            MD5_Update(&md5ctx, option.data(), option.size());
        }
    }
    UniqueId result;
    MD5_Final(result.Data(), &md5ctx);
    return result.ToString();
}

void AnalyzerImpl::AnalyzeContext::StoreAnalysisData()
{
    auto incrementalSettings = myAnalyzer->myWorkspace.GetOptions().incrementalSettings;
    if (!incrementalSettings.readMode && !incrementalSettings.writeMode) {
        return;
    }
    std::unordered_set<std::string> unitHashes;
    ProblemsHolder::UnitsMapping unitsMapping;
    for (auto& unit : myUnits) {
        auto entry = FileEntriesCache::GetInstance().GetFileEntry(unit->GetMainFileName());
        if (entry == nullptr) {
            // LCOV_EXCL_START
            continue;
            // LCOV_EXCL_STOP
        }
        auto streamHash = GetStreamHash(*unit, *entry);
        unitHashes.insert(streamHash);
        unitsMapping[entry->hash] = unit.get();
        if (incrementalSettings.writeMode && unit->IsInited()) {
            StoreAnalysisData(unit, streamHash);
        }
    }
    std::string streamName = PROBLEMS_FILE;
    myHolder->FlushProblems(streamName, unitsMapping);
    if (incrementalSettings.writeMode) {
        unitHashes.emplace(std::move(streamName));
        myAnalyzer->myWorkspace.ClearUnusedAnalysisData(unitHashes);
    }
}

void AnalyzerImpl::AnalyzeContext::ParseDefinitions(TranslationUnitPtr& unit)
{
    auto& tuContext = GetTUContext(*unit);
    tuContext.isLoaded = LoadAnalysisData(unit);
    if (!tuContext.isLoaded && !myAnalyzer->myParser.ParseDefinitions(*unit, myDefinitionsContext.get())) {
        ParseAST(*unit, true);  // parse unit if PreParse is not implemented
    }
    if (unit->IsInited()) {
        for (auto& node : unit->GetNodes()) {
            if (auto funcDecl = Node::Cast<FunctionDecl>(node); funcDecl != nullptr && funcDecl->GetBody() != nullptr) {
                myCrossTUContext.AddFunctionDef(funcDecl->GetQualifiedName(), unit.get());
            }
        }
        myDfaContext.ResolveVirtualFunctions(unit);
    }
}

void AnalyzerImpl::AnalyzeContext::LoadMetaInfo(IOStream& stream)
{
    if (!stream.IsEOF()) {
        uint32_t curId = 0;
        for (auto& unit : myUnits) {
            stream.AddIdMapping(++curId, unit.get());
        }
        myDfaContext.SerializeRecords(stream);
        myCrossTUContext.Serialize(stream);
    }
}

void AnalyzerImpl::AnalyzeContext::StoreMetaInfo(IOStream& stream)
{
    // use order from package, because this order is used when units are loaded
    for (uint32_t i = 0, n = myDumpPackage->GetEntriesCount(); i < n; ++i) {
        stream.AddPtrMapping(myDumpPackage->GetTUnit(i).get(), i + 1);
    }
    myDfaContext.SerializeRecords(stream);
    myCrossTUContext.Serialize(stream);
}
