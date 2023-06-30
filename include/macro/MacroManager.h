/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_MACRO_MACROMANAGER_H_
#define COODDY_ANALYZER_INCLUDE_MACRO_MACROMANAGER_H_

#include <macro/Macro.h>

#include <unordered_map>

namespace HCXX {

class MacroManager {
public:
    using LocToDeclArray = std::vector<std::pair<Location, Location>>;

    // COODDY_SUPPRESS ParameterNumberChecker
    void AddMacro(std::string name, SourceRange declRange, SourceRange nameRange, bool isSystem, unsigned numParams);

    void AddMacroExpansion(const char* macroName, SourceRange range, std::vector<std::vector<std::string>>&& args);

    const std::unordered_map<std::string, Macro>& GetMacros() const;

    std::vector<const MacroExpansion*> GetMacroExpansionsInRange(const char* macroName, const SourceRange& range) const;

    bool IsMacroExpansionRange(SourceRange sourceRange) const;

    const LocToDeclArray& GetExpansions() const
    {
        return myExpansions;
    }

protected:
    ~MacroManager() = default;

    void Init();
    void Clear();

    virtual Location AddMacroDeclaration(const std::vector<SourceRange>& macroDecls) = 0;

private:
    std::unordered_map<std::string, Macro> myMacros;  // key is macro name
    std::unordered_map<Location, std::vector<SourceRange>> myExpansionsMap;
    LocToDeclArray myExpansions;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_MACRO_MACROMANAGER_H_
