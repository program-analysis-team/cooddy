/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "HastExpressionImpl.h"

#include "HastWalkerClient.h"

namespace HCXX::Hast::Details {

std::unique_ptr<HastExpressionImpl> HastExpressionImpl::BuildExpression(const std::string_view expression,
                                                                        // TODO HCAT-665
                                                                        // COODDY_SUPPRESS PtrArgIsCheckedChecker
                                                                        std::string* errorMsg)
{
    IHastWalkerClientPtr defaultState = HastWalkerClient::Create(expression, errorMsg);
    if (!defaultState) {
        return std::unique_ptr<HastExpressionImpl>();
    }

    return std::make_unique<HastExpressionImpl>(std::move(defaultState));
}

HastExpressionImpl::HastExpressionImpl(IHastWalkerClientPtr&& defaultState) : myDefaultState(std::move(defaultState)) {}

void HastExpressionImpl::BeginTranslationUnitProcessing()
{
    //  We should add the initial state to our state machine "All sub-expressions refers to nothere, their state is
    //  unknown."
    std::stack<AllPossibleStates>().swap(myPossibleStates);
    myPossibleStates.emplace().states.emplace_back(myDefaultState->Clone());
}

void HastExpressionImpl::ClearMatches()
{
    myMatchedNodes.clear();
}

void HastExpressionImpl::EnterChildNode(const Node& node)
{
    AllPossibleStates newStates;
    AllPossibleStates& currentState = myPossibleStates.top();
    for (auto& state : currentState.states) {
        auto twoStates = state->EnterChildNode(node);
        if (twoStates.first) {
            ProcessStateOnEnterChildNode(twoStates.first, newStates);
        }
        if (twoStates.second) {
            ProcessStateOnEnterChildNode(twoStates.second, newStates);
        }
    }
    myPossibleStates.push(std::move(newStates));
}

void HastExpressionImpl::ProcessStateOnEnterChildNode(IHastWalkerClientPtr& ref, AllPossibleStates& newStates)
{
    if (!ref->IsFullDefinedPostion()) {
        newStates.states.emplace_back(std::move(ref));
        return;
    }

    this->myMatchedNodes.emplace(ref->GetMatchedNode());
}

void HastExpressionImpl::LeaveChildNode()
{
    myPossibleStates.pop();
}

const std::set<const Node*>& HastExpressionImpl::GetMatchedResult() const
{
    return myMatchedNodes;
}

}  //  namespace HCXX::Hast::Details
