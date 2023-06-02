/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_CROSSTUCONTEXT_H_
#define COODDY_ANALYZER_INCLUDE_CROSSTUCONTEXT_H_

#include <TranslationUnit.h>

namespace HCXX {

class CrossTUContext {
public:
    struct VarInfo {
        TranslationUnit* unit = nullptr;
        uint32_t uniqueId = 0;
        uint32_t sizeOfType = 0;
        int64_t initValue = 0;

        void Serialize(IOStream& stream)
        {
            stream << unit << uniqueId << sizeOfType << initValue;
        }
    };

    uint32_t GetGlobalVarsCount()
    {
        return myGlobalVars.size();
    }
    void AddGlobalVar(std::string&& varName, VarInfo&& var)
    {
        std::unique_lock<std::mutex> lock(myMutex);
        auto& varInfos = myGlobalVars[varName];
        if (!varInfos.empty() && varInfos.back().unit == nullptr) {
            varInfos.clear();
        }
        if (varInfos.empty() || var.unit != nullptr) {
            varInfos.emplace_back(var);
        }
    }

    const std::vector<VarInfo>* FindGlobalVar(const std::string& varName) const
    {
        auto it = myGlobalVars.find(varName);
        return it != myGlobalVars.end() ? &it->second : nullptr;
    }

    struct FuncInfo {
        TranslationUnit* unit = nullptr;
        bool hasExternalUsage = false;

        void Serialize(IOStream& stream)
        {
            stream << unit << hasExternalUsage;
        }
    };

    void AddFunctionDef(std::string&& funcName, TranslationUnit* unit)
    {
        std::unique_lock<std::mutex> lock(myMutex);
        myFunctions[funcName].emplace_back(FuncInfo{unit});
    }

    void AddFunctionUsage(const std::string& funcName, TranslationUnit* unit)
    {
        auto it = myFunctions.find(funcName);
        if (it != myFunctions.end() && it->second.front().unit != unit) {
            it->second.front().hasExternalUsage = true;
        }
    }

    const std::vector<FuncInfo>* FindFunctionDef(const std::string& funcName) const
    {
        auto it = myFunctions.find(funcName);
        return it != myFunctions.end() ? &it->second : nullptr;
    }

    void InitUniqueIds()
    {
        vector<std::pair<const std::string, std::vector<VarInfo>>*> keys;
        keys.reserve(myGlobalVars.size());
        for (auto& v : myGlobalVars) {
            keys.push_back(&v);
        }
        std::sort(keys.begin(), keys.end(),
                  [](const std::pair<const std::string, std::vector<VarInfo>>* a,
                     const std::pair<const std::string, std::vector<VarInfo>>* b) { return a->first < b->first; });
        for (int i = 0; i < keys.size(); i++) {
            for (auto& info : keys[i]->second) {
                info.uniqueId = i + 1;
            }
        }
    }

    void Serialize(IOStream& stream)
    {
        if (stream.IsOut() || !stream.IsEOF()) {
            stream << myFunctions;
        }
        if (stream.IsOut() || !stream.IsEOF()) {
            stream << myGlobalVars;
        }
    }

private:
    std::mutex myMutex;
    std::unordered_map<std::string, std::vector<FuncInfo>> myFunctions;
    std::unordered_map<std::string, std::vector<VarInfo>> myGlobalVars;
};

}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_CROSSTUCONTEXT_H_
