/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
// Describes an C or C++ initializer list.
//
// InitListExpr describes an initializer list, which can be used to initialize objects of different types, including
// struct/class/union types, arrays, and vectors. For example:
//
// struct foo x = { 1, { 2, 3 } };

#ifndef COODDY_INITLISTEXPRESSION_H
#define COODDY_INITLISTEXPRESSION_H

#include <ast/Node.h>

#include <string>

namespace HCXX {

class InitListExpression : public Node {
public:
    explicit InitListExpression(std::vector<const Node*>&& initList, bool array = false)
        : myInitList(initList), myForArray(array)
    {}

    DECLARE_KIND(Node, Node::Kind::INIT_LIST_EXPRESSION);
    DECLARE_SERIALIZE(InitListExpression, myInitList << myForArray);

    bool IsForArray() const
    {
        return myForArray;
    }

    const std::vector<const Node*>& GetInitList() const
    {
        return myInitList;
    }
    void TraverseChildren(TraverseCallback callback) const override
    {
        for (auto& node : myInitList) {
            CALL_CALLBACK(node, callback);
        }
    }

    void Print(std::string& source, int entryOffset) const override
    {
        auto r = GetRange();
        SafePrint(source, entryOffset, "{", r);
        SafePrint(source, entryOffset, "}", r, r.Size() - 1);
        Base::Print(source, entryOffset);
    }

private:
    std::vector<const Node*> myInitList;
    bool myForArray = false;
};

};  // namespace HCXX

#endif  // COODDY_INITLISTEXPRESSION_H
