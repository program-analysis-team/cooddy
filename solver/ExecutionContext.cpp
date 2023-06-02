/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "ExecutionContext.h"

#include <utils/LocaleUtils.h>
#include <utils/Log.h>

#include "FunctionBehaviorImpl.h"
#include "TraverseExpression.h"

class ConfigWithParams : public z3::config {
    ConfigWithParams()
    {
        set("model", true);
        set("proof", false);
#ifdef PRODUCTION_BUILD
        set("rlimit", 10000000);
        set("timeout", 5000);
#else
        set("rlimit", 100000000);
        set("timeout", 50000);
#endif
    }

public:
    static z3::config& GetInstance()
    {
        static ConfigWithParams config;
        return config;
    }
};

std::shared_mutex MemoryMonitor::myMutex;
bool MemoryMonitor::myNeedResetMemory = false;

ExecutionContext::ExecutionContext(FunctionBehaviorImpl* functionBehavior)
    : myContext(ConfigWithParams::GetInstance()), myTrueExpr(CreateBoolExpr(true)), myFunctionBehavior(functionBehavior)
{}

ExecutionContext::~ExecutionContext()
{
    myLogicalOperands.clear();
}

const SymbolId ExecutionContext::THROW_SYMBOL_ID = SymbolId{{0, 0}, THROW_INSTRUCTION};

bool ExecutionContext::EnterFunction(FunctionBehaviorImpl& function, SymbolsContextPtr entrySymbols,
                                     uint32_t entryBlock)
{
    uint16_t curCallId = myCallInfo.size();
    myCallInfo.emplace_back(CallInfo{uint16_t(myCurStack.size()), GetCurExecId(), &function});
    myExecBranchesInfo.EnterFunction(entryBlock, curCallId);

    auto prevSymbols = myCurStack.empty() ? nullptr : myCurStack.back().symbols;
    if (myCurStack.empty()) {
        entrySymbols->SetSymbol(THROW_SYMBOL_ID, CreateIntegerExpr(0));
    }
    myCurStack.emplace_back(StackNode{curCallId, GetCurExecId(), CreateBoolExpr(false),
                                      static_cast<uint32_t>(function.GetBlocks().size())});
    myCurStack.back().symbols = entrySymbols;
    myCurStack.back().callStackPos = GetCallStackSize();

    if (myCurStack.back().callStackPos != 0) {
        auto& args = GetCurCallArgs();
        for (size_t i = 0; i < args.size(); ++i) {
            auto argSymbolId = FunctionBehaviorImpl::GetArgSymbolId(GetStackPos(), i);
            SetSymbol(argSymbolId, args[i].expr, &args[i].symbolId);
        }
    }
    entrySymbols->Init(prevSymbols, curCallId, myTrueExpr);
    return true;
}

void ExecutionContext::LeaveFunction(SymbolsContextPtr exitSymbols)
{
    myExitSymbols = exitSymbols;
    auto curStackPos = GetStackPos();
    auto endlessLoopCondition = myCurStack.back().endlessLoopExitCondition;
    if (!endlessLoopCondition.is_false()) {
        endlessLoopCondition = !endlessLoopCondition;
        AddThrowExpr(&endlessLoopCondition);
    }
    myExitSetSymbols.swap(myCurStack.back().setSymbols);
    myCurStack.pop_back();
    if (myCurStack.empty()) {
        return;
    }
    // copy modified arguments' fields to the previous stack
    for (auto& symbol : myExitSetSymbols) {
        if (symbol.GetStackPos() < curStackPos) {
            SetSymbol(symbol, myExitSymbols->GetSymbol(*this, symbol));
        } else if (symbol.GetVarInstr() == RET_INSTRUCTION) {
            SetCallResult(myExitSymbols->GetSymbol(*this, symbol));
        }
    }
}

z3::expr ExecutionContext::Execute(const BasicBlock& block, SymbolsContextPtr symbols, ExecutionCallbackRef callback)
{
    myCurStack.back().symbols = symbols;
    UpdateLoopsState(block);

    auto result = myTrueExpr;
    try {
        result = block.Execute(*this, callback);
    }
    // LCOV_EXCL_START
    catch (z3::exception& ex) {
        Log(LogLevel::ERROR) << "Z3 execute error: " << ex.what() << std::endl;
    }
    // LCOV_EXCL_STOP
    return result;
}

const z3::expr& ExecutionContext::SetSymbol(const SymbolId& symbolId, const z3::expr& symbolExpr, const SymbolId* srcId)
{
    if (srcId != nullptr) {
        mySymbolAliases.AddAlias(symbolId, *srcId);
    }
    myCurStack.back().setSymbols.insert(symbolId);
    return myCurStack.back().symbols->SetSymbol(symbolId, symbolExpr);
}

const z3::expr& ExecutionContext::GetSymbolExpr(const SymbolId& symbolId)
{
    return myCurStack.back().symbols->GetSymbol(*this, symbolId);
}

const z3::expr& ExecutionContext::GetSymbol(SymbolId& symbolId)
{
    symbolId = ResolveSymbolId(symbolId);
    return GetSymbolExpr(symbolId);
}

const z3::expr& ExecutionContext::GetSubSymbol(SymbolId& symbolId, VirtualOffset offset)
{
    if (symbolId.GetOffset().CanAddSubOffset()) {
        symbolId.AddSubOffset(offset);
    } else {
        // case when offset limit is reached
        auto& uniqueOffset = myUniqueOffsets[symbolId.GetOffset().GetRawValue()];
        if (uniqueOffset.IsSourceObject()) {
            uniqueOffset = CreateUniqueOffset();
        }
        symbolId = SymbolId(symbolId, uniqueOffset + offset);
    }
    return GetSymbol(symbolId);
}

std::optional<z3::expr> ExecutionContext::GetOffsetExprFromPointerOffset(const z3::expr& expr)
{
    if (expr.decl().decl_kind() != Z3_OP_ADD) {
        return std::nullopt;
    }
    auto offsetExpr = expr.arg(1);
    if (offsetExpr.decl().decl_kind() != Z3_OP_MUL) {
        return std::nullopt;  // LCOV_EXCL_LINE
    }
    return offsetExpr;
}

std::optional<SymbolId> ExecutionContext::ConvertPointerOffsetIntoIndex(const z3::expr& expr)
{
    auto offsetExpr = GetOffsetExprFromPointerOffset(expr);
    if (!offsetExpr) {
        return std::nullopt;
    }
    auto arrSymbolId = SymbolId::CreateSymbolIdFromExpr(expr.arg(0));
    if (!arrSymbolId) {
        return std::nullopt;
    }
    auto indexExpr = offsetExpr->arg(0);
    arrSymbolId->AddSubOffset(GetIndexOffset(indexExpr));
    return *arrSymbolId;
}

std::optional<z3::expr> ExecutionContext::ConvertIndexIntoPointerOffset(SymbolId& symbolId, uint32_t elementSize)
{
    VirtualOffset offset = symbolId.GetOffset();
    auto [kind, idx] = offset.ExtractSubOffset();

    if (kind == VirtualOffset::Kind::INDEX) {
        symbolId = SymbolId{symbolId, offset - VirtualOffset{VirtualOffset::Kind::INDEX}};
        return GetSymbol(symbolId) + CreateIntegerExpr(idx) * CreateIntegerExpr(elementSize);
    }

    return std::nullopt;
}

bool ExecutionContext::IsExitSymbol(const SymbolId& symbol)
{
    if (myFunctionBehavior->IsExitSymbol(symbol)) {
        return true;
    }

    if (!symbol.IsSubOffset()) {
        return false;
    }
    auto strippedSymb = SymbolId(symbol, VirtualOffset());
    auto expr = myExitSymbols->GetSymbol(*this, strippedSymb);
    if (!expr.is_ite()) {
        return false;
    }
    std::optional<SymbolId> outputSymb;
    TraverseSinkExpression(expr, [&](const z3::expr& expr) {
        if (!expr.is_ite()) {
            std::optional resolvedId = SymbolId::CreateSymbolIdFromExpr(expr);
            if (resolvedId.has_value() && myFunctionBehavior->IsExitSymbol(*resolvedId)) {
                outputSymb = resolvedId;
                return false;
            }
        }
        return true;
    });

    return outputSymb.has_value();
}

SymbolId ExecutionContext::ResolveSymbolId(const SymbolId& symbolId)
{
    auto resolveAlias = [this](const SymbolId& id) -> std::optional<SymbolId> {
        const z3::expr& expr = GetSymbolExpr(SymbolId(id, VirtualOffset{}));
        std::optional resolvedId = SymbolId::CreateSymbolIdFromExpr(expr);
        if (resolvedId.has_value()) {
            return *resolvedId + id.GetOffset();
        }

        return std::nullopt;
    };

    auto offset = symbolId.GetOffset();
    if (!offset.IsSourceObject() && offset != VirtualOffset(VirtualOffset::Kind::ADDR_OF)) {
        auto symbolIdResolved = resolveAlias(symbolId);

        if (symbolIdResolved && symbolIdResolved->GetStackPos().pos <= symbolId.GetStackPos().pos) {
            if (!symbolIdResolved->GetOffset().IsSourceObject()) {
                auto symbolIdResolved2 = resolveAlias(*symbolIdResolved);

                if (symbolIdResolved2) {
                    return *symbolIdResolved2;
                }
            }
            return *symbolIdResolved;
        }
    }
    if (!myCurStack.empty() && myCurStack.back().callStackPos != 0) {
        auto& callArgs = GetCallArgs(myCurStack.back().callStackPos - 1);
        uint32_t argPos = FunctionBehaviorImpl::GetArgPos(symbolId);

        if (argPos < callArgs.size() && symbolId.IsOutputArg(callArgs[argPos].type)) {
            return callArgs[argPos].symbolId + offset;
        }
    }
    return symbolId;
}

z3::expr ExecutionContext::CreateSymbolExpr(const SymbolId& symbolId)
{
    return symbolId.CreateExpr(myContext);
}

z3::expr ExecutionContext::CreateIntegerExpr(int64_t value)
{
    return myContext.int_val(value);
}

z3::expr ExecutionContext::CreateUIntegerExpr(uint64_t value)
{
    return myContext.int_val(value);
}

z3::expr ExecutionContext::CreateIntegerCondition(z3::expr opExpr, IntTypeInfo opInfo)
{
    CastToBV(opExpr);
    return opExpr >= CreateIntegerExpr(opInfo.GetMinValue()) && opExpr <= CreateUIntegerExpr(opInfo.GetMaxValue());
}

VirtualOffset ExecutionContext::CreateUniqueOffset()
{
    VirtualOffset result(VirtualOffset::Kind::INDEX, INT8_MAX);
    for (auto curOffset = myCurUniqueOffset++; curOffset != 0; curOffset /= INT8_MAX) {
        result += VirtualOffset(VirtualOffset::Kind::INDEX, curOffset % INT8_MAX);
    }
    return result;
}

z3::expr& ExecutionContext::CastToBV(z3::expr& expr)
{
    if (expr.is_bool()) {
        expr = z3::ite(expr, CreateIntegerExpr(1), CreateIntegerExpr(0));
    }
    return expr;
}

z3::expr& ExecutionContext::CastToBool(z3::expr& expr)
{
    if (expr.is_int()) {
        expr = (expr != CreateIntegerExpr(0));
    }
    return expr;
}

z3::expr ExecutionContext::CreateBoolExpr(bool value)
{
    return myContext.bool_val(value);
}

void ExecutionContext::AddThrowExpr(const z3::expr* addCondition)
{
    auto expr = CreateIntegerExpr(UNDEFINED_VALUE);
    if (addCondition != nullptr) {
        auto condition = *addCondition;
        expr = z3::ite(CastToBool(condition), expr, GetSymbolExpr(THROW_SYMBOL_ID));
    }
    SetSymbol(THROW_SYMBOL_ID, expr);
}

z3::expr ExecutionContext::GetCurConstraint()
{
    z3::expr condition = myTrueExpr;
    for (auto& stack : myCurStack) {
        condition = condition && stack.symbols->GetCondition();
    }
    condition = condition && (GetSymbolExpr(THROW_SYMBOL_ID) == 0);
    return condition;
}

void ExecutionContext::CopyCallArg(uint32_t srcPos, uint32_t dstPos, VirtualOffset dstOffset, VirtualOffset srcOffset)
{
    auto& args = GetCurCallArgs();
    if (srcPos >= args.size()) {
        return;  // LCOV_EXCL_LINE: HCAT-2778
    }

    if (dstPos >= args.size()) {
        auto retSubExpr = SymbolId::CreateSubExpr(args[srcPos].expr, dstOffset);
        if (!retSubExpr) {
            return;  // LCOV_EXCL_LINE
        }
        *myCurExpr = *retSubExpr;
        SetCallResult(*retSubExpr);
    } else if (args[dstPos].symbolId.IsValid()) {
        SymbolId srcSymbolId = args[srcPos].symbolId + srcOffset;
        auto srcExpr = GetSymbol(srcSymbolId);
        auto subDstSymbol = args[dstPos].symbolId;
        subDstSymbol.AddSubOffset(srcOffset);
        subDstSymbol = ResolveSymbolId(subDstSymbol);
        SetSymbol(subDstSymbol, srcExpr);
    }
}

void ExecutionContext::AssignSizeExpr(uint32_t argPos, VirtualOffset offset, uint32_t arrayArgPos)
{
    auto& args = GetCurCallArgs();
    if (argPos != UINT32_MAX && argPos >= args.size() || arrayArgPos >= args.size()) {
        return;  // LCOV_EXCL_LINE
    }
    auto& arrayExpr = args[arrayArgPos].expr;
    uint32_t numeralValue = 0;
    auto newValue = arrayExpr.is_numeral_u(numeralValue)
                        ? CreateIntegerExpr(numeralValue >> 16U)
                        : CreateSymbolSizeExpr(args[arrayArgPos].symbolId, arrayExpr, true);

    if (argPos == UINT32_MAX) {
        CallStack::SetCallResult(newValue);
        *myCurExpr = newValue;
    } else {
        // LCOV_EXCL_START
        if (offset.IsSourceObject()) {
            CallStack::SetCallArg(argPos, newValue);
        }
        // LCOV_EXCL_STOP
    }
}

void ExecutionContext::AssignCallArgValue(uint32_t argPos, VirtualOffset offset, int64_t value)
{
    auto& args = GetCurCallArgs();
    if (argPos >= args.size()) {
        return;  // LCOV_EXCL_LINE
    }
    SymbolId symbolId = ResolveSymbolId(args[argPos].symbolId + offset);
    SymbolId newSymbolId(GetStackPos(), args[argPos].instruction);
    newSymbolId.SetChanged();
    z3::expr newValue = value == UNDEFINED_VALUE ? CreateSymbolExpr(newSymbolId) : CreateIntegerExpr(value);

    if (value == UNDEFINED_VALUE) {
        uint32_t aliasIndex = 0;
        mySymbolAliases.TraverseAliases(symbolId, [&](const SymbolId& alias) {
            if (alias.GetStackPos().pos < GetStackPos().pos || !mySymbolAliases.HasAlias(alias)) {
                SymbolId aliasId(newSymbolId, VirtualOffset(VirtualOffset::Kind::INDEX, ++aliasIndex));
                SetSymbol(alias, CreateSymbolExpr(aliasId));
            }
        });
    }
    SetSymbol(symbolId, newValue);

    if (offset.IsSourceObject()) {
        CallStack::SetCallArg(argPos, newValue);
    }
}

z3::expr ExecutionContext::GetArgExprByPos(uint32_t argPos, VirtualOffset* offset)
{
    if (CallStack::GetCallStackSize() == 0) {
        return CreateSymbolExpr(FunctionBehaviorImpl::GetArgSymbolId(StackPos(), argPos));
    }
    auto& args = GetCurCallArgs();
    if (argPos >= args.size()) {
        // return value
        return GetCallResult();
    }
    auto symbolId = args[argPos].symbolId;
    if (offset != nullptr && !offset->IsSourceObject()) {
        return GetSymbolExpr(symbolId + *offset);
    }
    if (!symbolId.GetOffset().IsAddrOf()) {
        return args[argPos].expr;
    }
    return GetSymbolExpr(symbolId + VirtualOffset(VirtualOffset::Kind::DEREF));
}

void ExecutionContext::AddUntrustedSource(UntrustedSourceExpr&& source)
{
    if (!myUntrustedSources.Add(source)) {
        return;
    }
    auto symbolId = SymbolId::CreateSymbolIdFromExpr(source.expr);
    if (!symbolId) {
        symbolId = ConvertPointerOffsetIntoIndex(source.expr);
    }
    if (symbolId && !symbolId->GetOffset().IsSourceObject()) {
        auto ownerSource = source;
        ownerSource.expr = CreateSymbolExpr(SymbolId(*symbolId, VirtualOffset()));
        ownerSource.propagateToSubOffsets = false;
        myUntrustedSources.Add(ownerSource);
    }
}

void ExecutionContext::AddCallArgUntrustedSource(uint32_t argPos, SourceId sourceId)
{
    auto expr = GetArgExprByPos(argPos);
    UntrustedSourceExpr source{UntrustedSource::CALL_ARG, sourceId, argPos, argPos, nullptr, expr, myTrueExpr, expr};
    if (CallStack::GetCallStackSize() != 0) {
        source.execId = GetCurExecId();
        source.condition = GetCurConstraint();
    }
    source.intTypeInfo = GetCallArgInfo(argPos).type.intInfo;
    AddUntrustedSource(std::move(source));
}

void ExecutionContext::CopyCallArgUntrustedSource(uint32_t dstArgPos, uint32_t srcArgPos)
{
    z3::expr expr = GetArgExprByPos(srcArgPos);
    auto untrustedSource = myUntrustedSources.Check(expr);
    if (untrustedSource && *untrustedSource != nullptr) {
        AddUntrustedSource(UntrustedSourceExpr{UntrustedSource::PROPAGATION, srcArgPos, GetCurExecId(), dstArgPos,
                                               *untrustedSource, GetArgExprByPos(dstArgPos), GetCurConstraint(), expr});
    }
}

void ExecutionContext::CopySubOffsetUntrustedSource(const z3::expr& dstExpr, const z3::expr& srcExpr)
{
    auto untrustedSource = myUntrustedSources.Find(srcExpr);
    if (untrustedSource != nullptr) {
        AddUntrustedSource(UntrustedSourceExpr{UntrustedSource::NONE, 0, GetCurExecId() + 1, 0, untrustedSource,
                                               dstExpr, untrustedSource->condition, srcExpr, false});
    }
}

void ExecutionContext::SetSymbolSize(const z3::expr& symbol, const z3::expr& symbolSize, bool isLiteral)
{
    mySymbolSizes.emplace(symbol.id(), SymbolSizeInfo{symbol, symbolSize, myTrueExpr, false, isLiteral});
}

void ExecutionContext::AddLoopExitCondition()
{
    auto& curStack = myCurStack.back();
    auto& loops = curStack.loopsStack;
    if (!loops.empty()) {
        loops.back().breakCondition = loops.back().breakCondition && curStack.symbols->GetCondition();
        if (loops.back().entranceBlock.IsEndlessLoopEntry()) {
            curStack.endlessLoopExitCondition = curStack.endlessLoopExitCondition || curStack.symbols->GetCondition();
        }
    }
}

const ExecutionContext::SymbolSizeInfo* ExecutionContext::FindSizeExpr(const z3::expr& expr)
{
    std::unordered_map<int32_t, const SymbolSizeInfo*> processed;
    std::function<const SymbolSizeInfo*(const z3::expr&)> traverse = [&](const z3::expr& it) {
        auto entry = processed.emplace(it.id(), nullptr);
        auto& res = entry.first->second;
        if (!entry.second) {
            return res;
        }
        if (it.is_ite()) {
            auto res1 = traverse(it.arg(1));
            auto res2 = traverse(it.arg(2));
            res = res1 == res2 ? res1 : nullptr;
        } else if (it.is_const() && !it.is_numeral()) {
            auto sizeIt = mySymbolSizes.find(it.id());
            res = sizeIt != mySymbolSizes.end() ? &sizeIt->second : nullptr;
        } else {
            for (uint32_t i = 0, n = it.num_args(); i < n && res == nullptr; ++i) {
                res = traverse(it.arg(i));
            }
        }
        return res;
    };
    return traverse(expr);
}

void ExecutionContext::AddLoopIterator(Instruction loopInstr, Instruction iterInstr, int32_t increment,
                                       IntTypeInfo typeInfo)
{
    if (increment == 0 || myCurStack.back().loopsStack.empty()) {
        return;
    }
    auto& loop = myCurStack.back().loopsStack.back();
    if (myCurStack.back().block != loop.entranceBlock.GetPosition()) {
        return;  // LCOV_EXCL_LINE: HCAT-2778
    }
    SymbolId iterSymbolId(GetStackPos(), iterInstr);
    auto initExpr = GetSymbol(iterSymbolId);
    if (myUntrustedSources.Check(initExpr) == std::nullopt && FindSizeExpr(initExpr) == nullptr) {
        return;
    }
    loop.iterInfo.push_back({iterSymbolId, increment, typeInfo, myTrueExpr});
    loop.initExpr.push_back(initExpr);

    if (loop.iterInfo.size() == 1) {
        loop.iterStepExpr = CreateSymbolExpr(SymbolId(GetStackPos(), loopInstr));
    }
    auto multExpr = CreateIntegerExpr(increment);
    for (uint32_t i = 1; i < loop.iterInfo.size(); ++i) {
        multExpr = multExpr * CreateIntegerExpr(1);
    }
    auto iterExpr = initExpr + loop.iterStepExpr * multExpr;
    loop.iterExpr.push_back(iterExpr);

    auto iterConstraint = increment > 0 ? iterExpr < initExpr : iterExpr > initExpr;
    AddThrowExpr(&iterConstraint);

    SetSymbol(iterSymbolId, iterExpr);
}

void ExecutionContext::UpdateLoopsState(const BasicBlock& block)
{
    auto& loops = myCurStack.back().loopsStack;

    while (!loops.empty() && loops.back().entranceBlock.GetEndOfLoop() > block.GetPosition()) {
        auto& loop = loops.back();
        for (auto i = 0; i < loop.iterInfo.size(); i++) {
            auto exitValue = loop.initExpr[i];
            auto& boundCondition = loop.iterInfo[i].boundCondition;
            if (loop.iterInfo[i].increment > 0 && boundCondition.is_numeral() && !block.IsInLoop()) {
                exitValue = loop.breakCondition.is_true() ? boundCondition
                                                          : z3::ite(loop.breakCondition, exitValue, boundCondition);
            }
            SetSymbol(loop.iterInfo[i].symbolId, exitValue);
        }
        loops.pop_back();
    }
    if (block.IsLoopEntrance() && (loops.empty() || loops.back().entranceBlock.GetPosition() > block.GetPosition())) {
        loops.emplace_back(LoopNode{block, myTrueExpr, myContext, myContext, myTrueExpr});
    }
}

bool ExecutionContext::CheckLoopCondition(z3::expr& condition, LoopNode& loop, bool loopEntrance)
{
    bool foundIter = false;
    z3::expr_vector src(myContext);
    z3::expr_vector dst(myContext);
    TraverseSinkExpression(condition, [&](const z3::expr& expr) {
        auto kind = expr.decl().decl_kind();
        bool isCompare = kind == Z3_OP_DISTINCT || kind == Z3_OP_EQ || kind == Z3_OP_GE || kind == Z3_OP_GT ||
                         kind == Z3_OP_LE || kind == Z3_OP_LT;

        for (auto i = 0; i < loop.iterExpr.size(); ++i) {
            const auto& iter = loop.iterExpr[i];
            if (iter.id() == expr.id()) {
                foundIter = true;
                return false;
            }
            auto pos = !isCompare ? -1 : iter.id() == expr.arg(0).id() ? 1 : (iter.id() == expr.arg(1).id() ? 0 : -1);
            if (pos == -1) {
                continue;
            }
            z3::expr argExpr = expr.arg(pos);
            if (!loop.addedItersAsUntrusted) {
                loop.iterInfo[i].boundCondition = argExpr;
            }
            if (loopEntrance && kind == Z3_OP_DISTINCT || !loopEntrance && kind == Z3_OP_EQ) {
                src.push_back(expr);
                auto newCond = loop.iterInfo[i].increment > 0 ? (iter < argExpr) : (iter > argExpr);
                dst.push_back(loopEntrance ? newCond : !newCond);
            }
        }
        return true;
    });
    if (!src.empty()) {
        condition = condition.substitute(src, dst);
    }
    return foundIter;
}

void ExecutionContext::ModifyLoopCondition(z3::expr& condition, LoopNode& loop, bool loopEntrance, uint32_t branch)
{
    if (!CheckLoopCondition(condition, loop, loopEntrance)) {
        loop.shouldExecute = loop.shouldExecute && !loopEntrance;
        return;
    }
    auto newCondition = condition.substitute(loop.iterExpr, loop.initExpr);
    auto untrustedSource = branch == 0 ? myUntrustedSources.Check(newCondition) : std::nullopt;
    if (loopEntrance) {
        if (branch == 0) {
            auto throwCondition = newCondition && !condition;
            AddThrowExpr(&throwCondition);
        }
        condition = newCondition;
    }
    if (!loop.addedItersAsUntrusted && untrustedSource) {
        AddLoopItersAsUntrusted(condition, loop, *untrustedSource);
        loop.addedItersAsUntrusted = true;
    }
}

void ExecutionContext::AddLoopItersAsUntrusted(const z3::expr& boundCondition, LoopNode& loop,
                                               const UntrustedSourceExpr* untrustedSource)
{
    for (auto i = 0; i < loop.iterExpr.size(); ++i) {
        auto& iterInfo = loop.iterInfo[i];
        auto cond = iterInfo.boundCondition.is_true() ? boundCondition : iterInfo.boundCondition;
        UntrustedSourceExpr source{UntrustedSource::ITERATOR, 0,          GetCurExecId(), 0,    untrustedSource,
                                   loop.iterExpr[i],          myTrueExpr, cond,           true, iterInfo.typeInfo};
        if (untrustedSource != nullptr) {
            source.condition = untrustedSource->condition;
            source.sourceExpr = untrustedSource->expr;
        }
        myUntrustedSources.Add(std::move(source));
    }
}

z3::expr ExecutionContext::GetJoinCondition(const BasicBlock& src, const BasicBlock& dst, uint32_t branch,
                                            z3::expr& terminator, z3::expr& joinedCondition)
{
    z3::expr joinCond = dst.GetJoinCondition(*this, branch, terminator, joinedCondition);

    if (!myCurStack.back().loopsStack.empty()) {
        auto& loop = myCurStack.back().loopsStack.back();
        if (!loop.iterExpr.empty() && (src.IsLoopEntrance() || src.IsLoopExit())) {
            ModifyLoopCondition(joinCond, loop, src.IsLoopEntrance(), branch);
        }
    }
    myTerminatorsExecIds[terminator.id()] = GetCurExecId();

    myExecBranchesInfo.AddBranchBlock(joinCond, src, terminator, src.GetPosition(), dst.GetPosition(),
                                      myCurStack.back().callId);
    return joinCond;
}

bool ExecutionContext::IsCallExpr(CallExecInfo* callExecInfo) const
{
    if (myCurInstrKind != Node::Kind::CALL_EXPRESSION && myCurInstrKind != Node::Kind::CXX_MEMBER_CALL_EXPRESSION &&
        myCurInstrKind != Node::Kind::CXX_OPERATOR_CALL_EXPRESSION &&
        myCurInstrKind != Node::Kind::CXX_CONSTRUCT_EXPRESSION && myCurInstrKind != Node::Kind::CXX_DTOR_EXPRESSION &&
        myCurInstrKind != Node::Kind::DELETE_EXPRESSION && myCurInstrKind != Node::Kind::CXX_NEW_EXPRESSION) {
        return false;
    }
    if (callExecInfo == nullptr) {
        return true;
    }
    callExecInfo->instruction = GetCurInstruction();
    auto [isIndirect, isVirtual] = IsIndirectCall();
    if (!isIndirect) {
        return true;
    }
    auto callSymbolId = SymbolId::CreateSymbolIdFromExpr(GetCallExpr());
    if (isVirtual) {
        callExecInfo->isVirtualCall = true;
        if (callSymbolId) {
            auto recordId = myRecordIds.find(SymbolId(*callSymbolId, VirtualOffset{}));
            if (recordId != myRecordIds.end()) {
                callExecInfo->recordId = recordId->second;
            }
        }
    } else if (callSymbolId && !callSymbolId->IsGlobal()) {
        callExecInfo->instruction = callSymbolId->GetVarInstr();
        callExecInfo->callId = myCallInfo[callSymbolId->GetStackPos().uniqueId].execId;
    }
    return true;
}

size_t ExecutionContext::GetCallArgsCount() const
{
    if (CallStack::GetCallStackSize() == 0) {
        return myFunctionBehavior->GetArgTypes().size();
    }
    return GetCurCallArgs().size();
}

::ArgInfo ExecutionContext::GetCallArgInfo(uint32_t argPos) const
{
    if (CallStack::GetCallStackSize() == 0) {
        return {myFunctionBehavior->GetArgType(argPos), 0};
    }
    auto& args = GetCurCallArgs();
    if (argPos < args.size()) {
        return args[argPos];
    }
    return {GetCallResultType(), myCurInstruction};
}

VirtualOffset ExecutionContext::GetIndexOffset(z3::expr& indexExpr)
{
    // try find index value in loops iterators
    for (auto& loop : myCurStack.back().loopsStack) {
        for (auto i = 0; i < loop.iterExpr.size(); ++i) {
            if (loop.iterExpr[i].id() == indexExpr.id()) {
                indexExpr = loop.initExpr[i];
                break;
            }
        }
    }
    uint32_t offset = 0;
    if (indexExpr.is_numeral_u(offset)) {
        return VirtualOffset(offset != 0 ? VirtualOffset::Kind::INDEX : VirtualOffset::Kind::DEREF, offset);
    }
    auto& result =
        myIndexOffsets.emplace(indexExpr.id(), IndexInfo{indexExpr, VirtualOffset{}, myTrueExpr}).first->second.offset;
    if (result.IsSourceObject()) {
        result = CreateUniqueOffset();
    }
    return result;
}

void ExecutionContext::SetCurExecInfo(Instruction curInst, Node::Kind curInstrKind, z3::expr& curExpr,
                                      SymbolId& curSymbolId)
{
    myCurInstruction = curInst;
    myCurInstrKind = curInstrKind;
    myCurExpr = &curExpr;
    myCurSymbolId = &curSymbolId;
    if (curInstrKind != Node::Kind::PARAM_VAR_DECL) {
        myExecBranchesInfo.SetCurExecId(myCurStack.back().block, myCurStack.back().callId, myExecInfo.size());
    }
    myExecInfo.emplace_back(ExecInfo{myCurStack.back().callExecId, myCurInstruction});
}

const BasicBlock* ExecutionContext::GetNextBlockForExecution()
{
    auto& blocks = myCallInfo[myCurStack.back().callId].function->GetBlocks();
    auto& currentBlock = myCurStack.back().block;

    uint32_t nextBlock = 0;
    currentBlock = ShouldExecuteLoop(nextBlock) ? nextBlock : currentBlock - 1;
    return currentBlock < blocks.size() ? &blocks[currentBlock] : nullptr;
}

bool ExecutionContext::GetSuccessorForLastBlockInLoop(uint32_t& successor)
{
    auto& loops = myCurStack.back().loopsStack;
    if (loops.empty()) {
        return false;  // LCOV_EXCL_LINE
    }
    auto& loop = loops.back();
    if (loop.shouldExecute && loop.entranceBlock.IsDoWhileLoop()) {
        return true;
    }

    auto& blocks = myCallInfo[myCurStack.back().callId].function->GetBlocks();
    auto successorsCount = blocks[loop.entranceBlock.GetPosition()].GetSuccessorsCount();
    successor = successorsCount > 1 ? blocks[loop.entranceBlock.GetPosition()].GetSuccessor(successorsCount - 1)
                                    : blocks.size();  // LCOV_EXCL_LINE
    return successor < myCurStack.back().block;
}

bool ExecutionContext::ShouldExecuteLoop(uint32_t& successor)
{
    auto& loops = myCurStack.back().loopsStack;
    if (!loops.empty()) {
        auto& currentBlock = myCurStack.back().block;
        auto& loop = loops.back();
        if (loop.shouldExecute && currentBlock == loop.entranceBlock.GetEndOfLoop() &&
            loop.entranceBlock.IsDoWhileLoop()) {
            loop.shouldExecute = false;
            auto& blocks = myCallInfo[myCurStack.back().callId].function->GetBlocks();
            successor = blocks[loop.entranceBlock.GetEndOfLoop()].GetSuccessor(0);
            return true;
        }
    }
    return false;
}

SymbolId ExecutionContext::CreateGlobalSymbolId(uint32_t uniqueId, uint32_t instr)
{
    if (uniqueId == 0 || uniqueId == UINT32_MAX) {
        auto curFunc = myCallInfo[myCurStack.back().callId].function;
        const uint16_t id = myGlobalIds.emplace(curFunc, myGlobalIds.size() + 1).first->second;
        SymbolId symbolId({GetStackPos().pos, id}, instr);
        symbolId.SetGlobal();
        return symbolId;
    }
    SymbolId symbolId({0, (uint16_t)uniqueId}, 0);
    symbolId.SetGlobal();
    return symbolId;
}

z3::expr ExecutionContext::GetExprFromCache(Instruction instruction)
{
    auto& cache = myCurStack.back().exprCache;
    auto it = cache.find(instruction);
    return it != cache.end() ? it->second : CreateIntegerExpr(0);
}

const z3::expr& ExecutionContext::GetArrayExpr(SymbolId& arrSymbolId, const z3::expr& arrExpr, z3::expr& idxExpr)
{
    AddOperands({arrExpr, idxExpr});
    if (arrSymbolId.GetOffset().IsDeref()) {
        auto offset = arrSymbolId.GetOffset();
        offset.ExtractSubOffset(false);
        offset += VirtualOffset(VirtualOffset::Kind::INDEX, 0);
        arrSymbolId = SymbolId(arrSymbolId, offset);
    }
    auto& result = GetSubSymbol(arrSymbolId, GetIndexOffset(idxExpr));
    if (!result.is_const() || result.is_numeral() || myUntrustedSources.Check(idxExpr)) {
        return result;
    }
    auto it = myArrMaxValue.find(arrExpr.id());
    if (it == myArrMaxValue.end()) {
        return result;
    }
    auto cond = result >= 0 && result <= CreateIntegerExpr(it->second.second);
    AddUntrustedSource(
        UntrustedSourceExpr{UntrustedSource::NONE, 0, GetCurExecId() + 1, 0, nullptr, result, cond, result});
    return result;
}
