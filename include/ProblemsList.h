/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// ProblemsList class implements ProblemsHolder interface and stores found problems in Problems list
//
#ifndef COODDY_ANALYZER_INCLUDE_PROBLEMSLIST_H_
#define COODDY_ANALYZER_INCLUDE_PROBLEMSLIST_H_

#include <ProblemsHolder.h>

#include <ostream>

namespace HCXX {

class ProblemsList : public ProblemsHolder {
public:
    explicit ProblemsList(const Workspace& workspace) : ProblemsHolder(workspace) {}
    ~ProblemsList() = default;

    const std::vector<Problem>& GetProblems() const
    {
        return myProblems;
    };

    /**
     * @brief Check if the list contains a particular problem.
     *
     * Example usage:
     * ASSERT_TRUE(problems.Contains([](auto& problem) { return problem.line == 10 && problem.column == 5; }));
     */
    template <class F>
    bool Contains(F unaryPredicate) const
    {
        return std::find_if(myProblems.begin(), myProblems.end(), unaryPredicate) != myProblems.end();
    }

    /// Debug print operator
    friend inline std::ostream& operator<<(std::ostream& os, const ProblemsList& obj)
    {
        os << "[\n";
        for (auto& problem : obj.GetProblems()) {
            os << "\t" << problem << ",\n";
        }
        os << "]";

        return os;
    }

protected:
    void RegisterProblemImpl(const Problem& problem) override
    {
        myProblems.emplace_back(problem);
    }

private:
    std::vector<Problem> myProblems;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_PROBLEMSLIST_H_
