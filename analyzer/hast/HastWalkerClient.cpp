/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "HastWalkerClient.h"

#include <utils/LocaleUtils.h>

#include <string>

namespace HCXX::Hast::Details {

IHastWalkerClientPtr HastWalkerClient::Create(std::string_view expression, std::string* errorMsg)
{
    //  At the moment we support only following statements in the expressions
    //  -   '/' - direct child node
    //  -   '//' - child or grandchild or etc. node
    //  -   <node type>
    //  Examples:
    //  -   '//FunctionDecl' - method anythere in the AST
    //  -   '/FunctionDecl' - method is the top of the AST

    std::string_view::size_type pos = expression.find_first_not_of(" \t");
    if (pos == std::string::npos) {
        if (errorMsg != nullptr) {
            *errorMsg = HCXX::StrLocales::GetStringLocale("EXPRESSION_EMPTY_OR_CONTAINS_ONLY_WHITE_SPACES");
        }
        return IHastWalkerClientPtr();
    }

    //  Let' detect if the expression starts from '//' or '/'
    bool directChildrenOnly = true;
    if (expression[pos] != '/') {
        if (errorMsg != nullptr) {
            *errorMsg = HCXX::StrLocales::GetStringLocale("NODE_DESCRIPTION_MUST_START_FROM");
        }
        return IHastWalkerClientPtr();
    }
    if (pos + 1 < expression.size() && expression[pos + 1] == '/') {
        directChildrenOnly = false;
        ++pos;
    }

    std::string_view::size_type charsUsedByCurrentNodeMatcher = std::string_view::npos;
    auto currentNodeMatcher =
        ICurrentNodeMatcher::Create(expression.substr(pos + 1), errorMsg, charsUsedByCurrentNodeMatcher);
    if (!currentNodeMatcher) {
        return IHastWalkerClientPtr();
    }
    std::string_view::size_type nextItemPos = pos + charsUsedByCurrentNodeMatcher + 1;

    IHastWalkerClientPtr childWalker;  // LCOV_EXCL_LINE
    if (nextItemPos != expression.length()) {
        childWalker = Create(expression.substr(nextItemPos), errorMsg);
        if (!childWalker) {
            return IHastWalkerClientPtr();
        }
    }

    std::unique_ptr<HastWalkerClient> ptr =
        std::make_unique<HastWalkerClient>(directChildrenOnly, currentNodeMatcher, std::move(childWalker));
    return ptr;
}

HastWalkerClient::HastWalkerClient(bool directChildrenOnly,
                                   const std::shared_ptr<ICurrentNodeMatcher>& currentNodeMatcher,
                                   IHastWalkerClientPtr&& childWalkerClient, const Node* currentPosition)
    : myDirectChildrenOnly(directChildrenOnly),
      myCurrentNodeMatcher(currentNodeMatcher),
      myChildWalkerClient(std::move(childWalkerClient)),
      myCurrentPosition(currentPosition)
{}

HastWalkerClient::~HastWalkerClient() {}

IHastWalkerClientPtr HastWalkerClient::Clone() const
{
    return IHastWalkerClientPtr(CloneImpl());
}

HastWalkerClient* HastWalkerClient::CloneImpl() const
{
    auto childWalkerClient = myChildWalkerClient ? myChildWalkerClient->Clone() : nullptr;
    return new HastWalkerClient(myDirectChildrenOnly, myCurrentNodeMatcher, std::move(childWalkerClient),
                                myCurrentPosition);
}

std::pair<IHastWalkerClientPtr, IHastWalkerClientPtr> HastWalkerClient::EnterChildNode(const Node& node) const
{
    if (myCurrentPosition != nullptr) {
        if (!myChildWalkerClient) {
            //  We have got fully defined position before the step
            //  To be honest, we shouldn't get here
            return std::pair<IHastWalkerClientPtr, IHastWalkerClientPtr>();  // LCOV_EXCL_LINE
        }

        auto pairStates = myChildWalkerClient->EnterChildNode(node);
        IHastWalkerClientPtr firstRes;
        IHastWalkerClientPtr secondRes;  // LCOV_EXCL_LINE
        if (pairStates.first) {
            firstRes = std::make_unique<HastWalkerClient>(myDirectChildrenOnly, myCurrentNodeMatcher,
                                                          std::move(pairStates.first), myCurrentPosition);
        }
        if (pairStates.second) {
            secondRes = std::make_unique<HastWalkerClient>(myDirectChildrenOnly, myCurrentNodeMatcher,
                                                           std::move(pairStates.second), myCurrentPosition);
        }

        std::pair<IHastWalkerClientPtr, IHastWalkerClientPtr> tmp(std::move(firstRes), std::move(secondRes));
        return tmp;
    }

    //  Now we are sure that m_currentPosition is null
    IHastWalkerClientPtr firstRes;
    if (!myDirectChildrenOnly) {
        firstRes = Clone();
    }
    IHastWalkerClientPtr secondRes;
    std::unique_ptr<HastWalkerClient> newState(CloneImpl());
    newState->myCurrentPosition = &node;
    bool isMatched = newState->myCurrentNodeMatcher->CheckMatch(node);
    if (!isMatched) {
        //  We are sure that current node doesn't match => let's avoid processing it.
        newState.reset();
    }

    secondRes = std::move(newState);
    return std::make_pair(std::move(firstRes), std::move(secondRes));
}

bool HastWalkerClient::IsFullDefinedPostion() const
{
    if (myCurrentPosition == nullptr) {
        return false;
    }

    if (myChildWalkerClient) {
        return myChildWalkerClient->IsFullDefinedPostion();
    }

    return true;
}

const Node* HastWalkerClient::GetMatchedNode() const
{
    if (myChildWalkerClient) {
        return myChildWalkerClient->GetMatchedNode();
    }

    return myCurrentPosition;
}

//  LCOV_EXCL_START
void HastWalkerClient::Dump(std::ostream& os) const
{
    os << "(pos=" << static_cast<const void*>(myCurrentPosition) << ')';

    if (myChildWalkerClient) {
        os << "->";
        myChildWalkerClient->Dump(os);
    }
}
//  LCOV_EXCL_STOP

}  // namespace HCXX::Hast::Details
