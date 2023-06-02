/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CXXNEWEXPRESSION_H
#define COODDY_CXXNEWEXPRESSION_H

#include <ast/CxxRecordDecl.h>

#include <vector>

#include "ast/CallExpression.h"

namespace HCXX {

class CxxNewExpression : public HCXX::CallExpression {
public:
    explicit CxxNewExpression(const Node* operatorNew, std::vector<const Node*>&& placementArgs,
                              const Node* operatorNewDecl, const Node* arrayElemCount, Type type)
        : HCXX::CallExpression(operatorNew, operatorNewDecl, {}, type),
          myPlacementArgs(std::move(placementArgs)),
          myArrayElemCount(arrayElemCount),
          myType(type)
    {}

    ~CxxNewExpression() = default;

    bool IsReplacementNew() const
    {
        return !myPlacementArgs.empty();
    }

    std::vector<const Node*> GetPlacementArgs() const
    {
        return myPlacementArgs;
    }

    bool IsArrayNew() const
    {
        return myArrayElemCount != nullptr;
    }

    uint32_t GetNumOfElementsInArray() const;

    const Node* GetArrayElemCount() const
    {
        return myArrayElemCount;
    }

    HCXX::Type GetType() const override
    {
        return myType;
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        for (auto& arg : myPlacementArgs) {
            CALL_CALLBACK(arg, callback);
        }
        CALL_CALLBACK(myArrayElemCount, callback);
        CallExpression::TraverseChildren(callback);
    }

    DECLARE_KIND(CallExpression, Node::Kind::CXX_NEW_EXPRESSION);
    // COODDY_SUPPRESS
    DECLARE_SERIALIZE(CxxNewExpression, myPlacementArgs << myArrayElemCount << myType);

private:
    std::vector<const Node*> myPlacementArgs;
    const Node* myArrayElemCount;
    Type myType;
};
}  // namespace HCXX

#endif  // COODDY_CXXNEWEXPRESSION_H
