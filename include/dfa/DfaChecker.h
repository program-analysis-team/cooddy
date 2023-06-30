/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// class DfaChecker declares interface for dfa state propagation and problems reporting.
// Instance of this class is created by Analyzer and is used by DataFlowAnalyzer.
// DataFlowAnalyzer traverses CFG from the end to the beginning of the function and
// passes corresponding states to the interface methods.
//
#ifndef COODDY_ANALYZER_INCLUDE_DFA_PROPAGATION_H_
#define COODDY_ANALYZER_INCLUDE_DFA_PROPAGATION_H_

#include <Checker.h>
#include <dfa/DfaState.h>
namespace HCXX {
class DfaFunctionContext;
struct AccessInfo;

class DfaChecker : public Checker {
public:
    // return DFA_CHECKER to distinguish with AST_CHECKER
    Kind GetKind() override
    {
        return Kind::DFA_CHECKER;
    }

    bool CheckUnit(const TranslationUnit& unit, ProblemsHolder& holder) override
    {
        return true;
    }
    // LCOV_EXCL_START
    void CheckNode(const Node& node, ProblemsHolder& holder) final {}
    // LCOV_EXCL_STOP

    // This method is called at the beginning of function analysis
    // Allows to add some data into the memory
    virtual void InitFunction(DfaFunctionState& state){};

    // Allows to check annotations in the function state and report problems
    virtual void CheckFunction(DfaFunctionState& state, ProblemsHolder& holder){};

    // Allows to check declaration annotations and report problems
    virtual void CheckDeclaration(DfaState& state, ProblemsHolder& holder){};

    // Allows to check global variable annotations and report problems
    virtual void CheckGlobalVar(DfaState& state, ProblemsHolder& holder){};

    // Allows to annotated undefined function
    virtual void CheckUndefFunction(const DfaFunctionContext& funcCtx){};

    // Allows to check terminator of a cfg block and report problems
    virtual void CheckTerminator(const Node& terminator, DfaFunctionState& state, ProblemsHolder& holder){};

    // Allows to add some annotations to the state before all checkers start check it
    virtual void InitState(DfaState& state){};

    // Allows to check annotations in the state and report problems
    virtual void CheckState(DfaState& state, ProblemsHolder& holder){};

    // Allows to propagate some annotations from the state to the parent or other nodes
    virtual void ExitState(DfaState& state){};

    struct SourceExecInfo {
        ExprId exprId;
        const Annotation& annotation;
        SolverContext& context;
        const SuspiciousPath& path;
    };

    // Allows to add symbolic conditions for the source point
    // If return value is false, source is ignored
    virtual bool OnSourceExecuted(const SourceExecInfo& sourceInfo)
    {
        return true;
    };

    struct SinkExecInfo : public SourceExecInfo {
        std::vector<ExprId>& sources;
    };

    // Allows to add symbolic conditions for the sink point
    // If return value is false, sink is ignored
    virtual bool OnSinkExecuted(const SinkExecInfo& sinkInfo)
    {
        return true;
    };

    // If result is true, then source or sink annotation
    // will be added to paths checking in solver
    virtual bool ShouldProcessAnnotation(const Annotation& annotation, bool defResult, bool sourceFound)
    {
        return defResult;
    };

    struct ProblemInfo : public Problem {
        ProblemInfo(Problem&& problem, const AccessInfo* accessInfo) : Problem(problem), accessInfo(accessInfo){};

        const AccessInfo* accessInfo = nullptr;
    };

    virtual bool OnReportProblem(ProblemInfo& problemInfo)
    {
        return true;
    };
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_DFA_PROPAGATION_H_
