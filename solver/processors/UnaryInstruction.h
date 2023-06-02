/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_UNARYINSTRUCTION_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_UNARYINSTRUCTION_H_

#include <ast/UnaryExpression.h>

namespace Processor {

class UnaryInstruction : public InstructionProcessor {
    DECLARE_ENUM(ObjectType, UNKNOWN, ARRAY_LIKE);

    void Compile(const Node& node, CompileContext& context) override
    {
        auto expr = static_cast<const UnaryExpression*>(&node);
        auto operand = expr->GetOperand();
        context.AddEnum(expr->GetOperation());
        if (expr->GetOperation() == UnaryExpression::Operation::SIZEOF) {
            auto typedNode = operand != nullptr ? operand : expr;
            context.Add<uint32_t>(node.GetTranslationUnit()->GetSizeOfType(typedNode->GetType()));
        } else {
            context.Compile(operand);

            if (expr->GetOperation() == UnaryExpression::Operation::ADDR_OF) {
                if (uint32_t arrayElemSize = GetArrayElementSize(expr->GetDeclaration()); arrayElemSize != 0) {
                    context.AddEnum<ObjectType>(ObjectType::ARRAY_LIKE);
                    context.Add<uint32_t>(arrayElemSize);
                } else {
                    context.AddEnum<ObjectType>(ObjectType::UNKNOWN);
                }
            }
        }
    }

    uint32_t GetArrayElementSize(const Node* decl)
    {
        if (auto* varDecl = Node::Cast<VarDecl>(decl); varDecl != nullptr && varDecl->GetType().IsArray()) {
            auto arraySize = varDecl->GetNumOfElementsInArray();
            return arraySize ? (decl->GetType().GetSizeInBits() / 8) / arraySize : 0;
        }
        if (auto* paramDecl = Node::Cast<ParamVarDecl>(decl);
            paramDecl != nullptr && paramDecl->GetArraySizeExpr() != nullptr) {
            return paramDecl->GetType().GetPointedType().GetSizeInBits() / 8;
        }
        return 0;
    }

    z3::expr CreateNotExpr(ExecutionContext& context, const z3::expr& expr)
    {
        int32_t numVal = 0;
        if (expr.is_numeral_i(numVal)) {
            return context->CreateIntegerExpr(~uint32_t(numVal));
        }
        return -expr;  // LCOV_EXCL_LINE
    }

    z3::expr CreateMinusExpr(ExecutionContext& context, const z3::expr& expr)
    {
        int32_t numVal = 0;
        if (expr.is_numeral_i(numVal)) {
            return context->CreateIntegerExpr(-numVal);
        }
        return -expr;
    }

    z3::expr Execute(ExecutionContext& context, SymbolId& symbolId) override
    {
        auto op = context.GetEnum<UnaryExpression::Operation>();
        if (op == UnaryExpression::Operation::SIZEOF) {
            return context->CreateIntegerExpr(context.Get<uint32_t>() / 8);
        }
        z3::expr operand = context.Execute(&symbolId);
        switch (op) {
            case UnaryExpression::Operation::DEREF:
                // Convert expressions "*(arr + (N * S))" to "arr[N]"
                // Where N is number of elements, S is the size of one element
                if (auto addrSymbolId = context->ConvertPointerOffsetIntoIndex(operand); addrSymbolId) {
                    symbolId = *addrSymbolId;
                    return context->GetSymbol(symbolId);
                }
                return context->GetSubSymbol(symbolId, VirtualOffset(VirtualOffset::Kind::DEREF));
            case UnaryExpression::Operation::ADDR_OF: {
                auto objectType = context.GetEnum<ObjectType>();

                if (objectType == ObjectType::ARRAY_LIKE) {
                    auto possibleResult = context->ConvertIndexIntoPointerOffset(symbolId, context.Get<uint32_t>());
                    if (possibleResult.has_value()) {
                        return *possibleResult;
                    }
                }
                return context->GetSubSymbol(symbolId, VirtualOffset(VirtualOffset::Kind::ADDR_OF));
            }
            case UnaryExpression::Operation::MINUS:
                return CreateMinusExpr(context, context->CastToBV(operand));
            case UnaryExpression::Operation::PLUS:
                return context->CastToBV(operand);
            case UnaryExpression::Operation::NOT:
                return CreateNotExpr(context, context->CastToBV(operand));
            case UnaryExpression::Operation::LNOT:
                return !context->CastToBool(operand);
            case UnaryExpression::Operation::PRE_INC:
                return context->SetSymbol(symbolId, context->CastToBV(operand) + context->CreateIntegerExpr(1));
            case UnaryExpression::Operation::POST_INC:
                context.postIncs.emplace_back(std::make_pair(symbolId, 1));
                return context->CastToBV(operand);
            case UnaryExpression::Operation::PRE_DEC:
                return context->SetSymbol(symbolId, context->CastToBV(operand) - context->CreateIntegerExpr(1));
            case UnaryExpression::Operation::POST_DEC:
                context.postIncs.emplace_back(std::make_pair(symbolId, -1));
                return context->CastToBV(operand);
                // LCOV_EXCL_START
            default:
                return context->CreateSymbolExpr(symbolId);
                // LCOV_EXCL_STOP
        }
    }
};

};  // namespace Processor

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_PROCESSORS_UNARYINSTRUCTION_H_
