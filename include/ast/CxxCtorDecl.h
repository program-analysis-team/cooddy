/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Constructor representation for any CXXRecordDecl
//

#ifndef COODDY_CXXCONSTRUCTORDECL_H
#define COODDY_CXXCONSTRUCTORDECL_H

#include <ast/CxxCtorInitializerDecl.h>
#include <ast/CxxMethodDecl.h>

using std::string;
using std::vector;

namespace HCXX {

class CxxCtorDecl : public CxxMethodDecl {
public:
    using Initializers = std::vector<NodePtr<Node>>;

    CxxCtorDecl(FunctionDecl&& funcDecl, const CxxRecordDecl* cxxRecord, Initializers&& init)
        : CxxMethodDecl(std::move(funcDecl), cxxRecord), myInitList(std::move(init))
    {}

    DECLARE_KIND(CxxMethodDecl, Node::Kind::CXX_CTOR_DECL);
    DECLARE_SERIALIZE(CxxCtorDecl, myInitList);

    const Initializers& GetInitList() const
    {
        return myInitList;
    }
    bool IsExplicit() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::EXPLICIT)];
    }
    bool IsDefault() const
    {
        return myQualifiers[static_cast<int>(FunctionQualifiersIndex::DEFAULT)];
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        for (auto& init : myInitList) {
            CALL_CALLBACK(init, callback);
        }
        FunctionDecl::TraverseChildren(callback);
    }

    std::optional<std::string> GetAttribute(std::string_view attrName) const
    {
        if (attrName == "IsCopy") {
            return std::to_string(IsCopy());
        }
        if (attrName == "IsMove") {
            return std::to_string(IsMove());
        }
        if (attrName == "IsExplicit") {
            return std::to_string(IsExplicit());
        }
        if (attrName == "IsDefault") {
            return std::to_string(IsDefault());
        }
        if (attrName == "IsImplicit") {
            //  LCOV_EXCL_START
            return std::to_string(IsImplicit());
            //  LCOV_EXCL_STOP
        }

        return FunctionDecl::GetAttribute(attrName);
    }

private:
    Initializers myInitList;
};

};  // namespace HCXX

#endif  // COODDY_CXXCONSTRUCTORDECL_H
