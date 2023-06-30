/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_LABELSTATEMENT_H
#define COODDY_LABELSTATEMENT_H

#include <ast/NamedNode.h>

namespace HCXX {

class LabelStatement : public NamedNode<Node> {
public:
    LabelStatement(const char* name, const Node* subStatement) : Base(nullptr, name), mySubStatement(subStatement) {}
    ~LabelStatement() = default;

    // LCOV_EXCL_START
    DECLARE_KIND(NamedNode<Node>, Node::Kind::LABEL_STATEMENT);
    DECLARE_SERIALIZE(LabelStatement, mySubStatement);

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(mySubStatement, callback);
    }

    const Node* GetSubStatement() const
    {
        return mySubStatement;
    }
    // LCOV_EXCL_STOP

private:
    const Node* mySubStatement = nullptr;
};

}  // namespace HCXX

#endif  // COODDY_LABELSTATEMENT_H
