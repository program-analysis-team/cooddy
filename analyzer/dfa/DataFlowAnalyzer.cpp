/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#include "DataFlowAnalyzer.h"

#include <CrossTUContext.h>
#include <ast/BinaryExpression.h>
#include <ast/CallExpression.h>
#include <ast/CxxMethodDecl.h>
#include <ast/RefExpression.h>
#include <ast/UnaryExpression.h>
#include <ast/VarDecl.h>
#include <utils/Log.h>

#include <thread>

void DataFlowAnalyzer::Analyze(const FunctionContext& context, const Cfg& cfg, ProblemsHolder& holder)
{
    Timer timer;
    auto& blocks = cfg.GetBlocks();
    std::vector<std::unique_ptr<DfaFunctionState>> state(blocks.size());
    for (size_t i = 0; i < blocks.size(); ++i) {
        state[i] = std::make_unique<DfaFunctionState>(context, blocks[i]);
    }
    auto& checkers = myCheckers[context.GetTranslationUnit().GetLanguage()];
    AnalyzedSet analyzedSet;
    for (size_t i = 0; i < blocks.size(); ++i) {
        const CfgBlock& block = blocks[i];

        if (i == 0) {
            CALL_CHECKERS(checkers, checker->InitFunction(*state[i]), analysis);
        }

        for (auto& successor : block.GetSuccessors()) {
            if (successor > i) {
                state[i]->Join(*state[successor]);
            }
        }

        auto terminator = block.GetTerminatorStatement();
        if (terminator != nullptr) {
            CALL_CHECKERS(checkers, checker->CheckTerminator(*terminator, *state[i], holder), analysis);
        }
        // process cfg elements from the block
        if (block.GetTerminatorCondition() != nullptr) {
            AnalyzeState(state[i]->GetState(block.GetTerminatorCondition()), holder, analyzedSet);
        }
        for (int e = static_cast<int>(block.GetElements().size()) - 1; e >= 0; --e) {
            const Node* node = block.GetElements()[e].GetNode();
            if (node != nullptr) {
                AnalyzeState(state[i]->GetState(node), holder, analyzedSet);
            }
        }

        if (i == blocks.size() - 1) {
            for (auto& arg : context.GetFunction()->GetParams()) {
                CALL_CHECKERS(checkers, checker->CheckDeclaration(state[i]->GetState(arg), holder), analysis);
            }
            CALL_CHECKERS(checkers, checker->CheckFunction(*state[i], holder), analysis);
        }

        // join current function state into predecessors memory state
        auto& predecessors = block.GetPredecessors();
        for (auto& predecessor : predecessors) {
            if (predecessor > i) {
                state[predecessor]->Join(*state[i], predecessor == predecessors.back());
            }
        }
        state[i].reset();
    }
    timer.Flush(analysisTime);
}

void DataFlowAnalyzer::AnalyzeState(DfaState& state, HCXX::ProblemsHolder& holder, AnalyzedSet& analyzedSet)
{
    if (!analyzedSet.insert(state.GetNode()).second) {
        return;
    }
    auto& checkers = myCheckers[state.GetNode()->GetTranslationUnit()->GetLanguage()];
    auto varDecl = state.GetNodeAs<VarDecl>();
    if (varDecl != nullptr) {
        if (!varDecl->IsLocalVariableDeclaration()) {
            CALL_CHECKERS(checkers, checker->CheckGlobalVar(state, holder), analysis);
            return;
        }
        CALL_CHECKERS(checkers, checker->CheckDeclaration(state, holder), analysis);
    }
    CALL_CHECKERS(checkers, checker->InitState(state), analysis);

    uint32_t childrenCount = 0;
    state.GetNode()->TraverseChildren([&](const Node& node) {
        if (++childrenCount > MAX_TRAVERSED_CHILDREN_COUNT) {
            return;  // LCOV_EXCL_LINE: HCAT-2777
        }
        auto& childState = state.GetFuncState().GetState(&node);
        childState.SetParentState(&state);
        AnalyzeState(childState, holder, analyzedSet);
    });

    state.InitSubStates();
    CheckState(state, holder, checkers);

    for (auto& subState : state.GetSubStates()) {
        CheckState(*subState.second, holder, checkers);
    }
}

void DataFlowAnalyzer::CheckState(DfaState& state, HCXX::ProblemsHolder& holder,
                                  const std::vector<DfaChecker*>& checkers)
{
    CALL_CHECKERS(checkers, checker->CheckState(state, holder), analysis);
    CALL_CHECKERS(checkers, checker->ExitState(state), analysis);
}

void DataFlowAnalyzer::LogMemoryStatistics()
{
    static uint32_t loggedFunctionsCount = 0;
    static const uint32_t functionsCountFrameSize = 1000;
    static const uint32_t mbSize = 1024 * 1024;

    if (functionsCount - loggedFunctionsCount >= functionsCountFrameSize) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::INFO) << "Analysis info: functions=" << functionsCount << "("
                                        << (functionsMemorySize / mbSize)
                                        << " mbytes), pending=" << pendingFunctionsCount << "("
                                        << (pendingMemorySize / mbSize) << " mbytes)\n";
        loggedFunctionsCount = functionsCount;
        // LCOV_EXCL_STOP
    }
}

static bool CanAddToCallees(const Node& node, const FunctionDecl* func)
{
    if (func == nullptr || func->IsImplicit() && func->GetKind() == Node::Kind::CXX_METHOD_DECL) {
        return false;
    }
    auto expr = node.GetParentOfType<UnaryExpression>();
    if (expr != nullptr && expr->GetOperation() == UnaryExpression::Operation::SIZEOF) {
        return false;
    }
    return true;
}

static const FunctionDecl* GetFunctionByNode(const Node& node, const FunctionDecl*& curFunction)
{
    auto call = Node::Cast<CallExpression>(&node);
    if (call != nullptr && CanAddToCallees(node, call->GetFunction())) {
        curFunction = call->GetFunction();
        return curFunction;
    }
    auto ref = Node::Cast<RefExpression>(&node);
    if (ref != nullptr) {
        auto func = Node::Cast<FunctionDecl>(ref->GetDeclaration());
        if (func != nullptr && func != curFunction && CanAddToCallees(node, func)) {
            return func;
        }
    }
    return nullptr;
}

static CalleeList GetCalleeList(FunctionContext& funcCtx)
{
    CalleeList result;
    const FunctionDecl* lastCalledFunc = nullptr;
    funcCtx.GetTranslationUnit().GetNodesInRange(funcCtx.GetFunction()->GetRange(), [&](const Node* node) {
        auto function = GetFunctionByNode(*node, lastCalledFunc);
        if (function != nullptr && funcCtx.GetBehavior() != nullptr) {
            result.emplace_back(function, funcCtx.GetInstruction(*node));
        }
        return false;
    });
    return result;
}

static auto GetFunctionList(TranslationUnitPtr& unit, std::unordered_set<std::string>& calledFunctions)
{
    const FunctionDecl* curFunction = nullptr;
    Location functionEnd = 0;
    return unit->GetNodes([&](const Node* node) {
        auto function = GetFunctionByNode(*node, curFunction);
        if (function != nullptr && node->GetRange().end < functionEnd) {
            calledFunctions.emplace(function->GetMangledName());
        }
        function = Node::Cast<FunctionDecl>(node);
        if (function == nullptr || function->GetBody() == nullptr || function->IsPure()) {
            return false;
        }
        functionEnd = function->GetRange().end;
        return true;
    });
}

void DataFlowAnalyzer::AnalyzeContext::ResolveVirtualFunctions(TranslationUnitPtr& unit)
{
    for (auto& node : unit->GetNodes()) {
        if (auto recordDecl = Node::Cast<CxxRecordDecl>(node); recordDecl) {
            ResolveVirtualFunctions(*recordDecl);
        }
    }
}

void DataFlowAnalyzer::AnalyzeContext::ResolveVirtualFunctions(const CxxRecordDecl& recordDecl)
{
    std::vector<RecordUniqueId> baseRecords;
    baseRecords.reserve(recordDecl.GetBaseRecords().size());
    for (auto base : recordDecl.GetBaseRecords()) {
        baseRecords.emplace_back(base->GetUniqueId());
    }
    auto recordNode = myRecordsTree.AddRecord(recordDecl.GetUniqueId(), baseRecords);
    if (!recordNode) {
        return;
    }
    for (auto base : recordDecl.GetBaseRecords()) {
        ResolveVirtualFunctions(*base);
    }
    for (auto method : recordDecl.GetChildren<CxxMethodDecl>(false)) {
        if (method->IsVirtual()) {
            ResolveVirtualFunction(*recordNode.value(), method->GetName(), method->GetMangledName());
        }
    }
}

void DataFlowAnalyzer::AnalyzeContext::ResolveVirtualFunction(RecordsTree::Node& node, const std::string& name,
                                                              const std::string& signature)
{
    auto namePos = signature.rfind(name);
    if (namePos == std::string::npos) {
        return;  // LCOV_EXCL_LINE
    }
    auto funcName = signature.substr(namePos);

    std::unique_lock<std::mutex> lock(myMutex);
    auto funcCtx = RegisterFunction(signature);
    node.virtualMethods.emplace(funcName, funcCtx.get());
    for (auto& base : node.baseRecords) {
        if (auto baseMethod = base->virtualMethods.find(funcName); baseMethod != base->virtualMethods.end()) {
            baseMethod->second->AddOverriddenContext(node.recordId, *funcCtx);
        }
    }
}

void DataFlowAnalyzer::AnalyzeContext::CheckUnit(TranslationUnitPtr& unit)
{
    std::unordered_set<std::string> calledFunctions;
    auto functions = GetFunctionList(unit, calledFunctions);

    FunctionMap unitFunctions;
    FunctionList functionCtxs;
    for (auto it : functions) {
        auto function = Node::Cast<FunctionDecl>(it);
        if (function == nullptr) {
            continue;
        }
        auto& unitCtx = unitFunctions[function->GetMangledName()];
        if (unitCtx != nullptr) {
            continue;
        }
        auto cfg = unit->GetCfg(function);
        std::unique_lock<std::mutex> lock(myMutex);
        unitCtx = RegisterFunction(function->GetMangledName());
        unitCtx->Attach(unit, std::move(cfg));
        SetFunctionFlags(*unitCtx, *function,
                         calledFunctions.find(function->GetMangledName()) != calledFunctions.end());
        functionCtxs.emplace_back(unitCtx);
    }
    for (auto& functionCtx : functionCtxs) {
        auto callees = GetCalleeList(*functionCtx);
        CheckFunction(*functionCtx, [&] { ResolveCallees(unitFunctions, *functionCtx, callees); });
    }

    std::unique_lock<std::mutex> lock(myMutex);
    myUnitFunctions.emplace(unit.get(), std::move(functionCtxs));
}

static bool IsFunctionPublic(const FunctionDecl& funcDecl)
{
    bool isFunction = funcDecl.GetFuncKind() == FunctionDecl::FunctionKind::FUNCTION;
    if (isFunction) {
        if (funcDecl.IsStatic()) {
            return false;
        }
    } else if (funcDecl.GetAccessKind() != FunctionDecl::AccessKind::PUBLIC ||
               funcDecl.GetFuncKind() != FunctionDecl::FunctionKind::METHOD || funcDecl.IsOperator() ||
               funcDecl.IsImplicit()) {
        return false;
    }
    auto crossContext = funcDecl.GetTranslationUnit()->GetCrossTUContext();
    auto funcInfo = crossContext != nullptr ? crossContext->FindFunctionDef(funcDecl.GetName()) : nullptr;
    return funcInfo == nullptr || funcInfo->empty() || !funcInfo->front().hasExternalUsage ||
           isFunction && funcInfo->size() > 1;
}

void DataFlowAnalyzer::AnalyzeContext::SetFunctionFlags(FunctionContext& funcCtx, const FunctionDecl& funcDecl,
                                                        bool usedInTU)
{
    uint32_t functionFlags = 0;
    if (funcDecl.IsSystem()) {
        functionFlags |= FunctionContext::SYSTEM;
    }
    if (funcDecl.IsNoReturn()) {
        functionFlags |= FunctionContext::NO_RETURN;
    }
    if (funcDecl.IsPure()) {
        functionFlags |= FunctionContext::PURE;
    } else if (!funcCtx.IsAttached()) {
        if (funcDecl.IsSystem() || funcDecl.IsImplicit()) {
            functionFlags |= FunctionContext::UNDEFINED;
        } else if (!funcDecl.IsOperator() && !funcDecl.IsDefined() && !IsFunctionDefined(funcDecl)) {
            Log(LogLevel::WARNING) << "Can't find definition: " << funcDecl.GetMangledName() << std::endl;
            ++myAnalyzer.undefinedFunctionsCount;
            functionFlags |= FunctionContext::UNDEFINED;
        }
    }
    auto cxxMethod = Node::Cast<CxxMethodDecl>(&funcDecl);
    if (cxxMethod != nullptr && !funcDecl.IsStatic()) {
        functionFlags |= FunctionContext::CLASS_MEMBER;
    }
    if (!usedInTU && IsFunctionPublic(funcDecl)) {
        functionFlags |= FunctionContext::PUBLIC_API;
    }
    if (funcDecl.IsAnnotatedTainted()) {
        functionFlags |= FunctionContext::ANNOTATED_TAINTED;
    }

    funcCtx.SetFunctionFlags(functionFlags, &funcDecl, funcDecl.GetTranslationUnit()->GetLanguage());
}

bool DataFlowAnalyzer::AnalyzeContext::IsFunctionDefined(const FunctionDecl& funcDecl)
{
    auto crossContext = funcDecl.GetTranslationUnit()->GetCrossTUContext();
    auto funcInfo = crossContext->FindFunctionDef(funcDecl.GetQualifiedName());
    if (funcInfo == nullptr) {
        funcInfo = crossContext->FindFunctionDef(funcDecl.GetMangledName());
    }
    if (funcInfo == nullptr) {
        return false;
    }
    //  LCOV_EXCL_START
    if (!funcInfo->empty() && funcInfo->front().unit != nullptr) {
        myPoolTask.TUnitIncreasePriority(*funcInfo->front().unit);
    }
    return true;
    //  LCOV_EXCL_STOP
}

// LCOV_EXCL_START
void DataFlowAnalyzer::AnalyzeContext::PostCheck()
{
    for (auto& [_, funcCtx] : myGlobalFunctions) {
        if (funcCtx != nullptr && !funcCtx->IsAttached() && !funcCtx->IsUndefined() && !funcCtx->IsPure() &&
            funcCtx->GetFunction() != nullptr) {
            Log(LogLevel::WARNING) << "Undefined function: " << funcCtx->GetSignature() << std::endl;
            ++myAnalyzer.undefinedFunctionsCount;
        }
    }
    for (auto& funcCtx : myGlobalFunctions) {
        if (funcCtx.second != nullptr && !funcCtx.second->IsAnalyzed()) {
            myPoolTask.AddTask(
                std::make_shared<PoolSubTask>(myPoolTask, [&]() { this->CheckFunction(*funcCtx.second, [&] {}); }));
        }
    }
}
// LCOV_EXCL_STOP

void DataFlowAnalyzer::AnalyzeContext::OnTaskCompleted()
{
    std::vector<const DfaFunctionContext*> globFuncs;
    for (auto& [key, func] : myGlobalFunctions) {
        if (func != nullptr) {
            globFuncs.push_back(func.get());
        }
    }
    myAnalyzer.GetWorkspace().GenerateFunctionAnnotations(globFuncs);
}

FunctionContextPtr DataFlowAnalyzer::AnalyzeContext::RegisterFunction(const std::string& signature)
{
    auto it = myGlobalFunctions.emplace(signature, nullptr).first;
    if (it->second != nullptr && it->second->IsSystem()) {
        // don't replace system function in the global map
        return std::make_shared<FunctionContext>(myAnalyzer, myRecordsTree, it->first);  // LCOV_EXCL_LINE
    }
    if (it->second == nullptr || it->second->IsAttached()) {
        it->second = std::make_shared<FunctionContext>(myAnalyzer, myRecordsTree, it->first);
    }
    return it->second;
}

FunctionContextPtr DataFlowAnalyzer::AnalyzeContext::ResolveFunction(const std::string& signature,
                                                                     FunctionMap& unitFunctions, bool isSystem)
{
    if (auto it = unitFunctions.find(signature); it != unitFunctions.end()) {
        return it->second;
    }
    auto it = myGlobalFunctions.emplace(signature, nullptr).first;
    if (it->second == nullptr || isSystem && it->second->IsAttached()) {
        it->second = std::make_shared<FunctionContext>(myAnalyzer, myRecordsTree, it->first);
    }
    return it->second;
}

void DataFlowAnalyzer::AnalyzeContext::ResolveCallees(FunctionMap& unitFunctions, FunctionContext& funcCtx,
                                                      const CalleeList& callees)
{
    for (auto& [calleeFunc, calleeInstr] : callees) {
        auto calleeCtx = ResolveFunction(calleeFunc->GetMangledName(), unitFunctions, calleeFunc->IsSystem());
        if (calleeCtx == nullptr) {
            continue;  // LCOV_EXCL_LINE
        }
        if (!calleeCtx->IsAttached() && !calleeCtx->IsUndefined()) {
            SetFunctionFlags(*calleeCtx, *calleeFunc);

            if (calleeCtx->IsUndefined()) {
                auto& checkers = myAnalyzer.myCheckers[funcCtx.GetLanguage()];
                CALL_CHECKERS(checkers, checker->CheckUndefFunction(*calleeCtx), analysis);
                calleeCtx->SortAnnotations();
            }
        }
        funcCtx.AddCallee(calleeInstr, *calleeCtx);
    }
}

void DataFlowAnalyzer::AnalyzeContext::CheckFunction(FunctionContext& funcCtx,
                                                     const std::function<void()>& resolveCallees)
{
    std::unique_lock<std::mutex> lock(myMutex);
    resolveCallees();
    funcCtx.SetInited();

    std::vector<FunctionContext*> analysisOrder;
    funcCtx.GetAnalysisOrder(++myCurRevisionId, analysisOrder);
    lock.unlock();

    for (auto ctx : analysisOrder) {
        ctx->Analyze(myHolder, myPoolTask, myConsumer);
        if (ctx->SetAnalyzed()) {
            ctx->CleanUp();
        }
    }
}

void DataFlowAnalyzer::AnalyzeContext::ResolveCallees(FunctionMap& unitFunctions, FunctionContext& funcCtx,
                                                      DataStream& stream, IdToStr& mapping, UnitLanguage lang)
{
    uint32_t calleesCount = stream.Get<uint32_t>();
    for (auto j = 0; j < calleesCount; ++j) {
        Instruction calleeInstruction = stream.Get<Instruction>();
        uint32_t overriddenIndex = stream.Get<uint16_t>();
        const std::string& calleeSignature = mapping[stream.Get<uint16_t>()];
        uint32_t functionFlags = stream.Get<uint16_t>();

        auto calleeCtx = ResolveFunction(calleeSignature, unitFunctions, functionFlags & FunctionContext::SYSTEM);
        if (!calleeCtx->IsAttached()) {
            // LCOV_EXCL_START
            calleeCtx->SetFunctionFlags(functionFlags, nullptr, lang);
            // LCOV_EXCL_STOP
        }
        funcCtx.AddCallee(calleeInstruction, *calleeCtx, overriddenIndex);
    }
}

void DataFlowAnalyzer::AnalyzeContext::LoadAnalysisData(TranslationUnitPtr& unit, DataStream& stream)
{
    IdToStr funcMapping;
    for (uint32_t i = 0, n = stream.Get<uint16_t>(); i < n; ++i) {
        uint16_t funcId = stream.Get<uint16_t>();
        funcMapping.emplace(funcId, stream.GetString());
    }
    std::vector<Annotation::Kind> annoMapping(Annotation::MAX_ANNOTATIONS_COUNT);
    for (uint32_t i = 0, n = stream.Get<uint16_t>(); i < n; ++i) {
        uint16_t kind = stream.Get<uint16_t>();
        annoMapping[kind] = Annotation::RegisterKind(stream.GetString(), 0);
    }
    FunctionMap unitFunctions;
    FunctionList functionCtxs;
    auto n = stream.Get<uint32_t>();
    functionCtxs.reserve(n);
    for (uint32_t i = 0; i < n; ++i) {
        std::string signature = funcMapping[stream.Get<uint16_t>()];

        std::unique_lock<std::mutex> lock(myMutex);
        auto funcCtx = RegisterFunction(signature);
        funcCtx->LoadAnalysisData(unit, stream, annoMapping);
        unitFunctions[signature] = funcCtx;
        functionCtxs.emplace_back(funcCtx);
    }

    for (auto& funcCtx : functionCtxs) {
        CheckFunction(*funcCtx,
                      [&] { ResolveCallees(unitFunctions, *funcCtx, stream, funcMapping, unit->GetLanguage()); });
    }
    std::unique_lock<std::mutex> lock(myMutex);
    myUnitFunctions.emplace(unit.get(), std::move(functionCtxs));
}

void DataFlowAnalyzer::AnalyzeContext::CollectDependencies(TranslationUnitPtr& unit,
                                                           std::unordered_map<const FileEntry*, uint32_t>& entries)
{
    for (auto& func : myUnitFunctions[unit.get()]) {
        for (auto& [_, callee] : func->GetCallees()) {
            if (!callee.second->IsAttached() || callee.second->IsUndefined()) {
                //  LCOV_EXCL_START
                continue;
                //  LCOV_EXCL_STOP
            }
            auto& tu = callee.second->GetTranslationUnit();
            if (&tu != unit.get()) {
                auto entry = FileEntriesCache::GetInstance().GetFileEntry(tu.GetMainFileName());
                entries.emplace(entry, 0);
            }
        }
    }
}

void DataFlowAnalyzer::AnalyzeContext::StoreMappingData(FunctionList& funcList, DataStream& stream,
                                                        StrToId& funcMapping)
{
    std::unordered_set<Annotation::Kind> annoMapping;
    for (auto& func : funcList) {
        funcMapping.emplace(func->GetSignature(), funcMapping.size());
        for (auto& [_, callee] : func->GetCallees()) {
            funcMapping.emplace(callee.second->GetSignature(), funcMapping.size());
        }
        for (auto& it : func->GetAnnotation()) {
            for (auto& anno : it) {
                annoMapping.insert(anno.GetKind());
            }
        }
    }
    stream.Add<uint16_t>(funcMapping.size());
    for (auto& it : funcMapping) {
        stream.Add<uint16_t>(it.second);
        stream.AddString(it.first.c_str());
    }
    stream.Add<uint16_t>(annoMapping.size());
    for (auto it : annoMapping) {
        stream.Add<uint16_t>(it);
        stream.AddString(Annotation::GetKindName(it));
    }
}

void DataFlowAnalyzer::AnalyzeContext::StoreAnalysisData(TranslationUnitPtr& unit, DataStream& stream)
{
    auto& funcList = myUnitFunctions[unit.get()];

    StrToId funcMapping;
    StoreMappingData(funcList, stream, funcMapping);

    stream.Add<uint32_t>(funcList.size());

    for (auto& func : funcList) {
        stream.Add<uint16_t>(funcMapping[func->GetSignature()]);
        func->StoreAnalysisData(stream);
    }
    for (auto& func : funcList) {
        auto& callees = func->GetCallees();
        stream.Add<uint32_t>(callees.size());
        for (auto& [instr, callee] : callees) {
            stream.Add<uint32_t>(instr);
            stream.Add<uint16_t>(callee.first);
            stream.Add<uint16_t>(funcMapping[callee.second->GetSignature()]);
            stream.Add<uint16_t>(callee.second->GetFunctionFlags());
        }
    }
}

void DataFlowAnalyzer::AnalyzeContext::SerializeRecords(IOStream& stream)
{
    struct RecordNode {
        uint32_t orderPos = 0;
        RecordUniqueId recordId = 0;
        std::vector<RecordUniqueId> baseRecords;
        std::unordered_map<std::string, uint32_t> virtualMethods;

        void Serialize(IOStream& stream)
        {
            stream << recordId << baseRecords << virtualMethods;
        }
    };

    std::vector<RecordNode> records;
    if (!stream.IsOut()) {
        if (!stream.IsEOF()) {
            stream << records;
        }
        for (auto& it : records) {
            auto& node = myRecordsTree.AddRecordRaw(it.recordId, it.baseRecords);
            for (auto& [func, pos] : it.virtualMethods) {
                ResolveVirtualFunction(node, func.substr(pos), func);
            }
        }
        return;
    }
    for (auto [_, record] : myRecordsTree.GetRecords()) {
        auto& it = records.emplace_back(RecordNode{record.orderPos, record.recordId});
        for (auto base : record.baseRecords) {
            it.baseRecords.emplace_back(base->recordId);
        }
        for (auto& [name, func] : record.virtualMethods) {
            it.virtualMethods.emplace(func->GetSignature(), func->GetSignature().size() - name.size());
        }
    }
    std::sort(records.begin(), records.end(), [](auto& r1, auto& r2) { return r1.orderPos < r2.orderPos; });
    stream << records;
}
