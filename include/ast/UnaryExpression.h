/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_UNARYEXPRESSION_H
#define COODDY_UNARYEXPRESSION_H

#include <ast/CastExpression.h>
#include <ast/Node.h>
#include <ast/TypedNode.h>

namespace HCXX {

class UnaryExpression : public TypedNode {
public:
    DECLARE_ENUM(Operation, POST_INC, POST_DEC, PRE_INC, PRE_DEC, ADDR_OF, DEREF, PLUS, MINUS, NOT, LNOT, REAL, IMAG,
                 EXTENSION, COAWAIT, SIZEOF, ALIGN_OF, VEC_STEP, OPENMP_REQUIRED_SIMD_ALIGN, PREFFERED_ALIGN_OF,
                 UNKNOWN);

    UnaryExpression(const Node* operand, Operation operation, const Type& type, SourceRange operatorRange)
        : TypedNode(type), myOperand(operand), myOperation(operation), myOperatorRange(operatorRange)
    {}

    DECLARE_KIND(TypedNode, Node::Kind::UNARY_EXPRESSION);
    DECLARE_SERIALIZE(UnaryExpression, myOperand << myOperation);

    const Node* GetOperand() const
    {
        return myOperand;
    }

    Operation GetOperation() const
    {
        return myOperation;
    }

    /**
     * @brief SourceRange for the unary operator without the operands.
     *
     * Warning: Due to a bug in clang, might include additional tokens inserted between operator and operand, such as
     * block-style comments.
     *
     * Warning: Invalid when unary operator is a type inspection keyword such as sizeof.
     */
    SourceRange GetOperatorRange() const
    {
        return myOperatorRange;
    }

    bool IsArithmetic() const
    {
        return IsUnaryArithmetic() || IsPrefix() || IsPostfix();
    }

    bool IsUnaryArithmetic() const
    {
        return myOperation == Operation::PLUS || myOperation == Operation::MINUS;
    }

    bool IsUnaryLogic() const
    {
        return myOperation == Operation::NOT || myOperation == Operation::LNOT;
    }

    bool IsPrefix() const
    {
        return myOperation == Operation::PRE_INC || myOperation == Operation::PRE_DEC;
    }

    bool IsPostfix() const
    {
        return myOperation == Operation::POST_INC || myOperation == Operation::POST_DEC;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Operation") {
            return OperationToCStr(GetOperation());
        }

        return Node::GetAttribute(attrName);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myOperand, callback);
    }

    const Node* GetDeclaration() const override
    {
        if (myOperation == Operation::DEREF || myOperation == Operation::ADDR_OF) {
            auto baseNode = Node::Cast<TypedNode>(myOperand);
            return baseNode != nullptr ? baseNode->GetDeclaration() : nullptr;
        }
        return nullptr;
    }

    std::optional<VirtualOffset> GetOffsetInDeclaration() const override
    {
        auto baseNode = Node::Cast<TypedNode>(myOperand);
        if (baseNode == nullptr || myOperation != Operation::DEREF && myOperation != Operation::ADDR_OF) {
            return TypedNode::GetOffsetInDeclaration();
        }
        VirtualOffset result;
        if (myOperation == Operation::DEREF) {
            result += VirtualOffset(VirtualOffset::Kind::DEREF);
        } else {
            result += VirtualOffset(VirtualOffset::Kind::ADDR_OF);
        }
        auto baseOffset = baseNode->GetOffsetInDeclaration();
        return baseOffset ? *baseOffset + result : result;
    }

    bool IsConstExpr() const override
    {
        if (myOperation == Operation::SIZEOF) {
            return true;
        }
        return CALL_TYPED_NODE(myOperand, IsConstExpr);
    }

    int64_t GetSLimitedValue() const override
    {
        auto result = CALL_TYPED_NODE(myOperand, GetSLimitedValue);
        switch (myOperation) {
            // LCOV_EXCL_START: HCAT-2777
            case Operation::PLUS:
                return result;
            // LCOV_EXCL_STOP
            case Operation::MINUS:
                return -result;
            case Operation::NOT:
                return ~uint64_t(result);
            case Operation::SIZEOF:
                return myOperand != nullptr ? myOperand->GetSizeOfType() : GetSizeOfType();
            default:
                return 0;
        }
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        if (myOperand != nullptr) {
            uint32_t usage = (myOperation == Operation::ADDR_OF ? usageFlags : 0);
            if (myOperation == Operation::POST_INC || myOperation == Operation::PRE_INC) {
                usage |= UsageFlags::WRITE;
                usage |= UsageFlags::INCREMENT;
            }
            if (myOperation == Operation::POST_DEC || myOperation == Operation::PRE_DEC) {
                usage |= UsageFlags::WRITE;
                usage |= UsageFlags::DECREMENT;
            }
            myOperand->TraverseDeclUsage(callback, usage);
        }
    }

    bool IsEqualNode(const Node* comparedNode) const override
    {
        auto unaryExpression = CompareAndCast<UnaryExpression>(comparedNode);
        return unaryExpression != nullptr && myOperation == unaryExpression->GetOperation();
    }
    void Print(std::string& source, int entryOffset) const override
    {
        std::string names[] = {
            // POST_INC, POST_DEC, PRE_INC, PRE_DEC, ADDR_OF, DEREF, PLUS, MINUS, NOT, LNOT, REAL, IMAG,
            "++", "--", "++", "--", "&", "*", "+", "-", "~", "!", "R", "I",
            // EXTENSION, COAWAIT, SIZEOF, ALIGN_OF, VEC_STEP, OPENMP_REQUIRED_SIMD_ALIGN, PREFFERED_ALIGN_OF,
            "", "", "sizeof", "", "", "", "",
            // UNKNOWN
            ""};
        auto r = GetRange();
        switch (GetOperation()) {
            case Operation::POST_INC:
            case Operation::POST_DEC:
                SafePrint(source, entryOffset, names[static_cast<int>(GetOperation())], r, r.Size() - 2);
                break;
            default:
                SafePrint(source, entryOffset, names[static_cast<int>(GetOperation())], r);
                break;
        }
        Base::Print(source, entryOffset);
    }

private:
    const Node* myOperand = nullptr;
    Operation myOperation = Operation::UNKNOWN;
    SourceRange myOperatorRange;
};

};  // namespace HCXX

#endif  // COODDY_UNARYEXPRESSION_H
