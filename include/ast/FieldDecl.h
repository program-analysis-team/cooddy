/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Class or Union member
//

#ifndef COODDY_FIELDDECL_H
#define COODDY_FIELDDECL_H

#include <ast/VarDecl.h>

#include <utility>

namespace HCXX {

using RecordUniqueId = uint32_t;

class FieldDecl : public VarDecl {
public:
    DECLARE_KIND(VarDecl, Node::Kind::FIELD_DECL);
    DECLARE_ENUM(AccessKind, PUBLIC, PROTECTED, PRIVATE, NONE);
    DECLARE_SERIALIZE(FieldDecl, myAccessKind << myIndex << myOffset << myRecordUniqueId << myAlign);

    FieldDecl(Node* qualifier, std::string&& name, const Type& type, const DeclQualifier* declQualifier,
              const DeclName* declName, const Node* initializer, AccessKind accessKind, uint32_t index, uint32_t offset,
              uint32_t align, Attributes attributes, const Node* arraySizeExpr)
        : VarDecl(qualifier, VariableDeclKind::CXX_CLASS_MEMBER, std::move(name), type, declQualifier, declName,
                  initializer, attributes, arraySizeExpr),
          myAccessKind(accessKind),
          myIndex(index),
          myOffset(offset),
          myRecordUniqueId(0),
          myAlign(align)
    {}

    bool IsPublic() const
    {
        return myAccessKind == AccessKind::PUBLIC;
    }

    bool IsAnonymous() const
    {
        return myAttributes[IS_ANONYMOUS];
    }

    AccessKind GetAccessKind() const
    {
        return myAccessKind;
    }

    std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "AccessKind") {
            return AccessKindToCStr(GetAccessKind());
        }

        return VarDecl::GetAttribute(attrName);
    }

    uint32_t GetIndex() const
    {
        return myIndex;
    }

    void SetIndex(uint32_t index)
    {
        myIndex = index;
    }

    uint32_t GetOffset() const
    {
        return myOffset;
    }

    uint32_t GetAlign() const
    {
        return myAlign;
    }

    // Returns cross unit unique identifier for the declaration which contains this field
    // LCOV_EXCL_START
    RecordUniqueId GetRecordUniqueId() const
    {
        return myRecordUniqueId;
    }
    // LCOV_EXCL_STOP

    void SetRecordUniqueId(RecordUniqueId recordUniqueId) const
    {
        myRecordUniqueId = recordUniqueId;
    }
    // LCOV_EXCL_START
    bool IsDeclaration() const override
    {
        return true;
    }
    // LCOV_EXCL_STOP

private:
    AccessKind myAccessKind = AccessKind::NONE;
    uint32_t myIndex = 0;
    uint32_t myOffset = 0;
    mutable RecordUniqueId myRecordUniqueId = 0;
    uint32_t myAlign = 0;
};

};  // namespace HCXX

#endif  // COODDY_FIELDDECL_H
