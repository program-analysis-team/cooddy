/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_CXXMEMBERCALLEXPRESSION_H_
#define COODDY_ANALYZER_INCLUDE_AST_CXXMEMBERCALLEXPRESSION_H_

#include <ast/CallExpression.h>
#include <ast/MemberExpression.h>

namespace HCXX {

class CxxMemberCallExpression : public CallExpression {
public:
    explicit CxxMemberCallExpression(CallExpression&& callExpr, const Node* object, bool isVirtualCall = false)
        : CallExpression(std::move(callExpr)), myObject(object), myIsVirtualCall(isVirtualCall)
    {}

    DECLARE_KIND(CallExpression, Node::Kind::CXX_MEMBER_CALL_EXPRESSION);
    DECLARE_SERIALIZE(CxxMemberCallExpression, myObject << myIsVirtualCall);

    const Node* GetObject() const
    {
        auto memberExpr = Node::Cast<MemberExpression>(GetCallee());
        return memberExpr != nullptr ? memberExpr->GetBase() : myObject;
    }

    bool IsVirtualCall() const
    {
        return myIsVirtualCall;
    }
    void Print(std::string& source, int entryOffset) const override
    {
        auto obj = GetObject();
        if (obj != nullptr) {
            auto r = obj->GetRange();
            obj->Print(source, entryOffset);
            SafePrint(source, entryOffset, ".", r, r.Size());
        }
        Base::Print(source, entryOffset);
    }
    int GetPrintableNameOffset() const override
    {
        auto obj = GetObject();
        if (obj != nullptr) {
            auto r = obj->GetRange();
            return r.Size() + 1;
        }
        return 0;  // LCOV_EXCL_LINE
    }

private:
    const Node* myObject = nullptr;
    bool myIsVirtualCall = false;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_CXXMEMBERCALLEXPRESSION_H_
