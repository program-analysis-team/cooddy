/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.

#include <ast/CallExpression.h>
#include <ast/MemberExpression.h>
#include <ast/RefExpression.h>
#include <dfa/DfaChecker.h>

using namespace HCXX;

class NullAfterFreeChecker : public DfaChecker {
    Annotation::Kind myFreeSourceKind;
    Annotation::Kind myWriteKind;

public:
    NullAfterFreeChecker()
    {
        myFreeSourceKind = Annotation::GetKind("FreeSource");
        myWriteKind = Annotation::GetKind("Write");
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        auto member = state.GetNodeAs<MemberExpression>();
        if (member != nullptr && state.HasAnnotation(myFreeSourceKind) && state.GetDeclState() != nullptr) {
            auto baseNode = Node::Cast<RefExpression>(member->GetBase()->GetInnerNode());
            if (baseNode != nullptr) {
                auto varDecl = Node::Cast<VarDecl>(baseNode->GetDeclaration());
                if (varDecl != nullptr && varDecl->IsLocalVariableDeclaration() && !varDecl->GetType().IsPointer()) {
                    return;
                }
            }
            for (auto& [a, v] : state.GetDeclState()->GetAnnotationSources(myWriteKind)) {
                auto writeMember = Node::Cast<MemberExpression>(v);
                if ((writeMember == nullptr || member->GetValue() == writeMember->GetValue()) &&
                    (v->GetRange().begin >= state.GetNode()->GetRange().begin)) {
                    return;
                }
            }
            state.RegisterProblem(holder, *this, *state.GetNode());
        }
    };
};

std::unique_ptr<HCXX::Checker> CreateNullAfterFreeChecker()
{
    return std::make_unique<NullAfterFreeChecker>();
}
