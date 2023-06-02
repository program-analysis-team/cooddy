/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_CXXMETHODDECL_H_
#define COODDY_ANALYZER_INCLUDE_AST_CXXMETHODDECL_H_

#include <ast/FunctionDecl.h>

namespace HCXX {

class CxxRecordDecl;

class CxxMethodDecl : public FunctionDecl {
public:
    CxxMethodDecl(FunctionDecl&& funcDecl, const CxxRecordDecl* cxxRecord)
        : FunctionDecl(std::move(funcDecl)), myCxxRecord(cxxRecord)
    {}

    DECLARE_KIND(FunctionDecl, Node::Kind::CXX_METHOD_DECL);
    DECLARE_SERIALIZE(CxxMethodDecl, myCxxRecord);

    bool IsCopy() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::COPY)];
    }
    bool IsMove() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::MOVE)];
    }

    bool IsOverride() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::OVERRIDE)];
    }

    bool IsStatic() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::STATIC)];
    }

    bool IsVirtual() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::VIRTUAL)];
    }
    // LCOV_EXCL_START
    std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "IsCopy") {
            return std::to_string(IsCopy());
        }

        if (attrName == "IsMove") {
            return std::to_string(IsMove());
        }

        if (attrName == "IsOverride") {
            return std::to_string(IsOverride());
        }

        if (attrName == "IsStatic") {
            return std::to_string(IsStatic());
        }

        return FunctionDecl::GetAttribute(attrName);
    }
    // LCOV_EXCL_STOP

    const CxxRecordDecl* GetCxxRecord() const
    {
        return myCxxRecord;
    }

    static const ParamVarDecl* GetThisDecl()
    {
        return &myThisDecl;
    }

private:
    static ParamVarDecl myThisDecl;
    const CxxRecordDecl* myCxxRecord = nullptr;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_CXXMETHODDECL_H_
