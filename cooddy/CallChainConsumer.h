/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_TOOL_CALLCHAINCONSUMER_H_
#define COODDY_ANALYZER_TOOL_CALLCHAINCONSUMER_H_

#include <ast/CallExpression.h>
#include <dfa/DfaFunctionContext.h>
#include <solver/FunctionBehavior.h>
#include <utils/LocaleUtils.h>
#include <utils/json-cpp.h>

#include <fstream>
#include <unordered_map>
using namespace HCXX;

struct CallChainNde {
    std::map<std::string, CallChainNde> calleeMap;

    template <class X>
    // TODO HCAT-513
    // COODDY_SUPPRESS CamelCaseChecker
    void serialize(jsoncpp::Generator<X>& p)
    {
        jsoncpp::serialize(p, calleeMap);
    }
};

class CallChainConsumer : public Analyzer::Consumer {
public:
    CallChainConsumer(const std::string& entryPoint, int recursionDepth)
        : myEntryPoint(entryPoint), myRecursionDepth(recursionDepth)
    {}
    ~CallChainConsumer() = default;

    void OutReport(const std::string& outFilePath)
    {
        std::filesystem::path resultPath = outFilePath;
        if (std::filesystem::is_directory(resultPath)) {
            resultPath.append("hca_result.json");
        }
        std::ofstream outStream(resultPath.string());
        OutReport(outStream);
    }

    void OutReport(std::ostream& outStream)
    {
        if (!myIsEntryPointFound) {
            // LCOV_EXCL_START
            Log(LogLevel::ERROR) << "Entry point |" << myEntryPoint << "| not found" << std::endl;
            return;
            // LCOV_EXCL_STOP
        }
        outStream << jsoncpp::to_string(myCallGraph);
    }

private:
    bool ShouldAnalyzeUnit(TranslationUnit& unit) override
    {
        return false;
    };
    bool ShouldAnalyzeFunction(DfaFunctionContext& functionContext) override
    {
        if (myIsEntryPointFound) {
            return false;  // LCOV_EXCL_LINE: HCAT-2779
        }
        auto funcDecl = functionContext.GetFunction();
        auto fullName = funcDecl->GetQualifiedName();
        {
            std::lock_guard<std::mutex> lock(myMutex);
            myFuncNames[&functionContext] = fullName;
        }
        if (fullName == myEntryPoint) {
            myIsEntryPointFound = true;
            auto context = functionContext.GetBehavior()->StartExecution();
            ExecuteFunction(functionContext, *context, myCallGraph);
        }
        return false;
    }

    bool EnterCall(DfaFunctionContext& funcCtx)
    {
        if (myCurStack.size() == myRecursionDepth) {
            return false;
        }
        for (auto& it : myCurStack) {
            if (it == &funcCtx) {
                return false;  // LCOV_EXCL_LINE
            }
        }
        myCurStack.push_back(&funcCtx);
        return true;
    }

    void LeaveCall()
    {
        myCurStack.pop_back();
    }

    void ExecuteFunction(DfaFunctionContext& funcCtx, SolverContext& context, CallChainNde& curNode)
    {
        if (!EnterCall(funcCtx)) {
            return;
        }
        funcCtx.GetBehavior()->Execute(context, [&]() {
            if (!context.IsCallExpr()) {
                return ExecutionResult::OK;
            }
            auto calleeContext = funcCtx.GetCalleeContext(context.GetCurInstruction());
            if (calleeContext == nullptr || !context.IsReachable()) {
                return ExecutionResult::OK;
            }
            auto calleeName = myFuncNames[calleeContext];
            if (calleeContext->GetBehavior() == nullptr) {
                if (calleeName.empty() && calleeContext->GetFunction() != nullptr) {
                    calleeName = calleeContext->GetFunction()->GetQualifiedName();
                }
                curNode.calleeMap.emplace(calleeName, CallChainNde());
            } else {
                ExecuteFunction(*calleeContext, context,
                                curNode.calleeMap.emplace(calleeName, CallChainNde()).first->second);
            }
            return ExecutionResult::OK;
        });
        LeaveCall();
    }

    CallChainNde myCallGraph;
    std::unordered_map<const DfaFunctionContext*, std::string> myFuncNames;
    std::vector<DfaFunctionContext*> myCurStack;
    std::string myEntryPoint;
    int myRecursionDepth;
    bool myIsEntryPointFound = false;
    std::mutex myMutex;
};

#endif  // COODDY_ANALYZER_TOOL_CALLCHAINCONSUMER_H_
