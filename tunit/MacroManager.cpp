/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <macro/MacroManager.h>

#include <vector>

namespace HCXX {

// COODDY_SUPPRESS ParameterNumberChecker
void MacroManager::AddMacro(std::string name, SourceRange declRange, SourceRange nameRange, bool isSystem,
                            unsigned numParams)
{
    auto macroIt = myMacros.emplace(std::move(name), Macro()).first;
    Macro& macro = macroIt->second;
    macro.declSourceRange = declRange;
    macro.nameSourceRange = nameRange;
    macro.numParams = numParams;
    macro.isSystem = isSystem;
}

void MacroManager::AddMacroExpansion(const char* macroName, SourceRange range,
                                     std::vector<std::vector<std::string>>&& args)
{
    if (macroName != nullptr) {
        auto& macro = myMacros[macroName];
        macro.macroExpansions.emplace_back(range, std::move(args));
        myExpansionsMap[range.begin].emplace_back(macro.declSourceRange);
    }
}

const std::unordered_map<std::string, Macro>& MacroManager::GetMacros() const
{
    return myMacros;
}

std::vector<const MacroExpansion*> MacroManager::GetMacroExpansionsInRange(const char* macroName,
                                                                           const SourceRange& range) const
{
    std::vector<const MacroExpansion*> result;
    auto expansions = myMacros.find(macroName);
    if (expansions == myMacros.end()) {
        return result;
    }

    for (auto& it : expansions->second.macroExpansions) {
        if (it.GetRange().begin >= range.begin && it.GetRange().end <= range.end) {
            result.push_back(&it);
        }
    }
    return result;
}

bool MacroManager::IsMacroExpansionRange(SourceRange sourceRange) const
{
    if (!sourceRange.IsValid()) {
        return false;
    }
    auto it = std::lower_bound(myExpansions.begin(), myExpansions.end(), std::make_pair(sourceRange.begin, 0U));
    return it != myExpansions.end() && it->first == sourceRange.begin;
}

void MacroManager::Init()
{
    for(auto& [loc, decls] : myExpansionsMap) {
        myExpansions.emplace_back(loc, AddMacroDeclaration(decls));
    }
    std::sort(myExpansions.begin(), myExpansions.end());
    myExpansions.shrink_to_fit();
}

void MacroManager::Clear()
{
    myMacros = decltype(myMacros)();
    myExpansionsMap = decltype(myExpansionsMap)();
}

};  // namespace HCXX
