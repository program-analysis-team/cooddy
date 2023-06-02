/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_CFG_CFGELEMENT_H_
#define COODDY_ANALYZER_INCLUDE_CFG_CFGELEMENT_H_

#include <ast/Node.h>
#include <utils/EnumMapping.h>

namespace HCXX {

class CfgElement {
public:
    DECLARE_ENUM(Kind, INITIALIZER, SCOPE_BEGIN, SCOPE_END, NEW_ALLOCATOR, LIFE_TIME_ENDS, LOOP_EXIT, STATEMENT,
                 CONSTRUCTOR, CXXRECORD_TYPED_CALL, AUTOMATIC_OBJECT_DTOR, DELETE_DTOR, BASE_DTOR, MEMBER_DTOR,
                 TEMPORARY_DTOR);

    CfgElement(Kind kind = Kind::STATEMENT, const Node* node = nullptr) : myKind(kind), myNode(node) {}
    ~CfgElement() = default;

    Kind GetKind() const
    {
        return myKind;
    }

    const Node* GetNode() const
    {
        return myNode;
    }

    void Serialize(IOStream& stream)
    {
        stream << myKind << myNode;
    }

private:
    Kind myKind;
    NodePtr<Node> myNode;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_CFG_CFGELEMENT_H_
