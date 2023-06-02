/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_TOOL_ENTRYFUNCCONSUMER_H_
#define COODDY_ANALYZER_TOOL_ENTRYFUNCCONSUMER_H_

#include <Analyzer.h>
#include <CrossTUContext.h>
#include <ast/CallExpression.h>
#include <dfa/DfaFunctionContext.h>
#include <dfa/SuspiciousPath.h>
#include <solver/FunctionBehavior.h>
#include <utils/EnvironmentUtils.h>
#include <utils/LocaleUtils.h>

#include <unordered_map>

using namespace HCXX;

class EntryFuncConsumer : public Analyzer::Consumer {
public:
    bool SetEntryFunc(const std::string& entryFunc, const std::string& callGraphPath, int checkPathLine = 0)
    {
        myIsExportMode = false;
        myEntryFunction = entryFunc;
        myCheckPathLine = checkPathLine;
        std::ifstream callGraphStream(callGraphPath);
        if (!callGraphStream.good()) {
            Log(LogLevel::ERROR) << "Can't open call graph file " << callGraphPath << std::endl;
            return false;
        }
        uint32_t entryPos = UINT32_MAX;
        uint32_t curFunc = 0;
        for (std::string line; std::getline(callGraphStream, line); ++curFunc) {
            auto data = StrUtils::Split(line, ',');
            FuncInfo funcInfo{std::string(data[0]), std::string(data[1])};
            for (auto i = 2; i < data.size(); ++i) {
                funcInfo.callees.insert(std::strtol(data[i].data(), nullptr, 10));
            }
            if (entryFunc.empty()) {
                myUnits[funcInfo.unitName].emplace_back(myFuncInfo.size());
            } else if (entryFunc == funcInfo.mangledName) {
                entryPos = curFunc;
            }
            myFuncInfo.emplace_back(std::move(funcInfo));
        }
        if (entryFunc.empty()) {
            return true;
        }
        if (entryPos == UINT32_MAX) {
            Log(LogLevel::ERROR) << "Can't find entry function in call graph " << entryFunc << std::endl;
            myFuncInfo.clear();
            return false;
        }
        std::unordered_set<uint32_t> processed;
        InitUnits(entryPos, processed);
        return true;
    }

    bool ShouldParseUnit(TranslationUnit& unit, Parser::ParseStages parseStage) override
    {
        if (myIsExportMode) {
            return parseStage == Parser::ParseStages::AST;
        }
        auto it = myUnits.find(unit.GetMainFileName());
        if (it == myUnits.end()) {
            return false;
        }
        if (parseStage != Parser::ParseStages::DEFINITIONS) {
            return true;
        }
        for (auto& funcPos : it->second) {
            unit.GetCrossTUContext()->AddFunctionDef(std::string(myFuncInfo[funcPos].mangledName), &unit);
        }
        return false;
    }

    bool ShouldAnalyzeUnit(TranslationUnit& unit) override
    {
        if (!myIsExportMode) {
            return true;
        }
        std::unique_lock<std::mutex> lock(myMutex);
        for (auto& node : unit.GetNodes()) {
            auto funcDecl = Node::Cast<FunctionDecl>(node);
            if (funcDecl == nullptr || funcDecl->GetBody() == nullptr || funcDecl->GetMangledName().empty()) {
                continue;
            }
            std::unordered_set<uint32_t> callees;
            for (auto& callee : funcDecl->GetChildren<CallExpression>(true)) {
                auto calleeFunc = callee->GetFunction();
                if (calleeFunc != nullptr && !calleeFunc->IsSystem()) {
                    callees.insert(GetFuncId(*calleeFunc));
                }
            }
            myFuncIdToPos.emplace(GetFuncId(*funcDecl), myFuncInfo.size());
            myFuncInfo.emplace_back(FuncInfo{funcDecl->GetMangledName(), unit.GetMainFileName(), std::move(callees)});
        }
        return false;
    }

    bool ShouldAnalyzeFunction(DfaFunctionContext& functionContext) override
    {
        if (myIsExportMode) {
            return false;
        }
        return myEntryFunction.empty() || myFunctions.find(functionContext.GetSignature()) != myFunctions.end();
    }

    void StoreCallGraph(const std::string& callGraphPath)
    {
        std::ofstream callGraphStream(callGraphPath);
        if (!callGraphStream.good()) {
            // LCOV_EXCL_START
            Log(LogLevel::ERROR) << "Can't open call graph file " << callGraphPath << std::endl;
            return;
            // LCOV_EXCL_STOP
        }
        for (auto func : myFuncInfo) {
            callGraphStream << func.mangledName << ',' << func.unitName;
            for (auto callee : func.callees) {
                if (auto it = myFuncIdToPos.find(callee); it != myFuncIdToPos.end()) {
                    callGraphStream << ',' << it->second;
                }
            }
            callGraphStream << std::endl;
        }
    }

    bool ShouldCheckPath(DfaFunctionContext& funcCtx, const SuspiciousPath& path) override
    {
        if (myCheckPathLine == 0) {
            return true;
        }
        if (funcCtx.GetSignature() != myEntryFunction) {
            return false;  // LCOV_EXCL_LINE: HCAT-2779
        }
        auto jointRange = funcCtx.GetBehavior()->GetSourceRange(path.jointInstruction);
        return funcCtx.GetTranslationUnit().GetLocationInfo(jointRange).line == myCheckPathLine;
    }

private:
    struct FuncInfo {
        std::string mangledName;
        std::string unitName;
        std::unordered_set<uint32_t> callees;
    };

    size_t GetFuncId(const FunctionDecl& func)
    {
        return myFuncNameToId.emplace(func.GetMangledName(), myFuncNameToId.size()).first->second;
    }

    void InitUnits(uint32_t funcId, std::unordered_set<uint32_t>& processed)
    {
        if (!processed.insert(funcId).second) {
            return;  // LCOV_EXCL_LINE: HCAT-2779
        }
        auto& funcInfo = myFuncInfo[funcId];
        myUnits[funcInfo.unitName].emplace_back(funcId);
        myFunctions.insert(funcInfo.mangledName);
        for (auto& callee : funcInfo.callees) {
            InitUnits(callee, processed);
        }
    }

    std::mutex myMutex;
    std::string myEntryFunction;
    std::vector<FuncInfo> myFuncInfo;
    std::unordered_map<std::string, size_t> myFuncNameToId;
    std::unordered_map<size_t, size_t> myFuncIdToPos;
    std::unordered_map<std::string, std::vector<size_t>> myUnits;
    std::unordered_set<std::string> myFunctions;
    bool myIsExportMode = true;
    int myCheckPathLine = 0;
};

#endif  // COODDY_ANALYZER_TOOL_ENTRYFUNCCONSUMER_H_
