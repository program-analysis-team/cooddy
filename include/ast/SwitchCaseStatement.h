/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SWITCHCASESTATEMENT_H
#define COODDY_SWITCHCASESTATEMENT_H

#include <ast/Node.h>

namespace HCXX {

class SwitchCaseStatement : public Node {
public:
    explicit SwitchCaseStatement(const Node* body) : myBody(body) {}

    virtual ~SwitchCaseStatement() = default;

    DECLARE_KIND(Node, Node::Kind::SWITCH_CASE_STATEMENT);
    DECLARE_SERIALIZE(SwitchCaseStatement, myBody);  // LCOV_EXCL_LINE

    const Node* GetBody() const
    {
        return myBody;
    }
    // LCOV_EXCL_START
    virtual bool IsDefault() const
    {
        return false;
    }
    // LCOV_EXCL_STOP

    virtual const Node* GetCaseExpr() const
    {
        return nullptr;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const
    {
        if (attrName == "IsDefault") {
            return std::to_string(IsDefault());
        }

        return Node::GetAttribute(attrName);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myBody, callback);
    }

private:
    const Node* myBody = nullptr;
};

class CaseStatement : public SwitchCaseStatement {
public:
    explicit CaseStatement(const Node* body, const Node* leftStatement, const Node* rightStatement)
        : SwitchCaseStatement(body), myLHS(leftStatement), myRHS(rightStatement)
    {}

    DECLARE_KIND(SwitchCaseStatement, Node::Kind::CASE_STATEMENT);
    DECLARE_SERIALIZE(CaseStatement, myLHS << myRHS);

    const Node* GetLHS() const
    {
        return myLHS;
    };
    const Node* GetRHS() const
    {
        return myRHS;
    };

    bool IsDefault() const override
    {
        return false;
    }

    const Node* GetCaseExpr() const override
    {
        return myLHS;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        SwitchCaseStatement::TraverseChildren(callback);
        CALL_CALLBACK(myLHS, callback);
        CALL_CALLBACK(myRHS, callback);
    }

private:
    const Node* myLHS = nullptr;
    const Node* myRHS = nullptr;
};

class DefaultStatement : public SwitchCaseStatement {
public:
    explicit DefaultStatement(const Node* body) : SwitchCaseStatement(body) {}

    DECLARE_KIND(SwitchCaseStatement, Node::Kind::DEFAULT_STATEMENT);
    DECLARE_SERIALIZE(DefaultStatement, nullptr);

    bool IsDefault() const override
    {
        return true;
    }
};

}  // namespace HCXX

#endif  // COODDY_SWITCHCASESTATEMENT_H
