/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ACCESSSPECDECL_H
#define COODDY_ACCESSSPECDECL_H

#include <ast/Node.h>

namespace HCXX {

class AccessSpecDecl : public HCXX::Node {
public:
    DECLARE_ENUM(AccessKind, PUBLIC, PROTECTED, PRIVATE, NONE);

    explicit AccessSpecDecl(AccessKind accessKind) : myAccessKind(accessKind) {}

    DECLARE_KIND(Node, Node::Kind::ACCESS_SPEC_DECL);
    DECLARE_SERIALIZE(AccessSpecDecl, myAccessKind);

    AccessKind GetAccessKind() const
    {
        return myAccessKind;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const
    {
        if (attrName == "AccessKind") {
            return AccessKindToCStr(GetAccessKind());
        }

        return Node::GetAttribute(attrName);
    }

protected:
    AccessKind myAccessKind = AccessKind::NONE;
};

};  //  namespace HCXX

#endif  //  COODDY_ACCESSSPECDECL_H
