/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_CALLSTACK_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_CALLSTACK_H_

#include "SymbolId.h"

class CallStack {
public:
    struct ArgInfo : public ::ArgInfo {
        SymbolId symbolId;
        z3::expr expr;
    };
    using ArgsArray = std::vector<ArgInfo>;

    void AddCallArg(ArgInfo&& argInfo)
    {
        myCallStack.back().args.emplace_back(argInfo);
    }
    void EnterCall(const z3::expr& callExpr, Instruction callInstr, ArgType resultType)
    {
        myCallStack.emplace_back(CallNode{ArgsArray(), callExpr, callExpr, resultType, callInstr});
    }
    const ArgsArray& GetCurCallArgs() const
    {
        return myCallStack.back().args;
    }
    const ArgsArray& GetCallArgs(uint32_t stackPos) const
    {
        return myCallStack[stackPos].args;
    }
    ArgType GetCallResultType() const
    {
        return myCallStack.back().resultType;
    }

    const z3::expr& GetCallResult() const
    {
        return myCallStack.back().result;
    }

    void SetCallResult(const z3::expr& callResult, bool isFinalResult = false)
    {
        if (!myCallStack.back().isFinalResult) {
            myCallStack.back().result = callResult;
            myCallStack.back().isFinalResult = isFinalResult;
        }
    }
    void SetCallArg(uint32_t argPos, const z3::expr& expr)
    {
        myCallStack.back().args[argPos].expr = expr;
    }
    void SetCallExpr(const z3::expr& callExpr, bool isVirtual)
    {
        myCallStack.back().callExpr = callExpr;
        myCallStack.back().isVirtual = isVirtual;
    }
    const z3::expr& GetCallExpr() const
    {
        return myCallStack.back().callExpr;
    }
    std::pair<bool, bool> IsIndirectCall() const
    {
        return {myCallStack.back().callExpr.id() != myCallStack.back().result.id(), myCallStack.back().isVirtual};
    }
    z3::expr LeaveCall()
    {
        auto result = myCallStack.back().result;
        myCallStack.pop_back();
        return result;
    }
    size_t GetCallStackSize() const
    {
        return myCallStack.size();
    }
    Instruction GetCurCallInstruction() const
    {
        return !myCallStack.empty() ? myCallStack.back().instruction : 0;
    }
    void ClearCallStack(uint32_t stackPos)
    {
        myCallStack.erase(myCallStack.begin() + stackPos, myCallStack.end());
    }

private:
    struct CallNode {
        ArgsArray args;
        z3::expr callExpr;
        z3::expr result;
        ArgType resultType;
        Instruction instruction;
        bool isVirtual = false;
        bool isFinalResult = false;
    };
    std::vector<CallNode> myCallStack;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_CALLSTACK_H_
