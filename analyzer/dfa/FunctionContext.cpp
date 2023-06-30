/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "FunctionContext.h"

#include <ast/CxxRecordDecl.h>

#include <deque>
#include <thread>

#include "CheckPathContext.h"
#include "DataFlowAnalyzer.h"

FunctionContext::FunctionContext(DataFlowAnalyzer& analyzer, const RecordsTree& recordsTree,
                                 const std::string& signature)
    : myAnalyzer(analyzer), myRecordsTree(recordsTree), mySignatureRef(signature)
{
    ++myAnalyzer.functionsCount;
    myAnalyzer.functionsMemorySize += sizeof(FunctionContext) + mySignatureRef.capacity();
}

FunctionContext::~FunctionContext()
{
    myAnalyzer.functionsMemorySize -= sizeof(FunctionContext) + mySignatureRef.capacity();
    --myAnalyzer.functionsCount;
}

void FunctionContext::SetFunctionFlags(uint32_t functionFlags, const FunctionDecl* function, UnitLanguage lang)
{
    myLanguage = lang;
    if (myFunctionFlags == 0) {
        auto& workspace = myAnalyzer.GetWorkspace();
        if (!workspace.GetAnnotation(myAnalyzer.GetParser().GetMangledNameWithoutAbi(mySignatureRef), myAnnotation,
                                     myLanguage)) {
            if (function != nullptr &&
                !workspace.GetAnnotation(function->GetQualifiedName(), myAnnotation, myLanguage)) {
                if (auto templatePattern = function->GetTemplateInstantiationPattern(); templatePattern != nullptr) {
                    workspace.GetAnnotation(templatePattern->GetQualifiedName(), myAnnotation, myLanguage);
                }
            }
        }
    }
    if (functionFlags == 0 && IsUndefined()) {
        Log(LogLevel::WARNING) << "Found definition for undefined " << mySignatureRef << std::endl;  // LCOV_EXCL_LINE
    }
    if (function != nullptr) {
        myVariadicPos = function->IsVariadic() ? function->GetParams().size() : 0;
    }
    myFunctionFlags = functionFlags;
    myFunction = function;
    myIsInited = (myFunctionFlags & PURE);
}

void FunctionContext::Attach(TranslationUnitPtr& unit, std::shared_ptr<Cfg> cfg)
{
    myUnit = unit;
    myCfg = cfg;
    if (cfg != nullptr) {
        myBehavior = FunctionBehavior::Create(*cfg);
        ++myAnalyzer.pendingFunctionsCount;
        myAnalyzer.functionsMemorySize += cfg->GetMemorySize() + myBehavior->GetMaxInstruction();
    }
    myIsAttached = true;
}

void FunctionContext::AddCallee(Instruction calleeInstr, FunctionContext& calleeCtx, uint32_t overriddenIndex)
{
    if (calleeInstr == 0) {
        return;
    }
    myAnalyzer.functionsMemorySize += sizeof(std::pair<Instruction, CalleeRef>);
    myCallees.emplace(calleeInstr, CalleeRef{overriddenIndex, &calleeCtx});
    if (!calleeCtx.IsUndefined()) {
        calleeCtx.myCallers.emplace_back(this);
    }
}

FunctionContext* FunctionContext::GetCallee(Instruction instruction) const
{
    if (!myIsInited) {
        return nullptr;  // LCOV_EXCL_LINE
    }
    auto it = myCallees.find(instruction);
    return it != myCallees.end() ? it->second.second : nullptr;
}

void FunctionContext::SortOverriddenContexts()
{
    std::vector<CalleeRef*> order;
    for (auto& [instr, ref] : myCallees) {
        if (instr & OVERRIDDEN_BIT) {
            order.emplace_back(&ref);
        }
    }
    std::sort(order.begin(), order.end(),
              [](auto& c1, auto& c2) { return c1->second->GetSignature() < c2->second->GetSignature(); });

    myOverriddenContexts.resize(order.size());
    uint32_t idx = 0;
    for (auto& ref : order) {
        myOverriddenContexts[idx] = ref->second;
        ref->first = ++idx;
    }
}

void FunctionContext::AddOverriddenContext(RecordUniqueId recordId, FunctionContext& funcCtx)
{
    if (myOverriddenContexts.size() != MAX_OVERRIDDEN_CONTEXTS) {
        myOverriddenContexts.emplace_back(&funcCtx);
        AddCallee(recordId | OVERRIDDEN_BIT, funcCtx, myOverriddenContexts.size());
    }
}

FunctionContext::CalleeRef FunctionContext::GetOverriddenContext(RecordUniqueId recordId)
{
    if (auto callee = myCallees.find(recordId | OVERRIDDEN_BIT); callee != myCallees.end()) {
        return callee->second;
    }
    if (auto recordNode = myRecordsTree.FindRecord(recordId); recordNode != nullptr) {
        for (auto& base : recordNode->baseRecords) {
            if (auto callee = myCallees.find(base->recordId | OVERRIDDEN_BIT); callee != myCallees.end()) {
                return callee->second;
            }
        }
    }
    return {0, nullptr};  // LCOV_EXCL_LINE
}

std::vector<DfaFunctionContext*> FunctionContext::GetCalleesContexts() const
{
    std::vector<DfaFunctionContext*> result;
    result.reserve(myCallees.size());
    for (auto& [instr, callee] : myCallees) {
        result.emplace_back(callee.second);
    }
    return result;
}

void FunctionContext::GetAnalysisOrder(uint32_t& revisionId, std::vector<FunctionContext*>& order)
{
    std::deque<FunctionContext*> bfsQueue(1, this);
    while (!bfsQueue.empty()) {
        FunctionContext* ctx = bfsQueue.front();
        bfsQueue.pop_front();
        int32_t curSize = order.size();
        bool ret = ctx->AddToAnalysisOrder(revisionId + 1, order);
        for (int32_t i = int32_t(order.size()) - 1; i >= curSize; --i) {
            ctx = order[i];
            if (!ret) {
                // LCOV_EXCL_START
                ctx->myRevisionResult = false;
                order.pop_back();
                continue;
                // LCOV_EXCL_STOP
            }
            for (auto caller : ctx->myCallers) {
                if (caller->myRevisionId < revisionId) {
                    caller->myRevisionId = revisionId;
                    bfsQueue.push_back(caller);
                }
            }
        }
    }
    ++revisionId;
    if (!order.empty()) {
        SortAnalysisOrder(revisionId, order);
    }
}

bool FunctionContext::AddToAnalysisOrder(uint32_t revisionId, std::vector<FunctionContext*>& order)
{
    if (myRevisionId == revisionId) {
        return myRevisionResult;
    }
    myRevisionId = revisionId;
    myRevisionResult = myIsInited;
    if (!myIsInited || myIsAnalyzed) {
        return myRevisionResult;
    }
    for (auto [_, callee] : myCallees) {
        if (!callee.second->IsUndefined() && !callee.second->AddToAnalysisOrder(revisionId, order)) {
            return myRevisionResult = false;
        }
    }
    order.emplace_back(this);
    return true;
}

void FunctionContext::SortAnalysisOrder(uint32_t& revisionId, std::vector<FunctionContext*>& order)
{
    auto baseId = ++revisionId;
    for (auto it : order) {
        it->myRevisionId = baseId;
    }
    std::vector<FunctionContext*> result;
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        if (FunctionContext* compRoot = *it; compRoot->myRevisionId == baseId) {
            auto curSize = result.size();
            compRoot->MakeLinkedComponent(baseId, ++revisionId, compRoot);
            compRoot->SortLinkedComponent(revisionId++, result);
            std::reverse(result.begin() + curSize, result.end());
        }
    }
    std::reverse(result.begin(), result.end());
    std::swap(result, order);
}

void FunctionContext::MakeLinkedComponent(uint32_t revisionId, uint32_t compId, FunctionContext*& compRoot)
{
    myRevisionId = compId;
    if (GetSignature() < compRoot->GetSignature()) {
        compRoot = this;  // define root of the component as function with minimal signature
    }
    for (auto caller : myCallers) {
        if (caller->myRevisionId == revisionId) {
            caller->MakeLinkedComponent(revisionId, compId, compRoot);
        }
    }
}

void FunctionContext::SortLinkedComponent(uint32_t compId, std::vector<FunctionContext*>& order)
{
    myRevisionId++;
    std::vector<FunctionContext*> callees;
    for (auto it : myCallees) {
        auto callee = it.second.second;
        if (callee->myRevisionId == compId) {
            callees.emplace_back(callee);
        }
    }
    std::sort(callees.begin(), callees.end(),
              [](auto& c1, auto& c2) { return c1->GetSignature() < c2->GetSignature(); });
    for (auto callee : callees) {
        if (callee->myRevisionId == compId) {
            callee->SortLinkedComponent(compId, order);
        }
    }
    if (myAnalysingCounter.compare_exchange_strong(myMaxAnalysingCounter, myMaxAnalysingCounter + 1)) {
        ++myMaxAnalysingCounter;
        SortOverriddenContexts();
        order.emplace_back(this);
    }
}

void FunctionContext::Analyze(ProblemsHolder& holder, PoolTask& poolTask, Analyzer::Consumer& consumer)
{
    if (++myAnalyzedCounter == 1) {
        if (consumer.ShouldAnalyzeFunction(*this) && myCfg != nullptr) {
            myAnalyzer.Analyze(*this, *myCfg, holder);
        }
        SortAnnotations();
        for (int32_t i = int32_t(mySuspiciousPaths.size()) - 1; i >= 0; --i) {
            if (!consumer.ShouldCheckPath(*this, mySuspiciousPaths[i])) {
                mySuspiciousPaths.erase(mySuspiciousPaths.begin() + i);
            }
        }
        if (!mySuspiciousPaths.empty()) {
            poolTask.AddTask(std::make_shared<PoolSubTask>(poolTask, [&]() { this->CheckSuspiciousPaths(holder); }));
        }
        myIsAnalyzed = true;
    }
    while (!myIsAnalyzed) {
        // wait if analysis started in another thread
        std::this_thread::yield();  // LCOV_EXCL_LINE
    }
}

bool FunctionContext::SetAnalyzed()
{
    if (--myAnalysingCounter != 0) {
        return false;  // LCOV_EXCL_LINE: multithreading issue
    }
    myIsAnalyzed = true;
    return true;
}

void FunctionContext::CleanUp()
{
    myFunction = nullptr;
    if (myBehavior != nullptr) {
        myBehavior->CleanUpMapping();
    }
    if (myCfg != nullptr) {
        myAnalyzer.functionsMemorySize -= myCfg->GetMemorySize();
        --myAnalyzer.pendingFunctionsCount;
        myCfg.reset();
    }
}

const ParamAnnotation& FunctionContext::GetAnnotation(uint32_t paramPos) const
{
    static ParamAnnotation empty;
    if (myVariadicPos != 0 && paramPos > myVariadicPos + 1) {
        // use annotations for the last parameter of variadic function
        paramPos = myVariadicPos + 1;
    }
    return paramPos < myAnnotation.size() ? myAnnotation[paramPos] : empty;
}

void FunctionContext::AddAnnotation(uint32_t paramPos, const ParamAnnotation& annotation) const
{
    myAnnotation.resize(std::max(myAnnotation.size(), size_t(paramPos) + 1));
    myAnnotation[paramPos].insert(myAnnotation[paramPos].end(), annotation.begin(), annotation.end());
}

void FunctionContext::AddAnnotation(uint32_t paramPos, const Annotation& annotation) const
{
    myAnnotation.resize(std::max(myAnnotation.size(), size_t(paramPos) + 1));
    myAnnotation[paramPos].push_back(annotation);
}

static bool AnnotationsCompare(const Annotation& a1, const Annotation& a2)
{
    if (a1.GetKind() != a2.GetKind()) {
        return a1.GetKind() < a2.GetKind();
    }
    return a1.GetMemoryOffset() != a2.GetMemoryOffset() ? a1.GetMemoryOffset() < a2.GetMemoryOffset()
                                                        : a1.GetInstruction() < a2.GetInstruction();
}

void FunctionContext::SortAnnotations()
{
    for (auto& it : myAnnotation) {
        std::stable_sort(it.begin(), it.end(), AnnotationsCompare);
        for (auto& a : it) {
            myHasConditions |= (a.GetKind() == myAnalyzer.conditionKind) || (a.GetKind() == myAnalyzer.resultKind);
        }
        myAnalyzer.functionsMemorySize += it.capacity() * sizeof(Annotation);
    }
}

void FunctionContext::AddSuspiciousPath(SuspiciousPath&& path) const
{
    std::sort(path.sinkAnnotations.begin(), path.sinkAnnotations.end(),
              [](const Annotation& a1, const Annotation& a2) { return a1.GetInstruction() < a2.GetInstruction(); });

    for (auto& it : mySuspiciousPaths) {
        if (it.sourceKind == path.sourceKind && it.checkPriority == path.checkPriority &&
            it.sinkAnnotations == path.sinkAnnotations) {
            it.sourceAnnotations.insert(it.sourceAnnotations.end(), path.sourceAnnotations.begin(),
                                        path.sourceAnnotations.end());
            return;
        }
    }
    mySuspiciousPaths.emplace_back(path);
}

void FunctionContext::CheckSuspiciousPaths(ProblemsHolder& holder) const
{
    CheckPathContext checkPathContext(myAnalyzer, *this, holder);
    checkPathContext.CheckSuspiciousPaths(mySuspiciousPaths);
    mySuspiciousPaths = std::vector<SuspiciousPath>();
}

std::vector<Annotation> FunctionContext::CollectAnnotations(const Annotation& source) const
{
    std::vector<Annotation> result;
    auto& argAnno = myAnnotation[source.GetArgPos()];
    if (source.GetArgPos() < myAnnotation.size() && source.GetIndexInArgAnnotations() < argAnno.size()) {
        auto sourceOffset = argAnno[source.GetIndexInArgAnnotations()].GetMemoryOffset();
        Annotation annotationKey(source.GetKind(), Annotation::ArgInfo(), 0, sourceOffset);
        auto it = std::lower_bound(argAnno.begin(), argAnno.end(), annotationKey, AnnotationsCompare);
        while (it != argAnno.end() && it->GetKind() == source.GetKind() && it->GetMemoryOffset() == sourceOffset) {
            result.emplace_back(*it++);
        }
    }
    return result;
}

bool FunctionContext::IsCalleeDefined(const CallExpression* call) const
{
    if (call == nullptr || call->GetFunction() == nullptr) {
        // LCOV_EXCL_START
        return false;
        // LCOV_EXCL_STOP
    }
    auto calleeContext = GetCallee(GetInstruction(*call));
    return calleeContext != nullptr && !calleeContext->IsUndefined();
}

bool FunctionContext::LoadAnalysisData(TranslationUnitPtr& unit, DataStream& stream,
                                       std::vector<Annotation::Kind>& mapping)
{
    myUnit = unit;
    if (stream.Get<bool>()) {
        myBehavior = FunctionBehavior::Create(stream);
    }
    uint32_t argsCount = stream.Get<uint32_t>();
    ParamAnnotation annotation;
    for (auto i = 0; i < argsCount; ++i) {
        uint32_t annotationsCount = stream.Get<uint32_t>();
        annotation.resize(annotationsCount);
        stream.GetBuffer<Annotation>(&annotation[0], annotationsCount);
        for (auto& it : annotation) {
            it.SetKind(mapping[it.GetKind()]);
        }
        AddAnnotation(i, annotation);
    }
    myIsAttached = true;
    return true;
}

void FunctionContext::StoreAnalysisData(DataStream& stream)
{
    stream.Add<bool>(myBehavior != nullptr);
    if (myBehavior != nullptr) {
        myBehavior->StoreToStream(stream);
    }

    stream.Add<uint32_t>(myAnnotation.size());
    for (auto& it : myAnnotation) {
        stream.Add<uint32_t>(it.size());
        stream.AddBuffer<Annotation>(it.data(), it.size());
    }
}

void FunctionContext::TraverseAnnotationLocationsHelper(const Annotation& annotation, TraverseCallback callback,
                                                        AnnotationPath& path) const
{
    path.emplace_back(*this, annotation);
    if (annotation.IsSourceRange() || annotation.GetInstruction() == 0) {
        callback(path);
        return;
    }
    auto callee = GetCallee(annotation.GetInstruction());
    if (callee != nullptr) {
        for (auto& it : callee->CollectAnnotations(annotation)) {
            callee->TraverseAnnotationLocationsHelper(it, callback, path);
            path.pop_back();
        }
    }
}

void FunctionContext::TraverseAnnotationLocations(const Annotation& annotation, TraverseCallback callback) const
{
    AnnotationPath p;
    TraverseAnnotationLocationsHelper(annotation, callback, p);
}

std::string FunctionContext::GetTaintProblemReasonCode() const
{
    if (IsAnnotatedTainted()) {
        return StrLocales::GetStringLocale("ANNOTATED_BY_MACRO");
    }
    if (IsUndefined()) {
        return StrLocales::GetStringLocale("UNDEFINED_FUNCTION");
    }
    if (IsPublicApi()) {
        return StrLocales::GetStringLocale("PUBLIC_FUNCTION");
    }
    return "";  // LCOV_EXCL_LINE
}

void FunctionContext::SetTaintProblemInfo(Problem& problem) const
{
    problem.sourceId = GetFQN();
    if (problem.reasonCode.empty()) {
        problem.reasonCode = GetTaintProblemReasonCode();
    }
    if (myAnalyzer.GetWorkspace().GetOptions().problemSeverityLevel >= problem.severity) {
        myIsTaintProblemFound = true;
    }
}

std::string FunctionContext::GetFQN() const
{
    return IsUndefined() && myFunction != nullptr ? myFunction->GetQualifiedName()
                                                  : myAnalyzer.GetParser().GetQualifiedName(GetSignature());
}

// LCOV_EXCL_START
Node::Kind FunctionContext::GetInstructionKind(Instruction instruction) const
{
    return myBehavior->GetInstructionKind(instruction);
}
// LCOV_EXCL_STOP

std::string FunctionContext::GetArgName(Instruction argInstr, uint32_t argPos) const
{
    auto range = GetSourceRange(argInstr);
    if (range.IsValid() && !GetTranslationUnit().IsMacroExpansionRange(range)) {
        return "\"" + GetTranslationUnit().GetSourceInRange(range) + "\"";
    }
    if (++argPos == 0) {
        return "result";  // LCOV_EXCL_LINE
    }
    static const char* numEnding[] = {"", "st", "nd", "rd"};
    return "the " + std::to_string(argPos) + (argPos >= 4 ? "th" : numEnding[argPos]) + " argument";
}

std::string FunctionContext::GetVarName(Instruction instr) const
{
    auto range = GetSourceRange(instr);
    if (!range.IsValid()) {
        return {};  // LCOV_EXCL_LINE
    }
    if (myUnit->IsMacroExpansionRange(range)) {
        GetCalleeNameCb getCalleeNameCb = [this](Instruction instruction) {
            auto funcCtx = GetCallee(instruction);
            return funcCtx ? funcCtx->GetFQN() : "";
        };
        GetSourceInRangeCb getSourceByRangeCb = [this](SourceRange range) { return myUnit->GetSourceInRange(range); };
        return myBehavior->GetName(instr, getCalleeNameCb, getSourceByRangeCb);
    }
    return myUnit->GetSourceInRange(range);
}
