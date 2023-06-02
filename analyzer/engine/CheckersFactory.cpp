/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Checker.h>
#include <CheckersFactory.h>
#include <utils/SharedObjectUtils.h>
#include <workspace/Profile.h>

#include <algorithm>
#include <filesystem>
#include <unordered_map>

#include "utils/EnvironmentUtils.h"

#define DECLARE_BUILDIN_CHECKER(name)                     \
    extern std::unique_ptr<HCXX::Checker> Create##name(); \
    factory.Register("#" #name, Create##name);

/**
 * @brief Add utility (built-in) checkers to the list of checkers made by this factory.
 *
 * When calling CheckersFactory::Create(), the resulting vector will have the checkers in the same order they are
 * registered here. This in turn means that during analysis, checkers will be executed in the same order.
 */
void RegisterBuildInCheckers(HCXX::CheckersFactory& factory)
{
    DECLARE_BUILDIN_CHECKER(AnnotationChecker)
    DECLARE_BUILDIN_CHECKER(VarDeclChecker)
    DECLARE_BUILDIN_CHECKER(ReadWriteChecker)
    DECLARE_BUILDIN_CHECKER(CallArgsAnnotation)
    DECLARE_BUILDIN_CHECKER(AllocSourceChecker)  // must be registered after ReadWriteChecker and CallArgsAnnotation
    DECLARE_BUILDIN_CHECKER(FuncDeclAnnotation)
    DECLARE_BUILDIN_CHECKER(CtorInitializerChecker)
    DECLARE_BUILDIN_CHECKER(DereferenceChecker)
    DECLARE_BUILDIN_CHECKER(UntrustedSourceChecker)

    DECLARE_CHECKER(GenericTaintChecker)
    DECLARE_CHECKER(HastBasedChecker)
}

// LCOV_EXCL_START
#ifdef _MSC_VER
#undef DECLARE_CHECKER_BEGIN
#define DECLARE_CHECKER_BEGIN(scope)

#undef DECLARE_CHECKER_END
#define DECLARE_CHECKER_END()

void RegisterCheckers(HCXX::CheckersFactory& factory)
{
#include "../source/checkers/codestyle/Checkers.cpp"
#include "../source/checkers/cwe/Checkers.cpp"
#include "../source/checkers/performance/Checkers.cpp"
#include "../source/checkers/unused_vars/Checkers.cpp"
}
#else
void RegisterCheckers(HCXX::CheckersFactory& factory) {}
#endif
// LCOV_EXCL_STOP

namespace HCXX {

CheckersFactory& CheckersFactory::GetInstance()
{
    static CheckersFactory instance;
    return instance;
}

void CheckersFactory::AddModule(const std::string& module)
{
    std::unique_lock<std::mutex> lock(myMutex);
    if (!myModules.insert(module).second) {
        return;
    }
    auto modulePath = EnvironmentUtils::GetSelfExecutableDir() / module;
    auto dllHandle = SharedObjectUtils::LoadSharedObject(modulePath.string());
    if (dllHandle == nullptr) {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "Can't load checkers module " << modulePath << "\n";
        return;
        // LCOV_EXCL_STOP
    }
    using TRegFunc = void (*)(HCXX::CheckersFactory&);
    auto regFunc = SharedObjectUtils::GetSymbol<TRegFunc>(dllHandle, "Register");
    if (regFunc == nullptr) {
        // LCOV_EXCL_START
        Log(LogLevel::ERROR) << "Can't find 'Register' function in " << modulePath << "\n";
        return;
        // LCOV_EXCL_STOP
    }
    regFunc(*this);
    Log(LogLevel::INFO) << "Loaded checkers module " << modulePath << "\n";
}

const std::unordered_set<UnitLanguage> LangsFromStrings(std::unordered_set<std::string>& langs)
{
    std::unordered_set<UnitLanguage> result;
    for (auto lang : langs) {
        if (lang == "c") {
            result.emplace(UnitLanguage::C);
        } else if (lang == "c++") {
            result.emplace(UnitLanguage::CPP);
        } else if (lang == "java") {
            result.emplace(UnitLanguage::JAVA);
        } else {
            throw std::invalid_argument("Unknown language in profile: " + lang);  // LCOV_EXCL_LINE
        }
    }
    return result;
}
void CheckersFactory::Create(const HCXX::Profile& profile, std::vector<std::unique_ptr<HCXX::Checker>>& result)
{
    uint32_t checkerIndex = 0;
    for (auto& factory : myCheckers) {
        if (factory.first.front() == '#' || profile.IsCheckerEnabled(factory.first)) {
            result.emplace_back(factory.second());
            auto langs = profile.GetCheckerCfg(factory.first).languages;

            result.back()->SetLangNameAndIndex(LangsFromStrings(langs), factory.first, checkerIndex++);
        }
    }
}

void CheckersFactory::Register(const char* checkerName, Factory factory)
{
    if (checkerName != nullptr && *checkerName != 0) {
        myCheckers.emplace_back(std::make_pair(checkerName, factory));
    }
}

void HCXX::CheckersFactory::Init()
{
    RegisterBuildInCheckers(*this);
    RegisterCheckers(*this);
}

}  // namespace HCXX
