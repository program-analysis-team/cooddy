/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLALIASES_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLALIASES_H_

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SymbolId.h"

class SymbolAliases {
public:
    using TraverseCallback = const std::function<void(const SymbolId& alias)>&;

    void AddAlias(const SymbolId& symbolId, const SymbolId& alias)
    {
        myAliases[SymbolId(symbolId, VirtualOffset{})].emplace_back(
            std::make_pair(symbolId.GetOffset(), ResolveSymbol(alias, VirtualOffset::Kind::ADDR_OF)));
    }

    bool HasAlias(const SymbolId& symbolId)
    {
        const auto& c = myAliases[SymbolId(symbolId, VirtualOffset{})];
        for (const auto& el : c) {
            if (el.first == symbolId.GetOffset() && el.second != symbolId) {
                return true;
            }
        }
        return false;
    }

    void TraverseAliases(const SymbolId& symbolId, TraverseCallback callback)
    {
        SymbolsSet processed;
        TraverseAliases(ResolveSymbol(symbolId, VirtualOffset::Kind::DEREF), callback, processed);
    }

private:
    using AliasesArray = std::vector<std::pair<VirtualOffset, SymbolId>>;
    using SymbolsSet = std::unordered_set<SymbolId, SymbolId::Hash>;
    using AliasesMapping = std::unordered_map<SymbolId, AliasesArray, SymbolId::Hash>;

    SymbolId ResolveSymbol(const SymbolId& symbolId, VirtualOffset::Kind offsetKind)
    {
        auto offset = symbolId.GetOffset();
        if (offset.ExtractSubOffset(false).first != offsetKind) {
            return symbolId;
        }
        return SymbolId(symbolId, VirtualOffset{}) + offset;
    }

    void TraverseAliases(const SymbolId& symbolId, TraverseCallback callback, SymbolsSet& processed)
    {
        if (!processed.insert(symbolId).second) {
            return;
        }
        auto offset = symbolId.GetOffset();
        for (auto& it : myAliases[SymbolId(symbolId.GetStackPos(), symbolId.GetVarInstr())]) {
            if (it.first.IsSubOffsetOf(offset)) {
                TraverseAliases(it.second, callback, processed);
                callback(it.second);
            }
        }
    }

    AliasesMapping myAliases;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLALIASES_H_
