/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_TOOL_CALLGRAPHCONSUMER_H_
#define COODDY_ANALYZER_TOOL_CALLGRAPHCONSUMER_H_

#include <ast/CallExpression.h>
#include <deps/md5.h>
#include <dfa/DfaFunctionContext.h>
#include <solver/FunctionBehavior.h>
#include <utils/EnvironmentUtils.h>
#include <utils/LocaleUtils.h>
#include <utils/json-cpp.h>

#include <fstream>
#include <unordered_map>

using namespace HCXX;

struct CallGraphNode {
    std::string fuid;
    std::string function;
    std::string tu;
    std::unordered_set<DfaFunctionContext*> callsContexts;
    std::set<std::string> called;
    std::set<std::string> calls;

    DECLARE_FIELDS("FUID", fuid, "function", function, "path", tu, "calls", calls, "called", called);
};

inline bool operator<(const CallGraphNode& a, const CallGraphNode& b)
{
    return a.fuid < b.fuid;
}

class CallGraph {
public:
    void Finalize()
    {
        if (myFinalized) {
            return;  // LCOV_EXCL_LINE: HCAT-2779
        }
        myFinalized = true;
        for (auto& it : functions) {
            auto& function = it.second;
            for (auto callContext : function.callsContexts) {
                auto pair = functions.find(callContext);
                if (pair != functions.end()) {
                    function.calls.insert(pair->second.fuid);
                    pair->second.called.insert(function.fuid);
                } else {
                    auto undefinedPair = undefinedFunctions.find(callContext);
                    if (undefinedPair != undefinedFunctions.end()) {
                        function.calls.insert(undefinedPair->second.fuid);
                        undefinedPair->second.called.insert(function.fuid);
                    }
                }
            }
        }
        for (auto& it : functions) {
            myNodes.push_back(it.second);
            myTU[it.second.tu].push_back(it.second);
        }
        for (auto p : undefinedFunctions) {
            myNodes.push_back(p.second);
            myTU[""].push_back(p.second);
        }
        std::sort(myNodes.begin(), myNodes.end());
        CallGraphNode* previousNode = nullptr;
        for (auto& node : myNodes) {
            if (previousNode != nullptr && previousNode->fuid == node.fuid) {
                // LCOV_EXCL_START
                Log(LogLevel::ERROR) << "Duplicated function unique ID " << node.fuid << " " << node.function
                                     << std::endl;
                // LCOV_EXCL_STOP
            }
            previousNode = &node;
        }
    }
    std::string ToJson()
    {
        Finalize();
        return jsoncpp::to_string(*this);
    }
    std::string ToDot()
    {
        Finalize();
        std::stringstream result;
        result << "digraph CallGraph {" << std::endl;
        int cluster = 0;
        for (auto tu : myTU) {
            if (!tu.first.empty()) {
                result << "\tsubgraph cluster" << std::to_string(cluster++) << " {" << std::endl;
                result << "\t\tlabel = \"" << tu.first << "\";" << std::endl;
                result << "\t\tcolor = blue;" << std::endl;
                for (auto node : tu.second) {
                    result << "\t\t_" << node.fuid << " [label=\"" << node.function << "\" shape=box];" << std::endl;
                }
                result << "\t}" << std::endl;
            }
        }
        for (auto node : myTU[""]) {
            result << "\t_" << node.fuid << " [label=\"" << node.function << "\" shape=box];" << std::endl;
        }
        for (auto node : myNodes) {
            for (auto call : node.calls) {
                result << "\t_" << node.fuid << " -> _" << call << ";" << std::endl;
            }
        }
        result << "}";
        return result.str();
    }

    std::unordered_map<DfaFunctionContext*, CallGraphNode> functions;
    std::unordered_map<DfaFunctionContext*, CallGraphNode> undefinedFunctions;

    DECLARE_FIELDS("call_graph", myNodes);

private:
    bool myFinalized = false;
    std::unordered_map<std::string, std::vector<CallGraphNode>> myTU;
    std::vector<CallGraphNode> myNodes;
};

class CallGraphConsumer : public Analyzer::Consumer {
public:
    explicit CallGraphConsumer(bool disableCheckers) : myDisableCheckers(disableCheckers) {}
    ~CallGraphConsumer() = default;

    void OutReport(const std::string& outFilePath)
    {
        std::filesystem::path resultPath = outFilePath;
        if (std::filesystem::is_directory(resultPath)) {
            resultPath.append("call_graph.json");
        }
        std::ofstream outStream(resultPath.string());
        OutReport(outStream, resultPath.extension() == ".gv" || resultPath.extension() == ".dot");
    }

    void OutReport(std::ostream& outStream, bool gv = false)
    {
        outStream << (gv ? myGraph.ToDot() : myGraph.ToJson());
    }
    void SetPath(std::filesystem::path& path)
    {
        if (path.empty()) {
            return;  // LCOV_EXCL_LINE: HCAT-2778
        }
        myPath = absolute(path);
        if (!is_directory(myPath)) {
            myPath = myPath.remove_filename();
        }
        myPath = std::filesystem::canonical(myPath);
    }

private:
    bool ShouldAnalyzeUnit(TranslationUnit& unit) override
    {
        return !myDisableCheckers;
    }

    bool ShouldAnalyzeFunction(DfaFunctionContext& functionContext) override
    {
        auto funcDecl = functionContext.GetFunction();
        if (funcDecl == nullptr) {
            return !myDisableCheckers;  // LCOV_EXCL_LINE
        }
        {
            std::lock_guard<std::mutex> lock(myMutex);
            myGraph.functions[&functionContext] = CallGraphNode{
                CreateFUID(funcDecl),
                funcDecl->GetQualifiedName(),
                GetFileLocation(funcDecl->GetTranslationUnit(), funcDecl->GetRange()),
            };
        }
        for (auto calleeContext : functionContext.GetCalleesContexts()) {
            std::lock_guard<std::mutex> lock(myMutex);
            myGraph.functions[&functionContext].callsContexts.emplace(calleeContext);
            if (calleeContext->IsUndefined()) {
                MD5_CTX md5ctx;
                MD5_Init(&md5ctx);
                std::string signature = calleeContext->GetSignature();
                MD5_Update(&md5ctx, signature.c_str(), signature.size());
                UniqueId result;
                MD5_Final(result.Data(), &md5ctx);
                myGraph.undefinedFunctions.emplace(calleeContext,
                                                   CallGraphNode{result.ToString(), calleeContext->GetFQN()});
            }
        }
        return !myDisableCheckers;
    }

    std::string CreateFUID(const FunctionDecl* function)
    {
        auto definition = function->GetDefinition();
        if (definition != nullptr) {
            function = definition;  // LCOV_EXCL_LINE
        }
        MD5_CTX md5ctx;
        MD5_Init(&md5ctx);
        std::string file = GetFileLocation(function->GetTranslationUnit(), function->GetRange());
        MD5_Update(&md5ctx, file.c_str(), file.size());
        std::string name = function->GetMangledName();
        MD5_Update(&md5ctx, name.c_str(), name.size());
        UniqueId result;
        MD5_Final(result.Data(), &md5ctx);
        return result.ToString();
    }

    std::string GetFileLocation(const TranslationUnit* tu, SourceRange sourceRange)
    {
        if (tu != nullptr && sourceRange.IsValid()) {
            LocationInfo locInfo = tu->GetLocationInfo(sourceRange);
            if (!locInfo.filename.empty()) {
                std::filesystem::path declFilePath(locInfo.filename);
                declFilePath = std::filesystem::canonical(declFilePath);
                std::string result = declFilePath.lexically_relative(myPath).string();
                return EnvironmentUtils::NormalizePath(result);
            }
        }
        return "";  // LCOV_EXCL_LINE
    }
    std::filesystem::path myPath;
    bool myDisableCheckers;
    CallGraph myGraph;
    std::mutex myMutex;
};

#endif  // COODDY_ANALYZER_TOOL_CALLGRAPHCONSUMER_H_
