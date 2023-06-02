/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_RETURNSTATEMENT_HPP
#define COODDY_RETURNSTATEMENT_HPP

#include <ast/Node.h>

namespace HCXX {

class ReturnStatement : public Node {
public:
    explicit ReturnStatement(const Node* retValue) : myRetValue(retValue) {}

    DECLARE_KIND(Node, Node::Kind::RETURN_STATEMENT);
    DECLARE_SERIALIZE(ReturnStatement, myRetValue);

    const Node* GetRetValue() const
    {
        return myRetValue;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myRetValue, callback);
    }

    void Print(std::string& source, int entryOffset) const override
    {
        SafePrint(source, entryOffset, "return", GetRange());
        Base::Print(source, entryOffset);
    }

private:
    const Node* myRetValue = nullptr;
};

}  // namespace HCXX

#endif  // COODDY_RETURNSTATEMENT_HPP
