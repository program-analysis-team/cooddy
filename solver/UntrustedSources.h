/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_UNTRUSTEDSOURCES_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_UNTRUSTEDSOURCES_H_

#include <solver/SolverContext.h>

#include "TraverseExpression.h"

struct UntrustedSourceExpr : public ::UntrustedSource {
    z3::expr expr;
    z3::expr condition;
    z3::expr sourceExpr;
    bool propagateToSubOffsets = true;
    IntTypeInfo intTypeInfo = {0};

    bool IsExternal() const
    {
        return sourceKind == UntrustedSource::CALL_ARG || sourceKind == UntrustedSource::PROPAGATION ||
               sourceKind == UntrustedSource::SENSITIVE_DATA;
    }
};

class UntrustedSources {
public:
    bool Add(const UntrustedSourceExpr& source)
    {
        if (source.expr.is_bool() || source.expr.is_numeral()) {
            return false;
        }
        return mySources.emplace(source.expr.id(), std::move(source)).second;
    }

    const UntrustedSourceExpr* Find(const z3::expr& expr)
    {
        auto source = mySources.find(expr.id());
        if (source != mySources.end()) {
            return &source->second;
        }
        auto symbolId = SymbolId::CreateSymbolIdFromExpr(expr);
        if (symbolId && !symbolId->GetOffset().IsSourceObject()) {
            source = mySources.find(SymbolId(*symbolId, VirtualOffset{}).CreateExpr(expr.ctx()).id());
        }
        if (source != mySources.end() && source->second.propagateToSubOffsets) {
            return &source->second;
        }
        return nullptr;
    }

    /// Check if expression consists of constants or untrusted sources
    /// If succeed, returns the first untrusted source or nullptr if it is constant expression
    std::optional<const UntrustedSourceExpr*> Check(const z3::expr& expr)
    {
        if (expr.is_numeral()) {
            return nullptr;
        }
        auto source = Find(expr);
        if (source != nullptr) {
            return source;
        }
        auto it = myCache.emplace(expr.id(), std::nullopt);
        auto& res = it.first->second;
        if (!it.second) {
            return res;
        }
        if (expr.is_ite()) {
            res = Check(expr.arg(1));
            return res ? res : res = Check(expr.arg(2));
        }
        for (uint32_t i = 0; i < expr.num_args(); ++i) {
            auto argRes = Check(expr.arg(i));
            if (!argRes) {
                return res = std::nullopt;
            }
            if (!res || *res == nullptr) {
                res = argRes;
            }
        }
        return res;
    }

    struct FindResult {
        const UntrustedSource* source = nullptr;
        bool hasNonConstSources = false;
        bool hasUnreachableSource = false;
    };
    using ResultsMap = std::unordered_map<uint32_t, FindResult>;

    /// Returns the first reachable source applicable for the z3 model
    /// If source is not found FindResult::mySource will be nullptr
    FindResult FindReachable(z3::model& model, const z3::expr& expr, ResultsMap& processed, bool wasPropagation = false)
    {
        auto entry = processed.emplace(expr.id(), FindResult());
        FindResult& result = entry.first->second;
        if (!entry.second || expr.is_numeral()) {
            return result;
        }
        if (expr.is_ite()) {
            auto argPos = model.eval(expr.arg(0)).is_true() ? 1 : 2;
            result = FindReachable(model, expr.arg(argPos), processed, wasPropagation);
            return result;
        }
        result.hasNonConstSources = expr.is_const();

        auto source = Find(expr);
        if (source != nullptr) {
            bool isExternalSource = source->IsExternal();
            result.hasUnreachableSource = !isExternalSource;
            bool checkExpr = !wasPropagation && source->sourceKind != UntrustedSource::ITERATOR;
            if (model.eval(source->condition).is_false() || checkExpr && !model.eval(expr).is_numeral()) {
                return result;
            }
            if (source->expr.id() != source->sourceExpr.id()) {
                auto propagation = FindReachable(model, source->sourceExpr, processed, true);
                if (propagation.source == nullptr && propagation.hasNonConstSources) {
                    return result;  // LCOV_EXCL_LINE
                }
                // COODDY_SUPPRESS
                const_cast<UntrustedSourceExpr*>(source)->propagation = propagation.source;
            }
            if (!isExternalSource) {
                auto& sourceValue = const_cast<UntrustedSourceExpr*>(source)->sourceId;
                sourceValue = 0;
                model.eval(source->sourceExpr).is_numeral_u64(sourceValue);
            }
            result.source = source;
            return result;
        }
        for (uint32_t i = 0; i < expr.num_args(); ++i) {
            auto argSource = FindReachable(model, expr.arg(i), processed, wasPropagation);
            if (argSource.source != nullptr) {
                return argSource;
            }
            result.hasNonConstSources |= argSource.hasNonConstSources;
            result.hasUnreachableSource |= argSource.hasUnreachableSource;
        }
        return result;
    }

private:
    std::unordered_map<uint32_t, UntrustedSourceExpr> mySources;
    std::unordered_map<uint32_t, std::optional<const UntrustedSourceExpr*>> myCache;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_UNTRUSTEDSOURCES_H_
