/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#ifndef COODDY_ANALYZER_INCLUDE_AST_FORSTATEMENT_H_
#define COODDY_ANALYZER_INCLUDE_AST_FORSTATEMENT_H_

#include <ast/Node.h>

namespace HCXX {

class LoopStatement : public Node {
public:
    DECLARE_ENUM(LoopKind, FOR, RANGED_FOR, WHILE, DO_WHILE);

    explicit LoopStatement(LoopKind kind, const Node* init, const Node* condition, const Node* myBody,
                           const Node* conditionVariable, const Node* increment, const Node* rangedInit = nullptr)
        : myKind(kind),
          myInit(init),
          myCondition(condition),
          myBody(myBody),
          myConditionVariable(conditionVariable),
          myIncrement(increment),
          myRangeInit(rangedInit)
    {}

    DECLARE_KIND(Node, Node::Kind::LOOP_STATEMENT);
    DECLARE_SERIALIZE(LoopStatement,
                      myKind << myInit << myConditionVariable << myCondition << myBody << myIncrement << myRangeInit);

    LoopKind GetLoopKind() const
    {
        return myKind;
    }
    const Node* GetInit() const
    {
        return myInit;
    }

    // LCOV_EXCL_START
    const Node* GetRangeInit() const
    {
        return myRangeInit;
    }
    // LCOV_EXCL_STOP

    const Node* GetCondition() const
    {
        return myCondition;
    }
    const Node* GetBody() const
    {
        return myBody;
    }
    const Node* GetIncrement() const
    {
        return myIncrement;
    }
    const Node* GetConditionVariable() const
    {
        return myConditionVariable;
    }

    std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Kind") {
            return LoopKindToCStr(GetLoopKind());
        }

        return Node::GetAttribute(attrName);
    }

    // LCOV_EXCL_START
    std::string Dump() const override
    {
        return Node::Dump() + ", kind: " + std::string(LoopKindToCStr(myKind));
    }
    // LCOV_EXCL_STOP

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myInit, callback);
        CALL_CALLBACK(myConditionVariable, callback);
        CALL_CALLBACK(myCondition, callback);
        CALL_CALLBACK(myIncrement, callback);
        CALL_CALLBACK(myBody, callback);
        CALL_CALLBACK(myRangeInit, callback);
    }

    bool IsEqualNode(const Node* comparedNode) const override
    {
        auto loopStatement = CompareAndCast<LoopStatement>(comparedNode, Node::Kind::COMPOUND_STATEMENT);
        return loopStatement != nullptr && myKind == loopStatement->GetLoopKind();
    }
    void Print(std::string& source, int entryOffset) const override
    {
        Base::Print(source, entryOffset);
        switch (GetLoopKind()) {
            case LoopKind::RANGED_FOR:
                PrintRangedFor(source, entryOffset);
                break;
            case LoopKind::FOR:
                PrintFor(source, entryOffset);
                break;
            case LoopKind::DO_WHILE:
                PrintDoWhile(source, entryOffset);
                break;
            default:
                PrintWhile(source, entryOffset);
                break;
        }
        if (myBody != nullptr) {
            auto bodyRange = myBody->GetRange();
            SafePrint(source, entryOffset, "{", bodyRange);
            SafePrint(source, entryOffset, "}", bodyRange, bodyRange.Size() - 1);
        }
    }
    void PrintDoWhile(std::string& source, int entryOffset) const
    {
        SafePrint(source, entryOffset, "do", GetRange());
        auto cond = GetCondition();
        if (cond != nullptr) {
            auto r = cond->GetRange();
            if (myBody != nullptr) {
                auto bodyRange = myBody->GetRange();
                SafePrint(source, entryOffset, "while", bodyRange, bodyRange.Size() + 1);
            }
            SafePrint(source, entryOffset, "(", r, -1);
            SafePrint(source, entryOffset, ")", r, r.Size());
        }
    }
    void PrintWhile(std::string& source, int entryOffset) const
    {
        SafePrint(source, entryOffset, "while", GetRange());
        auto cond = GetCondition();
        if (cond != nullptr) {
            auto r = cond->GetRange();
            SafePrint(source, entryOffset, "(", r, -1);
            SafePrint(source, entryOffset, ")", r, r.Size());
        }
    }
    void PrintFor(std::string& source, int entryOffset) const
    {
        SafePrint(source, entryOffset, "for", GetRange());
        if (myInit != nullptr && myCondition != nullptr && myIncrement != nullptr) {
            auto r = myInit->GetRange();
            SafePrint(source, entryOffset, "(", r, -1);
            SafePrint(source, entryOffset, ";", r, r.Size() - 1);
            r = myCondition->GetRange();
            SafePrint(source, entryOffset, ";", r, r.Size());
            r = myIncrement->GetRange();
            SafePrint(source, entryOffset, ")", r, r.Size());
        }
    }
    void PrintRangedFor(std::string& source, int entryOffset) const
    {
        SafePrint(source, entryOffset, "for", GetRange());
        if (myConditionVariable != nullptr) {
            auto r = myConditionVariable->GetRange();
            SafePrint(source, entryOffset, "(", r, -1);
        }
        if (myRangeInit != nullptr) {
            auto r = myRangeInit->GetRange();
            SafePrint(source, entryOffset, ")", r, r.Size());
        }
    }

private:
    LoopKind myKind = LoopKind::FOR;
    const Node* myInit = nullptr;
    const Node* myConditionVariable = nullptr;
    const Node* myCondition = nullptr;
    const Node* myBody = nullptr;
    const Node* myIncrement = nullptr;
    const Node* myRangeInit = nullptr;
};
}  // namespace HCXX
#endif  // COODDY_ANALYZER_INCLUDE_AST_FORSTATEMENT_H_
