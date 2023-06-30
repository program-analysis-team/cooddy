/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "TraceContext.h"

#include "DataFlowAnalyzer.h"

bool TraceContext::Report(DataFlowAnalyzer& analyzer)
{
    mySinkPath = myPathInfo.IsSinkIdDefined() ? myContext.myExprMapping[myPathInfo.sinkExpr] : nullptr;
    mySourcePath = myPathInfo.IsSourceIdDefined() ? myContext.myExprMapping[myPathInfo.sourceExpr] : nullptr;
    if (mySinkPath == nullptr && mySourcePath == nullptr) {
        return false;  // LCOV_EXCL_LINE
    }
    auto problemPath = mySinkPath != nullptr ? mySinkPath : mySourcePath;

    mySuspPath = &problemPath->context.path;
    if (!(mySuspPath->checkPathFlags & CheckPathParams::CHECK_UNREACHABLE_SINK)) {
        mySinkExecId = problemPath->execId;
        if (mySourcePath != nullptr) {
            mySinkExecId = std::max(mySinkExecId, mySourcePath->execId);
        }
    }
    AddPath(mySinkPath);
    if (!AddSources()) {
        return false;
    }
    AddReturns();
    AddBranches();
    BuildTrace(myCallGraph[UNDEF_EXEC_ID], -1);

    if (!myTrace.empty()) {
        ReportProblem(analyzer);
    }
    return true;
}

void TraceContext::ReportProblem(DataFlowAnalyzer& analyzer)
{
    auto& checkerName = mySuspPath->checker->GetName();

    DfaChecker::ProblemInfo problem(Problem({}, std::move(myTrace), mySuspPath->problemKind, checkerName),
                                    myPathInfo.accessInfo);
    if (!mySuspPath->checker->OnReportProblem(problem)) {
        return;
    }
    auto& profile = analyzer.GetWorkspace().GetProfile().GetInspectionCfg(checkerName, problem.kind);
    if (!profile.isEnabled) {
        return;  // LCOV_EXCL_LINE
    }
    problem.severity = profile.severity;

    if (problem.description.empty()) {
        problem.description = profile.description;
    } else {
        myAddAdditionalDesc = false;
    }

    auto funcCtx = problem.trace.back().funcCtx;
    if (funcCtx == nullptr) {
        return;  // LCOV_EXCL_LINE
    }
    problem.scopeRange = funcCtx->GetBehavior()->GetFunctionRange();
    problem.solverDuration = Timer::Seconds(myPathInfo.checkDuration) * 1000;

    problem.description = HCXX::StrUtils::ReplacePlaceholders(problem.description, problem.replacements);
    problem.replacements.clear();

    problem.functionName = funcCtx->GetFQN();

    if (mySinkPath != nullptr && mySinkPath->function != nullptr) {
        problem.sinkFunction = mySinkPath->function->GetFQN();
    }
    if (myLastCallWithSource == nullptr && mySourcePath != nullptr) {
        myLastCallWithSource = mySourcePath->function;
    }
    FillSourceInfoAndSeverity(problem);

    FillDescriptions(problem, mySinkPath != nullptr ? mySinkPath->annotation.GetKind() : 0);

    myContext.myHolder.RegisterProblem(*mySuspPath->checker, *problem.trace.back().tu, problem.trace.back().range,
                                       std::move(problem));
}

void TraceContext::AddEvent(ExecId execId, TraceNode&& node, const AddNodeCallback& callback)
{
    auto execInfo = myContext.myContext->GetExecInfo(execId);
    auto& callNode = myCallGraph[execInfo.callId];
    if (callNode.funcCtx == nullptr) {
        callNode.execId = execInfo.callId;
        callNode.funcCtx = myContext.myCallsMapping[execInfo.callId].funcCtx;
        if (execInfo.callId != UNDEF_EXEC_ID && callback != nullptr) {
            callback(callNode);
        }
    }
    node.tu = &callNode.funcCtx->GetTranslationUnit();
    if (!node.range.IsValid()) {
        Instruction instruction = node.annotation.GetInstruction();
        if (instruction == 0) {
            instruction = execInfo.instruction;
        }
        node.range = callNode.funcCtx->GetSourceRange(instruction);
    }
    if (!node.range.IsValid() ||
        (node.kind == HCXX::TraceNode::Kind::BRANCH && node.tu->IsMacroExpansionRange(node.range))) {
        return;
    }
    auto it = callNode.events.emplace(execId, node);
    auto& event = it.first->second;
    if (!it.second) {
        return;
    }

    event.funcCtx = callNode.funcCtx;
    event.scopeRange = callNode.funcCtx->GetBehavior()->GetFunctionRange();
}

void TraceContext::AddPath(CheckPathContext::PathPtr& path)
{
    if (path == nullptr) {
        return;
    }
    TraceNode event(nullptr, path->sourceRange, {}, path->annotation);
    AddEvent(path->execId, std::move(event), [&](CallNode& node) {
        if (path->parent != nullptr) {
            path->parent->execId = node.execId;  // adjust parent execId for the case with do/while loop
        }
    });
    AddPath(path->parent);
}

Instruction TraceContext::GetInstructionByArgPos(ExecId execId, uint32_t argPos, Instruction defValue)
{
    auto& callInfo = myContext.myCallsMapping[execId];
    return argPos < callInfo.argInstructions.size() ? callInfo.argInstructions[argPos] : defValue;
}

Annotation TraceContext::GetUntrustedSourceAnnotation(ExecId callerExecId, SourceId sourceId, uint32_t calleeArgPos)
{
    uint32_t callerArgPos = Annotation::ArgInfo::MAX_ARG_POS;

    auto& callInfo = myContext.myCallsMapping[callerExecId];
    if (callInfo.funcCtx != nullptr) {
        // try to find argument pos via function annotations
        auto& annotations = callInfo.funcCtx->GetAnnotation();
        for (size_t i = 0; i < annotations.size(); ++i) {
            auto it = std::find_if(annotations[i].begin(), annotations[i].end(), [&](const Annotation& ann) {
                return ann.GetKind() == myContext.myAnalyzer.untrustedSourceKind &&
                       ann.GetUserData<SourceId>() == sourceId && ann.GetArgPos() == calleeArgPos;
            });
            if (it != annotations[i].end()) {
                callerArgPos = i;
                break;
            }
        }
    }
    Annotation::ArgInfo argInfo(callerArgPos, Annotation::ArgInfo::MAX_INDEX, false);
    return Annotation(mySuspPath->sourceKind, argInfo, GetInstructionByArgPos(callerExecId, callerArgPos - 1));
}

std::string GetArgName(uint32_t argPos)
{
    if (argPos == 0) {
        return "result";
    }
    static const char* numEnding[] = {"", "st", "nd", "rd"};
    return std::to_string(argPos) + (argPos >= 4 ? "th" : numEnding[argPos]) + " argument";
}

std::string TraceContext::GetArgName(const DfaFunctionContext* funcCtx, const Annotation& annotation)
{
    uint32_t argPos = annotation.GetArgPos();
    if (funcCtx != nullptr) {
        auto calleeFunc = funcCtx->GetCalleeContext(annotation.GetInstruction());
        if (calleeFunc != nullptr && calleeFunc->IsClassMember() && argPos > 0 && --argPos == 0) {
            return StrLocales::GetStringLocale("FIELD_OF_OBJECT");
        }
    }
    return ::GetArgName(argPos);
}

std::string TraceContext::GetAdditionalDescription()
{
    auto accessInfo = myPathInfo.accessInfo;
    if (!myAddAdditionalDesc || accessInfo == nullptr) {
        return "";
    }
    std::stringstream ss;
    ss << " (";
    const char* curPrefix = "";
    if (accessInfo->allocatedSize < UNDEFINED_VALUE) {
        ss << curPrefix << "memory size: " << accessInfo->allocatedSize;
        curPrefix = ", ";
    }
    if (accessInfo->accessOffsetBegin != UNDEFINED_VALUE) {
        ss << curPrefix << "access offset: " << accessInfo->accessOffsetBegin;
        auto accessSize = accessInfo->accessOffsetEnd - accessInfo->accessOffsetBegin;
        if (accessInfo->accessOffsetEnd != UNDEFINED_VALUE && accessSize > 0) {
            ss << ", access size: " << accessSize;
        }
    }
    ss << ")";
    return ss.str();
}

std::string TraceContext::MakeSinkEventDescription(const DfaFunctionContext* funcCtx, const Annotation& annotation)
{
    return StrLocales::GetStringLocale("PASSED_AS_THE", {mySourceName, GetArgName(funcCtx, annotation)});
}

std::string TraceContext::MakeSourceEventDescription(const DfaFunctionContext* funcCtx, const Annotation& annotation)
{
    if (annotation.IsSourceRange()) {
        if (mySourceDesc.empty()) {
            return StrLocales::GetStringLocale("SOURCE", {mySourceName});
        }
        return StrLocales::GetStringLocale("SOURCE_WITH_DESC", {mySourceName, mySourceDesc});
    }
    if (annotation.GetArgPos() == Annotation::ArgInfo::MAX_ARG_POS) {
        // LCOV_EXCL_START
        return StrLocales::GetStringLocale("RETURNED_FROM_CALL", {mySourceName});
        // LCOV_EXCL_STOP
    }
    return StrLocales::GetStringLocale("RETURNED_AS_THE", {mySourceName, GetArgName(funcCtx, annotation)});
}

std::string TraceContext::GetTaintProblemReasonCode(FunctionContext* fCtx, uint32_t argPos)
{
    for (auto& annot : fCtx->GetAnnotation(argPos + 1)) {
        if (annot.GetKind() == myContext.myAnalyzer.untrustedSourceKind && annot.IsSourceRange() &&
            annot.GetArgPos() == argPos + 1) {
            return StrLocales::GetStringLocale("ANNOTATED_BY_CONFIG");
        }
    }
    return fCtx->GetTaintProblemReasonCode();
}
void TraceContext::AddUntrustedSourceEvent(const UntrustedSource& source)
{
    mySourceName = StrLocales::GetStringLocale("UNTRUSTED_DATA");
    // COODDY_SUPPRESS
    auto fCtx = reinterpret_cast<FunctionContext*>(source.sourceId);

    uint32_t argPos = source.argPos + 1;

    AddNodeCallback callback = [&](CallNode& node) {
        Annotation annotation = GetUntrustedSourceAnnotation(node.execId, source.sourceId, argPos);
        TraceNode event(nullptr, {}, MakeSourceEventDescription(node.funcCtx, annotation), annotation);
        AddEvent(node.execId, std::move(event), callback);
        argPos = annotation.GetArgPos();
    };
    Annotation annotation(mySuspPath->sourceKind, Annotation::ArgInfo(argPos),
                          GetInstructionByArgPos(source.execId, source.argPos));
    auto callerCtx = myContext.myCallsMapping[source.execId].funcCtx;
    mySourceDesc = StrUtils::ToLower(GetTaintProblemReasonCode(fCtx, source.argPos)) + ": " + fCtx->GetFQN();
    TraceNode event(nullptr, {}, MakeSourceEventDescription(callerCtx, annotation), annotation);
    AddEvent(source.execId, std::move(event), callback);
}

FunctionContext* GetUntrustedSourceFunction(const UntrustedSource& source)
{
    if (source.sourceKind != UntrustedSource::CALL_ARG) {
        return nullptr;
    }
    // COODDY_SUPPRESS
    return reinterpret_cast<FunctionContext*>(source.sourceId);
}

std::string GetInstructionName(Instruction instruction, const FunctionContext* functionContext)
{
    std::string instructionName;
    if (functionContext == nullptr) {
        return instructionName;  // LCOV_EXCL_LINE
    }
    auto range = functionContext->GetSourceRange(instruction);
    if (!functionContext->GetTranslationUnit().IsMacroExpansionRange(range)) {
        instructionName = functionContext->GetTranslationUnit().GetSourceInRange(range);
    }
    return instructionName;
}

std::string GetUntrustedPropagationDescription(uint32_t sourcePos, Instruction sourceInstruction, uint32_t argPos,
                                               Instruction argInstruction, const FunctionContext* functionContext)
{
    std::string argName;
    if (argPos == 0) {
        if (auto calleeContext = functionContext->GetCallee(argInstruction); calleeContext != nullptr) {
            argName = StrLocales::GetStringLocale(
                "OF", {StrLocales::GetStringLocale("RESULT"), "\"" + calleeContext->GetFQN() + "\""});
        } else {
            argName = StrLocales::GetStringLocale("RESULT");  // LCOV_EXCL_LINE
        }
    } else {
        argName = GetInstructionName(argInstruction, functionContext);
        if (argName.empty()) {
            argName = GetArgName(argPos);
        } else {
            argName = "\"" + argName + "\"";
        }
    }
    std::string sourceName;
    if (sourceInstruction != argInstruction) {
        sourceName = GetInstructionName(sourceInstruction, functionContext);
    }
    if (sourceName.empty()) {
        sourceName = GetArgName(sourcePos);  // LCOV_EXCL_LINE
    } else {
        sourceName = "\"" + sourceName + "\"";
    }
    return StrLocales::GetStringLocale("UNTRUSTED_DATA_PROPAGATION_FROM_ARG", {move(sourceName), move(argName)});
}

void TraceContext::AddUntrustedSourcePropagations(const UntrustedSource& source)
{
    if (source.propagation == nullptr) {
        return;
    }
    AddUntrustedSourcePropagations(*source.propagation);

    if (source.sourceKind == UntrustedSource::PROPAGATION) {
        auto execInfo = myContext.myContext->GetExecInfo(source.execId);
        Instruction instruction = GetInstructionByArgPos(source.execId, source.argPos, execInfo.instruction);
        auto& callNode = myCallGraph[execInfo.callId];
        auto functionContext = callNode.funcCtx;
        if (functionContext != nullptr && instruction != 0) {
            std::string desc = GetUntrustedPropagationDescription(
                source.sourceId + 1, GetInstructionByArgPos(source.execId, source.sourceId, execInfo.instruction),
                source.argPos + 1, instruction, functionContext);
            auto range = callNode.funcCtx->GetSourceRange(instruction);
            AddEvent(source.execId, TraceNode{nullptr, range, desc}, {});
        }
    }
}

void TraceContext::AddAdditionalUntrustedEvents(const UntrustedSource& source, bool& sourceAdded)
{
    if (source.propagation != nullptr) {
        AddAdditionalUntrustedEvents(*source.propagation, sourceAdded);
    }
    if (source.execId == UNDEF_EXEC_ID || source.sourceId == 0) {
        return;
    }
    const char* strDesc = nullptr;
    switch (source.sourceKind) {
        case UntrustedSource::BOUNDARY_CONDITION:
        case UntrustedSource::ITERATOR:
            strDesc = "BOUNDARY_CONDITION_DESC";
            break;
        case UntrustedSource::STRING_LENGTH:
            strDesc = "STRING_LENGTH_DESC";
            sourceAdded = true;
            break;
        default:
            return;
    }
    auto execInfo = myContext.myContext->GetExecInfo(source.execId);
    auto& callNode = myCallGraph[execInfo.callId];
    if (callNode.funcCtx == nullptr) {
        return;  // LCOV_EXCL_LINE: HCAT-2777
    }
    auto range = callNode.funcCtx->GetSourceRange(execInfo.instruction);
    auto desc = StrLocales::GetStringLocale(strDesc, {std::to_string(source.sourceId)});
    AddEvent(source.execId, TraceNode{nullptr, range, desc}, {});
}

bool TraceContext::AddDefaultUntrustedSource()
{
    if (mySourcePath == nullptr) {
        // don't report a problem if there is no source point and sink is in callee function
        return &mySinkPath->owner == myContext.myCurFuncCtx;
    }
    auto& firstEvent = myCallGraph[UNDEF_EXEC_ID].events.begin()->second;
    if (&mySourcePath->owner == myContext.myCurFuncCtx && mySourcePath->sourceRange.begin >= firstEvent.range.begin) {
        // don't add source point if it is located after the first event
        return true;
    }
    if (myPathInfo.accessInfo != nullptr && myPathInfo.accessInfo->sourceExprValue != UNDEFINED_VALUE) {
        auto sourceVal = std::to_string(myPathInfo.accessInfo->sourceExprValue);
        mySourceDesc = StrLocales::GetStringLocale("ACCESS_SOURCE_DESC", {std::move(sourceVal)});
    }
    AddPath(mySourcePath);
    return true;
}

bool TraceContext::AddSources()
{
    bool sourceAdded = false;
    for (auto& it : myPathInfo.untrustedSources) {
        auto& source = it.GetRoot();
        if (source.execId != UNDEF_EXEC_ID && GetUntrustedSourceFunction(source) != nullptr) {
            AddUntrustedSourceEvent(source);
            AddUntrustedSourcePropagations(it);
            sourceAdded = true;
        }
        AddAdditionalUntrustedEvents(it, sourceAdded);
    }
    if (!sourceAdded) {
        if (mySuspPath->sourceKind == myContext.myAnalyzer.untrustedSourceKind && mySinkPath != nullptr) {
            return AddDefaultUntrustedSource();
        }
        AddPath(mySourcePath);
    }
    return true;
}
void TraceContext::AddBranches()
{
    for (auto& it : myPathInfo.branches) {
        AddEvent(it.execId, {nullptr, {}, it.result, {}, TraceNode::Kind::BRANCH, {}}, nullptr);
    }
}
void TraceContext::AddReturns()
{
    for (auto& it : myPathInfo.returns) {
        AddNodeCallback callback = [&](CallNode& node) {
            auto desc = StrLocales::GetStringLocale("ASSUME_FUNCTION_IS_EXECUTED", {node.funcCtx->GetFQN()});
            TraceNode event(nullptr, {}, desc, {},
                            it.weakAssumption ? TraceNode::Kind::WEAKASSUMPTION : TraceNode::Kind::ASSUMPTION, {});
            AddEvent(node.execId, std::move(event), callback);
        };
        AddEvent(it.execId,
                 {nullptr,
                  {},
                  StrLocales::GetStringLocale("ASSUME_RETURN_HERE"),
                  {},
                  it.weakAssumption ? TraceNode::Kind::WEAKASSUMPTION : TraceNode::Kind::ASSUMPTION,
                  {}},
                 callback);
    }
}

void TraceContext::BuildTrace(CallNode& callNode, int parentId)
{
    for (auto& [execId, event] : callNode.events) {
        event.id = myLastEventId++;
        auto callee = execId != UNDEF_EXEC_ID ? &myCallGraph[execId] : nullptr;
        bool source = event.annotation.GetKind() == mySuspPath->sourceKind;
        bool reverse = source && event.kind != TraceNode::Kind::ASSUMPTION;
        bool addEvent = execId <= mySinkExecId && !(myTrace.empty() && event.kind != TraceNode::Kind::DEFAULT);
        if (callee != nullptr && reverse) {
            BuildTrace(*callee, addEvent ? event.id : parentId);
        }
        if (addEvent) {
            if (source && myLastCallWithSource == nullptr && parentId != -1) {
                myLastCallWithSource = callNode.funcCtx;
            }
            event.parentId = parentId;
            myTrace.emplace_back(event);
        }
        if (callee != nullptr && !reverse) {
            BuildTrace(*callee, addEvent ? event.id : parentId);
        }
    }
}

void TraceContext::FillDescriptions(Problem& problem, Annotation::Kind sinkKind)
{
    auto sourceKind = problem.trace.front().annotation.GetKind();
    if (sourceKind != myContext.myAnalyzer.untrustedSourceKind || mySourceName.empty()) {
        mySourceName = mySuspPath->sourceName;
    }
    for (auto& it : problem.trace) {
        if (it.kind == TraceNode::Kind::BRANCH) {
            it.description =
                StrLocales::GetStringLocale("ASSUME_CONDITION", {it.tu->GetSourceInRange(it.range), it.description});
        }
        if (it.kind != TraceNode::Kind::DEFAULT || !it.description.empty()) {
            continue;
        }
        if (sinkKind != it.annotation.GetKind()) {
            it.description = MakeSourceEventDescription(it.funcCtx, it.annotation);
        } else if (it.annotation.IsSourceRange() || &it == &problem.trace.back()) {
            it.description = problem.description + GetAdditionalDescription();
        } else {
            it.description = MakeSinkEventDescription(it.funcCtx, it.annotation);
        }
    }
}

void TraceContext::FillSourceInfoAndSeverity(Problem& problem)
{
    if (myPathInfo.untrustedSources.empty() && myLastCallWithSource != nullptr) {
        problem.sourceId = myLastCallWithSource->GetFQN();
    }
    if (myPathInfo.accessInfo != nullptr && myPathInfo.accessInfo->allocatedSize > UNDEFINED_VALUE) {
        problem.severity = Problem::Severity::WARNING;
    }
    for (auto& it : myPathInfo.untrustedSources) {
        auto rootSource = it.GetRoot();
        auto funcCtx = GetUntrustedSourceFunction(rootSource);
        if (funcCtx == nullptr) {
            continue;
        }
        if (problem.sourceId.empty()) {
            problem.reasonCode = GetTaintProblemReasonCode(funcCtx, it.argPos);
            funcCtx->SetTaintProblemInfo(problem);
        }
        if (funcCtx->IsPublicApi() && !funcCtx->IsAnnotatedTainted()) {
            auto argInfo = myContext.myContext->GetCallArgInfo(rootSource.argPos);
            if (!argInfo.type.isIntegral) {
                problem.severity = Problem::Severity::WARNING;
            }
        }
    }
}
