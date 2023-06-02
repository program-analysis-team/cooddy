/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#ifndef COODDY_ANALYZER_INCLUDE_AST_IFSTATEMENT_H_
#define COODDY_ANALYZER_INCLUDE_AST_IFSTATEMENT_H_

#include <ast/Node.h>

namespace HCXX {

class ElseKeyword : public Node {
public:
    DECLARE_KIND(Node, Node::Kind::ELSE_KEYWORD);
    DECLARE_SERIALIZE(ElseKeyword, nullptr);
};

class IfStatement : public Node {
public:
    explicit IfStatement(const Node* condition, const Node* thenPart, const ElseKeyword* elseKeyword,
                         const Node* elsePart, const Node* initPart, const Node* condVarDeclStmt)
        : myCondition(condition),
          myThenPart(thenPart),
          myElseKeyword(elseKeyword),
          myElsePart(elsePart),
          myInitPart(initPart),
          myCondVarDeclStmt(condVarDeclStmt)
    {}

    DECLARE_KIND(Node, Node::Kind::IF_STATEMENT);
    DECLARE_SERIALIZE(IfStatement, myCondition << myThenPart << myElsePart << myInitPart << myCondVarDeclStmt);

    const Node* GetCondition() const
    {
        return myCondition;
    }
    const Node* GetThenPart() const
    {
        return myThenPart;
    }
    bool HasElsePart() const
    {
        return myElsePart != nullptr;
    }
    const Node* GetElsePart() const
    {
        return myElsePart;
    }
    const ElseKeyword* GetElseKeyword() const
    {
        return myElseKeyword;
    }
    const Node* GetInitPart() const
    {
        return myInitPart;
    }
    // LCOV_EXCL_START
    const Node* GetCondVarDeclStmt() const
    {
        return myCondVarDeclStmt;
    }
    // LCOV_EXCL_STOP

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myCondition, callback);
        CALL_CALLBACK(myCondVarDeclStmt, callback);
        CALL_CALLBACK(myThenPart, callback);
        CALL_CALLBACK(myElsePart, callback);
        CALL_CALLBACK(myInitPart, callback);
    }

    void Print(std::string& source, int entryOffset) const override
    {
        auto range = GetRange();
        SafePrint(source, entryOffset, "if", range);
        if (myCondition != nullptr) {
            auto condRange = myCondition->GetRange();
            SafePrint(source, entryOffset, "(", condRange, -1);
            SafePrint(source, entryOffset, ")", condRange, condRange.Size());
        }
        Base::Print(source, entryOffset);
        if (myThenPart != nullptr) {
            auto r = myThenPart->GetRange();
            SafePrint(source, entryOffset, "{", r);
            SafePrint(source, entryOffset, "}", r, r.Size() - 1);
        }
        if (myElsePart != nullptr) {
            auto r = myElsePart->GetRange();
            SafePrint(source, entryOffset, "else", r, -4);
            SafePrint(source, entryOffset, "{", r);
            SafePrint(source, entryOffset, "}", r, r.Size());
        }
    }

private:
    const Node* myCondition = nullptr;
    const Node* myThenPart = nullptr;
    const ElseKeyword* myElseKeyword = nullptr;
    const Node* myElsePart = nullptr;
    const Node* myInitPart = nullptr;
    const Node* myCondVarDeclStmt = nullptr;
};
}  // namespace HCXX
#endif  // COODDY_ANALYZER_INCLUDE_AST_IFSTATEMENT_H_
