/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLSCONTEXT_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLSCONTEXT_H_

#include <unordered_map>

#include "ExecutionContext.h"

class SymbolsContext {
public:
    using SymbolsMap = std::unordered_map<SymbolId, z3::expr, SymbolId::Hash>;
    using Ptr = std::shared_ptr<SymbolsContext>;

    explicit SymbolsContext(const z3::expr& condition) : myCondition(condition) {}
    ~SymbolsContext() = default;

    const z3::expr& SetSymbol(const SymbolId& symbolId, const z3::expr& symbolExpr)
    {
        return mySymbols.insert_or_assign(symbolId, symbolExpr).first->second;
    }

    const z3::expr& GetSymbol(ExecutionContext& context, const SymbolId& symbolId)
    {
        auto joinIt = myJoinedSymbols.begin();
        if (myJoinedSymbols.size() == 1) {
            auto it = mySymbols.find(symbolId);
            return it != mySymbols.end() ? it->second : (*joinIt)->GetSymbol(context, symbolId);
        }
        auto it = mySymbols.emplace(symbolId, myCondition);
        auto& expr = it.first->second;
        if (!it.second) {
            return expr;
        }
        if (myJoinedSymbols.empty() && symbolId.GetStackPos().uniqueId >= myCallId) {
            expr = context.CreateSymbolExpr(symbolId);
            return expr;
        }
        auto& defSymbols = myJoinedSymbols.empty() ? myPrevSymbols : *joinIt++;
        expr = defSymbols->GetSymbol(context, symbolId);
        for (auto defId = expr.id(); joinIt != myJoinedSymbols.end(); ++joinIt) {
            auto joinExpr = (*joinIt)->GetSymbol(context, symbolId);
            if (defId != joinExpr.id()) {
                expr = z3::ite((*joinIt)->myCondition, context.CastToBV(joinExpr), context.CastToBV(expr));
            }
        }
        return expr;
    }

    void Init(const SymbolsContext::Ptr& prevSymbols, uint16_t callId, const z3::expr& condition)
    {
        myCondition = condition;
        myPrevSymbols = prevSymbols;
        myCallId = callId;
    }

    void Join(const SymbolsContext::Ptr& source, const z3::expr& joinCondition)
    {
        myCondition = (myCondition || (source->myCondition && joinCondition));
        myJoinedSymbols.emplace_back(source);
    }

    const z3::expr& GetCondition() const
    {
        return myCondition;
    }

private:
    z3::expr myCondition;
    SymbolsMap mySymbols;
    SymbolsContext::Ptr myPrevSymbols;
    uint16_t myCallId = 0;
    std::vector<SymbolsContext::Ptr> myJoinedSymbols;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLSCONTEXT_H_
