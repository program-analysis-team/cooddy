/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CONTINUESTATEMENT_H
#define COODDY_CONTINUESTATEMENT_H

#include "ast/Node.h"

namespace HCXX {

class ContinueStatement : public Node {
public:
    DECLARE_KIND(Node, Node::Kind::CONTINUE_STATEMENT);
    DECLARE_SERIALIZE(ContinueStatement, nullptr);
};

}  // namespace HCXX

#endif  // COODDY_CONTINUESTATEMENT_H
