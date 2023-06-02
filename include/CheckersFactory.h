/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Declaration of base class for checkers.
//

#ifndef HCXXANALYZER_CHECKER_FACTORY_H
#define HCXXANALYZER_CHECKER_FACTORY_H

#include <Checker.h>

#include <mutex>

namespace HCXX {
class Profile;

class CheckersFactory {
public:
    using Factory = std::unique_ptr<Checker> (*)();

    void Init();

    virtual void AddModule(const std::string& module);

    virtual void Create(const Profile& profile, std::vector<std::unique_ptr<HCXX::Checker>>& result);

    virtual void Register(const char* checkerName, Factory factory);

    static CheckersFactory& GetInstance();

private:
    std::mutex myMutex;
    std::unordered_set<std::string> myModules;
    std::vector<std::pair<std::string, Factory>> myCheckers;
};

#define DECLARE_CHECKER_BEGIN(scope)                          \
    extern "C" void Register(HCXX::CheckersFactory& factory); \
    void Register(HCXX::CheckersFactory& factory)             \
    {
#define DECLARE_CHECKER(name)                             \
    extern std::unique_ptr<HCXX::Checker> Create##name(); \
    factory.Register(#name, Create##name);

#define DECLARE_CHECKER_END() }

}  // namespace HCXX

#endif  // HCXXANALYZER_CHECKER_FACTORY_H
