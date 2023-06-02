/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CXXDTORDECL_H
#define COODDY_CXXDTORDECL_H

#include <ast/CxxMethodDecl.h>

namespace HCXX {

class CxxDtorDecl : public CxxMethodDecl {
public:
    CxxDtorDecl(FunctionDecl&& funcDecl, const CxxRecordDecl* cxxRecord) : CxxMethodDecl(std::move(funcDecl), cxxRecord)
    {}

    DECLARE_KIND(CxxMethodDecl, Node::Kind::CXX_DTOR_DECL);
    DECLARE_SERIALIZE(CxxDtorDecl, nullptr);
};

};  // namespace HCXX

#endif  // COODDY_CXXDTORDECL_H
