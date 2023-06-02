/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_AST_CASTEXPRESSION_H_
#define COODDY_ANALYZER_INCLUDE_AST_CASTEXPRESSION_H_
#include <ast/TypedNode.h>

namespace HCXX {
class CastExpression : public TypedNode {
public:
    DECLARE_ENUM(TypeKind, CSTYLE_CAST, STATIC_CAST, CONST_CAST, DYNAMIC_CAST, REINTERPRET_CAST, IMPLICIT_CAST);

    // To get description of each kind, see clang/AST/OperationKinds.def
    DECLARE_ENUM(Kind, DEPENDENT, BIT_CAST, LVALUE_BIT_CAST, LVALUE_TO_RVALUE_BIT_CAST, LVALUE_TO_RVALUE, NO_OP,
                 BASE_TO_DERIVED, DERIVED_TO_BASE, UNCHECKED_DERIVED_TO_BASE, DYNAMIC, TO_UNION, ARRAY_TO_POINTER_DECAY,
                 FUNCTION_TO_POINTER_DECAY, NULL_TO_POINTER, NULL_TO_MEMBER_POINTER, BASE_TO_DERIVED_MEMBER_POINTER,
                 DERIVED_TO_BASE_MEMBER_POINTER, MEMBER_POINTER_TO_BOOLEAN, REINTERPRET_MEMBER_POINTER,
                 USER_DEFINED_CONVERSION, CONSTRUCTOR_CONVERSION, INTEGRAL_TO_POINTER, POINTER_TO_INTEGRAL,
                 POINTER_TO_BOOLEAN, TO_VOID, MATRIX_CAST, VECTOR_SPLAT, INTEGRAL_CAST, INTEGRAL_TO_BOOLEAN,
                 INTEGRAL_TO_FLOATING, FIXED_POINT_CAST, FIXED_POINT_TO_INTEGRAL, INTEGRAL_TO_FIXED_POINT,
                 FIXED_POINT_TO_BOOLEAN, FLOATING_TO_INTEGRAL, FLOATING_TO_BOOLEAN, BOOLEAN_TO_SIGNED_INTEGRAL,
                 FLOATING_CAST, CPOINTER_TO_OBJC_POINTER_CAST, BLOCK_POINTER_TO_OBJC_POINTER_CAST,
                 ANY_POINTER_TO_BLOCK_POINTER_CAST, OBJC_OBJECT_LVALUE_CAST, FLOATING_REAL_TO_COMPLEX,
                 FLOATING_COMPLEX_TO_REAL, FLOATING_COMPLEX_TO_BOOLEAN, FLOATING_COMPLEX_CAST,
                 FLOATING_COMPLEX_TO_INTEGRAL_COMPLEX, INTEGRAL_REAL_TO_COMPLEX, INTEGRAL_COMPLEX_TO_REAL,
                 INTEGRAL_COMPLEX_TO_BOOLEAN, INTEGRAL_COMPLEX_CAST, INTEGRAL_COMPLEX_TO_FLOATING_COMPLEX,
                 ARC_PRODUCE_OBJECT, ARC_CONSUME_OBJECT, ARC_RECLAIM_RETURNED_OBJECT, ARC_EXTEND_BLOCK_OBJECT,
                 ATOMIC_TO_NON_ATOMIC, NON_ATOMIC_TO_ATOMIC, COPY_AND_AUTORELEASE_BLOCK_OBJECT, BUILTIN_FN_TO_FN_PTR,
                 ZERO_TO_OCL_OPAQUE_TYPE, ADDRESS_SPACE_CONVERSION, INT_TO_OCL_SAMPLER, UNKNOWN);

    CastExpression(Node* castedExpression, Kind kind, const Type& castType, TypeKind castTypeKind)
        : TypedNode(castType), myCastedExpression(castedExpression), myCastKind(kind), myCastTypeKind(castTypeKind)
    {}

    DECLARE_KIND(TypedNode, Node::Kind::CAST_EXPRESSION);
    DECLARE_SERIALIZE(CastExpression, myCastKind << myCastTypeKind << myCastedExpression);

    bool IsImplicit() const
    {
        return myCastTypeKind == TypeKind::IMPLICIT_CAST;
    }

    bool IsExplicit() const
    {
        return !IsImplicit();
    }

    const Node* GetCastedExpression() const
    {
        return myCastedExpression;
    }

    Kind GetCastKind() const
    {
        return myCastKind;
    }

    TypeKind GetCastTypeKind() const
    {
        return myCastTypeKind;
    }

    const Node* GetInnerNode() const override
    {
        return myCastedExpression != nullptr ? myCastedExpression->GetInnerNode() : this;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "CastTypeKind") {
            return TypeKindToCStr(GetCastTypeKind());
        }

        if (attrName == "CastKind") {
            return KindToCStr(GetCastKind());
        }

        return TypedNode::GetAttribute(attrName);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myCastedExpression, callback);
    }

    const Node* GetDeclaration() const override
    {
        return CALL_TYPED_NODE(myCastedExpression, GetDeclaration);
    }

    std::optional<VirtualOffset> GetOffsetInDeclaration() const override
    {
        return CALL_TYPED_NODE(myCastedExpression, GetOffsetInDeclaration);
    }

    bool IsConstExpr() const override
    {
        return CALL_TYPED_NODE(myCastedExpression, IsConstExpr);
    }

    int64_t GetSLimitedValue() const override
    {
        return CALL_TYPED_NODE(myCastedExpression, GetSLimitedValue);
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        if (myCastedExpression != nullptr) {
            myCastedExpression->TraverseDeclUsage(callback, usageFlags);
        }
    }

    bool IsEqualNode(const Node* comparedNode) const override
    {
        auto castExpression = CompareAndCast<CastExpression>(comparedNode);
        return castExpression != nullptr && GetCastTypeKind() == castExpression->GetCastTypeKind();
    }

private:
    Kind myCastKind = Kind::UNKNOWN;
    TypeKind myCastTypeKind = TypeKind::CSTYLE_CAST;
    const Node* myCastedExpression = nullptr;
};
}  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_AST_CASTEXPRESSION_H_
