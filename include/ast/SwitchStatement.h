/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SWITCHSTATEMENT_H
#define COODDY_SWITCHSTATEMENT_H

#include <ast/Node.h>
#include <ast/SwitchCaseStatement.h>

namespace HCXX {

class SwitchStatement : public Node {
public:
    using CaseChildren = std::vector<SwitchCaseStatement*>;

    explicit SwitchStatement(const Node* condition, const Node* body, CaseChildren&& caseChildren)
        : myCondition(condition), myBody(body), myCaseChildren(std::move(caseChildren))
    {}

    DECLARE_KIND(Node, Node::Kind::SWITCH_STATEMENT);
    DECLARE_SERIALIZE(SwitchStatement, myCondition << myBody << myCaseChildren);

    const Node* GetCondition() const
    {
        return myCondition;
    }

    const Node* GetBody() const
    {
        return myBody;
    }

    const CaseChildren& GetCaseChildren() const
    {
        return myCaseChildren;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myCondition, callback);
        CALL_CALLBACK(myBody, callback);
    }

private:
    const Node* myCondition = nullptr;
    const Node* myBody = nullptr;
    CaseChildren myCaseChildren;
};
}  // namespace HCXX

#endif  // COODDY_SWITCHSTATEMENT_H
