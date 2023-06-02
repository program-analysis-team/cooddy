/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_TRAVERSEEXPRESSION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_TRAVERSEEXPRESSION_H_

using TraverseSinkCallback = const std::function<bool(const z3::expr&)>&;

inline bool TraverseSinkChildren(const z3::expr& sinkExpr, TraverseSinkCallback callback)
{
    for (uint32_t i = sinkExpr.is_ite() ? 1 : 0; i < sinkExpr.num_args(); ++i) {
        if (!callback(sinkExpr.arg(i))) {
            return false;
        }
    }
    return true;
}

inline bool TraverseSinkExpression(const z3::expr& sinkExpr, TraverseSinkCallback callback)
{
    std::unordered_set<int32_t> processed;
    std::function<bool(const z3::expr&)> traverse = [&](const z3::expr& it) {
        if (!processed.insert(it.id()).second) {
            return true;
        }
        return callback(it) ? TraverseSinkChildren(it, traverse) : false;
    };
    return traverse(sinkExpr);
}

inline z3::expr operator||(z3::expr& a, z3::expr const& b)
{
    return (a.is_true() || b.is_false()) ? a : (a.is_false() || b.is_true()) ? b : z3::operator||(a, b);
}

inline z3::expr operator&&(z3::expr& a, z3::expr const& b)
{
    return (a.is_false() || b.is_true()) ? a : (a.is_true() || b.is_false()) ? b : z3::operator&&(a, b);
}

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_TRAVERSEEXPRESSION_H_
