/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CheckPathContext.h"

#include "DataFlowAnalyzer.h"
#include "TraceContext.h"

void CheckPathContext::CheckSuspiciousPaths(std::vector<SuspiciousPath>& suspPaths)
{
    std::stable_sort(suspPaths.begin(), suspPaths.end(), [](auto& p1, auto& p2) {
        return p1.checkPriority != p2.checkPriority ? p1.checkPriority < p2.checkPriority
                                                    : p1.jointInstruction < p2.jointInstruction;
    });

    myContext = myCurFuncCtx->GetBehavior()->StartExecution();
    myPaths.reserve(suspPaths.size());

    PathsMapping paths;
    for (auto& path : suspPaths) {
        auto& pathContext = myPaths.emplace_back(PathContext{path});

        for (auto& source : path.sourceAnnotations) {
            PathToCheck p{*myCurFuncCtx, source, pathContext, myCurFuncCtx->GetSourceRange(source.GetInstruction())};
            paths[source.GetInstruction()].emplace_back(std::make_shared<PathToCheck>(p));
        }
        for (auto& sink : path.sinkAnnotations) {
            PathToCheck p{*myCurFuncCtx, sink, pathContext, myCurFuncCtx->GetSourceRange(sink.GetInstruction())};
            paths[sink.GetInstruction()].emplace_back(std::make_shared<PathToCheck>(p));
        }
    }
    InitCallInfo(*myCurFuncCtx);
    AddUntrustedSourcesForArguments();
    ExecuteSolver(paths);
    myExecutionTimer.Flush(myAnalyzer.executionTime);
    if (!myTimeoutIsReached) {
        CheckSolverPaths();
    }
}

void CheckPathContext::CheckSolverPaths()
{
    std::vector<CheckPathParams> params;
    for (auto& pathctx : myPaths) {
        params.emplace_back(CheckPathParams{pathctx.path.checkPathFlags, pathctx.sinks, pathctx.sources,
                                            pathctx.path.sinkCondition, uintptr_t(pathctx.path.checker)});
    }
    uint64_t duration = MAX_EXECUTION_TIME;
    bool result = myContext->CheckPaths(params, duration, [&](PathInfo&& path) {
        TraceContext traceContext = {*this, path};
        return traceContext.Report(myAnalyzer);
    });

    for (auto& pathctx : myPaths) {
        ++pathctx.path.checker->checkedPaths;
        pathctx.path.checker->checkPathTime += (duration / myPaths.size());
    }

    if (!result) {
        // LCOV_EXCL_START

        ++myPaths[0].path.checker->timeoutsCount;
        Log(LogLevel::WARNING) << "Timeout (" << Timer::Seconds(duration)
                               << ") during analyzing path in: " << myCurFuncCtx->GetSignature() << "\n";
        // LCOV_EXCL_STOP
    }
}

void CheckPathContext::AddUntrustedSourcesForArguments()
{
    uint32_t argsCount = myContext->GetCallArgsCount();
    bool wasNonPointerArg = false;
    for (int32_t i = argsCount - 1; i >= 0; --i) {
        for (auto& it : myCurFuncCtx->GetAnnotation(i + 1)) {
            if (it.GetKind() == myAnalyzer.conditionKind && it.GetMemoryOffset().IsSourceObject()) {
                myContext->AddStringCondition(StrUtils::GetStringById(it.GetUserData()), false, i + 1,
                                              it.GetMemoryOffset());
            } else if (it.GetKind() == myAnalyzer.taintInputKind) {
                myContext->AddCallArgUntrustedSource(i, reinterpret_cast<SourceId>(myCurFuncCtx));
                auto argType = myContext->GetCallArgInfo(i).type;
                wasNonPointerArg |= !argType.isPointer;
                if (!wasNonPointerArg && !argType.isReference && argType.isBuffer) {
                    myContext->SetCallArgSize(i, VirtualOffset(), UNDEF_EXPR_ID, false);
                }
            }
        }
    }
}

void CheckPathContext::InitCallInfo(const FunctionContext& funcCtx)
{
    auto& callInfo = myCallsMapping[myContext->GetCurExecId()];
    callInfo.funcCtx = &funcCtx;

    uint32_t argsCount = myContext->GetCallArgsCount();
    callInfo.argInstructions.resize(argsCount);
    for (auto i = 0; i < argsCount; ++i) {
        callInfo.argInstructions[i] = myContext->GetCallArgInfo(i).instruction;
    }
}

bool CheckPathContext::ShouldExecuteCallee(FunctionContext* callee, PathsMapping& curPaths, PathsMapping& calleePaths)
{
    if (callee == nullptr || callee->GetBehavior() == nullptr) {
        return false;
    }
    if (!IsRecursionLimit()) {
        return true;
    }
    if (myRecursionDepth >= MAX_RECURSION_LIMIT) {
        return false;  // LCOV_EXCL_LINE: HCAT-2777
    }
    for (auto& paths : calleePaths) {
        for (auto& path : paths.second) {
            if (path->context.canAddSink && path->context.sinks.empty() &&
                Annotation::IsUntrustedSink(path->annotation.GetKind())) {
                // execute function if there are some pending tainted sinks
                return true;
            }
        }
    }
    if (!curPaths.empty() && callee->GetCallees().empty() &&
        callee->GetBehavior()->GetMaxInstruction() < MAX_SMALL_FUNCTION_INSTRUCTIONS_COUNT) {
        // execute small functions if recursion limit is reached and there are some unprocessed paths
        calleePaths.clear();
        return true;
    }
    return false;
}

CheckPathContext::PathArray& CheckPathContext::GetPathArray(PathsMapping& paths, Instruction instr)
{
    static PathArray emptyArray;
    auto pathsIt = paths.find(instr);
    return pathsIt != paths.end() ? pathsIt->second : emptyArray;
}

void CheckPathContext::ExecuteCallee(FunctionContext& callee, PathsMapping& calleePaths)
{
    auto oldFuncCtx = myCurFuncCtx;
    auto oldIndexInPaths = myCurIndexInPaths;
    myCurFuncCtx = &callee;
    myRecursionDepth++;
    myCurIndexInPaths = 0;
    ExecuteSolver(calleePaths);
    myCurIndexInPaths = oldIndexInPaths;
    myRecursionDepth--;
    myCurFuncCtx = oldFuncCtx;
}

void CheckPathContext::ExecuteSolver(PathsMapping& paths)
{
    if (!myTimeoutIsReached && myExecutionTimer.Duration() > MAX_EXECUTION_TIME) {
        // LCOV_EXCL_START
        Log(LogLevel::WARNING) << "Timeout (" << Timer::Seconds(myExecutionTimer.Duration())
                               << ") during symbolic execution in: " << myCurFuncCtx->GetSignature() << "\n";
        myTimeoutIsReached = true;
        return;
        // LCOV_EXCL_STOP
    }
    myCurFuncCtx->GetBehavior()->Execute(*myContext, [&]() {
        if (myTimeoutIsReached || ++myExecutedInstructions > MAX_EXECUTED_INSTRUCTIONS) {
            return ExecutionResult::CANCEL;
        }
        auto curInstr = myContext->GetCurInstruction();
        auto& pathArr = GetPathArray(paths, curInstr);
        uint32_t indexOfOverridden = 0;
        auto callee = ProcessCallInstruction(indexOfOverridden, pathArr);

        PathsMapping calleePaths;
        for (auto& path : pathArr) {
            if (path->annotation.GetArgInfo().indexOfOverridden != indexOfOverridden) {
                continue;
            }
            path->execId = myContext->GetCurExecId();
            if (myRecursionDepth == 0 && path->context.isSourceFound) {
                path->context.canAddSink = true;
            }
            if (ShouldProcessPath(path) || (callee != nullptr && !CollectCalleePaths(*callee, path, calleePaths))) {
                ProcessPath(path);
            }
        }
        if (ShouldExecuteCallee(callee, paths, calleePaths)) {
            ExecuteCallee(*callee, calleePaths);
        }
        if (callee != nullptr) {
            ProcessRetValueConditions(*callee);
            ProcessOutArgSources(curInstr);
        }
        if (myCurIndexInPaths != 0 && myCurIndexInPaths < pathArr.size()) {
            return ExecutionResult::REPEAT;
        }
        myCurIndexInPaths = 0;
        return ExecutionResult::OK;
    });
}

void CheckPathContext::SetUndefCallResult(FunctionContext& callee)
{
    if (!callee.IsClassMember() || myContext->GetCallArgsCount() != 1) {
        return;
    }
    auto& offset = myUndefCallsMap[&callee];
    if (offset == 0) {
        offset = myContext->CreateUniqueOffset().GetRawValue();
    }
    myContext->CopyCallArg(0, -1, VirtualOffset(offset));
}

bool CheckPathContext::IsOutArgSource(PathPtr& path)
{
    auto callInstr = myContext->GetCurCallInstruction();
    auto argPos = path->annotation.GetArgPos();
    if (argPos == 0 && path->annotation.IsSourceRange()) {
        myOutArgSources.emplace_back(OutArgSource{callInstr, myRecursionDepth, path, VirtualOffset()});
        return true;
    }
    if (argPos == 0 || argPos == Annotation::ArgInfo::MAX_ARG_POS || !path->annotation.IsSourceRange()) {
        return false;
    }

    auto callee = myCurFuncCtx->GetCallee(callInstr);
    if (callee == nullptr) {
        return false;
    }
    auto annotations = callee->CollectAnnotations(path->annotation);
    bool hasAnnotation = !annotations.empty() && annotations.front().GetInstruction() == 0;
    if (hasAnnotation) {
        myOutArgSources.emplace_back(
            OutArgSource{callInstr, myRecursionDepth, path, annotations.front().GetMemoryOffset()});
    }
    return hasAnnotation;
}

VirtualOffset CheckPathContext::GetCallArgOffset(Annotation& annotation)
{
    if (auto callee = myCurFuncCtx->GetCallee(annotation.GetInstruction()); callee != nullptr) {
        if (auto innerAnno = callee->CollectAnnotations(annotation); !innerAnno.empty()) {
            return innerAnno[0].GetMemoryOffset();
        }
    }
    return VirtualOffset{};  // LCOV_EXCL_LINE
}

void CheckPathContext::ProcessOutArgSources(Instruction callInstr)
{
    while (!myOutArgSources.empty() && myOutArgSources.back().callInstruction == callInstr &&
           myOutArgSources.back().recursionDepth == myRecursionDepth) {
        auto& path = myOutArgSources.back().sourcePath;
        auto exprId = myContext->GetExprId(ExprLocation::CALL_ARG, path->annotation.GetArgPos() - 1);
        if (exprId != UNDEF_EXPR_ID) {
            myContext->AddOffset(exprId, myOutArgSources.back().argOffset);
            ProcessSourceExpr(path, exprId);
        }
        myOutArgSources.pop_back();
    }
}

void CheckPathContext::ProcessSourceExpr(PathPtr& path, ExprId exprId)
{
    auto& suspPath = path->context.path;
    if (suspPath.sourceOffset) {
        myContext->AddOffset(exprId, *suspPath.sourceOffset);
    }
    DfaChecker::SourceExecInfo sourceInfo{exprId, path->annotation, *myContext, suspPath};
    if (suspPath.checker->OnSourceExecuted(sourceInfo)) {
        path->context.sources.emplace_back(exprId);
    }
    myExprMapping[exprId] = path;
}

void CheckPathContext::ProcessPath(PathPtr& path)
{
    auto& pathctx = path->context;
    if (path->annotation.IsSourceRange() && path->annotation.GetArgPos() != Annotation::ArgInfo::MAX_ARG_POS) {
        path->function = path->owner.GetCallee(myContext->GetCurCallInstruction());
    }
    if (path->annotation.GetKind() == pathctx.path.sourceKind) {
        pathctx.isSourceFound = true;
        if (!IsOutArgSource(path)) {
            ProcessSourceExpr(path, myContext->GetExprId());
        }
        return;
    }
    auto exprId = myContext->GetExprId();
    if (!path->annotation.IsSourceRange()) {
        exprId = myContext->GetExprId(ExprLocation::CALL_ARG, path->annotation.GetArgPos() - 1);
        if (auto offset = GetCallArgOffset(path->annotation); !offset.IsSourceObject()) {
            myContext->AddOffset(exprId, offset);
        }
        if (exprId == UNDEF_EXPR_ID) {
            return;  // LCOV_EXCL_LINE
        }
    }
    DfaChecker::SinkExecInfo sinkInfo{exprId, path->annotation, *myContext, pathctx.path, pathctx.sources};
    if (pathctx.path.checker->OnSinkExecuted(sinkInfo)) {
        pathctx.sinks.emplace_back(exprId);
    }
    myExprMapping[exprId] = path;
}

bool CheckPathContext::ShouldProcessPath(PathPtr& path)
{
    auto& suspPath = path->context.path;
    bool defResult = path->annotation.IsSourceRange() &&
                     (path->annotation.GetKind() != suspPath.sinkKind || path->context.canAddSink ||
                      (suspPath.checkPathFlags & CheckPathParams::SINK_CAN_BE_BEFORE_SOURCE));
    return suspPath.checker->ShouldProcessAnnotation(path->annotation, defResult, path->context.isSourceFound);
}

bool CheckPathContext::CollectCalleePaths(FunctionContext& calleeCtx, PathPtr& path, PathsMapping& calleePaths)
{
    // change source range to the argument source range
    auto argInfo = myContext->GetCallArgInfo(path->annotation.GetArgPos() - 1);
    path->sourceRange = path->owner.GetSourceRange(argInfo.instruction);

    if (calleeCtx.GetBehavior() == nullptr) {
        return false;
    }
    auto annotations = calleeCtx.CollectAnnotations(path->annotation);
    if (annotations.empty()) {
        return true;  // LCOV_EXCL_LINE: HCAT-2777
    }
    if (annotations[0].GetInstruction() == 0) {
        // ignore callee annotations if instruction is unknown
        return false;
    }

    for (auto& it : annotations) {
        if (!myCalleePaths.emplace(PathKey{&path->context, it}).second) {
            continue;
        }
        PathToCheck p{calleeCtx,     it,
                      path->context, calleeCtx.GetSourceRange(it.GetInstruction()),
                      path,          myContext->GetCurExecId()};
        calleePaths[it.GetInstruction()].emplace_back(std::make_shared<PathToCheck>(p));
    }
    return true;
}

FunctionContext* CheckPathContext::ProcessCallInstruction(uint32_t& indexOfOverridden, std::vector<PathPtr>& paths)
{
    auto curInstr = myContext->GetCurInstruction();
    if (myContext->GetCurKind() == Node::Kind::REF_EXPRESSION && myCurFuncCtx->GetCallee(curInstr) != nullptr) {
        myContext->AddCondition(-1, {Condition::Operation::NE, 0L});
    }

    CallExecInfo callExecInfo;
    if (!myContext->IsCallExpr(&callExecInfo)) {
        return nullptr;
    }

    auto calleeOwner = callExecInfo.callId != 0 ? myCallsMapping[callExecInfo.callId].funcCtx : myCurFuncCtx;
    auto callee = calleeOwner != nullptr ? calleeOwner->GetCallee(callExecInfo.instruction) : nullptr;

    if (callee == nullptr) {
        ProcessArgsAnnotations(nullptr);
        return nullptr;
    }
    if (callExecInfo.recordId != 0) {
        auto [index, overriddenCallee] = callee->GetOverriddenContext(callExecInfo.recordId);
        if (overriddenCallee != nullptr) {
            indexOfOverridden = index;
            callee = overriddenCallee;
        }
    } else if (callExecInfo.isVirtualCall && myCurIndexInPaths < paths.size()) {
        if (myCurIndexInPaths == 0) {
            std::stable_sort(paths.begin(), paths.end(), [](const PathPtr& p1, const PathPtr& p2) {
                return p1->annotation.GetArgInfo().indexOfOverridden < p2->annotation.GetArgInfo().indexOfOverridden;
            });
        }
        indexOfOverridden = paths[myCurIndexInPaths]->annotation.GetArgInfo().indexOfOverridden;
        while (++myCurIndexInPaths < paths.size() &&
               indexOfOverridden == paths[myCurIndexInPaths]->annotation.GetArgInfo().indexOfOverridden) {
        }
        if (indexOfOverridden != 0 && indexOfOverridden <= callee->GetOverriddenContexts().size()) {
            callee = static_cast<FunctionContext*>(callee->GetOverriddenContexts()[indexOfOverridden - 1]);
        }
    }
    if (callee->GetBehavior() == nullptr) {
        SetUndefCallResult(*callee);
    }
    ProcessArgsAnnotations(callee);
    InitCallInfo(*callee);

    if (callee->IsUndefined()) {
        ProcessAllocAnnotations(*callee);
    }
    ProcessRetValueAnnotations(*callee);
    myCalleePaths.clear();
    return callee;
}

void CheckPathContext::ProcessAllocAnnotations(FunctionContext& calleeCtx)
{
    uint32_t sizeArg = UINT32_MAX;
    ExprId sizeExprId = UNDEF_EXPR_ID;
    VirtualOffset argOffset;

    auto& annotations = calleeCtx.GetAnnotation();
    for (size_t i = 0; i < annotations.size(); ++i) {
        for (auto& it : annotations[i]) {
            if (it.GetKind() == myAnalyzer.allocSizeKind) {
                if (sizeExprId != UNDEF_EXPR_ID) {
                    myContext->ApplyOperation(sizeExprId, SolverContext::Operation::MUL,
                                              myContext->GetExprId(ExprLocation::CALL_ARG, i - 1));
                } else if (i != 0) {
                    sizeExprId = myContext->GetExprId(ExprLocation::CALL_ARG, i - 1);
                }
            } else if (it.GetKind() == myAnalyzer.allocKind) {
                sizeArg = i;
                argOffset = it.GetMemoryOffset();
            }
        }
    }
    if (sizeArg != UINT32_MAX) {
        if (sizeExprId == UNDEF_EXPR_ID) {
            sizeExprId = myContext->GetExprId(ExprLocation::CONSTANT, UNDEF_ALLOCATION_SIZE);
        }
        myContext->SetCallArgSize(sizeArg - 1, argOffset, sizeExprId, true);
    }
}

void CheckPathContext::ProcessArgTaintAnnotations(FunctionContext& calleeCtx, uint32_t argPos,
                                                  const Annotation& annotation)
{
    if (!calleeCtx.IsUndefined()) {
        return;
    }
    if (annotation.GetKind() == myAnalyzer.taintPropagationKind) {
        if (annotation.GetUserData() <= myContext->GetCallArgsCount()) {
            myContext->CopyCallArgUntrustedSource(argPos, annotation.GetUserData() - 1);
        }
    } else if (annotation.GetKind() == myAnalyzer.untrustedSourceKind) {
        myContext->AddCallArgUntrustedSource(argPos, annotation.GetUserData<SourceId>());
    }
}

void CheckPathContext::AddOutArgNullCondition(uint32_t argPos, VirtualOffset offset, Condition condition)
{
    if (condition.operation == Condition::Operation::NONE) {
        return;  // LCOV_EXCL_LINE: HCAT-2777
    }
    auto argExprId = myContext->GetExprId(ExprLocation::CALL_ARG, argPos);
    myContext->AddOffset(argExprId, offset);
    myContext->AddCondition(argExprId, Condition{Condition::Operation::EQ, 0L});

    auto retExprId = myContext->GetExprId(ExprLocation::CURRENT);
    myContext->AddCondition(retExprId, condition);

    auto argCondId = myContext->GetExprId(ExprLocation::CONDITION, argExprId);
    auto retCondId = myContext->GetExprId(ExprLocation::CONDITION, retExprId);
    myContext->AddCondition(UNDEF_EXPR_ID, argCondId, Condition::Operation::EQ, retCondId);
}

void CheckPathContext::ProcessArgsAnnotations(FunctionContext* calleeCtx)
{
    bool undefFunc = calleeCtx == nullptr || calleeCtx->GetBehavior() == nullptr;
    bool useAnnotations = IsRecursionLimit();

    for (uint32_t i = 0, argCount = myContext->GetCallArgsCount(); i < argCount; i++) {
        auto argInfo = myContext->GetCallArgInfo(i);
        bool assignVal = undefFunc && !argInfo.type.isConst && argInfo.type.isPointer;
        if (assignVal) {
            VirtualOffset offset(argInfo.type.isReference ? VirtualOffset::Kind::REF : VirtualOffset::Kind::DEREF);
            myContext->AssignCallArgValue(i, offset);
        }
        if (calleeCtx == nullptr || !undefFunc && !useAnnotations) {
            continue;
        }
        for (auto& it : calleeCtx->GetAnnotation(i + 1)) {
            if (it.GetKind() == myAnalyzer.arrayIndexKind) {
                myContext->AssignSizeExpr(i, it.GetMemoryOffset(), it.GetUserData() - 1);  // LCOV_EXCL_LINE
            } else if (!assignVal && useAnnotations && it.GetKind() == myAnalyzer.changedKind) {
                VirtualOffset offset(argInfo.type.isReference ? VirtualOffset::Kind::REF : VirtualOffset::Kind::DEREF);
                myContext->AssignCallArgValue(i, offset);
                assignVal = true;
            }
        }
        for (auto& it : calleeCtx->GetAnnotation(i + 1)) {
            if (undefFunc && it.GetKind() == myAnalyzer.aliasKind) {
                myContext->CopyCallArg(i, it.GetArgPos() - 1, it.GetMemoryOffset(), it.GetMemoryOffset());
            } else if (!assignVal && it.GetKind() == myAnalyzer.writeKind) {
                myContext->AssignCallArgValue(i, it.GetMemoryOffset());
            } else if (it.GetKind() == myAnalyzer.initNullKind) {
                AddOutArgNullCondition(i, it.GetMemoryOffset(), it.GetUserData<Condition>());
            } else {
                ProcessArgTaintAnnotations(*calleeCtx, i, it);
            }
        }
    }
}

void CheckPathContext::ProcessRetValueAnnotations(FunctionContext& calleeCtx)
{
    if (calleeCtx.GetFunctionFlags() & FunctionContext::NO_RETURN) {
        myContext->AddThrowExpr();
    }
    auto& annotations = calleeCtx.GetAnnotation();
    if (annotations.empty()) {
        return;
    }
    for (auto& it : annotations[0]) {
        if (it.GetKind() == myAnalyzer.arrayIndexKind) {
            myContext->AssignSizeExpr(UINT32_MAX, it.GetMemoryOffset(), it.GetUserData() - 1);
        }
    }
    for (auto& it : annotations[0]) {
        if (it.GetKind() == myAnalyzer.noReturnKind) {
            myContext->AddThrowExpr();
        } else if (it.GetKind() == myAnalyzer.createObjectKind) {
            myContext->AddRecordId(it.GetUserData<RecordUniqueId>());
        } else if (calleeCtx.IsUndefined()) {
            ProcessArgTaintAnnotations(calleeCtx, -1, it);
        }
    }
}

void CheckPathContext::ProcessRetValueConditions(FunctionContext& calleeCtx)
{
    if (!calleeCtx.HasConditions()) {
        return;
    }
    auto& annotations = calleeCtx.GetAnnotation();
    for (auto i = 0; i < annotations.size(); ++i) {
        for (auto& it : annotations[i]) {
            if (it.GetKind() == myAnalyzer.resultKind) {
                myContext->AddStringCondition(StrUtils::GetStringById(it.GetUserData()), true, i, it.GetMemoryOffset());
            } else if (it.GetKind() == myAnalyzer.conditionKind) {
                myContext->AddStringCondition(StrUtils::GetStringById(it.GetUserData()), false, i,
                                              it.GetMemoryOffset());
            }
        }
    }
}

// LCOV_EXCL_START
void CheckPathContext::Dump()
{
    std::stringstream stream;
    stream << "Callees:\n";
    for (auto& [instr, ref] : myCurFuncCtx->GetCallees()) {
        stream << ref.second->GetSignature() << "," << instr << "," << ref.first << std::endl;
    }
    stream << "\nOverridden:\n";
    for (auto& ctx : myCurFuncCtx->GetOverriddenContexts()) {
        stream << ctx->GetSignature() << std::endl;
    }
    stream << "\nAnnotations:\n";
    for (auto& argAnno : myCurFuncCtx->GetAnnotation()) {
        for (auto& anno : argAnno) {
            stream << anno.GetArgPos() << "," << anno.IsSourceRange() << "," << Annotation::GetKindName(anno.GetKind())
                   << "," << anno.GetInstruction() << "," << anno.GetMemoryOffset().ToString() << std::endl;
        }
    }
    stream << "\nExecution:\n";
    for (auto& execInfo : myContext->GetExecInfoArray()) {
        auto funcCtx = myCallsMapping[execInfo.callId].funcCtx;
        auto instrKind = funcCtx->GetBehavior()->GetInstructionKind(execInfo.instruction);
        stream << Node::KindToCStr(instrKind) << "," << funcCtx->GetSignature() << "," << execInfo.callId << ","
               << execInfo.instruction << std::endl;
    }

    std::filesystem::path outPath = "dump";
    std::filesystem::create_directory(outPath);
    outPath /= StrUtils::Replace(myCurFuncCtx->GetSignature(), "/", "_");

    std::string oldContent;
    std::ifstream oldFile(outPath.string(), std::ios_base::ate | std::ios_base::binary);
    if (oldFile.good()) {
        oldContent.resize(oldFile.tellg());
        oldFile.seekg(0);
        oldFile.read(&oldContent[0], oldContent.size());
        oldFile.close();
        if (stream.str().compare(0, oldContent.size(), oldContent.c_str()) == 0) {
            return;
        }
        outPath = outPath.parent_path() / "diff";
        std::filesystem::create_directory(outPath);
        outPath /= myCurFuncCtx->GetSignature();
    }
    std::ofstream newFile(outPath.string());
    newFile << stream.str();
}
// LCOV_EXCL_STOP
