/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Checker.h>
#include <ast/CastExpression.h>

using namespace HCXX;

class IncompatiblePointerTypeChecker : public Checker {
public:
    void CheckNode(const Node& node, ProblemsHolder& holder) override
    {
        if (node.GetTranslationUnit()->IsMacroExpansionRange(node.GetRange())) {
            return;
        }
        auto castExpression = Node::Cast<CastExpression>(&node);
        if (castExpression == nullptr || !castExpression->GetType().IsPointer()) {
            return;
        }
        auto typedInnerNode = Node::Cast<TypedNode>(castExpression->GetCastedExpression());
        if (typedInnerNode == nullptr || !typedInnerNode->GetType().IsPointer()) {
            return;
        }
        auto declNode = typedInnerNode->GetDeclaration();
        if (declNode == nullptr || !declNode->GetType().IsTrivial()) {
            return;
        }
        if (IsIncompatiblePointerCast(castExpression->GetType(), typedInnerNode->GetType())) {
            holder.RegisterProblem(*this, *typedInnerNode, {{typedInnerNode->GetSourceText()}});
        } else if (IsWarningPointerCast(castExpression->GetType(), typedInnerNode->GetType())) {
            Problem problem;
            problem.kind = "CAST.TO.SMALLER.PTR";
            problem.replacements = {typedInnerNode->GetSourceText()};
            holder.RegisterProblem(*this, *typedInnerNode, std::move(problem));
        }
    }

private:
    uint64_t GetPointedFinalSize(Type type)
    {
        while (type.IsPointer()) {
            type = type.GetPointedType();
        }
        return type.GetSizeInBits();
    }
    bool IsWarningPointerCast(Type castExprType, Type innerExprType)
    {
        auto castExprTypeSize = GetPointedFinalSize(castExprType);
        auto innerExprTypeSize = GetPointedFinalSize(innerExprType);
        return castExprType != innerExprType && castExprTypeSize != 0 && innerExprTypeSize != 0 &&
               !castExprType.GetPointedType().IsUnsigned() && !innerExprType.GetPointedType().IsUnsigned() &&
               castExprType.IsPointedToConstant() == innerExprType.IsPointedToConstant() &&
               castExprTypeSize < innerExprTypeSize;
    }
    bool IsIncompatiblePointerCast(Type castExprType, Type innerExprType)
    {
        auto castExprTypeSize = GetPointedFinalSize(castExprType);
        auto innerExprTypeSize = GetPointedFinalSize(innerExprType);
        return castExprType != innerExprType && innerExprTypeSize != 0 && castExprTypeSize != 8 &&
               castExprType.IsPointedToConstant() == innerExprType.IsPointedToConstant() &&
               (castExprTypeSize > innerExprTypeSize ||
                castExprType.IsIntegralType() && innerExprType.IsFloatingType() ||
                innerExprType.IsIntegralType() && castExprType.IsFloatingType());
    }
};

std::unique_ptr<HCXX::Checker> CreateIncompatiblePointerTypeChecker()
{
    return std::make_unique<IncompatiblePointerTypeChecker>();
}
