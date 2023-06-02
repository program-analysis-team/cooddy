/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CXXCONVERSIONDECL_H
#define COODDY_CXXCONVERSIONDECL_H

#include <ast/CxxMethodDecl.h>

namespace HCXX {

class CxxConversionDecl : public CxxMethodDecl {
public:
    CxxConversionDecl(FunctionDecl&& funcDecl, const CxxRecordDecl* cxxRecord)
        : CxxMethodDecl(std::move(funcDecl), cxxRecord)
    {}

    DECLARE_KIND(CxxMethodDecl, Node::Kind::CXX_CONVERSION_DECL);
    DECLARE_SERIALIZE(CxxConversionDecl, nullptr);
};

};  // namespace HCXX

#endif  // COODDY_CXXCONVERSIONDECL_H
