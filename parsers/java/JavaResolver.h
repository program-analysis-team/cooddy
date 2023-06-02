/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_JAVAPARSERCONTEXT_H
#define COODDY_JAVAPARSERCONTEXT_H

#include <DeclResolver.h>
#include <ast/CxxRecordDecl.h>
#include <ast/Node.h>

#include "../../include/Parser.h"
#include "ASTConverter.h"

using namespace std;

class JavaResolver {
public:
    explicit JavaResolver(JavaParser::PackageDeclarationContext* packageContext, HCXX::DeclResolver* declResolver)
    {
        myTopResolver = declResolver;
        HCXX::DeclResolver* packageResolver = nullptr;
        if (packageContext != nullptr) {
            packageResolver = declResolver->FindResolver(packageContext->qualifiedName()->getText());
        }
        if (packageResolver == nullptr) {
            packageResolver = myTopResolver;
        }
        if (packageResolver != nullptr) {
            myResolvers.emplace_back(packageResolver);
        }
    }
    ~JavaResolver() = default;

    void AddImportDecl(std::string_view importPath)
    {
        auto resolver = myTopResolver != nullptr ? myTopResolver->FindResolver(importPath) : nullptr;
        if (resolver != nullptr) {
            myResolvers.emplace_back(resolver);
        }
    }

    void AddClassDecl(HCXX::CxxRecordDecl& classNode)
    {
        myClasses[classNode.GetName()] = &classNode;
    }

    HCXX::CxxRecordDecl* FindClassDecl(const string& className, bool waitForInit)
    {
        if (auto declNode = myClasses[className]; declNode) {
            return declNode;
        }
        for (auto& resolver : myResolvers) {
            auto classResolver = resolver->FindResolver(className, waitForInit);
            if (classResolver == nullptr) {
                continue;
            }
            auto declNode = classResolver->FindDecl("", waitForInit);
            if (declNode == nullptr) {
                declNode = classResolver->FindDecl(HCXX::StrUtils::Split(className, '.').back(), waitForInit);
            }
            if (declNode != nullptr) {
                return HCXX::Node::Cast<HCXX::CxxRecordDecl>(const_cast<HCXX::Node*>(declNode));
            }
        }
        return nullptr;
    }

private:
    HCXX::DeclResolver* myTopResolver = nullptr;
    std::vector<HCXX::DeclResolver*> myResolvers;
    std::unordered_map<std::string, HCXX::CxxRecordDecl*> myClasses;
};

#endif  // COODDY_JAVAPARSERCONTEXT_H
