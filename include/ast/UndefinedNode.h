/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_UNDEFINEDNODE_H_
#define COODDY_ANALYZER_INCLUDE_AST_UNDEFINEDNODE_H_

#include <ast/Node.h>

namespace HCXX {
// LCOV_EXCL_START
class UndefinedNode : public Node {
public:
    DECLARE_KIND(Node, Node::Kind::UNDEFINED);
    DECLARE_SERIALIZE(UndefinedNode, nullptr);
};
// LCOV_EXCL_STOP
};      // namespace HCXX
#endif  // COODDY_ANALYZER_INCLUDE_AST_UNDEFINEDNODE_H_
