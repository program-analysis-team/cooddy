/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_CALLEXPRESSION_H
#define COODDY_CALLEXPRESSION_H

#include <ast/FunctionDecl.h>
#include <ast/TypedNode.h>

#include <optional>

namespace HCXX {

class CallExpression : public TypedNode {
public:
    explicit CallExpression(const Node* callee, const Node* decl, std::vector<const Node*>&& arguments, Type returnType)
        : TypedNode(returnType), myCallee(callee), myDecl(decl), myArguments(std::move(arguments))
    {}

    DECLARE_KIND(TypedNode, Node::Kind::CALL_EXPRESSION);
    DECLARE_SERIALIZE(CallExpression, myStubDeclNode << myCallee << myDecl << myArguments);

    const Node* GetCallee() const
    {
        return myCallee;
    }

    const std::vector<const Node*>& GetArguments() const
    {
        return myArguments;
    }

    const FunctionDecl* GetFunction() const
    {
        return Cast<FunctionDecl>(myDecl);
    }

    const Node* GetDeclaration() const override
    {
        return &myStubDeclNode;
    }

    void Traverse(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myDecl, callback);
        Node::Traverse(callback);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myCallee, callback);
        for (auto& arg : myArguments) {
            CALL_CALLBACK(arg, callback);
        }
    }
    std::optional<uint32_t> FindArgument(const Node& arg) const
    {
        for (auto i = 0; i < myArguments.size(); i++) {
            if (&arg == myArguments[i]) {
                return i;
            }
        }
        return std::nullopt;
    }

    Type GetArgumentType(uint32_t argPos) const
    {
        auto function = GetFunction();
        if (function == nullptr) {
            // LCOV_EXCL_START
            return argPos < myArguments.size() ? myArguments[argPos]->GetType() : Type();
            // LCOV_EXCL_STOP
        }
        auto paramPos = argPos;
        if (GetKind() == Node::Kind::CXX_OPERATOR_CALL_EXPRESSION &&
            function->GetKind() == Node::Kind::CXX_METHOD_DECL) {
            paramPos--;
        }
        auto& params = function->GetParams();
        if ((function->IsVariadicTemplate() || paramPos >= params.size()) && paramPos < myArguments.size()) {
            return myArguments[paramPos]->GetType();
        }
        return paramPos < params.size() ? params[paramPos]->GetType() : Type();
    }

    void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags) const override
    {
        for (auto i = 0; i < myArguments.size(); ++i) {
            auto argType = GetArgumentType(i);
            uint32_t usage =
                argType.IsPointer() && !argType.IsPointedToConstant() ? UsageFlags::WRITE : UsageFlags::READ;
            myArguments[i]->TraverseDeclUsage(callback, usage);
        }
    }

    void Print(std::string& source, int entryOffset) const override
    {
        auto decl = Node::Cast<FunctionDecl>(myDecl);
        if (decl != nullptr) {
            std::string declName = decl->GetQualifiedName();
            auto splittedName = StrUtils::Split(declName, ':');
            declName = splittedName.back();
            SafePrint(source, entryOffset, declName + "(", GetRange(), GetPrintableNameOffset());
        }
        const Node* prevParam = nullptr;
        for (auto& param : myArguments) {
            if (prevParam != nullptr) {
                auto pr = prevParam->GetRange();
                SafePrint(source, entryOffset, ",", pr, pr.Size());
            }
            param->Print(source, entryOffset);
            prevParam = param;
        }
        SafePrint(source, entryOffset, ")", GetRange(), GetRange().Size() - 1);
    }
    virtual int GetPrintableNameOffset() const
    {
        return 0;
    }

private:
    TypedNode myStubDeclNode;
    const Node* myCallee = nullptr;
    const Node* myDecl = nullptr;
    std::vector<const Node*> myArguments;
};

};  // namespace HCXX

#endif  // COODDY_CALLEXPRESSION_H
