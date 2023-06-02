/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_BREAKSTATEMENT_H
#define COODDY_BREAKSTATEMENT_H

#include "ast/Node.h"

namespace HCXX {

class BreakStatement : public Node {
public:
    DECLARE_KIND(Node, Node::Kind::BREAK_STATEMENT);
    DECLARE_SERIALIZE(BreakStatement, nullptr);
    // LCOV_EXCL_START
    void Print(std::string& source, int entryOffset) const override
    {
        SafePrint(source, entryOffset, "break;", GetRange());
        Base::Print(source, entryOffset);
    }
    // LCOV_EXCL_STOP
};

}  // namespace HCXX

#endif  // COODDY_BREAKSTATEMENT_H
