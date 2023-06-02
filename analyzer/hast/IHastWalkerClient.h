/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef IHASTWALKERCLIENT_HEADER_FILE
#define IHASTWALKERCLIENT_HEADER_FILE

#include <memory>
#include <optional>
#include <ostream>

namespace HCXX {

class Node;

namespace Hast::Details {

class IHastWalkerClient;

using IHastWalkerClientPtr =
    std::unique_ptr<IHastWalkerClient>;  // TODO: should be replaced with  shared_ptr when HCAT-91 is implemented

class IHastWalkerClient {
public:
    virtual IHastWalkerClientPtr Clone() const = 0;

    virtual std::pair<IHastWalkerClientPtr, IHastWalkerClientPtr> EnterChildNode(const Node& node) const = 0;

    virtual bool IsFullDefinedPostion() const = 0;

    virtual const Node* GetMatchedNode() const = 0;

    virtual void Dump(std::ostream& oss) const = 0;

    virtual ~IHastWalkerClient() = default;
};

}  // namespace Hast::Details

}  //  namespace HCXX

#endif  //  #ifndef IHASTWALKERCLIENT_HEADER_FILE
