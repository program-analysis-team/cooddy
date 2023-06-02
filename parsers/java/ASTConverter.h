/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_JAVA_PARSER_ASTCONVERTER_H_
#define COODDY_ANALYZER_SOURCE_JAVA_PARSER_ASTCONVERTER_H_

#include <TranslationUnit.h>

#include "JavaParser.h"
#include "ast/CxxRecordDecl.h"
#include "ast/LiteralExpression.h"

struct JavaResolver;

class ASTConverter {
public:
    ASTConverter(HCXX::TranslationUnit& unit, HCXX::CxxRecordDecl& mainClass,
                 JavaParser::CompilationUnitContext& context, JavaParser& parser, JavaResolver& resolver, bool isSystem)
        : myUnit(unit),
          myMainClass(mainClass),
          myContext(context),
          myRuleNames(parser.getRuleNames()),
          myResolver(resolver),
          myIsSystem(isSystem)
    {}

    enum class Mode { DEFAULT, CLASS_DECLS, CLASS_MEMBERS };

    enum class PrimitiveTypeKind { BOOLEAN, BYTE, CHAR, SHORT, INTEGER, LONG, FLOAT, DOUBLE };

    void Convert();

    bool IsSystem() const
    {
        return myIsSystem;
    }

    Mode GetMode() const
    {
        return myMode;
    }

    HCXX::CxxRecordDecl* CheckMainClassDecl(antlr4::ParserRuleContext& classDecl, const std::string& className);

    HCXX::CxxRecordDecl& CreateClassDecl(HCXX::TranslationUnit& unit);

    static HCXX::CxxRecordDecl& CreateMainClass(HCXX::TranslationUnit& unit);

    template <typename TNode, typename... Args>
    TNode* CreateNode(Args... args)
    {
        TNode* node = static_cast<TNode*>(myUnit.AllocateNode(sizeof(TNode)));
        new (node) TNode(std::forward<Args>(args)...);
        return node;
    }

    void AddNode(HCXX::Node& parent, HCXX::Node& node, antlr4::ParserRuleContext& context, bool isSystem = false);

    HCXX::Node* GetOrCreateNode(antlr4::ParserRuleContext* context);

    JavaResolver& GetResolver() const
    {
        return myResolver;
    }

    void AddGlobalUndefDecl(HCXX::Node& declNode)
    {
        myGlobalDecl[declNode.GetName()] = &declNode;
        AddNode(myUnit, declNode, myContext, true);
    }
    void AddLocalVarDecl(const std::string& name, const HCXX::Node* declNode)
    {
        myLocalDecl[name] = declNode;
    }
    const HCXX::Node* GetVarDeclNode(const std::string& declName) const;

    HCXX::CxxRecordDecl* GetCurrentClass() const
    {
        return myClassStack.empty() ? nullptr : myClassStack.back();
    }

    std::string GetFQNPrefix() const;

    std::pair<const HCXX::CxxRecordDecl*, HCXX::Node*> GetClassObjNode(JavaParser::ExpressionContext* objContext);

    HCXX::Type ConvertType(JavaParser::TypeTypeContext* context, bool isConstant, bool isCollection = false);

    HCXX::SourceRange GetSourceRange(const antlr4::Token& startToken, const antlr4::Token* endToken = nullptr);

    HCXX::Location GetTokenLocation(const antlr4::Token& token);

    std::string DumpTree(antlr4::ParserRuleContext& context) const;

    HCXX::Type ConvertPrimitiveType(PrimitiveTypeKind kind) const;

private:
    void ConvertNode(HCXX::Node& node, antlr4::ParserRuleContext& context);
    template <class ClassOrInterfaceContext>
    void ConvertClassDecl(HCXX::CxxRecordDecl& classDecl, ClassOrInterfaceContext& context);
    void ConvertFuncDecl(HCXX::FunctionDecl& funcDecl, antlr4::ParserRuleContext& context);

    void AddClassDecl(HCXX::CxxRecordDecl& classDecl, JavaParser::ClassDeclarationContext& context);
    void AddClassDecl(HCXX::CxxRecordDecl& classDecl, JavaParser::InterfaceDeclarationContext& context);
    void SetClassFieldsAndMethods(HCXX::CxxRecordDecl& classDecl, JavaParser::InterfaceDeclarationContext& context);
    void SetClassFieldsAndMethods(HCXX::CxxRecordDecl& classDecl, JavaParser::ClassDeclarationContext& context);

    bool myIsSystem = false;
    Mode myMode = Mode::DEFAULT;
    JavaResolver& myResolver;
    HCXX::TranslationUnit& myUnit;
    HCXX::CxxRecordDecl& myMainClass;
    JavaParser::CompilationUnitContext& myContext;
    std::unordered_map<void*, HCXX::Node*> myNodes;
    const std::vector<std::string>& myRuleNames;
    std::vector<HCXX::CxxRecordDecl*> myClassStack;
    std::unordered_map<std::string, const HCXX::Node*> myLocalDecl;
    std::unordered_map<std::string, const HCXX::Node*> myGlobalDecl;
};

#endif  // COODDY_ANALYZER_SOURCE_JAVA_PARSER_ASTCONVERTER_H_
