/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Adaptor class for mixing declarations with statements and expressions.
//

#ifndef COODDY_DECLSTATEMENT_H
#define COODDY_DECLSTATEMENT_H

#include <ast/Node.h>

#include <vector>

namespace HCXX {

class DeclStatement : public Node {
public:
    explicit DeclStatement(std::vector<NodePtr<Node>>&& decls) : myDecls(std::move(decls)) {}

    DECLARE_KIND(Node, Node::Kind::DECL_STATEMENT);
    DECLARE_SERIALIZE(DeclStatement, myDecls);

    // LCOV_EXCL_START
    const Node* GetSingleDecl() const
    {
        return !myDecls.empty() ? myDecls.front() : nullptr;
    }
    // LCOV_EXCL_STOP

    const std::vector<NodePtr<Node>>& GetDeclGroup() const
    {
        return myDecls;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        for (auto& node : myDecls) {
            callback(*node);
        }
    }

private:
    std::vector<NodePtr<Node>> myDecls;
};

}  // namespace HCXX

#endif  // COODDY_DECLSTATEMENT_H
