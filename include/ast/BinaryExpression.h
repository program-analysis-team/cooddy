/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
#ifndef COODDY_BINARYEXPRESSION_H
#define COODDY_BINARYEXPRESSION_H

#include <ast/Type.h>
#include <ast/TypedNode.h>

namespace HCXX {

class BinaryExpression : public TypedNode {
public:
    DECLARE_ENUM(Operation, PTR_MEM_D, PTR_MEM_I, MUL, DIV, REM, ADD, SUB, SHL, SHR, CMP, LT, GT, LE, GE, EQ, NE, AND,
                 XOR, OR, L_AND, L_OR, ASSIGN, MUL_ASSIGN, DIV_ASSIGN, REM_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, SHL_ASSIGN,
                 SHR_ASSIGN, AND_ASSIGN, XOR_ASSIGN, OR_ASSIGN, COMMA, UNKNOWN);

    BinaryExpression(Node* lhv, Node* rhv, Operation operation, const Type& type, SourceRange operatorRange)
        : TypedNode(type), myLHV(lhv), myRHV(rhv), myOperation(operation), myOperatorRange(operatorRange)
    {}

    DECLARE_KIND(TypedNode, Node::Kind::BINARY_EXPRESSION);
    DECLARE_SERIALIZE(BinaryExpression, myLHV << myRHV << myOperation);

    const Node* GetLeftOperand() const
    {
        return myLHV;
    };

    virtual const Node* GetRightOperand() const
    {
        return myRHV;
    };

    virtual Operation GetOperation() const
    {
        return myOperation;
    };

    bool IsArithmeticOp() const
    {
        return myOperation >= Operation::MUL && myOperation <= Operation::SUB;
    }

    bool IsCompareOp() const
    {
        return myOperation >= Operation::CMP && myOperation <= Operation::NE;
    }

    bool IsBitwiseOp() const
    {
        return myOperation == Operation::SHR || myOperation == Operation::SHL || myOperation == Operation::AND ||
               myOperation == Operation::OR || myOperation == Operation::XOR;
    }

    bool IsArithmeticAssignOp() const
    {
        return myOperation >= Operation::MUL_ASSIGN && myOperation <= Operation::SUB_ASSIGN;
    };

    bool IsLogicalAssignOp() const
    {
        return myOperation >= Operation::SHL_ASSIGN && myOperation <= Operation::OR_ASSIGN;
    };

    bool IsAssignOp() const
    {
        return myOperation == Operation::ASSIGN || IsArithmeticAssignOp() || IsLogicalAssignOp();
    };

    bool IsLogicalAndOrOp() const
    {
        return myOperation == Operation::L_AND || myOperation == Operation::L_OR;
    };

    /**
     * @brief SourceRange for the binary operator without the operands.
     *
     * Warning: Due to a bug in clang, might include additional tokens inserted between operator and operands, such as
     * block-style comments.
     */
    SourceRange GetOperatorRange() const
    {
        return myOperatorRange;
    }

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Operation") {
            return OperationToCStr(GetOperation());
        }

        return TypedNode::GetAttribute(attrName);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        auto expr1 = myLHV;
        auto expr2 = myRHV;

        if (myOperation == Operation::COMMA) {
            std::swap(expr1, expr2);
        }

        CALL_CALLBACK(expr1, callback);
        CALL_CALLBACK(expr2, callback);
    }

    bool IsConstExpr() const override
    {
        return CALL_TYPED_NODE(myLHV, IsConstExpr) && CALL_TYPED_NODE(myRHV, IsConstExpr);
    }

    int64_t GetSLimitedValue() const override
    {
        auto op1 = CALL_TYPED_NODE(myLHV, GetSLimitedValue);
        auto op2 = CALL_TYPED_NODE(myRHV, GetSLimitedValue);
        switch (myOperation) {
            case Operation::MUL:
                return op1 * op2;
            case Operation::DIV:
                if (op2 == 0 || op1 == INT64_MIN && op2 == -1) {
                    return UNDEF_VALUE;  // LCOV_EXCL_LINE: HCAT-2777
                }
                return op1 / op2;
            case Operation::REM:
                if (op2 == 0 || op1 == INT64_MIN && op2 == -1) {
                    return UNDEF_VALUE;  // LCOV_EXCL_LINE: HCAT-2777
                }
                return op1 % op2;
            case Operation::ADD:
                return op1 + op2;
            case Operation::SUB:
                return op1 - op2;
            case Operation::AND:
                return uint64_t(op1) & uint64_t(op2);
            case Operation::OR:
                return uint64_t(op1) | uint64_t(op2);
            case Operation::XOR:
                return uint64_t(op1) ^ uint64_t(op2);
            case Operation::SHL:
                return uint64_t(op1) << uint64_t(op2);
            case Operation::SHR:
                return uint64_t(op1) >> uint64_t(op2);
                // LCOV_EXCL_START: HCAT-2777
            default:
                return UNDEF_VALUE;
                // LCOV_EXCL_STOP
        }
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        if (myLHV != nullptr) {
            uint32_t usage = IsAssignOp() ? UsageFlags::WRITE : UsageFlags::READ;
            if (myOperation == Operation::ADD_ASSIGN) {
                usage |= UsageFlags::INCREMENT;
            }
            if (myOperation == Operation::SUB_ASSIGN) {
                usage |= UsageFlags::DECREMENT;
            }
            myLHV->TraverseDeclUsage(callback, usage);
        }
        if (myRHV != nullptr) {
            myRHV->TraverseDeclUsage(callback);
        }
    }

    bool IsEqualNode(const Node* comparedNode) const override
    {
        const auto* binaryExpression = CompareAndCast<BinaryExpression>(comparedNode);
        return binaryExpression != nullptr && GetOperation() == binaryExpression->GetOperation();
    }
    // LCOV_EXCL_START
    std::string Dump() const override
    {
        return Base::Dump() + " op: " + OperationToCStr(GetOperation());
    }
    // LCOV_EXCL_STOP
    void Print(std::string& source, int entryOffset) const override
    {
        std::string names[] = {
            // PTR_MEM_D, PTR_MEM_I, MUL, DIV, REM, ADD, SUB, SHL, SHR, CMP, LT, GT, LE, GE, EQ, NE, AND,
            "", "", "*", "/", "%", "+", "-", "<<", ">>", "", "<", ">", "<=", ">=", "==", "!=", "&",
            //    XOR, OR, L_AND, L_OR, ASSIGN, MUL_ASSIGN, DIV_ASSIGN, REM_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, SHL_ASSIGN,
            "^", "|", "&&", "||", "=", "*=", "/=", "%=", "+=", "-=", "<<=",
            //    SHR_ASSIGN, AND_ASSIGN, XOR_ASSIGN, OR_ASSIGN, COMMA, UNKNOWN
            ">>=", "&=", "^=", "|=", ",", ""};
        std::string name = names[static_cast<int>(GetOperation())];
        if (myLHV != nullptr && myRHV != nullptr) {
            auto offset = (myRHV->GetRange().begin - myLHV->GetRange().end - name.size()) / 2;
            SafePrint(source, entryOffset, name, myLHV->GetRange(), myLHV->GetRange().Size() + offset);
        }
        Base::Print(source, entryOffset);
    }

private:
    const Node* myLHV = nullptr;
    const Node* myRHV = nullptr;
    Operation myOperation = Operation::UNKNOWN;
    SourceRange myOperatorRange;
};

}  // namespace HCXX

#endif  // COODDY_BINARYEXPRESSION_H
