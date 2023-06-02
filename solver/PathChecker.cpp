/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "PathChecker.h"

#include <utils/Log.h>

#include "FunctionBehaviorImpl.h"
#include "StringToExprConverter.h"
#include "TraverseExpression.h"

bool PathChecker::IsSourcePassedToCall(const SymbolId& sourceId, uint16_t callId)
{
    auto& argTypes = myCallInfo[callId].function->GetArgTypes();
    for (auto i = 0; i < argTypes.size(); ++i) {
        if (!argTypes[i].isPointer || argTypes[i].isConst) {
            continue;
        }
        auto symbolId = FunctionBehaviorImpl::GetArgSymbolId(StackPos{myCallInfo[callId].stackPos, callId}, i);
        bool foundAlias = false;
        mySymbolAliases.TraverseAliases(symbolId,
                                        [&](const SymbolId& alias) { foundAlias |= sourceId.IsSubSymbolId(alias); });
        if (foundAlias) {
            return true;
        }
    }
    return false;
}

const AccessInfo* PathChecker::GetAccessInfoByModel(z3::model& model, ExprId exprId, ExprId sourceId)
{
    auto accessInfoIt = myAccessInfoMap.find(exprId);
    if (accessInfoIt == myAccessInfoMap.end()) {
        return nullptr;
    }
    auto& accessInfo = accessInfoIt->second;
    model.eval(accessInfo.memoryInfo->sizeExpr).is_numeral_u64(accessInfo.evaledInfo.allocatedSize);
    model.eval(accessInfo.beginOffsetExpr).is_numeral_i64(accessInfo.evaledInfo.accessOffsetBegin);
    model.eval(accessInfo.endOffsetExpr).is_numeral_i64(accessInfo.evaledInfo.accessOffsetEnd);
    if (sourceId != UNDEF_EXPR_ID) {
        model.eval(myStoredExpressions[sourceId].expr).is_numeral_i64(accessInfo.evaledInfo.sourceExprValue);
    }
    return &accessInfo.evaledInfo;
}

std::optional<PathInfo> PathChecker::FindReachableSource(CheckContext& context, uint32_t sinkPos)
{
    auto sinkId = context.params.sinks[sinkPos];
    PathInfo result{sinkId};
    z3::model model = mySolver.get_model();

    for (auto it : context.pathSources[sinkPos]) {
        if (model.eval(myStoredExpressions[it].condition).is_true()) {
            result.sourceExpr = it;
            break;
        }
    }
    if (!(context.params.flags & CheckPathParams::USE_UNTRUSTED_SOURCES)) {
        if (result.sourceExpr == UNDEF_EXPR_ID) {
            return std::nullopt;  // LCOV_EXCL_LINE
        }
        myExecBranchesInfo.ProcessExecutedBranches(result, model, [&](uint16_t callId) {
            return IsSourcePassedToCall(myStoredExpressions[result.sourceExpr].symbolId, callId);
        });
        return result;
    }
    UntrustedSources::FindResult untrustedSource;
    UntrustedSources::ResultsMap processed;
    untrustedSource = myUntrustedSources.FindReachable(model, myStoredExpressions[sinkId].expr, processed);
    if (untrustedSource.source != nullptr) {
        result.untrustedSources.emplace_back(*untrustedSource.source);
    }
    if ((result.sourceExpr == UNDEF_EXPR_ID || untrustedSource.hasUnreachableSource) &&
        result.untrustedSources.empty()) {
        return std::nullopt;
    }
    result.accessInfo = GetAccessInfoByModel(model, sinkId, result.sourceExpr);
    if (result.accessInfo != nullptr &&
        (result.accessInfo->allocatedSize == 0 || result.accessInfo->sourceExprValue == 0)) {
        return std::nullopt;  // LCOV_EXCL_LINE
    }
    myExecBranchesInfo.ProcessExecutedBranches(result, model, [&](uint16_t callId) { return false; });
    return result;
}

std::optional<PathInfo> PathChecker::FindReachablePath(CheckContext& context)
{
    if (context.params.flags & CheckPathParams::CHECK_UNREACHABLE_SINK) {
        return FindReachableSource(context);
    }
    auto& sinkIds = context.params.sinks;
    auto& reachableSinks = context.reachableSinks[context.params.sinkKind];
    for (auto i = 0; i < sinkIds.size(); ++i) {
        if (!CheckTimer()) {
            return std::nullopt;  // LCOV_EXCL_LINE
        }
        if (reachableSinks.find(myStoredExpressions[sinkIds[i]].expr.id()) != reachableSinks.end()) {
            continue;
        }
        if (context.conditions.size() == 1 || CheckConstraint(context.conditions[i]) == z3::sat) {
            return FindReachableSource(context, i);
        }
    }
    return std::nullopt;  // LCOV_EXCL_LINE
}

std::optional<z3::expr> PathChecker::CheckReturnExpr(const z3::expr& sinkExpr, MakePathContext& context)
{
    if (context.sinkId.GetVarInstr() != ExecutionContext::RET_INSTRUCTION) {
        return std::nullopt;
    }
    auto sinkSym = SymbolId::CreateSymbolIdFromExpr(sinkExpr);
    if (!sinkSym.has_value()) {
        return std::nullopt;
    }
    z3::expr_vector result(myContext);
    for (auto& sym : myExitSetSymbols) {
        if (sinkSym.value().IsSubSymbolId(sym)) {
            result.push_back(MakePathCondition(myExitSymbols->GetSymbol(*this, sym), std::move(context)));
        }
    }
    if (result.empty()) {
        return std::nullopt;
    }
    return z3::mk_or(result);
}

void PathChecker::MakeSinkConditions(CheckContext& context)
{
    auto& params = context.params;
    for (auto sourceId : params.sources) {
        context.sourceIds[myStoredExpressions[sourceId].expr.id()].emplace_back(sourceId);
    }
    for (auto sinkId : params.sinks) {
        auto& sink = myStoredExpressions[sinkId];
        auto& sinkCond = sink.condition;
        MakePathContext pathCtx{
            sink.symbolId, context.sourceIds, context.params.flags,
            (context.params.flags & CheckPathParams::SINK_CAN_BE_BEFORE_SOURCE) ? UNDEF_EXEC_ID : sink.execId};

        sinkCond = sinkCond && MakePathCondition(sink.expr, std::move(pathCtx), true);
        context.pathSources.emplace_back(std::move(pathCtx.pathSources));

        if (params.sinkCondition.operation != Condition::Operation::NONE) {
            sinkCond = sinkCond && CreateExprByCondition(CastToBV(sink.expr), params.sinkCondition);
        }
        context.conditions.push_back(sinkCond);
    }
    context.orExpr = z3::mk_or(context.conditions);
}

void PathChecker::FindSinkSameAsSource(const CheckPathParams& params, PathCallback callback)
{
    if (params.sources.size() != 1) {
        return;  // LCOV_EXCL_LINE: HCAT-2778
    }
    auto sourceExprId = myStoredExpressions[params.sources.front()].expr.id();
    for (auto sinkId : params.sinks) {
        if (myStoredExpressions[sinkId].expr.id() == sourceExprId) {
            callback(PathInfo{sinkId, params.sources.front()});
            break;
        }
    }
}

void PathChecker::MakeUnreachableSinkConditions(CheckContext& context)
{
    for (auto& sourceId : context.params.sources) {
        auto& source = myStoredExpressions[sourceId];
        ExpressionsMap sourceIds = {{source.expr.id(), {sourceId}}};
        z3::expr sinkCond = CreateBoolExpr(false);
        for (auto& sinkId : context.params.sinks) {
            auto& sink = myStoredExpressions[sinkId];
            if (source.execId < sink.execId) {
                auto pathExpr = MakePathCondition(sink.expr, {sink.symbolId, sourceIds, context.params.flags});
                AddAddrOfConditions(sink.condition);
                sinkCond = sinkCond || sink.condition && pathExpr;
            }
        }
        context.conditions.push_back(source.condition && !sinkCond);
    }
    auto throwExpr = (myExitSymbols->GetSymbol(*this, THROW_SYMBOL_ID) == 0);
    context.orExpr = throwExpr && z3::mk_or(context.conditions);
}

std::optional<PathInfo> PathChecker::FindReachableSource(CheckContext& context)
{
    z3::model model = mySolver.get_model();
    for (size_t i = 0; i < context.conditions.size(); ++i) {
        if (!model.eval(context.conditions[i]).is_true()) {
            continue;  // LCOV_EXCL_LINE
        }
        PathInfo result{UNDEF_EXPR_ID, context.params.sources[i]};
        uint16_t curCallId = UINT16_MAX;
        myExecBranchesInfo.ProcessExecutedBranches(result, model, [&](uint16_t callId) {
            curCallId = std::min(curCallId, callId);
            return callId == curCallId;
        });
        return std::move(result);
    }
    return std::nullopt;  // LCOV_EXCL_LINE
}

bool PathChecker::CheckPaths(const std::vector<CheckPathParams>& params, uint64_t& duration, PathCallback callback)
{
    myTimeout = duration;
    myTimer = Timer();
    ReachableSinksMap reachableSinks;
    for (auto& param : params) {
        if (!CheckTimer()) {
            return false;  // LCOV_EXCL_LINE
        }
        if (param.flags & CheckPathParams::SINK_IS_THE_SAME_AS_SOURCE) {
            FindSinkSameAsSource(param, callback);
            continue;
        }
        if (param.flags & CheckPathParams::USE_EXIT_SYMBOLS_AS_SINKS) {
            for (auto& symbol : myExitSetSymbols) {
                if (IsExitSymbol(symbol)) {
                    param.sinks.push_back(StoreSymbolExpr(myExitSymbols->GetSymbol(*this, symbol), symbol));
                }
            }
        }
        CheckContext context{param, myContext, myTrueExpr, reachableSinks};
        if (param.flags & CheckPathParams::CHECK_UNREACHABLE_SINK) {
            MakeUnreachableSinkConditions(context);
        } else {
            MakeSinkConditions(context);
        }
        myContexts.emplace_back(std::move(context));
    }
    CheckPaths(0, myContexts.size(), callback);
    duration = myTimer.Duration();
    return CheckTimer();
}

void PathChecker::CheckPaths(size_t l, size_t r, PathCallback callback)
{
    if (l == r || !CheckTimer()) {
        return;
    }
    if (r - l == 1) {
        auto& context = myContexts[l];
        if (context.conditions.empty() || CheckConstraint(context.orExpr) != z3::sat) {
            return;
        }
        auto result = FindReachablePath(context);
        if (result) {
            auto sinkId = result->sinkExpr;
            result->checkDuration = myTimer.Duration();
            if (callback(std::move(*result)) && (context.params.flags & CheckPathParams::USE_UNTRUSTED_SOURCES)) {
                context.reachableSinks[context.params.sinkKind].emplace(myStoredExpressions[sinkId].expr.id());
            }
        }
        return;
    }
    z3::expr_vector constraints(myContext);
    for (size_t i = l; i < r; ++i) {
        constraints.push_back(myContexts[i].orExpr);
    }
    if (CheckConstraint(z3::mk_or(constraints), false) == z3::sat) {
        size_t mid = (l + r) / 2;
        CheckPaths(l, mid, callback);
        CheckPaths(mid, r, callback);
    }
}

void PathChecker::AddAddrOfConditions(const z3::expr& expr)
{
    if (!myProcessedAddrOff.emplace(expr.id(), expr).second) {
        return;
    }
    auto symbolId = SymbolId::CreateSymbolIdFromExpr(expr);
    if (symbolId && symbolId->GetOffset().IsAddrOf()) {
        myConstraints = myConstraints && (expr != CreateIntegerExpr(0));
    }
    for (uint32_t i = 0; i < expr.num_args(); ++i) {
        AddAddrOfConditions(expr.arg(i));
    }
}

bool PathChecker::MakeFieldCondition(const std::vector<ExprId>& sourceIds, ExprId sinkId, VirtualOffset offset,
                                     bool addDerefCondition)
{
    if (sinkId >= myStoredExpressions.size()) {
        return false;  // LCOV_EXCL_LINE: HCAT-2778
    }
    ExpressionsMap newSourceIds;
    for (auto sourceId : sourceIds) {
        if (sourceId >= myStoredExpressions.size()) {
            continue;  // LCOV_EXCL_LINE
        }
        auto expr = SymbolId::CreateSubExpr(myStoredExpressions[sourceId].expr, offset);
        if (expr) {
            newSourceIds.insert({expr->id(), std::vector<ExprId>()});
        }
    }
    if (newSourceIds.empty()) {
        return false;  // LCOV_EXCL_LINE: HCAT-2778
    }
    auto& fieldInfo = myStoredExpressions[sinkId];

    SymbolId objSymbolId(fieldInfo.symbolId, VirtualOffset{});
    fieldInfo.condition = fieldInfo.condition && MakePathCondition(fieldInfo.expr, {fieldInfo.symbolId, newSourceIds});
    fieldInfo.expr = GetSymbol(objSymbolId);

    if (addDerefCondition) {
        SymbolId derefSymbolId = objSymbolId + VirtualOffset(VirtualOffset::Kind::DEREF);
        ExpressionsMap derefSources = {{derefSymbolId.CreateExpr(myContext).id(), std::vector<ExprId>()}};
        fieldInfo.condition =
            fieldInfo.condition && MakePathCondition(GetSymbol(derefSymbolId), {derefSymbolId, derefSources});
    }
    return true;
}

z3::expr PathChecker::MergeItePathConditions(const z3::expr& iteExpr, MakePathContext& context)
{
    auto iteCond = iteExpr.arg(0);
    AddAddrOfConditions(iteCond);
    auto lhsExpr = MakePathCondition(iteExpr.arg(1), std::move(context), true);
    auto rhsExpr = MakePathCondition(iteExpr.arg(2), std::move(context), true);
    if (lhsExpr.id() == rhsExpr.id()) {
        return lhsExpr;
    }
    lhsExpr = lhsExpr && iteCond;
    rhsExpr = rhsExpr && !iteCond;
    return lhsExpr || rhsExpr;
}

z3::expr PathChecker::CheckTaintCondition(const z3::expr& expr, MakePathContext& context)
{
    if (expr.is_numeral() || expr.is_bool()) {
        return myTrueExpr;
    }
    if (!(context.checkFlags & CheckPathParams::USE_CONSTANTS_ONLY)) {
        auto source = myUntrustedSources.Find(expr);
        if (source != nullptr && (source->execId == UNDEF_EXEC_ID || source->execId <= context.sinkExecId)) {
            auto condition = source->condition;
            if ((context.checkFlags & CheckPathParams::USE_INT_TYPE_LIMITS) && source->intTypeInfo.sizeInBits != 0) {
                condition = condition && CreateIntegerCondition(source->expr, source->intTypeInfo);
            }
            if (expr.id() != source->sourceExpr.id()) {
                return condition && MakePathCondition(source->sourceExpr, std::move(context));
            }
            context.hasUntrustedSources = source->IsExternal();
            return condition;
        }
    }
    if (context.checkFlags & CheckPathParams::USE_MEMORY_ADDRESSES) {
        auto memAddress = mySymbolSizes.find(expr.id());
        if (memAddress != mySymbolSizes.end()) {
            return memAddress->second.condition;
        }
    }
    if (expr.is_const()) {
        context.hasFreeSymbols = true;
        return expr == 1;  // for free symbols use constant value
    }
    return MakePathConditionFromArgs(expr, context);
}

z3::expr PathChecker::MakePathConditionFromArgs(const z3::expr& expr, MakePathContext& context)
{
    z3::expr res = myTrueExpr;
    for (size_t i = 0, n = expr.num_args(); i < n; ++i) {
        res = res && MakePathCondition(expr.arg(i), std::move(context));
    }
    return res;
}

z3::expr PathChecker::CheckPathCondition(const z3::expr& expr, MakePathContext& context)
{
    uint32_t checkFlags = context.checkFlags;
    if (checkFlags & CheckPathParams::USE_UNTRUSTED_SOURCES) {
        return CheckTaintCondition(expr, context);
    }
    if ((checkFlags & CheckPathParams::USE_POINTER_ARITHMETIC) && !expr.is_const()) {
        auto declKind = expr.decl().decl_kind();
        if (declKind == Z3_OP_ADD || declKind == Z3_OP_SUB) {
            return MakePathCondition(expr.arg(0), std::move(context));
        }
    }
    auto res = CheckReturnExpr(expr, context);
    if ((checkFlags & CheckPathParams::CHECK_UNREACHABLE_SINK) && !res) {
        auto symId = SymbolId::CreateSymbolIdFromExpr(expr);
        if (symId && symId->IsChanged()) {
            // ignore changed symbols in unreachable paths mode
            res = myTrueExpr;
        }
    }
    return res ? *res : CreateBoolExpr(false);
}

z3::expr PathChecker::MakePathCondition(const z3::expr& sinkExpr, MakePathContext&& context, bool checkFreeSymbols)
{
    auto pMapItem = context.processedExpr.emplace(sinkExpr.id(), CreateBoolExpr(false));
    auto& res = pMapItem.first->second;
    if (!pMapItem.second) {
        return res;
    }
    auto sourceIt = context.sourceIds.find(sinkExpr.id());
    if (sourceIt != context.sourceIds.end()) {
        if (sourceIt->second.empty()) {
            return myTrueExpr;
        }
        for (auto& it : sourceIt->second) {
            if (myStoredExpressions[it].execId <= context.sinkExecId) {
                res = res || myStoredExpressions[it].condition;
                context.pathSources.emplace_back(it);
            }
        }
        if (!(context.checkFlags & CheckPathParams::USE_UNTRUSTED_SOURCES) && !res.is_false()) {
            return res;
        }
    }
    if (sinkExpr.is_ite()) {
        res = MergeItePathConditions(sinkExpr, context);
    } else {
        res = CheckPathCondition(sinkExpr, context);
        if (checkFreeSymbols && context.hasFreeSymbols) {
            if (!context.hasUntrustedSources) {
                res = CreateBoolExpr(false);
            }
            context.hasFreeSymbols = false;
        }
    }
    return res;
}

z3::check_result PathChecker::CheckConstraint(const z3::expr& expr, bool useCache)
{
    try {
        auto it = myCachedVerdicts.emplace(expr.id(), z3::unknown);
        auto& res = it.first->second;
        if (useCache && !it.second && res != z3::sat) {
            return res;
        }
        std::vector<z3::expr> values = {expr, myConstraints};
        res = mySolver.check(values.size(), values.data());
        // LCOV_EXCL_START
        if (res == z3::unknown) {
            myWasTimeout = true;
            Log(LogLevel::WARNING) << "z3::unknown result: " << mySolver.reason_unknown() << std::endl;
        }
        return res;
    } catch (z3::exception& ex) {
        Log(LogLevel::ERROR) << "CheckConstraint::Z3 error: " << ex.what() << std::endl;
    }
    return z3::unknown;
    // LCOV_EXCL_STOP
}

void PathChecker::SetCallArgSize(uint32_t argPos, VirtualOffset offset, ExprId sizeExprId, bool isAlloc)
{
    bool isUnknownSize = sizeExprId >= myStoredExpressions.size();
    auto sizeExpr = !isUnknownSize ? myStoredExpressions[sizeExprId].expr : CreateIntegerExpr(UNDEFINED_VALUE + 1);
    auto untrustedSource = myUntrustedSources.Check(sizeExpr);
    if (!untrustedSource) {
        return;
    }
    if (CallStack::GetCallStackSize() == 0) {
        auto argExpr = CreateSymbolExpr(FunctionBehaviorImpl::GetArgSymbolId(StackPos(), argPos));
        mySymbolSizes.emplace(argExpr.id(),
                              SymbolSizeInfo{argExpr, sizeExpr, myTrueExpr, isUnknownSize, false, isAlloc});
        return;
    }
    auto& args = GetCurCallArgs();
    SymbolId symbolId = argPos < args.size() ? args[argPos].symbolId : *myCurSymbolId;
    symbolId = symbolId + offset;
    auto argExpr = GetSymbol(symbolId);
    mySymbolSizes.emplace(argExpr.id(),
                          SymbolSizeInfo{argExpr, sizeExpr, GetCurConstraint(), isUnknownSize, false, isAlloc});
}

ExprId PathChecker::StoreSymbolExpr(const z3::expr& expr, const SymbolId& symbolId)
{
    myStoredExpressions.emplace_back(StoredExpr{expr, myTrueExpr, symbolId});
    return myStoredExpressions.size() - 1;
}

ExprId PathChecker::GetExprId(ExprLocation loc, int32_t param)
{
    if (loc == ExprLocation::CONSTANT) {
        return StoreSymbolExpr(CreateIntegerExpr(param));
    }
    if (loc == ExprLocation::CONDITION) {
        return param < myStoredExpressions.size() ? StoreSymbolExpr(myStoredExpressions[param].condition)
                                                  : UNDEF_EXPR_ID;
    }
    if (loc == ExprLocation::OPERAND) {
        auto operand = GetOperand(param);
        return operand ? StoreSymbolExpr(*operand) : UNDEF_EXPR_ID;
    }

    const auto* expr = myCurExpr;
    const auto* symbolId = myCurSymbolId;
    if (loc == ExprLocation::CALL_ARG || loc == ExprLocation::CALL_ARG_SIZE) {
        if (myCurStack.empty()) {
            // LCOV_EXCL_START
            myStoredExpressions.emplace_back(
                StoredExpr{GetArgExprByPos(param), myTrueExpr, SymbolId(StackPos()), uint32_t(param)});
            return myStoredExpressions.size() - 1;
            // LCOV_EXCL_STOP
        }
        if (myCurStack.back().callStackPos >= GetCallStackSize()) {
            return UNDEF_EXPR_ID;  // LCOV_EXCL_LINE: HCAT-2778
        }
        auto& args = GetCallArgs(myCurStack.back().callStackPos);
        expr = param < args.size() ? &args[param].expr : &GetCallResult();
        symbolId = param < args.size() ? &args[param].symbolId : myCurSymbolId;
    }
    if (loc == ExprLocation::CURRENT_SIZE || loc == ExprLocation::CALL_ARG_SIZE) {
        auto sizeExpr = CreateSymbolSizeExpr(*symbolId, *expr, false);
        myStoredExpressions.emplace_back(StoredExpr{sizeExpr, GetCurConstraint(), *symbolId, GetCurExecId()});
    } else {
        myStoredExpressions.emplace_back(StoredExpr{*expr, GetCurConstraint(), *symbolId, GetCurExecId()});
    }
    return myStoredExpressions.size() - 1;
}

void PathChecker::ApplyOperation(ExprId exprId, Operation op, ExprId rhe)
{
    if (exprId >= myStoredExpressions.size() || rhe >= myStoredExpressions.size()) {
        return;  // LCOV_EXCL_LINE: HCAT-2778
    }
    auto& lhsExpr = CastToBV(myStoredExpressions[exprId].expr);
    auto& rhsExpr = CastToBV(myStoredExpressions[rhe].expr);

    switch (op) {
        case Operation::ADD:
            lhsExpr = lhsExpr + rhsExpr;
            break;
            // LCOV_EXCL_START
        case Operation::SUB:
            lhsExpr = lhsExpr - rhsExpr;
            break;
        case Operation::DIV:
            lhsExpr = lhsExpr / rhsExpr;
            break;
            // LCOV_EXCL_STOP
        case Operation::MUL:
            lhsExpr = lhsExpr * rhsExpr;
            break;
        case Operation::ASSIGN:
            lhsExpr = rhsExpr;
            break;
            // LCOV_EXCL_START: -Werror=switch-default
        default:
            assert(false);
            // LCOV_EXCL_STOP
    }
}

bool PathChecker::AddCondition(ExprId exprId, const Condition& condition)
{
    auto result = myTrueExpr;
    if (exprId >= myStoredExpressions.size()) {
        result = CreateExprByCondition(CastToBV(*myCurExpr), condition);
        myConstraints = myConstraints && result;
    } else {
        auto& cond = myStoredExpressions[exprId].condition;
        result = CreateExprByCondition(CastToBV(myStoredExpressions[exprId].expr), condition, exprId);
        cond = cond && result;
    }
    return !result.is_false();
}

void PathChecker::AddCondition(ExprId exprId, ExprId lhe, Condition::Operation op, ExprId rhe)
{
    auto size = myStoredExpressions.size();
    if (rhe >= size || lhe >= size) {
        return;  // LCOV_EXCL_LINE: HCAT-2778
    }
    auto& lhsExpr = CastToBV(myStoredExpressions[lhe].expr);
    auto& rhsExpr = CastToBV(myStoredExpressions[rhe].expr);
    auto& condition = exprId < size ? myStoredExpressions[exprId].condition : myConstraints;

    condition = condition && CreateExprByOp(op, lhsExpr, rhsExpr, exprId);
}

z3::expr PathChecker::CreateExprByCondition(const z3::expr& op, Condition condition, ExprId exprId)
{
    if (condition.operation == Condition::Operation::INT_OVERFLOW) {
        return !CreateIntegerCondition(op, condition.intTypeInfo);
    }
    if (condition.operation == Condition::Operation::INT_MAX_VAL) {
        return CreateExprByOp(Condition::Operation::EQ, op, CreateUIntegerExpr(condition.intTypeInfo.GetMaxValue()));
    }
    if (condition.operation == Condition::Operation::INT_MIN_VAL) {
        return CreateExprByOp(Condition::Operation::EQ, op, CreateIntegerExpr(condition.intTypeInfo.GetMinValue()));
    }
    return CreateExprByOp(condition.operation, op, CreateIntegerExpr(condition.operand), exprId);
}

z3::expr PathChecker::CreateExprByOp(Condition::Operation operation, const z3::expr& op1, const z3::expr& op2,
                                     ExprId exprId)
{
    switch (operation) {
        case Condition::Operation::EQ:
            return op1 == op2;
        case Condition::Operation::NE:
            return op1 != op2;
        case Condition::Operation::GT:
            return op1 > op2;
        case Condition::Operation::LT:
            return op1 < op2;
            // LCOV_EXCL_START
        case Condition::Operation::GE:
            return op1 >= op2;
        case Condition::Operation::LE:
            return op1 <= op2;
            // LCOV_EXCL_START
        case Condition::Operation::ACCESS_UNSINGED:
            return CreateAccessCondition(op1, op2, exprId, false);
        case Condition::Operation::ACCESS:
            return CreateAccessCondition(op1, op2, exprId, true);
            // LCOV_EXCL_START
        default:
            return CreateBoolExpr(false);
            // LCOV_EXCL_STOP
    }
}

static std::optional<z3::expr> FindSymbolicOperand(const z3::expr& expr, uint32_t depth = 0)
{
    auto declKind = expr.decl().decl_kind();
    if (declKind != Z3_OP_ADD && declKind != Z3_OP_SUB) {
        return expr.is_numeral() ? std::nullopt : std::optional<z3::expr>(expr);
    }
    auto res1 = FindSymbolicOperand(expr.arg(0), depth + 1);
    auto res2 = FindSymbolicOperand(expr.arg(1), depth + 1);
    return (depth == 0 && !res1 == !res2) ? std::nullopt : (res1 ? res1 : res2);
}

const z3::expr& PathChecker::GetOffsetBoundaryCondition(const z3::expr& offsetExpr, const z3::expr& sizeExpr)
{
    auto indexExpr = offsetExpr.arg(0);
    auto& indexInfo =
        myIndexOffsets.emplace(indexExpr.id(), IndexInfo{indexExpr, VirtualOffset{}, myTrueExpr}).first->second;
    if (indexInfo.isConditionInited) {
        return indexInfo.boundaryCondition;
    }
    indexInfo.isConditionInited = true;
    auto indexOpExpr = !myUntrustedSources.Check(indexExpr) ? FindSymbolicOperand(indexExpr) : std::nullopt;

    TraverseSinkExpression(myCurStack.front().symbols->GetCondition(), [&](const z3::expr& it) {
        auto declKind = it.decl().decl_kind();
        if (declKind != Z3_OP_LT && declKind != Z3_OP_LE && declKind != Z3_OP_GE && declKind != Z3_OP_GT) {
            return true;
        }
        auto lhe = it.arg(0);
        auto rhe = it.arg(1);
        if (lhe.id() == indexExpr.id()) {
            if (auto rheSymbolId = SymbolId::CreateSymbolIdFromExpr(rhe); rheSymbolId) {
                indexInfo.boundaryCondition = indexInfo.boundaryCondition && (rhe * offsetExpr.arg(1) <= sizeExpr);
            }
        }
        auto opExpr = indexOpExpr ? FindSymbolicOperand(lhe) : std::nullopt;
        if (!opExpr || opExpr->id() != indexOpExpr->id()) {
            return true;
        }
        auto rheSource = myUntrustedSources.Check(rhe);
        if (rheSource && *rheSource == nullptr && rhe.id() != CreateIntegerExpr(0).id()) {
            auto cond = lhe >= 0 && indexExpr >= 0 &&
                        (declKind == Z3_OP_LT || declKind == Z3_OP_GE ? lhe < rhe
                         : declKind == Z3_OP_GT                       ? lhe <= rhe
                                                                      : lhe <= rhe + 1);
            auto execId = UNDEF_EXEC_ID;
            if (auto execIt = myTerminatorsExecIds.find(it.id()); execIt != myTerminatorsExecIds.end()) {
                execId = execIt->second;
            }
            AddUntrustedSource(
                UntrustedSourceExpr{UntrustedSource::BOUNDARY_CONDITION, 0, execId, 0, nullptr, indexExpr, cond, rhe});
        }
        return true;
    });
    return indexInfo.boundaryCondition;
}

z3::expr PathChecker::CreateAccessCondition(const z3::expr& expr, const z3::expr& offset, ExprId exprId,
                                            bool isSignedType)
{
    const SymbolSizeInfo* sizeInfo = FindSizeExpr(expr);
    if (sizeInfo == nullptr) {
        return CreateBoolExpr(false);  // LCOV_EXCL_LINE
    }
    auto beginMemoryOffset = expr - sizeInfo->expr;
    auto endMemoryOffset = beginMemoryOffset + offset;
    if (exprId != UNDEF_EXPR_ID) {
        myAccessInfoMap.emplace(exprId, AccessInfo{sizeInfo, beginMemoryOffset, endMemoryOffset});
    }
    auto sizeExpr = sizeInfo->sizeExpr;
    if (sizeInfo->isUnknownSize) {
        sizeExpr = CreateIntegerExpr(1);  // if expression's size is undefined, use minimal buffer size
    }
    z3::expr resultExpr = endMemoryOffset > sizeExpr;
    if (sizeExpr.is_numeral() && myCurStack.size() <= 2) {
        if (auto offsetExpr = GetOffsetExprFromPointerOffset(expr); offsetExpr) {
            resultExpr = resultExpr && GetOffsetBoundaryCondition(*offsetExpr, sizeExpr);
        }
    }
    if (isSignedType) {
        resultExpr = beginMemoryOffset < 0 || endMemoryOffset == INT32_MAX || resultExpr;
    }
    return resultExpr && sizeInfo->condition && sizeExpr > 0;
}

z3::expr PathChecker::CreateSymbolSizeExpr(const SymbolId& symbolId, const z3::expr& symbolExpr, bool strLen)
{
    auto exprSymbolId = SymbolId::CreateSymbolIdFromExpr(symbolExpr);
    auto sizeSymbolId = exprSymbolId ? *exprSymbolId + CreateUniqueOffset() : symbolId;
    auto result = CreateSymbolExpr(sizeSymbolId);

    auto it = mySymbolSizes.find(symbolExpr.id());
    if (it != mySymbolSizes.end() && (it->second.isLiteral || !strLen)) {
        myConstraints = myConstraints && result == (strLen ? it->second.sizeExpr - 1 : it->second.sizeExpr);
        if (it->second.isLiteral) {
            AddUntrustedSource(UntrustedSourceExpr{UntrustedSource::STRING_LENGTH, 0, GetCurExecId(), 0, nullptr,
                                                   result, myTrueExpr, result});
        }
    } else {
        myConstraints = myConstraints && result >= 0;
        if (it != mySymbolSizes.end()) {
            it->second.isUnknownSize = false;
            myConstraints = myConstraints && result < it->second.sizeExpr;
        }
    }
    return result;
}

void PathChecker::AddOffset(ExprId exprId, VirtualOffset offset)
{
    if (exprId >= myStoredExpressions.size() || offset.IsSourceObject()) {
        return;
    }
    myStoredExpressions[exprId].symbolId.AddSubOffset(offset);
    myStoredExpressions[exprId].expr = GetSymbol(myStoredExpressions[exprId].symbolId);
}

void PathChecker::AddUnsignedCondition(const z3::expr& expr)
{
    if (expr.is_const() && myUnsignedExpr.emplace(expr.id(), expr).second) {
        myConstraints = myConstraints && (expr >= 0);
    }
}

void PathChecker::AddNotNullCondition(const z3::expr& expr)
{
    myConstraints = myConstraints && (expr != 0);
}

bool PathChecker::IsUntrustedSource(ExprId exprId)
{
    if (exprId >= myStoredExpressions.size()) {
        return false;  // LCOV_EXCL_LINE: HCAT-2778
    }
    return myUntrustedSources.Check(myStoredExpressions[exprId].expr).has_value();
}

bool PathChecker::IsCallArgument(ExprId exprId) const
{
    auto symbolId = SymbolId::CreateSymbolIdFromExpr(myStoredExpressions[exprId].expr);
    if (!symbolId) {
        return false;
    }
    auto& callArgs = GetCallArgs(myCurStack.back().callStackPos);
    uint32_t argPos = FunctionBehaviorImpl::GetArgPos(*symbolId);
    return argPos < callArgs.size();
}

void PathChecker::AddStringCondition(const std::string& cond, bool setResult, uint32_t argPos, VirtualOffset offset)
{
    z3::expr constraints = myTrueExpr;
    auto expr = StringToExprConverter(cond, setResult, argPos, offset,
                                      [this, &constraints, argPos](bool isFreeSymbol, bool isConst, uint32_t value,
                                                                   VirtualOffset* offset) {
                                          if (isFreeSymbol) {
                                              auto symbol = *myCurSymbolId + ExecutionContext::CreateUniqueOffset();
                                              auto freeSymbolExpr = CreateSymbolExpr(symbol);
                                              auto currExpr = GetArgExprByPos(argPos - 1, offset);
                                              constraints = constraints && freeSymbolExpr != currExpr;
                                              return freeSymbolExpr;
                                          } else if (isConst) {
                                              return CreateIntegerExpr(value);
                                          } else {
                                              auto expr = GetArgExprByPos(value - 1, offset);
                                              return CastToBV(expr);
                                          }
                                      })
                    .Convert();
    if (!expr) {
        Log(LogLevel::ERROR) << "Condition for solver from annotation is malformed: " << cond << "\n";
        return;
    }
    if (setResult && argPos == 0) {
        CallStack::SetCallResult(*expr);
    } else {
        myConstraints = myConstraints && *expr;
    };
    myConstraints = myConstraints && constraints;
}

void PathChecker::AddUntrustedSourceByKind(ExprId expr, const UntrustedSource::SourceKind kind)
{
    auto untrustedExpr = myStoredExpressions[expr];
    UntrustedSourceExpr source{
        kind, 0, untrustedExpr.execId, 0, nullptr, untrustedExpr.expr, untrustedExpr.condition, untrustedExpr.expr};
    AddUntrustedSource(std::move(source));
}
