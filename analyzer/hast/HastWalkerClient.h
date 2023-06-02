/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef HASTWALKERCLIENT_HEADER_FILE
#define HASTWALKERCLIENT_HEADER_FILE

#include <string_view>

#include "ICurrentNodeMatcher.h"
#include "IHastWalkerClient.h"

namespace HCXX::Hast::Details {

class HastWalkerClient : public IHastWalkerClient {
public:
    static IHastWalkerClientPtr Create(std::string_view expression, std::string* errorMsg);

public:
    HastWalkerClient(const HastWalkerClient&) = delete;
    HastWalkerClient& operator()(const HastWalkerClient&) = delete;

public:
    HastWalkerClient(bool directChildrenOnly, const std::shared_ptr<ICurrentNodeMatcher>& currentNodeMatcher,
                     IHastWalkerClientPtr&& childWalkerClient, const Node* currentPosition = nullptr);

    ~HastWalkerClient();

    virtual IHastWalkerClientPtr Clone() const override;

    virtual std::pair<IHastWalkerClientPtr, IHastWalkerClientPtr> EnterChildNode(const Node& node) const override;

    virtual bool IsFullDefinedPostion() const override;

    virtual const Node* GetMatchedNode() const override;

    virtual void Dump(std::ostream& oss) const override;

private:
    HastWalkerClient* CloneImpl() const;

private:
    bool myDirectChildrenOnly;
    std::shared_ptr<ICurrentNodeMatcher> myCurrentNodeMatcher;
    IHastWalkerClientPtr myChildWalkerClient;
    const Node* myCurrentPosition;
};

}  // namespace HCXX::Hast::Details

#endif  //  #ifndef HASTWALKERCLIENT_HEADER_FILE
