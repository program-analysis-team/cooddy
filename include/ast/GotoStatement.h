/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_GOTOSTATEMENT_H
#define COODDY_GOTOSTATEMENT_H

#include "ast/Node.h"

namespace HCXX {

class GotoStatement : public Node {
public:
    explicit GotoStatement(const Node* label) : myLabel(label) {}
    ~GotoStatement() = default;

    DECLARE_KIND(Node, Node::Kind::GOTO_STATEMENT);
    DECLARE_SERIALIZE(GotoStatement, myLabel);

    const Node* GetLabel() const
    {
        return myLabel;
    }

private:
    const Node* myLabel = nullptr;
};

}  // namespace HCXX

#endif  // COODDY_GOTOSTATEMENT_H
