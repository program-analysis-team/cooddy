/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Checker.h>
using namespace HCXX;

class SampleAstChecker : public Checker {
    void CheckNode(const Node& node, ProblemsHolder& holder) override
    {
        auto funcNode = Node::Cast<FunctionDecl>(&node);
        if (funcNode != nullptr && funcNode->GetName() == "main") {
            holder.RegisterProblem(*this, node);
        }
    }
};

std::unique_ptr<Checker> CreateSampleAstChecker()
{
    return std::make_unique<SampleAstChecker>();
}
