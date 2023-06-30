/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "ASTConverter.h"

#include <ast/ArraySubscriptExpression.h>
#include <ast/BinaryExpression.h>
#include <ast/BoolLiteralExpression.h>
#include <ast/BreakStatement.h>
#include <ast/CatchExpression.h>
#include <ast/CharLiteralExpression.h>
#include <ast/CompoundStatement.h>
#include <ast/ConditionalExpression.h>
#include <ast/ContinueStatement.h>
#include <ast/CxxCtorDecl.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxMethodDecl.h>
#include <ast/CxxNewExpression.h>
#include <ast/CxxRecordDecl.h>
#include <ast/DeclStatement.h>
#include <ast/FloatLiteralExpression.h>
#include <ast/IfStatement.h>
#include <ast/InitListExpression.h>
#include <ast/IntLiteralExpression.h>
#include <ast/LiteralExpression.h>
#include <ast/LoopStatement.h>
#include <ast/MemberExpression.h>
#include <ast/ParenExpression.h>
#include <ast/ReturnStatement.h>
#include <ast/StringLiteralExpression.h>
#include <ast/SwitchStatement.h>
#include <ast/TemporaryExpression.h>
#include <ast/ThisExpression.h>
#include <ast/ThrowExpression.h>
#include <ast/TryExpression.h>
#include <ast/UnaryExpression.h>
#include <ast/UndefinedNode.h>

#include <sstream>

#include "JavaResolver.h"

using namespace HCXX;

class ConverterBase {
public:
    virtual Node* Convert(ASTConverter* converter, void* context)
    {
        return nullptr;
    }
} g_defaultConverter;

std::vector<ConverterBase*> g_converters(static_cast<size_t>(JavaParser::RuleArguments) + 1, &g_defaultConverter);

// COODDY_SUPPRESS MacroLengthChecker
#define DECLARE_CONVERTER(TANTLR)                                                          \
    class TANTLR##Converter : public ConverterBase {                                       \
        static TANTLR##Converter myInstance;                                               \
        Node* Convert(ASTConverter* converter, void* context) override                     \
        {                                                                                  \
            return Convert(converter, static_cast<JavaParser::TANTLR##Context*>(context)); \
        }                                                                                  \
        TANTLR##Converter()                                                                \
        {                                                                                  \
            g_converters[JavaParser::Rule##TANTLR] = this;                                 \
        }                                                                                  \
        Node* Convert(ASTConverter* converter, JavaParser::TANTLR##Context* context);      \
    };                                                                                     \
    TANTLR##Converter TANTLR##Converter::myInstance;                                       \
    Node* TANTLR##Converter::Convert(ASTConverter* converter, JavaParser::TANTLR##Context* context)

Node* ASTConverter::GetOrCreateNode(antlr4::ParserRuleContext* context)
{
    if (context == nullptr) {
        return nullptr;
    }
    auto& node = myNodes[context];
    if (node == nullptr) {
        node = g_converters[context->getRuleIndex()]->Convert(this, context);
    }
    return node;
}

const HCXX::Node* ASTConverter::GetVarDeclNode(const std::string& declName) const
{
    if (auto decl = myLocalDecl.find(declName); decl != myLocalDecl.end()) {
        return decl->second;
    }
    for (auto it = myClassStack.rbegin(); it != myClassStack.rend(); ++it) {
        if (auto decl = (*it)->GetField(declName); decl != nullptr) {
            return decl;
        }
    }
    if (auto decl = myGlobalDecl.find(declName); decl != myGlobalDecl.end()) {
        return decl->second;
    }
    return nullptr;
}

HCXX::CxxRecordDecl* ASTConverter::CheckMainClassDecl(antlr4::ParserRuleContext& classDecl,
                                                      const std::string& className)
{
    if (myMode == Mode::DEFAULT || className != myMainClass.GetName()) {
        return nullptr;
    }
    myMainClass.SetRange(GetSourceRange(*classDecl.getStart(), classDecl.getStop()));
    myMainClass.Init();
    // COODDY_SUPPRESS ReturnMemberAddressChecker
    return &myMainClass;
}

CxxRecordDecl& ASTConverter::CreateMainClass(TranslationUnit& unit)
{
    std::filesystem::path unitPath(unit.GetMainFileName());
    std::string className = unitPath.stem().string();

    CxxRecordDecl* classNode = static_cast<CxxRecordDecl*>(unit.AllocateNode(sizeof(CxxRecordDecl)));
    RecordDecl recordNode(nullptr, std::string(className), {}, CxxRecordDecl::RecordType::CLASS, true, false, 0, {});
    new (classNode) CxxRecordDecl(std::move(recordNode), nullptr, {}, {});
    unit.AddNode(*classNode, unit, true, {});

    auto resolver = unit.GetResolver();
    if (resolver != nullptr) {
        resolver->AddDecl(className, classNode, false);
        resolver->SetInited();
    }
    return *classNode;
}

void ASTConverter::Convert()
{
    myMode = Mode::CLASS_DECLS;
    ConvertNode(myUnit, myContext);

    myMode = Mode::CLASS_MEMBERS;
    ConvertNode(myUnit, myContext);
    myNodes.clear();
    myGlobalDecl.clear();
    if (myUnit.GetResolver() != nullptr) {
        myUnit.GetResolver()->AddDecl(myMainClass.GetName(), &myMainClass, true);
    }
    if (!IsSystem()) {
        myMode = Mode::DEFAULT;
        ConvertNode(myUnit, myContext);
    }
}

void ASTConverter::ConvertNode(HCXX::Node& node, antlr4::ParserRuleContext& context)
{
    for (auto& it : context.children) {
        auto childContext = dynamic_cast<antlr4::ParserRuleContext*>(it);
        if (childContext == nullptr) {
            continue;
        }
        auto childNode = GetOrCreateNode(childContext);
        if (childNode == nullptr) {
            ConvertNode(node, *childContext);
            continue;
        }
        AddNode(node, *childNode, *childContext);

        if (auto classNode = Node::Cast<CxxRecordDecl>(childNode); classNode) {
            if (auto classDeclCtx = dynamic_cast<JavaParser::ClassDeclarationContext*>(childContext);
                classDeclCtx != nullptr) {
                ConvertClassDecl(*classNode, *classDeclCtx);
            } else if (auto interfaceDeclCtx = dynamic_cast<JavaParser::InterfaceDeclarationContext*>(childContext);
                       interfaceDeclCtx != nullptr) {
                ConvertClassDecl(*classNode, *interfaceDeclCtx);
            }
        } else if (auto funcDecl = Node::Cast<FunctionDecl>(childNode); funcDecl) {
            ConvertFuncDecl(*funcDecl, *childContext);
        } else {
            ConvertNode(*childNode, *childContext);
        }
    }
}

Location ASTConverter::GetTokenLocation(const antlr4::Token& token)
{
    auto& fileEntry = myUnit.GetFileEntries().begin()->second;
    auto offset = fileEntry.lineOffsets[token.getLine() - 1];
    return offset + token.getCharPositionInLine() + 1;
}

SourceRange ASTConverter::GetSourceRange(const antlr4::Token& startToken, const antlr4::Token* endToken)
{
    if (endToken == nullptr) {
        endToken = &startToken;
    }
    return SourceRange{GetTokenLocation(startToken),
                       GetTokenLocation(*endToken) + uint32_t(endToken->getText().size())};
}

void ASTConverter::AddNode(HCXX::Node& parent, HCXX::Node& node, antlr4::ParserRuleContext& context, bool isSystem)
{
    if (node.GetTranslationUnit() != nullptr) {
        return;  // already added
    }
    myUnit.AddNode(node, parent, isSystem | IsSystem(), GetSourceRange(*context.getStart(), context.getStop()));
}

Type ASTConverter::ConvertType(JavaParser::TypeTypeContext* context, bool isConstant, bool isCollection)
{
    Type type;
    if (context == nullptr) {
        return {};  // LCOV_EXCL_LINE
    }
    if (auto primitiveType = context->primitiveType(); primitiveType != nullptr) {
        if (primitiveType->BOOLEAN() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::BOOLEAN);
        } else if (primitiveType->BYTE() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::BYTE);
        } else if (primitiveType->CHAR() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::CHAR);
        } else if (primitiveType->SHORT() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::SHORT);
        } else if (primitiveType->INT() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::INTEGER);
        } else if (primitiveType->LONG() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::LONG);
        } else if (primitiveType->FLOAT() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::FLOAT);
        } else if (primitiveType->DOUBLE() != nullptr) {
            type = ConvertPrimitiveType(PrimitiveTypeKind::DOUBLE);
        }
    } else if (auto classOrInterface = context->classOrInterfaceType(); classOrInterface != nullptr) {
        type.myPointerFlags++;
        type.myIsDeclaration = 1;
        if (myMode != Mode::CLASS_DECLS) {
            JavaParser::ClassOrInterfaceTypeContext* classContext = context->classOrInterfaceType();
            std::string className = classContext == nullptr ? context->getText() : classContext->getText();
            type.myDeclNode = myResolver.FindClassDecl(className, false);
        }
    }
    if (isConstant) {
        type.myConstantFlags |= 1u;
    }
    if (isCollection || !context->LBRACK().empty()) {
        type.myIsArray = 1;
        type.myIsVariableArray = 1;
        type.myReferenceFlags |= 1u;
    }
    return type;
}

void ASTConverter::ConvertFuncDecl(HCXX::FunctionDecl& funcDecl, antlr4::ParserRuleContext& context)
{
    for (auto& param : funcDecl.GetParams()) {
        AddLocalVarDecl(param->GetName(), param);
        AddNode(funcDecl, const_cast<HCXX::ParamVarDecl&>(*param), context);
    }
    if (myMode == Mode::DEFAULT) {
        ConvertNode(funcDecl, context);
        myUnit.AddCfg(&funcDecl, std::make_shared<HCXX::Cfg>(funcDecl, std::vector<HCXX::CfgBlock>()));
    }
    myLocalDecl.clear();
}

// LCOV_EXCL_START
std::string ASTConverter::DumpTree(antlr4::ParserRuleContext& context) const
{
    std::stringstream ss;
    std::unordered_map<uint32_t, bool> scopes;
    std::function<void(antlr4::ParserRuleContext*, uint32_t, bool)> traverse = [&](antlr4::ParserRuleContext* node,
                                                                                   uint32_t offset, bool lastChildren) {
        if (offset > 0) {
            for (uint32_t i = 1; i < offset; ++i) {
                ss << (scopes[i] ? "| " : "  ");
            }
            ss << (lastChildren ? "`-" : "|-");
        }
        ss << myRuleNames[node->getRuleIndex()] << ": \"" << node->getText() << "\"\n";
        if (!lastChildren) {
            scopes[offset] = true;
        }
        auto children = node->children;
        std::vector<antlr4::ParserRuleContext*> childrenWithContext;
        // to correct pretty-dump it's needed to know count of ParserRuleContext children
        for (auto& i : children) {
            if (auto childRule = dynamic_cast<antlr4::ParserRuleContext*>(i); childRule != nullptr) {
                childrenWithContext.push_back(childRule);
            }
        }
        for (uint32_t i = 0; i < childrenWithContext.size(); ++i) {
            traverse(childrenWithContext[i], offset + 1, i == childrenWithContext.size() - 1);
        }
        scopes[offset] = false;
    };
    traverse(&context, 0, true);
    return ss.str();
}
// LCOV_EXCL_STOP

DECLARE_CONVERTER(FormalParameter)
{
    auto paramName = context->variableDeclaratorId()->identifier()->getText();
    bool isConstant = false;
    for (auto* mod : context->variableModifier()) {
        if (mod->FINAL() != nullptr) {
            isConstant = true;
        }
    }
    return converter->CreateNode<ParamVarDecl>(
        std::move(paramName), converter->ConvertType(context->typeType(), isConstant), context->typeType()->getText());
}

DECLARE_CONVERTER(LastFormalParameter)
{
    auto paramName = context->variableDeclaratorId()->identifier()->getText();
    bool isConstant = false;
    for (auto* mod : context->variableModifier()) {
        // LCOV_EXCL_START
        if (mod->FINAL() != nullptr) {
            isConstant = true;
        }
        // LCOV_EXCL_STOP
    }
    return converter->CreateNode<ParamVarDecl>(
        std::move(paramName), converter->ConvertType(context->typeType(), isConstant, context->ELLIPSIS() != nullptr),
        context->typeType()->getText());
}

DECLARE_CONVERTER(FieldDeclaration)
{
    auto decl = context->variableDeclarators()->variableDeclarator(0);
    auto fieldName = decl->variableDeclaratorId()->getText();
    auto initExpr = decl->variableInitializer();
    bool isConstant = false;
    FieldDecl::AccessKind accessKind = FieldDecl::AccessKind::NONE;
    if (auto classBodyDeclarationContext =
            dynamic_cast<JavaParser::ClassBodyDeclarationContext*>(context->parent->parent);
        classBodyDeclarationContext != nullptr) {
        for (auto modifier : classBodyDeclarationContext->modifier()) {
            auto classOrInterfaceModifier = modifier->classOrInterfaceModifier();
            if (classOrInterfaceModifier == nullptr) {
                // LCOV_EXCL_START
                continue;
                // LCOV_EXCL_STOP
            }
            if (classOrInterfaceModifier->FINAL()) {
                isConstant = true;
            }
            if (classOrInterfaceModifier->PRIVATE()) {
                accessKind = FieldDecl::AccessKind::PRIVATE;
            } else if (classOrInterfaceModifier->PUBLIC()) {
                accessKind = FieldDecl::AccessKind::PUBLIC;
            } else if (classOrInterfaceModifier->PROTECTED()) {
                accessKind = FieldDecl::AccessKind::PROTECTED;
            }
        }
    }

    Node* initNode = converter->GetOrCreateNode(initExpr);
    VarDecl::Attributes attributes;

    auto fieldDecl = converter->CreateNode<FieldDecl>(nullptr, std::move(fieldName),
                                                      converter->ConvertType(context->typeType(), isConstant), nullptr,
                                                      nullptr, initNode, accessKind, 0, 0, 0, attributes, nullptr);

    converter->AddLocalVarDecl(fieldName, fieldDecl);
    return fieldDecl;
}

void ASTConverter::AddClassDecl(CxxRecordDecl& classDecl, JavaParser::InterfaceDeclarationContext& context)
{
    for (auto bodyDecl : context.interfaceBody()->interfaceBodyDeclaration()) {
        auto memberContext = bodyDecl->interfaceMemberDeclaration();
        auto classContext = memberContext != nullptr ? memberContext->classDeclaration() : nullptr;
        auto classNode = Node::Cast<CxxRecordDecl>(GetOrCreateNode(classContext));
        if (classNode != nullptr && classContext != nullptr) {
            ConvertClassDecl(*classNode, *classContext);
        }
    }
    myResolver.AddClassDecl(classDecl);
}

void ASTConverter::AddClassDecl(CxxRecordDecl& classDecl, JavaParser::ClassDeclarationContext& context)
{
    for (auto bodyDecl : context.classBody()->classBodyDeclaration()) {
        auto memberContext = bodyDecl->memberDeclaration();
        auto classContext = memberContext != nullptr ? memberContext->classDeclaration() : nullptr;
        auto classNode = Node::Cast<CxxRecordDecl>(GetOrCreateNode(classContext));
        if (classNode != nullptr && classContext != nullptr) {
            ConvertClassDecl(*classNode, *classContext);
        }
    }
    myResolver.AddClassDecl(classDecl);
}

void ASTConverter::SetClassFieldsAndMethods(CxxRecordDecl& classDecl, JavaParser::InterfaceDeclarationContext& context)
{
    HCXX::RecordDecl::Fields fields;
    HCXX::CxxRecordDecl::Methods methods;
    for (auto bodyDecl : context.interfaceBody()->interfaceBodyDeclaration()) {
        if (auto memberDeclContext = bodyDecl->interfaceMemberDeclaration(); memberDeclContext != nullptr) {
            if (auto methodDecl = memberDeclContext->interfaceMethodDeclaration(); methodDecl != nullptr) {
                auto methodNode = Node::Cast<CxxMethodDecl>(GetOrCreateNode(methodDecl));
                if (methodNode != nullptr) {
                    methods.emplace_back(methodNode);
                }
            }
        }
    }
    classDecl.SetFields(std::move(fields));
    classDecl.SetMethods(std::move(methods));
}

void ASTConverter::SetClassFieldsAndMethods(CxxRecordDecl& classDecl, JavaParser::ClassDeclarationContext& context)
{
    HCXX::RecordDecl::Fields fields;
    HCXX::CxxRecordDecl::Methods methods;
    for (auto bodyDecl : context.classBody()->classBodyDeclaration()) {
        if (auto memberDeclContext = bodyDecl->memberDeclaration(); memberDeclContext != nullptr) {
            if (auto fieldDecl = memberDeclContext->fieldDeclaration(); fieldDecl != nullptr) {
                auto filedNode = Node::Cast<FieldDecl>(GetOrCreateNode(fieldDecl));
                if (filedNode != nullptr) {
                    filedNode->SetIndex(fields.size());
                    fields.emplace_back(filedNode);
                }
            } else if (auto methodDecl = memberDeclContext->methodDeclaration(); methodDecl != nullptr) {
                auto methodNode = Node::Cast<CxxMethodDecl>(GetOrCreateNode(methodDecl));
                if (methodNode != nullptr) {
                    methods.emplace_back(methodNode);
                }
            }
        }
    }
    classDecl.SetFields(std::move(fields));
    classDecl.SetMethods(std::move(methods));
}

template <class ClassOrInterfaceContext>
void ASTConverter::ConvertClassDecl(CxxRecordDecl& classDecl, ClassOrInterfaceContext& context)
{
    if (myMode == Mode::CLASS_DECLS) {
        AddClassDecl(classDecl, context);
        return;
    }
    myClassStack.emplace_back(&classDecl);
    SetClassFieldsAndMethods(classDecl, context);
    ConvertNode(classDecl, context);
    myClassStack.pop_back();
}

Node* ConvertClassDecl(ASTConverter& converter, antlr4::ParserRuleContext& context, std::string className,
                       JavaParser::TypeTypeContext* baseTypeCtx,
                       const std::vector<JavaParser::TypeListContext*>& typeList)
{
    if (auto mainClass = converter.CheckMainClassDecl(context, className); mainClass != nullptr) {
        return mainClass;
    }
    RecordDecl recordNode(converter.GetCurrentClass(), std::move(className), {}, HCXX::CxxRecordDecl::RecordType::CLASS,
                          true, false, 0, {});

    CxxRecordDecl::BaseRecords baseRecords;
    bool waitDecl = converter.GetMode() == ASTConverter::Mode::DEFAULT;
    if (baseTypeCtx != nullptr) {
        auto baseDecl = converter.GetResolver().FindClassDecl(baseTypeCtx->classOrInterfaceType()->getText(), waitDecl);
        if (auto baseRecord = Node::Cast<CxxRecordDecl>(baseDecl); baseRecord != nullptr) {
            baseRecords.push_back(baseRecord);
        }
    }
    if (!typeList.empty()) {
        for (auto baseTypeCtx : typeList[0]->typeType()) {
            auto baseDecl =
                converter.GetResolver().FindClassDecl(baseTypeCtx->classOrInterfaceType()->getText(), waitDecl);
            if (auto baseRecord = Node::Cast<CxxRecordDecl>(baseDecl); baseRecord != nullptr) {
                baseRecords.push_back(baseRecord);
            }
        }
    }

    return converter.CreateNode<CxxRecordDecl>(std::move(recordNode), nullptr, std::vector<const CxxCtorDecl*>(),
                                               std::move(baseRecords));
}

DECLARE_CONVERTER(InterfaceDeclaration)
{
    return ConvertClassDecl(*converter, *context, context->identifier()->getText(), nullptr, context->typeList());
}

DECLARE_CONVERTER(ClassDeclaration)
{
    return ConvertClassDecl(*converter, *context, context->identifier()->getText(), context->typeType(),
                            context->typeList());
}

DECLARE_CONVERTER(MethodBody)
{
    if (converter->GetMode() != ASTConverter::Mode::DEFAULT) {
        return nullptr;  // LCOV_EXCL_LINE: HCAT-2778
    }
    return converter->CreateNode<CompoundStatement>();
}

template <class T>
FunctionDecl ConvertFunctionDecl(T& context, ASTConverter& converter, antlr4::ParserRuleContext* bodyContext, Type type,
                                 std::string&& typeName, FunctionQualifiers qualifiers = {})
{
    std::vector<NodePtr<ParamVarDecl>> params;
    auto paramsContext = context.formalParameters();
    if (paramsContext != nullptr && paramsContext->formalParameterList() != nullptr) {
        params.reserve(paramsContext->formalParameterList()->formalParameter().size());
        for (auto paramContext : paramsContext->formalParameterList()->formalParameter()) {
            params.emplace_back(static_cast<ParamVarDecl*>(converter.GetOrCreateNode(paramContext)));
        }
    }

    std::string funcName = context.identifier()->getText();
    DeclarationNameInfo declInfo(funcName, SourceRange());

    Node* body = nullptr;
    if (converter.GetMode() != ASTConverter::Mode::CLASS_MEMBERS) {
        body = converter.GetOrCreateNode(bodyContext);
    }
    qualifiers[static_cast<int>(FunctionQualifiersIndex::DEFINED)] = !converter.IsSystem();

    std::string signature = converter.GetFQNPrefix() + funcName;
    std::string qualifiedName = signature;
    return FunctionDecl(std::move(declInfo), type, std::move(typeName), std::move(params), body,
                        FunctionDecl::FunctionKind::METHOD, qualifiers, std::move(signature), std::move(qualifiedName),
                        FunctionDecl::AccessKind::PUBLIC, nullptr, nullptr, {}, {});
}

template <class ClassBodyDeclContext, class MethodDeclContext>
Node* ConvertMethodDeclaration(ASTConverter& converter, ClassBodyDeclContext* classBodyDeclarationContext,
                               MethodDeclContext& methodDeclCtx)
{
    FunctionQualifiers qualifiers;
    qualifiers[static_cast<int>(FunctionQualifiersIndex::VIRTUAL)] = true;

    bool isConstant = false;
    if (classBodyDeclarationContext != nullptr) {
        for (auto modifier : classBodyDeclarationContext->modifier()) {
            auto classOrInterfaceModifier = modifier->classOrInterfaceModifier();
            if (classOrInterfaceModifier == nullptr) {
                continue;  // LCOV_EXCL_LINE
            }
            isConstant = classOrInterfaceModifier->FINAL() != nullptr;
            if (classOrInterfaceModifier->STATIC() != nullptr) {
                qualifiers[static_cast<int>(FunctionQualifiersIndex::VIRTUAL)] = false;
            }
        }
    }

    Type type;
    std::string typeName;
    if (methodDeclCtx.typeTypeOrVoid()->VOID() == nullptr) {
        type = converter.ConvertType(methodDeclCtx.typeTypeOrVoid()->typeType(), isConstant);
        typeName = methodDeclCtx.typeTypeOrVoid()->typeType()->getText();
    }

    auto funcDecl = ConvertFunctionDecl(methodDeclCtx, converter, methodDeclCtx.methodBody(), type, std::move(typeName),
                                        qualifiers);
    return converter.CreateNode<CxxMethodDecl>(std::move(funcDecl), converter.GetCurrentClass());
}

DECLARE_CONVERTER(InterfaceMethodDeclaration)
{
    return ConvertMethodDeclaration(*converter,
                                    dynamic_cast<JavaParser::InterfaceBodyDeclarationContext*>(context->parent->parent),
                                    *context->interfaceCommonBodyDeclaration());
}

DECLARE_CONVERTER(MethodDeclaration)
{
    return ConvertMethodDeclaration(
        *converter, dynamic_cast<JavaParser::ClassBodyDeclarationContext*>(context->parent->parent), *context);
}

Node* ConvertFloatLiteralExpression(ASTConverter& converter, JavaParser::FloatLiteralContext& context)
{
    auto literalStr = context.getText();
    auto suffix = literalStr.back();
    auto typeKind = ASTConverter::PrimitiveTypeKind::DOUBLE;
    if (suffix == 'f' || suffix == 'F') {
        typeKind = ASTConverter::PrimitiveTypeKind::FLOAT;
    }
    return converter.CreateNode<FloatLiteralExpression>(converter.ConvertPrimitiveType(typeKind),
                                                        std::stod(literalStr));
}

Node* ConvertIntLiteralExpression(ASTConverter& converter, JavaParser::IntegerLiteralContext& context)
{
    auto base = 10;
    auto literalStr = context.getText();
    auto typeKind = ASTConverter::PrimitiveTypeKind::INTEGER;
    if (context.DECIMAL_LITERAL() != nullptr) {
        if (literalStr.back() == 'L') {
            typeKind = ASTConverter::PrimitiveTypeKind::LONG;
        }
    } else if (context.OCT_LITERAL() != nullptr) {
        // prefix is "0", there is no need to cut it
        base = 8;
    } else if (context.HEX_LITERAL() != nullptr) {
        // prefix is "0x", there is no need to cut it
        base = 16;
    } else if (context.BINARY_LITERAL() != nullptr) {
        // cut "0b" prefix
        literalStr = literalStr.substr(2);
        base = 2;
    }
    return converter.CreateNode<IntLiteralExpression>(converter.ConvertPrimitiveType(typeKind),
                                                      std::stoull(literalStr, nullptr, base));
}

DECLARE_CONVERTER(Literal)
{
    if (auto nullContext = context->NULL_LITERAL(); nullContext != nullptr) {
        return converter->CreateNode<LiteralExpression>(Type());
    } else if (auto boolContext = context->BOOL_LITERAL(); boolContext != nullptr) {
        return converter->CreateNode<BoolLiteralExpression>(
            converter->ConvertPrimitiveType(ASTConverter::PrimitiveTypeKind::BOOLEAN),
            boolContext->getText() == "true");
    } else if (auto charContext = context->CHAR_LITERAL(); charContext != nullptr) {
        return converter->CreateNode<CharLiteralExpression>(
            converter->ConvertPrimitiveType(ASTConverter::PrimitiveTypeKind::CHAR), charContext->getText()[0],
            CharLiteralExpression::CharKind::UTF16);
    } else if (auto stringContext = context->STRING_LITERAL(); stringContext != nullptr) {
        return converter->CreateNode<StringLiteralExpression>(Type(), stringContext->getText());
    } else if (auto intContext = context->integerLiteral(); intContext != nullptr) {
        return ConvertIntLiteralExpression(*converter, *intContext);
    } else if (auto floatContext = context->floatLiteral(); floatContext != nullptr) {
        return ConvertFloatLiteralExpression(*converter, *floatContext);
    }
    // LCOV_EXCL_START
    return converter->CreateNode<LiteralExpression>(Type());
    // LCOV_EXCL_STOP
}

DECLARE_CONVERTER(LocalVariableDeclaration)
{
    vector<NodePtr<Node>> decls;
    for (auto variableDeclarator : context->variableDeclarators()->variableDeclarator()) {
        if (auto decl = converter->GetOrCreateNode(variableDeclarator); decl != nullptr) {
            decls.emplace_back(decl);
        }
    }
    return converter->CreateNode<DeclStatement>(std::move(decls));
}

DECLARE_CONVERTER(VariableDeclaratorId)
{
    return converter->CreateNode<DeclName>();
}

DECLARE_CONVERTER(VariableDeclarator)
{
    auto type = Type();
    VarDecl::VariableDeclKind varKind = VarDecl::VariableDeclKind::UNDEFINED;
    // COODDY_SUPPRESS UnusedVariablesChecker
    auto* parent = dynamic_cast<antlr4::ParserRuleContext*>(context->parent);
    while (parent != nullptr) {
        if (dynamic_cast<JavaParser::FieldDeclarationContext*>(parent) != nullptr) {
            return nullptr;
        }
        if (auto localVars = dynamic_cast<JavaParser::LocalVariableDeclarationContext*>(parent); localVars != nullptr) {
            varKind = VarDecl::VariableDeclKind::LOCAL_DECLARATION;
            bool isConstant = false;
            for (auto* mod : localVars->variableModifier()) {
                if (mod->FINAL() != nullptr) {
                    isConstant = true;
                }
            }
            type = converter->ConvertType(localVars->typeType(), isConstant);
        }
        if (parent->getRuleIndex() == JavaParser::RuleMethodDeclaration) {
            break;
        }
        parent = dynamic_cast<antlr4::ParserRuleContext*>(parent->parent);
    }
    auto varDeclId = context->variableDeclaratorId();
    auto declName = converter->GetOrCreateNode(varDeclId);
    auto varName = varDeclId->identifier()->getText();
    Node* initExpr = converter->GetOrCreateNode(context->variableInitializer());
    auto varNode =
        converter->CreateNode<VarDecl>(nullptr, varKind, std::string(varName), type, nullptr,
                                       static_cast<DeclName*>(declName), initExpr, VarDecl::Attributes(), nullptr);
    if (parent == nullptr) {
        return varNode;
    }
    converter->AddLocalVarDecl(varName, varNode);
    return varNode;
}

DECLARE_CONVERTER(ConstructorDeclaration)
{
    auto funcDecl = ConvertFunctionDecl<JavaParser::ConstructorDeclarationContext>(*context, *converter,
                                                                                   context->block(), Type(), "");
    CxxCtorDecl::Initializers initializers;
    return converter->CreateNode<CxxCtorDecl>(std::move(funcDecl), nullptr, std::move(initializers));
}

DECLARE_CONVERTER(VariableInitializer)
{
    if (context->arrayInitializer() != nullptr) {
        return converter->GetOrCreateNode(context->arrayInitializer());
    }
    return converter->GetOrCreateNode(context->expression());
}

#define CONVERT_BINARY(converter, expressions, operationContext, operation)                \
    if (operationContext != nullptr) {                                                     \
        auto lhs = converter.GetOrCreateNode(expressions[0]);                              \
        auto rhs = converter.GetOrCreateNode(expressions[1]);                              \
        SourceRange range = converter.GetSourceRange(*operationContext->getSymbol());      \
        return converter.CreateNode<BinaryExpression>(lhs, rhs, operation, Type(), range); \
    }

#define CONVERT_UNARY(converter, expressions, operationContext, operation)               \
    if (operationContext != nullptr) {                                                   \
        auto operand = converter.GetOrCreateNode(expressions[0]);                        \
        SourceRange range = converter.GetSourceRange(*operationContext->getSymbol());    \
        return converter.CreateNode<UnaryExpression>(operand, operation, Type(), range); \
    }

#define CONVERT_INC(converter, expressions, operationContext, preType, postType)                                    \
    if (operationContext != nullptr) {                                                                              \
        auto incType = operationContext->getSymbol()->getStartIndex() < expressions[0]->getStart()->getStartIndex() \
                           ? preType                                                                                \
                           : postType;                                                                              \
        CONVERT_UNARY(converter, expressions, operationContext, incType)                                            \
    }

Node* ConvertArraySubscriptExpression(ASTConverter& converter, JavaParser::ExpressionContext& context)
{
    if (context.LBRACK() == nullptr || context.RBRACK() == nullptr) {
        return nullptr;
    }
    auto expressions = context.expression();
    auto lhs = converter.GetOrCreateNode(expressions[0]);
    auto rhsPrimary = expressions[1]->primary();
    auto rhs = rhsPrimary != nullptr ? converter.GetOrCreateNode(rhsPrimary->literal()) : nullptr;
    return converter.CreateNode<ArraySubscriptExpression>(lhs, rhs, lhs, Type());
}

Node* ConvertUnaryExpr(ASTConverter& converter, JavaParser::ExpressionContext& context,
                       vector<JavaParser::ExpressionContext*>& expressions)
{
    CONVERT_UNARY(converter, expressions, context.BANG(), UnaryExpression::Operation::LNOT)
    CONVERT_UNARY(converter, expressions, context.TILDE(), UnaryExpression::Operation::NOT)
    CONVERT_UNARY(converter, expressions, context.ADD(), UnaryExpression::Operation::PLUS)
    CONVERT_UNARY(converter, expressions, context.SUB(), UnaryExpression::Operation::MINUS)
    CONVERT_INC(converter, expressions, context.INC(), UnaryExpression::Operation::PRE_INC,
                UnaryExpression::Operation::POST_INC)
    CONVERT_INC(converter, expressions, context.DEC(), UnaryExpression::Operation::PRE_DEC,
                UnaryExpression::Operation::POST_DEC)
    return converter.CreateNode<TemporaryExpression>(converter.GetOrCreateNode(context.primary()));
}

Node* ConvertBinaryExpr(ASTConverter& converter, JavaParser::ExpressionContext& context,
                        vector<JavaParser::ExpressionContext*>& expressions)
{
    if (auto arraySubscriptExpression = ConvertArraySubscriptExpression(converter, context);
        arraySubscriptExpression != nullptr) {
        return arraySubscriptExpression;
    }
    CONVERT_BINARY(converter, expressions, context.ASSIGN(), BinaryExpression::Operation::ASSIGN)
    CONVERT_BINARY(converter, expressions, context.EQUAL(), BinaryExpression::Operation::EQ)
    CONVERT_BINARY(converter, expressions, context.NOTEQUAL(), BinaryExpression::Operation::NE)
    CONVERT_BINARY(converter, expressions, context.GE(), BinaryExpression::Operation::GE)
    CONVERT_BINARY(converter, expressions, context.LE(), BinaryExpression::Operation::LE)
    CONVERT_BINARY(converter, expressions, context.GT(0),
                   context.GT().size() == 1 ? BinaryExpression::Operation::GT : BinaryExpression::Operation::SHR);
    CONVERT_BINARY(converter, expressions, context.LT(0),
                   context.LT().size() == 1 ? BinaryExpression::Operation::LT : BinaryExpression::Operation::SHL);
    CONVERT_BINARY(converter, expressions, context.AND(), BinaryExpression::Operation::L_AND)
    CONVERT_BINARY(converter, expressions, context.AND_ASSIGN(), BinaryExpression::Operation::AND_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.BITAND(0), BinaryExpression::Operation::AND)
    CONVERT_BINARY(converter, expressions, context.OR(), BinaryExpression::Operation::L_OR)
    CONVERT_BINARY(converter, expressions, context.OR_ASSIGN(), BinaryExpression::Operation::OR_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.BITOR(), BinaryExpression::Operation::OR)
    CONVERT_BINARY(converter, expressions, context.CARET(), BinaryExpression::Operation::XOR)
    CONVERT_BINARY(converter, expressions, context.XOR_ASSIGN(), BinaryExpression::Operation::XOR_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.LSHIFT_ASSIGN(), BinaryExpression::Operation::SHL_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.RSHIFT_ASSIGN(), BinaryExpression::Operation::SHR_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.DIV(), BinaryExpression::Operation::DIV)
    CONVERT_BINARY(converter, expressions, context.DIV_ASSIGN(), BinaryExpression::Operation::DIV_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.MUL(), BinaryExpression::Operation::MUL)
    CONVERT_BINARY(converter, expressions, context.MUL_ASSIGN(), BinaryExpression::Operation::MUL_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.ADD(), BinaryExpression::Operation::ADD)
    CONVERT_BINARY(converter, expressions, context.ADD_ASSIGN(), BinaryExpression::Operation::ADD_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.SUB(), BinaryExpression::Operation::SUB)
    CONVERT_BINARY(converter, expressions, context.SUB_ASSIGN(), BinaryExpression::Operation::SUB_ASSIGN)
    CONVERT_BINARY(converter, expressions, context.MOD(), BinaryExpression::Operation::REM)
    CONVERT_BINARY(converter, expressions, context.MOD_ASSIGN(), BinaryExpression::Operation::REM_ASSIGN)
    // LCOV_EXCL_START
    return converter.CreateNode<TemporaryExpression>(converter.GetOrCreateNode(context.primary()));
    // LCOV_EXCL_STOP
}

FunctionDecl* CreateEmptyFunctionDeclaration(ASTConverter& converter, const std::string& funcName)
{
    if (funcName.empty()) {
        return nullptr;  // LCOV_EXCL_LINE: HCAT-2779
    }
    // for undefined function add empty declaration to global scope to be able to get annotations from
    // annotations.json
    DeclarationNameInfo declInfo(funcName, SourceRange());
    std::vector<NodePtr<ParamVarDecl>> params;
    std::vector<SourceRange> commentBlocks;
    FunctionQualifiers functionQualifiers;
    auto decl = converter.CreateNode<FunctionDecl>(
        std::move(declInfo), Type(), "", std::move(params), nullptr, FunctionDecl::FunctionKind::METHOD,
        // COODDY_SUPPRESS RedundantValueCopyChecker
        functionQualifiers, funcName, funcName, FunctionDecl::AccessKind::PUBLIC, nullptr, nullptr,
        std::move(commentBlocks), std::vector<Type>());
    converter.AddGlobalUndefDecl(*decl);
    return decl;
}

FieldDecl* CreateEmptyFieldDeclaration(ASTConverter& converter, const std::string& fieldName)
{
    if (fieldName.empty()) {
        return nullptr;  // LCOV_EXCL_LINE: HCAT-2779
    }
    // for undefined variable add empty declaration to global scope to be able to get annotations from
    // annotations.json
    auto decl = converter.CreateNode<FieldDecl>(nullptr, fieldName, Type(), nullptr, nullptr, nullptr,
                                                FieldDecl::AccessKind::NONE, 0, 0, 0, FieldDecl::Attributes(), nullptr);
    converter.AddGlobalUndefDecl(*decl);
    return decl;
}

// remove this workaround after fixing HCAT-2168
Node* CreateParentImplicitCast(ASTConverter& converter, JavaParser::ExpressionContext& context, Node& node)
{
    auto implicitCast = converter.CreateNode<CastExpression>(&node, CastExpression::Kind::UNKNOWN, Type(),
                                                             CastExpression::TypeKind::IMPLICIT_CAST);
    converter.AddNode(*implicitCast, node, context);
    return implicitCast;
}

std::pair<const HCXX::CxxRecordDecl*, HCXX::Node*> ASTConverter::GetClassObjNode(
    JavaParser::ExpressionContext* objContext)
{
    if (objContext == nullptr || objContext->getText() == "this") {
        Type thisType;
        thisType.myIsDeclaration = 1;
        thisType.myPointerFlags = 1;
        thisType.myDeclNode = GetCurrentClass();
        return {GetCurrentClass(), CreateNode<ThisExpression>(thisType)};
    }
    auto objNode = GetOrCreateNode(objContext);
    if (objNode == nullptr || myMode != Mode::DEFAULT) {
        return {nullptr, objNode};  // LCOV_EXCL_LINE
    }
    auto objType = objNode->GetInnerNode()->GetType();
    auto classNode = Node::Cast<CxxRecordDecl>(objType.GetPointedDeclaration());
    std::string className = classNode != nullptr ? classNode->GetName() : objContext->getText();
    classNode = myResolver.FindClassDecl(className, true);
    return {classNode, objNode};
}

std::string ASTConverter::GetFQNPrefix() const
{
    std::string prefix;
    auto packageDecl = myContext.packageDeclaration();
    if (packageDecl != nullptr) {
        prefix = packageDecl->qualifiedName()->getText() + '.';
    }
    for (auto& classDecl : myClassStack) {
        prefix += classDecl->GetName() + '.';
    }
    return prefix;
}

MemberExpression* ConvertMemberExpression(ASTConverter& converter, JavaParser::ExpressionContext& context)
{
    auto [classNode, baseObj] = converter.GetClassObjNode(context.expression(0));

    std::string fieldName = context.identifier()->getText();
    auto fieldDecl = classNode != nullptr ? classNode->GetField(fieldName) : nullptr;
    if (fieldDecl == nullptr) {
        fieldDecl = CreateEmptyFieldDeclaration(converter, fieldName);
    }
    auto memberExpr = converter.CreateNode<MemberExpression>(Type(), "", baseObj, fieldDecl, true);
    if (baseObj != nullptr) {
        converter.AddNode(*memberExpr, *baseObj, *context.expression(0));
    }
    return memberExpr;
}

Node* ConvertMemberCallExpression(ASTConverter& converter, JavaParser::ExpressionContext& context)
{
    auto methodCallContext = context.methodCall();
    if (methodCallContext == nullptr || methodCallContext->identifier() == nullptr) {
        return nullptr;
    }
    auto objContext = context.expression(0);
    auto [classNode, callObj] = converter.GetClassObjNode(objContext);

    std::string funcName = methodCallContext->identifier()->getText();
    const FunctionDecl* funcDecl = classNode != nullptr ? classNode->GetMethod(funcName) : nullptr;
    if (funcDecl == nullptr) {
        funcDecl = CreateEmptyFunctionDeclaration(converter, funcName);
    }

    auto memberExpr = converter.CreateNode<MemberExpression>(Type(), "", callObj, funcDecl, true);
    if (callObj != nullptr) {
        antlr4::ParserRuleContext* nodeContext = objContext;
        converter.AddNode(*memberExpr, *callObj, nodeContext != nullptr ? *nodeContext : *methodCallContext);
    }

    std::vector<const Node*> args;
    auto expressionList = methodCallContext->expressionList();
    if (expressionList != nullptr) {
        args.reserve(expressionList->expression().size());
        for (auto expression : expressionList->expression()) {
            auto argNode = converter.GetOrCreateNode(expression);
            if (argNode == nullptr) {
                // LCOV_EXCL_START
                argNode = converter.CreateNode<TemporaryExpression>();
                converter.AddNode(*memberExpr, *argNode, *expression);
                // LCOV_EXCL_STOP
            }
            args.emplace_back(argNode);
        }
    }

    bool isVirtualCall = funcDecl != nullptr && funcDecl->IsVirtual();
    auto retType = funcDecl != nullptr ? funcDecl->GetRetType() : Type();
    CallExpression callExpr(memberExpr, memberExpr->GetValue(), std::move(args), retType);
    auto cxxMemberCallExpression =
        converter.CreateNode<CxxMemberCallExpression>(std::move(callExpr), memberExpr->GetBase(), isVirtualCall);
    converter.AddNode(*cxxMemberCallExpression, *memberExpr, context);
    return CreateParentImplicitCast(converter, context, *cxxMemberCallExpression);
}

Node* ConvertNewExpression(ASTConverter& converter, JavaParser::ExpressionContext& context)
{
    auto creatorCtx = context.creator();
    if (creatorCtx == nullptr) {
        return nullptr;  // LCOV_EXCL_LINE: HCAT-2779
    }
    Node* arraySize = nullptr;
    if (auto arrayCreatorRest = creatorCtx->arrayCreatorRest(); arrayCreatorRest != nullptr) {
        arraySize = converter.GetOrCreateNode(arrayCreatorRest->expression(0));
    }
    Node* callee = nullptr;
    Node* decl = nullptr;
    std::vector<const Node*> placementArgs;
    return converter.CreateNode<CxxNewExpression>(callee, std::move(placementArgs), decl, arraySize, Type());
}

DECLARE_CONVERTER(MethodCall)
{
    return nullptr;
}

DECLARE_CONVERTER(Expression)
{
    if (auto methodCall = ConvertMemberCallExpression(*converter, *context); methodCall != nullptr) {
        return methodCall;
    }
    auto expressions = context->expression();
    if (context->QUESTION() != nullptr && expressions.size() == 3) {
        return converter->CreateNode<ConditionalExpression>(converter->GetOrCreateNode(expressions[0]),
                                                            converter->GetOrCreateNode(expressions[1]),
                                                            converter->GetOrCreateNode(expressions[2]));
    }

    if (expressions.size() == 1) {
        if (auto primary = expressions[0]->primary(); primary != nullptr) {
            if (primary->identifier() != nullptr && context->identifier() != nullptr) {
                return ConvertMemberExpression(*converter, *context);
            }
        }
        return ConvertUnaryExpr(*converter, *context, expressions);
    } else if (expressions.size() == 2) {
        return ConvertBinaryExpr(*converter, *context, expressions);
    }

    if (context->NEW() != nullptr) {
        return ConvertNewExpression(*converter, *context);
    }

    return converter->CreateNode<TemporaryExpression>(converter->GetOrCreateNode(context->primary()));
}

DECLARE_CONVERTER(Primary)
{
    auto subContext = dynamic_cast<antlr4::ParserRuleContext*>(context->children.front());
    if (subContext == nullptr) {
        subContext = dynamic_cast<antlr4::ParserRuleContext*>(context->expression());
    }
    return converter->CreateNode<TemporaryExpression>(converter->GetOrCreateNode(subContext));
}

DECLARE_CONVERTER(Identifier)
{
    auto parentContext = context->parent;
    if (dynamic_cast<JavaParser::VariableDeclaratorIdContext*>(parentContext) != nullptr ||
        dynamic_cast<JavaParser::MethodCallContext*>(parentContext) != nullptr ||
        dynamic_cast<JavaParser::MethodDeclarationContext*>(parentContext) != nullptr ||
        dynamic_cast<JavaParser::ClassDeclarationContext*>(parentContext) != nullptr) {
        return nullptr;
    }
    auto varDecl = converter->GetVarDeclNode(context->getText());
    if (varDecl == nullptr) {
        return nullptr;
    }
    return converter->CreateNode<RefExpression>(varDecl, varDecl->GetType());
}

Node* ConvertLoop(ASTConverter& converter, JavaParser::StatementContext& context)
{
    if (context.FOR() != nullptr) {
        auto forControl = context.forControl();
        auto forInit = forControl->forInit();
        auto loopKind = forControl->enhancedForControl() != nullptr ? LoopStatement::LoopKind::RANGED_FOR
                                                                    : LoopStatement::LoopKind::FOR;
        Node* init = forInit != nullptr ? converter.GetOrCreateNode(forInit->localVariableDeclaration()) : nullptr;
        Node* condition = converter.GetOrCreateNode(forControl->expression());
        Node* increment = converter.GetOrCreateNode(forControl->expressionList());
        Node* body = !context.statement().empty() ? converter.GetOrCreateNode(context.statement(0)->block()) : nullptr;
        return converter.CreateNode<LoopStatement>(loopKind, init, condition, body, nullptr, increment);
    } else if (context.WHILE() != nullptr) {
        auto loopKind = context.DO() != nullptr ? LoopStatement::LoopKind::DO_WHILE : LoopStatement::LoopKind::WHILE;
        Node* condition = converter.GetOrCreateNode(context.parExpression());
        Node* body =
            !context.statement().empty() ? converter.GetOrCreateNode(context.statement((0))->block()) : nullptr;
        return converter.CreateNode<LoopStatement>(loopKind, nullptr, condition, body, nullptr, nullptr);
    }
    return nullptr;
}

DECLARE_CONVERTER(CatchClause)
{
    return converter->CreateNode<CatchExpression>();
}

Node* ConvertTry(ASTConverter& converter, JavaParser::StatementContext& context)
{
    // COODDY_SUPPRESS ContainerSpecificationChecker
    std::vector<const CatchExpression*> handlers;
    for (auto& handler : context.catchClause()) {
        handlers.push_back(static_cast<const CatchExpression*>(converter.GetOrCreateNode(handler)));
    }
    return converter.CreateNode<TryExpression>(converter.GetOrCreateNode(context.block()), std::move(handlers));
}

Node* ConvertSwitch(ASTConverter& converter, JavaParser::StatementContext& context)
{
    if (context.SWITCH() != nullptr) {
        Node* condition = converter.GetOrCreateNode(context.parExpression());
        auto body = converter.GetOrCreateNode(&context);
        auto labelGroups = context.switchBlockStatementGroup();
        SwitchStatement::CaseChildren caseChildren;
        for (auto labelGroup : labelGroups) {
            auto labels = labelGroup->switchLabel();
            for (auto label : labels) {
                caseChildren.emplace_back(static_cast<SwitchCaseStatement*>(converter.GetOrCreateNode(label)));
            }
        }
        auto switchStmt = converter.CreateNode<SwitchStatement>(condition, body, std::move(caseChildren));
        converter.AddNode(*switchStmt, *condition, context);
        return switchStmt;
    }
    return nullptr;
}

Node* ConvertIfStatement(ASTConverter& converter, JavaParser::StatementContext& context)
{
    if (context.IF() != nullptr) {
        auto condition = converter.GetOrCreateNode(context.parExpression());
        auto thenPart = converter.GetOrCreateNode(context.statement(0));
        auto elsePart = converter.GetOrCreateNode(context.statement(1));
        return converter.CreateNode<IfStatement>(condition, thenPart, nullptr, elsePart, nullptr, nullptr);
    }
    return nullptr;
}

DECLARE_CONVERTER(BlockStatement)
{
    auto statementCtx = context->statement();
    if (statementCtx == nullptr) {
        return nullptr;
    }
    if (auto switchStmt = ConvertSwitch(*converter, *statementCtx); switchStmt != nullptr) {
        return switchStmt;
    }

    return nullptr;
}

DECLARE_CONVERTER(Statement)
{
    if (auto ifStmt = ConvertIfStatement(*converter, *context); ifStmt != nullptr) {
        return ifStmt;
    } else if (context->RETURN() != nullptr) {
        auto exprs = context->expression();
        auto retValue = exprs.empty() ? nullptr : converter->GetOrCreateNode(exprs[0]);
        return converter->CreateNode<ReturnStatement>(retValue);
    } else if (context->BREAK() != nullptr) {
        return converter->CreateNode<BreakStatement>();
    } else if (context->CONTINUE() != nullptr) {
        return converter->CreateNode<ContinueStatement>();
    } else if (auto loop = ConvertLoop(*converter, *context)) {
        return loop;
    } else if (context->SWITCH() != nullptr) {
        return converter->CreateNode<CompoundStatement>();
    } else if (context->TRY() != nullptr) {
        return ConvertTry(*converter, *context);
    } else if (context->THROW() != nullptr) {
        return converter->CreateNode<ThrowExpression>();
    }
    return converter->CreateNode<CompoundStatement>();
}

DECLARE_CONVERTER(SwitchLabel)
{
    if (context->DEFAULT() != nullptr) {
        return converter->CreateNode<DefaultStatement>(nullptr);
    }
    return converter->CreateNode<CaseStatement>(nullptr, converter->GetOrCreateNode(context->expression()), nullptr);
}

DECLARE_CONVERTER(ExpressionList)
{
    return converter->CreateNode<CompoundStatement>();
}

DECLARE_CONVERTER(ParExpression)
{
    return converter->CreateNode<ParenExpression>(converter->GetOrCreateNode(context->expression()), Type());
}

DECLARE_CONVERTER(Block)
{
    return converter->CreateNode<CompoundStatement>();
}

DECLARE_CONVERTER(ArrayInitializer)
{
    auto contextInitList = context->variableInitializer();
    std::vector<const Node*> initList;
    initList.reserve(initList.size());
    for (auto init : contextInitList) {
        initList.push_back(converter->GetOrCreateNode(init));
    }
    return converter->CreateNode<InitListExpression>(std::move(initList), true);
}

DECLARE_CONVERTER(ImportDeclaration)
{
    if (converter->GetMode() == ASTConverter::Mode::CLASS_MEMBERS) {
        converter->GetResolver().AddImportDecl(context->qualifiedName()->getText());
    }
    return nullptr;
}

Type ASTConverter::ConvertPrimitiveType(PrimitiveTypeKind kind) const
{
    Type type;
    switch (kind) {
        case PrimitiveTypeKind::BOOLEAN: {
            type.mySizeInBits = 1;
            break;
        }
        case PrimitiveTypeKind::BYTE: {
            type.mySizeInBits = 8;
            type.myIsIntegerType = 1;
            break;
        }
        case PrimitiveTypeKind::CHAR: {
            type.mySizeInBits = 16;
            type.myIsIntegerType = 1;
            type.myIsUnsigned = 1;
            break;
        }
        case PrimitiveTypeKind::SHORT: {
            type.mySizeInBits = 16;
            type.myIsIntegerType = 1;
            break;
        }
        case PrimitiveTypeKind::INTEGER: {
            type.mySizeInBits = 32;
            type.myIsIntegerType = 1;
            break;
        }
        case PrimitiveTypeKind::LONG: {
            type.mySizeInBits = 64;
            type.myIsIntegerType = 1;
            break;
        }
        case PrimitiveTypeKind::FLOAT: {
            type.mySizeInBits = 32;
            type.myIsFloatingType = 1;
            break;
        }
        case PrimitiveTypeKind::DOUBLE: {
            type.mySizeInBits = 64;
            type.myIsFloatingType = 1;
            break;
        }
            // LCOV_EXCL_START: -Werror=switch-default
        default:
            assert(false);
            // LCOV_EXCL_STOP
    }
    return type;
}
