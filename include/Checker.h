/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Declaration of base class for checkers.
//

#ifndef HCXXANALYZER_CHECKER_H
#define HCXXANALYZER_CHECKER_H

#include <ProblemsHolder.h>
#include <ast/Node.h>
#include <utils/LocaleUtils.h>
#include <utils/Timer.h>

#include <istream>
#include <unordered_map>
#include <utility>

namespace HCXX {

struct CheckerStat {
    std::atomic<uint64_t> analysisTime = 0;
    std::atomic<uint64_t> checkPathTime = 0;
    std::atomic<uint32_t> problemsFound = 0;
    std::atomic<uint32_t> timeoutsCount = 0;
    std::atomic<uint32_t> checkedPaths = 0;
};

class Checker : public CheckerStat {
public:
    explicit Checker() = default;
    virtual ~Checker() = default;

    using Config = std::unique_ptr<std::istream>;
    enum class Kind { AST_CHECKER, DFA_CHECKER, GLOBAL_CHECKER };

    void SetLangNameAndIndex(const std::unordered_set<UnitLanguage>& langs, std::string name, uint32_t index)
    {
        myName = std::move(name);
        myIndex = index;
        myLangs = langs;
    }

    virtual std::unordered_set<UnitLanguage> GetLangs()
    {
        return myLangs;
    }

    bool CanProcessLang(UnitLanguage lang)
    {
        return myLangs.find(lang) != myLangs.end();
    }

    const std::string& GetName() const
    {
        return myName;
    }

    template <class TContext>
    TContext& GetContext(const TranslationUnit& unit) const
    {
        return unit.GetContext<TContext>(myIndex);
    }

    virtual Kind GetKind()
    {
        return Kind::AST_CHECKER;
    };

    virtual bool Initialize(const Config& config)
    {
        return true;
    };

    // This method is called before unit analysis
    // Return true if checker has analyzed the unit and doesn't need CheckNode calls
    virtual bool CheckUnit(const TranslationUnit& unit, ProblemsHolder& holder)
    {
        return false;
    }

    // LCOV_EXCL_START
    virtual void CheckNode(const Node& node, ProblemsHolder& holder) {}
    // LCOV_EXCL_STOP

    // This method is called all units have been analyzed
    virtual void PostCheck(std::vector<TranslationUnitPtr>& units, ProblemsHolder& holder) {}

private:
    std::string myName;
    uint32_t myIndex = 0;
    std::unordered_set<UnitLanguage> myLangs;
};

#define CALL_CHECKERS(checkers, call, optype)   \
    {                                           \
        Timer timer;                            \
        for (auto& checker : checkers) {        \
            call;                               \
            timer.Flush(checker->optype##Time); \
        }                                       \
    }
}  // namespace HCXX

#endif  // HCXXANALYZER_CHECKER_H
