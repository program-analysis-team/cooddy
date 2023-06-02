/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_STATICASSERTDECL_H
#define COODDY_STATICASSERTDECL_H

#include <ast/Node.h>

#include <string>

namespace HCXX {

class StaticAssertDecl : public Node {
public:
    StaticAssertDecl(const Node* assertExpr, std::string&& message, bool isFailed)
        : myAssertExpr(assertExpr), myMessage(std::move(message)), myIsFailed(isFailed)
    {}

    DECLARE_KIND(Node, Node::Kind::STATIC_ASSERT_DECL);
    DECLARE_SERIALIZE(StaticAssertDecl, myAssertExpr << myIsFailed);

    const Node* GetAssertExpression() const
    {
        return myAssertExpr;
    }

    const std::string& GetMessage() const
    {
        return myMessage;
    }

    bool Failed() const
    {
        return myIsFailed;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Message") {
            return myMessage;
        }

        if (attrName == "IsFailed") {
            return std::to_string(myIsFailed);
        }

        return Node::GetAttribute(attrName);
    }

    // LCOV_EXCL_START
    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myAssertExpr, callback);
    }
    // LCOV_EXCL_STOP

private:
    const Node* myAssertExpr = nullptr;
    std::string myMessage;
    bool myIsFailed = false;  //  TODO: Do we want to have inverted logic? Is isFailed==false good or bad?
};

};  // namespace HCXX

#endif  // COODDY_STATICASSERTDECL_H
