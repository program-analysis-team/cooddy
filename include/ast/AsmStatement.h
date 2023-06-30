/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_ASMSTATEMENT_H_
#define COODDY_ANALYZER_INCLUDE_AST_ASMSTATEMENT_H_

#include <ast/Node.h>

namespace HCXX {

class AsmStatement : public HCXX::CompoundNode {
public:
    DECLARE_KIND(CompoundNode, Node::Kind::ASM_STATEMENT);
    DECLARE_SERIALIZE(AsmStatement, nullptr);
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_ASMSTATEMENT_H_
