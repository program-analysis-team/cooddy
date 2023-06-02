/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Checker.h>
#include <ast/CxxMethodDecl.h>
#include <ast/FunctionDecl.h>
#include <ast/RefExpression.h>

#include <unordered_set>
#include <vector>

using namespace HCXX;

class UnusedParamChecker : public Checker {
public:
    void CheckNode(const Node& node, ProblemsHolder& holder) override
    {
        auto functionDecl = Node::Cast<FunctionDecl>(&node);
        if (functionDecl == nullptr) {
            return;
        }

        if (functionDecl->IsTemplated() || functionDecl->IsVariadic()) {
            return;
        }
        if (functionDecl->InMacro() || functionDecl->GetBody() == nullptr) {
            return;
        }

        CheckArgs(functionDecl, holder);
    }

private:
    void CheckArgs(const FunctionDecl* functionDecl, ProblemsHolder& holder)
    {
        if (auto method = Node::Cast<CxxMethodDecl>(functionDecl);
            method != nullptr && (method->IsOverride() || method->IsVirtual())) {
            return;
        }

        for (auto& unusedArg : GetUnusedArg(functionDecl)) {
            holder.RegisterProblem(*this, *unusedArg, {{unusedArg->GetName()}});
        }
    }

    std::vector<const Node*> GetUnusedArg(const FunctionDecl* func)
    {
        std::vector<const Node*> unusedArgs;
        std::unordered_set<const Node*> usedDecl;
        TraverseCallback cb = [&](const Node& node) {
            auto ref = Node::Cast<RefExpression>(&node);
            if (ref != nullptr && ref->GetDeclaration() != nullptr) {
                usedDecl.insert(ref->GetDeclaration());
            }
        };
        if (func->GetBody()->GetChildren().empty() && func->GetFuncKind() != FunctionDecl::FunctionKind::CONSTRUCTOR) {
            return unusedArgs;
        }
        func->Traverse(cb);
        for (auto& arg : func->GetParams()) {
            if (usedDecl.find(arg) == usedDecl.end()) {
                unusedArgs.push_back(arg);
            }
        }
        return unusedArgs;
    }
};

std::unique_ptr<HCXX::Checker> CreateUnusedParamChecker()
{
    return std::make_unique<UnusedParamChecker>();
}
