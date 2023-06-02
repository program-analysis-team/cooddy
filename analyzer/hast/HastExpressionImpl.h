/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef HAST_EXPRESION_IMPL_HEADER_FILE
#define HAST_EXPRESION_IMPL_HEADER_FILE

#include <ast/Node.h>

#include <stack>

#include "HastMatchingEngine.h"
#include "IHastWalkerClient.h"

namespace HCXX::Hast::Details {

class HastExpressionImpl : public HastExpression {
public:
    struct AllPossibleStates {
        std::vector<IHastWalkerClientPtr> states;
    };

public:
    static std::unique_ptr<HastExpressionImpl> BuildExpression(const std::string_view expression,
                                                               std::string* errorMsg);

public:
    virtual const std::set<const Node*>& GetMatchedResult() const override;

public:
    void BeginTranslationUnitProcessing();

    void ClearMatches();

    void EnterChildNode(const Node& node);

    void LeaveChildNode();

public:  //  TODO: try to make private
    explicit HastExpressionImpl(IHastWalkerClientPtr&& defaultState);

private:
    std::set<const Node*> myMatchedNodes;
    IHastWalkerClientPtr myDefaultState;
    std::stack<AllPossibleStates> myPossibleStates;

    void ProcessStateOnEnterChildNode(IHastWalkerClientPtr& ref, AllPossibleStates& newStates);
};

}  // namespace HCXX::Hast::Details

#endif  //   #ifndef HAST_EXPRESION_IMPL_HEADER_FILE
