/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Implementation of the AST visitor.
// Converts CLang AST-representation to Cooddy representation.
//

#include "ASTVisitor.h"

#include <CrossTUContext.h>
#include <ast/AccessSpecDecl.h>
#include <ast/ArraySubscriptExpression.h>
#include <ast/AsmStatement.h>
#include <ast/BinaryExpression.h>
#include <ast/BoolLiteralExpression.h>
#include <ast/BreakStatement.h>
#include <ast/CallExpression.h>
#include <ast/CastExpression.h>
#include <ast/CatchExpression.h>
#include <ast/CharLiteralExpression.h>
#include <ast/CompoundStatement.h>
#include <ast/ConditionalExpression.h>
#include <ast/ContinueStatement.h>
#include <ast/CxxConstructExpression.h>
#include <ast/CxxConversionDecl.h>
#include <ast/CxxCtorDecl.h>
#include <ast/CxxCtorInitializerDecl.h>
#include <ast/CxxDefaultArgExpression.h>
#include <ast/CxxDtorDecl.h>
#include <ast/CxxDtorExpression.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxMethodDecl.h>
#include <ast/CxxNewExpression.h>
#include <ast/CxxOperatorCallExpression.h>
#include <ast/CxxRecordDecl.h>
#include <ast/DeclStatement.h>
#include <ast/DeclarationNameInfo.h>
#include <ast/DeleteExpression.h>
#include <ast/EmptyStmt.h>
#include <ast/EnumConstantDecl.h>
#include <ast/EnumDecl.h>
#include <ast/FieldDecl.h>
#include <ast/FloatLiteralExpression.h>
#include <ast/FunctionDecl.h>
#include <ast/GotoStatement.h>
#include <ast/IfStatement.h>
#include <ast/ImplicitValueInitExpression.h>
#include <ast/InitListExpression.h>
#include <ast/IntLiteralExpression.h>
#include <ast/LabelStatement.h>
#include <ast/LambdaExpression.h>
#include <ast/LiteralExpression.h>
#include <ast/LoopStatement.h>
#include <ast/MemberExpression.h>
#include <ast/NamespaceDecl.h>
#include <ast/Node.h>
#include <ast/ParamVarDecl.h>
#include <ast/ParenExpression.h>
#include <ast/RefExpression.h>
#include <ast/ReturnStatement.h>
#include <ast/StaticAssertDecl.h>
#include <ast/StringLiteralExpression.h>
#include <ast/SwitchCaseStatement.h>
#include <ast/SwitchStatement.h>
#include <ast/TemporaryExpression.h>
#include <ast/ThisExpression.h>
#include <ast/ThrowExpression.h>
#include <ast/TryExpression.h>
#include <ast/TypeAliasDecl.h>
#include <ast/TypedefDecl.h>
#include <ast/UnaryExpression.h>
#include <ast/UndefinedNode.h>
#include <ast/VarDecl.h>
#include <clang/AST/Comment.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/Lex/Lexer.h>
#include <utils/LocaleUtils.h>

#include "Utils.h"

namespace HCXX {

class ConverterBase {
public:
    virtual uint32_t GetNodeSize()
    {
        return sizeof(UndefinedNode);
    }
    virtual void Convert(ASTVisitor* visitor, const void* clangNode, Node* node)
    {
        new (static_cast<UndefinedNode*>(node)) UndefinedNode();
    }
} g_defaultConverter;

std::vector<ConverterBase*> g_stmtConverters(static_cast<size_t>(clang::Stmt::WhileStmtClass) + 1, &g_defaultConverter);
std::vector<ConverterBase*> g_declConverters(static_cast<size_t>(clang::Decl::TranslationUnit) + 1,
                                             &g_defaultConverter);

CxxRecordDecl ConvertCxxRecord(ASTVisitor& visitor, const clang::CXXRecordDecl& clangNode);

struct CustomNodesRegistrar {
    CustomNodesRegistrar()
    {
        // COODDY_SUPPRESS MemoryAndResourceLeakChecker
        DeclName::RegisterCreator();
        CxxCtorInitializerDecl::RegisterCreator();
        CxxOperatorToken::RegisterCreator();
        DeclQualifier::RegisterCreator();
        UndefinedNode::RegisterCreator();
        ElseKeyword::RegisterCreator();
        CxxDtorExpression::RegisterCreator();
    }
    ~CustomNodesRegistrar() = default;
} g_customNodesRegistrar;

Node* ASTVisitor::GetNode(const clang::Stmt* clangNode)
{
    if (clangNode == nullptr) {
        return nullptr;
    }
    auto& node = myNodes[clangNode];
    if (node == nullptr) {
        ConverterBase* converter = g_stmtConverters[clangNode->getStmtClass()];
        node = myTranslationUnit.AllocateNode(converter->GetNodeSize());
        converter->Convert(this, clangNode, node);
    }
    return node;
}

Node* ASTVisitor::GetNode(const clang::Decl* clangNode)
{
    if (clangNode == nullptr) {
        return nullptr;
    }
    auto& node = myNodes[clangNode];
    if (node == nullptr) {
        ConverterBase* converter = g_declConverters[clangNode->getKind()];
        node = myTranslationUnit.AllocateNode(converter->GetNodeSize());
        converter->Convert(this, clangNode, node);
    }
    return node;
}

void ASTVisitor::AddNode(Node& parent, Node& node, clang::SourceRange clangRange)
{
    SourceRange range;
    if (clangRange.isValid()) {
        range = ConvertSourceRange(mySM, myContext.getLangOpts(), clangRange);
    } else {
        range = {parent.GetRange().end, parent.GetRange().end};
    }
    node.SetInMacro(clangRange.getBegin().isMacroID() || clangRange.getEnd().isMacroID());
    myTranslationUnit.AddNode(node, parent, false, range);
}

class ParentForCallExprBuilder {
public:
    ParentForCallExprBuilder(ASTVisitor& visitor, Node& node, const clang::Expr* clangNode)
        : myVisitor(visitor), myNode(node), myClangNode(clangNode)
    {}
    ~ParentForCallExprBuilder()
    {
        if (myParent != nullptr && clang::isa<clang::CallExpr>(myClangNode)) {
            myVisitor.AddNode(myNode, *myParent, myClangNode->getSourceRange());
        }
    }
    Node* Create()
    {
        myParent = myVisitor.GetNode(myClangNode);
        if (clang::isa<clang::CallExpr>(myClangNode)) {
            myParent = myVisitor.CreateNode<CastExpression>(myParent, CastExpression::Kind::UNKNOWN, Type(),
                                                            CastExpression::TypeKind::IMPLICIT_CAST);
        }
        return myParent;
    }

private:
    Node& myNode;
    Node* myParent;
    const clang::Expr* myClangNode;
    ASTVisitor& myVisitor;
};

bool CanDefineSizeForType(const clang::Type& clangType, clang::TagDecl* tagDecl)
{
    if (clangType.isDependentType()) {
        return false;
    }
    if (clangType.isArrayType()) {
        auto baseType = clangType.getBaseElementTypeUnsafe();
        return baseType != nullptr && !baseType->isIncompleteType();
    }

    if (tagDecl != nullptr) {
        return tagDecl->isCompleteDefinition();
    }
    return clangType.isBuiltinType() && !clangType.isSpecificBuiltinType(clang::BuiltinType::BuiltinFn) &&
           !clangType.isSpecificBuiltinType(clang::BuiltinType::BoundMember) &&
           !clangType.isSpecificBuiltinType(clang::BuiltinType::Overload);
}
void ASTVisitor::ParseTypeAttrs(Type& type, const clang::AttrVec& attrs)
{
    for (auto& attr : attrs) {
        if (attr->getKind() == clang::attr::Kind::Annotate &&
            static_cast<clang::AnnotateAttr*>(attr)->getAnnotation() == "__cooddy_security_sde") {
            type.myIsSensitiveData = true;
            break;
        }
    }
}
Type ASTVisitor::ConvertType(const clang::QualType& qualType, const clang::AttrVec& attrs)
{
    auto type = ConvertType(qualType);
    ParseTypeAttrs(type, attrs);
    return type;
}
Type ASTVisitor::ConvertType(const clang::QualType& qualType)
{
    // LCOV_EXCL_START
    if (qualType.isNull()) {
        return {};
    }
    // LCOV_EXCL_STOP

    clang::QualType originalType = qualType.getCanonicalType();
    const clang::Type* clangType = originalType.getTypePtr();
    Type type;
    if (clangType->isPointerType() || clangType->isReferenceType()) {
        type = ConvertType(clangType->getPointeeType());
        type.myPointerFlags++;
        type.myReferenceFlags <<= 1u;
        type.myConstantFlags <<= 1u;
        type.myReferenceFlags |= clangType->isReferenceType();
        type.myIsRvalueType = originalType->isRValueReferenceType();
    } else {
        type.myIsUnsigned = clangType->isUnsignedIntegerOrEnumerationType();
        type.myIsIntegerType = clangType->isIntegralOrEnumerationType();
        clang::TagDecl* tagDecl = clangType->getAsTagDecl();
        if (tagDecl != nullptr) {
            type.myIsDeclaration = 1;
            type.myDeclNode = GetNode(tagDecl);
        }
        if (CanDefineSizeForType(*clangType, tagDecl)) {
            clang::TypeInfo typeInfo = myContext.getTypeInfo(clangType);
            type.mySizeInBits = typeInfo.Width;
        }
        type.myIsFloatingType = clangType->isFloatingType();
        if (clangType->isArrayType() || clangType->isIncompleteArrayType()) {
            type.myIsArray = 1;
            type.myIsVariableArray = clangType->isVariableArrayType();
            type.myIsConstantArray = clangType->isConstantArrayType();
            auto baseType = clangType->getBaseElementTypeUnsafe();
            if (baseType != nullptr) {
                tagDecl = baseType->getAsTagDecl();
                if (tagDecl != nullptr) {
                    type.myIsDeclaration = 1;
                    type.myDeclNode = GetNode(tagDecl);
                }
            }
            type.myIsFloatingType |= baseType != nullptr && baseType->isFloatingType();
        }
    }
    type.myConstantFlags |= originalType.isConstQualified();

    return type;
}

Node* ASTVisitor::AddImplicitDtorCallNode(const clang::CFGImplicitDtor* cfgDtor, const clang::FunctionDecl* funcDecl,
                                          const clang::Stmt* prevStmt)
{
    if (cfgDtor->getKind() == clang::CFGElement::Kind::AutomaticObjectDtor) {
        if (auto body = GetNode(funcDecl->getBody()); body != nullptr) {
            auto cfgEl = cfgDtor->getAs<clang::CFGAutomaticObjDtor>();
            auto varDecl = GetNode(cfgEl->getVarDecl());
            auto dtorDecl = GetNode(cfgEl->getDestructorDecl(myContext));
            auto refExpr = CreateNode<RefExpression>(varDecl, ConvertType(cfgEl->getVarDecl()->getType()));
            auto memberExpr = CreateNode<MemberExpression>(Type(), "", refExpr, dtorDecl, false);
            CallExpression call(memberExpr, dtorDecl, {}, {});
            auto cxxDtorCallExpr = CreateNode<CxxDtorExpression>(std::move(call), refExpr);
            clang::SourceRange range = prevStmt != nullptr ? prevStmt->getSourceRange() : clang::SourceRange();
            AddNode(*body, *cxxDtorCallExpr, range);
            AddNode(*cxxDtorCallExpr, *memberExpr, range);
            AddNode(*memberExpr, *refExpr, range);
            return cxxDtorCallExpr;
        }
    }
    return nullptr;
}

void ASTVisitor::ProcessCFGBlock(clang::CFGBlock& block, std::vector<HCXX::CfgBlock>& blocks,
                                 const clang::FunctionDecl* funcDecl)
{
    std::vector<HCXX::CfgElement> elements;
    const clang::Stmt* prevStmt = nullptr;
    for (auto& el : block) {
        if (auto cfgDtor = el.getAs<clang::CFGImplicitDtor>(); cfgDtor) {
            auto dtorCallNode = AddImplicitDtorCallNode(cfgDtor.getPointer(), funcDecl, prevStmt);
            elements.emplace_back(static_cast<CfgElement::Kind>(cfgDtor->getKind()), dtorCallNode);
        } else {
            auto cfgEl = el.getAs<clang::CFGStmt>();
            const HCXX::Node* node = nullptr;
            if (cfgEl) {
                prevStmt = cfgEl->getStmt();
                node = GetNode(prevStmt);
            }
            elements.emplace_back(static_cast<CfgElement::Kind>(el.getKind()), node);
        }
    }

    HCXX::CfgBlock::BlocksRef successors;
    for (auto succ = block.succ_begin(); succ != block.succ_end(); ++succ) {
        auto succBlock = *succ != nullptr ? *succ : succ->getPossiblyUnreachableBlock();
        if (succBlock != nullptr) {
            successors.emplace_back(succBlock->getIndexInCFG());
        }
    }

    HCXX::CfgBlock::BlocksRef predecessors;
    for (auto pred = block.pred_begin(); pred != block.pred_end(); ++pred) {
        auto predBlock = *pred != nullptr ? *pred : pred->getPossiblyUnreachableBlock();
        if (predBlock != nullptr) {
            predecessors.emplace_back(predBlock->getIndexInCFG());
        }
    }
    auto condition = GetNode(block.getTerminatorCondition());
    auto label = GetNode(block.getLabel());
    auto clangTerminator = block.getTerminatorStmt();
    auto terminator = GetNode(clangTerminator);

    if (clangTerminator != nullptr && clang::isa<clang::BreakStmt>(clangTerminator)) {
        elements.emplace_back(CfgElement::Kind::LOOP_EXIT, terminator);
    }
    blocks.emplace_back(blocks.size(), std::move(elements), std::move(successors), std::move(predecessors), condition,
                        label, terminator, false, nullptr);
}

void ASTVisitor::ConvertCfg(FunctionDecl* funcNode, const clang::FunctionDecl* funcDecl)
{
    clang::Stmt* funcBody = funcDecl->getBody();
    if (funcNode == nullptr || funcBody == nullptr || mySM.isInSystemHeader(funcDecl->getBeginLoc())) {
        return;
    }
    clang::CFG::BuildOptions options;
    options.PruneTriviallyFalseEdges = true;
    if (!funcNode->IsTemplated() && !myTranslationUnit.HasParseErrors()) {
        options.AddImplicitDtors = true;
    }
    std::unique_ptr<clang::CFG> sourceCFG;

    auto& parseErrs = myTranslationUnit.GetParseErrors();
    auto parseErrIt = std::lower_bound(parseErrs.begin(), parseErrs.end(), funcNode->GetRange().begin);
    if (parseErrIt == parseErrs.end() || *parseErrIt > funcNode->GetRange().end) {
        // build cfg if there were no compilation errors in the function
        sourceCFG = clang::CFG::buildCFG(funcDecl, funcBody, &myContext, options);
    }

    if (sourceCFG == nullptr) {
        // LCOV_EXCL_START
        if (funcDecl->isTemplated()) {
            // CLang can't build CFG for some templated functions => don't log them
            return;
        }
        ++myParserStatistics.failedCFGCount;
        HCXX::Log(HCXX::LogLevel::WARNING)
            << "Can't build CFG for function: " << funcDecl->getQualifiedNameAsString() << "\n";
        return;
        // LCOV_EXCL_STOP
    }

    std::vector<HCXX::CfgBlock> blocks;
    for (auto block : *sourceCFG) {
        ProcessCFGBlock(*block, blocks, funcDecl);
    }
    myTranslationUnit.AddCfg(funcNode, std::make_unique<Cfg>(*funcNode, std::move(blocks)));
}

/**
 * @brief Produce the shortest possible fully-qualified typename string from a QualType.
 *
 * Since HCXX::Type is size-constrained, typename string can be stored separately where needed.
 */
inline std::string ConvertTypeName(const clang::QualType& type)
{
    using namespace StrUtils;

    std::string str = type.getCanonicalType().getAsString();
    str = Replace(str, "> ", ">");
    str = Replace(str, " &", "&");
    str = Replace(str, "struct ", "");
    str = Replace(str, "class ", "");

    // Align pointers left
    // More compact for non-const qualified pointers
    size_t pos = 0;
    while ((pos = str.find(" *", pos)) != str.npos) {
        char next = str[pos + 2];
        if (isalpha(next)) {
            // Add space after if const, volatile, or other qualifiers follow
            str.replace(pos, 2, "* ");
            pos += 2;
        } else {
            // Do not add space if pointer, reference, or other qualifiers follow
            str.replace(pos, 2, "*");
            pos++;
        }
    }

    return str;
}

template <typename TClangNodeType>
struct NodeTypesMapper;

#define DECLARE_CONVERTER(TClang, THcxx, TBase, ID)                                                    \
    template <>                                                                                        \
    struct NodeTypesMapper<clang::TClang> {                                                            \
        using OurNodeType = THcxx;                                                                     \
    };                                                                                                 \
    class TClang##Converter : public ConverterBase {                                                   \
        static TClang##Converter myInstance;                                                           \
        virtual uint32_t GetNodeSize() override                                                        \
        {                                                                                              \
            return sizeof(THcxx);                                                                      \
        }                                                                                              \
        virtual void Convert(ASTVisitor* visitor, const void* clangNode, Node* node)                   \
        {                                                                                              \
            Convert(visitor, static_cast<const clang::TClang*>(clangNode), static_cast<THcxx*>(node)); \
        }                                                                                              \
        TClang##Converter()                                                                            \
        {                                                                                              \
            g_##TBase##Converters[ID] = this;                                                          \
            THcxx::RegisterCreator();                                                                  \
        }                                                                                              \
        void Convert(ASTVisitor* visitor, const clang::TClang* clangNode, THcxx* node);                \
    };                                                                                                 \
    TClang##Converter TClang##Converter::myInstance;                                                   \
    void TClang##Converter::Convert(ASTVisitor* visitor, const clang::TClang* clangNode, THcxx* node)

#define DECLARE_STMT_CONVERTER(TClang, THcxx) DECLARE_CONVERTER(TClang, THcxx, stmt, clang::Stmt::TClang##Class)
#define DECLARE_DECL_CONVERTER(TClang, THcxx) DECLARE_CONVERTER(TClang##Decl, THcxx, decl, clang::Decl::TClang)

DECLARE_STMT_CONVERTER(MemberExpr, MemberExpression)
{
    ParentForCallExprBuilder builder(*visitor, *node, clangNode->getBase());
    auto decl = clangNode->getMemberDecl();
    new (node) MemberExpression(visitor->ConvertType(clangNode->getType(), decl->getAttrs()), decl->getNameAsString(),
                                builder.Create(), visitor->GetNode(decl), clangNode->isArrow());
}

template <typename TClangNodeType>
typename NodeTypesMapper<TClangNodeType>::OurNodeType* TypeSafeAddNode(ASTVisitor& visitor,
                                                                       // TODO HCAT-665
                                                                       // COODDY_SUPPRESS PtrArgIsCheckedChecker
                                                                       const TClangNodeType* clangNode)
{
    return static_cast<typename NodeTypesMapper<TClangNodeType>::OurNodeType*>(visitor.GetNode(clangNode));
}

DECLARE_DECL_CONVERTER(Label, LabelStatement)
{
    auto* labelStmt = clangNode->getStmt();
    if (labelStmt == nullptr) {
        new (node) LabelStatement("", nullptr);  // LCOV_EXCL_LINE
    } else {
        new (node) LabelStatement(labelStmt->getName(), visitor->GetNode(labelStmt->getSubStmt()));
    }
}

DECLARE_STMT_CONVERTER(ParenExpr, ParenExpression)
{
    new (node) ParenExpression(visitor->GetNode(clangNode->getSubExpr()), visitor->ConvertType(clangNode->getType()));
}

DECLARE_STMT_CONVERTER(ArraySubscriptExpr, ArraySubscriptExpression)
{
    ParentForCallExprBuilder builder(*visitor, *node, clangNode->getLHS());
    new (node)
        ArraySubscriptExpression(builder.Create(), visitor->GetNode(clangNode->getRHS()),
                                 visitor->GetNode(clangNode->getBase()), visitor->ConvertType(clangNode->getType()));
}

void ConvertBinaryExpression(const clang::BinaryOperator& clangNode, ASTVisitor& visitor, BinaryExpression* node)
{
    auto opLoc = clangNode.getOperatorLoc();
    SourceRange sourceRange =
        ConvertSourceRange(visitor.GetSourceManager(), visitor.GetContext().getLangOpts(),
                           clang::SourceRange(opLoc, opLoc.getLocWithOffset(clangNode.getOpcodeStr().size())));
    BinaryExpression::Operation op = BinaryExpression::Operation::UNKNOWN;
    switch (clangNode.getOpcode()) {
#define BINARY_OPERATION(Name, Spelling)                                 \
    case clang::BO_##Name:                                               \
        op = static_cast<BinaryExpression::Operation>(clang::BO_##Name); \
        break;
        // LCOV_EXCL_START: HCAT-2779
#include "clang/AST/OperationKinds.def"
        default:
            break;
            // LCOV_EXCL_STOP
    }
    new (node) BinaryExpression(visitor.GetNode(clangNode.getLHS()), visitor.GetNode(clangNode.getRHS()), op,
                                visitor.ConvertType(clangNode.getType()), sourceRange);
}

DECLARE_STMT_CONVERTER(CompoundAssignOperator, BinaryExpression)
{
    ConvertBinaryExpression(*static_cast<const clang::BinaryOperator*>(clangNode), *visitor, node);
}

DECLARE_STMT_CONVERTER(BinaryOperator, BinaryExpression)
{
    ConvertBinaryExpression(*clangNode, *visitor, node);
}

void ConvertUnaryExpression(const clang::UnaryOperator& clangNode, ASTVisitor& visitor, UnaryExpression* node)
{
    UnaryExpression::Operation op = UnaryExpression::Operation::UNKNOWN;

    auto opLoc = clangNode.getOperatorLoc();
    SourceRange sourceRange = ConvertSourceRange(
        visitor.GetSourceManager(), visitor.GetContext().getLangOpts(),
        clang::SourceRange(opLoc, opLoc.getLocWithOffset(clangNode.getOpcodeStr(clangNode.getOpcode()).size())));
    switch (clangNode.getOpcode()) {
#define UNARY_OPERATION(Name, Spelling)                                 \
    case clang::UO_##Name:                                              \
        op = static_cast<UnaryExpression::Operation>(clang::UO_##Name); \
        break;
#include "clang/AST/OperationKinds.def"
            // LCOV_EXCL_START: HCAT-2779
        default:
            break;
            // LCOV_EXCL_STOP
    }
    ParentForCallExprBuilder builder(visitor, *node, clangNode.getSubExpr());
    new (node) UnaryExpression(builder.Create(), op, visitor.ConvertType(clangNode.getType()), sourceRange);
}

DECLARE_STMT_CONVERTER(UnaryOperator, UnaryExpression)
{
    ConvertUnaryExpression(*clangNode, *visitor, node);
}

UnaryExpression::Operation ConvertUnaryOperation(const clang::UnaryExprOrTypeTrait& kind)
{
    // LCOV_EXCL_START
    switch (kind) {
        case clang::UnaryExprOrTypeTrait::UETT_SizeOf:
            return UnaryExpression::Operation::SIZEOF;
        case clang::UnaryExprOrTypeTrait::UETT_AlignOf:
            return UnaryExpression::Operation::ALIGN_OF;
        case clang::UnaryExprOrTypeTrait::UETT_VecStep:
            return UnaryExpression::Operation::VEC_STEP;
        case clang::UnaryExprOrTypeTrait::UETT_OpenMPRequiredSimdAlign:
            return UnaryExpression::Operation::OPENMP_REQUIRED_SIMD_ALIGN;
        case clang::UnaryExprOrTypeTrait::UETT_PreferredAlignOf:
            return UnaryExpression::Operation::PREFFERED_ALIGN_OF;
        default:
            return UnaryExpression::Operation::UNKNOWN;
    }
    // LCOV_EXCL_STOP
}

DECLARE_STMT_CONVERTER(UnaryExprOrTypeTraitExpr, UnaryExpression)
{
    UnaryExpression::Operation op = ConvertUnaryOperation(clangNode->getKind());
    if (clangNode->isArgumentType()) {
        new (node) UnaryExpression(nullptr, op, visitor->ConvertType(clangNode->getArgumentType()), {});
    } else {
        auto argExpr = clangNode->getArgumentExpr();
        if (argExpr->getStmtClass() == clang::Stmt::ParenExprClass) {
            argExpr = (static_cast<const clang::ParenExpr*>(argExpr))->getSubExpr();
        }
        new (node) UnaryExpression(visitor->GetNode(argExpr), op, visitor->ConvertType(clangNode->getType()), {});
    }
}

void InitCommonValuesOfVarDecl(VarDecl::Attributes& attributes, const Node*& arraySizeExpr,
                               clang::TypeSourceInfo* typeSourceInfo, ASTVisitor& visitor)
{
    if (typeSourceInfo == nullptr) {
        return;
    }
    auto typeLoc = typeSourceInfo->getTypeLoc();
    attributes[VarDecl::IS_VOLATILE] = typeLoc.getType().getCanonicalType().isVolatileQualified();

    if (auto arrayTypeLoc = typeLoc.getAs<clang::ConstantArrayTypeLoc>(); arrayTypeLoc) {
        arraySizeExpr = visitor.GetNode(arrayTypeLoc.getSizeExpr());

        attributes[VarDecl::IS_MATRIX] = arrayTypeLoc.getInnerType()->isArrayType();
    }
}

DECLARE_DECL_CONVERTER(Field, FieldDecl)
{
    DeclName* declName = visitor->CreateNode<DeclName>();

    uint32_t fieldIndex = clangNode->getFieldIndex();
    const clang::RecordDecl* parent = clangNode->getParent();
    Type type = visitor->ConvertType(clangNode->getType(), clangNode->getAttrs());
    auto align = 0;
    if (clangNode->isBitField()) {
        type.SetSizeInBits(clangNode->getBitWidthValue(visitor->GetContext()));
    }
    if (type.GetSizeInBits()) {
        align = clangNode->getASTContext().getTypeInfo(clangNode->getType()).Align;
    }

    VarDecl::Attributes attributes;
    attributes[VarDecl::IS_ANONYMOUS] = clangNode->isAnonymousStructOrUnion();
    const Node* arraySizeExpr = nullptr;
    InitCommonValuesOfVarDecl(attributes, arraySizeExpr, clangNode->getTypeSourceInfo(), *visitor);

    bool isParentDefined = parent != nullptr && !parent->isTemplated() && !parent->isInvalidDecl();
    uint32_t fieldOffset = isParentDefined ? visitor->GetContext().getFieldOffset(clangNode) : 0;
    new (node) FieldDecl(visitor->GetNode(parent), clangNode->getNameAsString(), type, nullptr, declName,
                         visitor->GetNode(clangNode->getInClassInitializer()),
                         static_cast<FieldDecl::AccessKind>(clangNode->getAccess()), fieldIndex, fieldOffset, align,
                         attributes, arraySizeExpr);

    auto nameLoc = clangNode->getLocation();
    auto name = clangNode->getName();
    visitor->AddNode(*node, *declName, {nameLoc, nameLoc.getLocWithOffset(name.empty() ? 0 : name.size() - 1)});
}

template <class T>
const T* FindLinkageInfo(ASTVisitor& visitor, const std::vector<T>& vecInfo)
{
    auto result = &vecInfo.front();
    auto& curDir = visitor.GetTranslationUnit().GetCompilerOptions().directory;
    size_t maxPrefix = 0;
    for (auto& info : vecInfo) {
        if (info.unit == nullptr) {
            continue;  // LCOV_EXCL_LINE
        }
        auto& dir = info.unit->GetCompilerOptions().directory;
        size_t prefix = 0;
        while (prefix < curDir.size() && prefix < dir.size() && curDir[prefix] == dir[prefix]) {
            ++prefix;
        }
        if (prefix > maxPrefix) {
            result = &info;
            maxPrefix = prefix;
        }
    }
    return result;
}

std::pair<Type, uint32_t> ASTVisitor::GetVarDeclInfo(const clang::VarDecl& varDecl)
{
    Type type = ConvertType(varDecl.getType(), varDecl.getAttrs());
    if (varDecl.isLocalVarDecl()) {
        return {type, 0};
    }
    static std::atomic<uint32_t> myGlobalVarUniqueIdCounter = 1;
    auto crossCtx = myTranslationUnit.GetCrossTUContext();
    if (crossCtx == nullptr) {
        HCXX::Log(HCXX::LogLevel::DEBUG) << "CrossTUContext not found " << varDecl.getNameAsString() << " id is "
                                         << (myGlobalVarUniqueIdCounter + 1) << "\n";
        return {type, myGlobalVarUniqueIdCounter++};
    }
    auto varInfoArr = crossCtx->FindGlobalVar(varDecl.getNameAsString());
    if (varInfoArr == nullptr || varInfoArr->empty()) {
        return {type, crossCtx->GetGlobalVarsCount() + myGlobalVarUniqueIdCounter++};
    }
    if (!type.IsArray() || type.mySizeInBits != 0) {
        return {type, varInfoArr->front().uniqueId};
    }
    type.mySizeInBits = FindLinkageInfo(*this, *varInfoArr)->sizeOfType;
    if (type.mySizeInBits > 0) {
        type.myIsConstantArray = 1;
    }
    return {type, varInfoArr->front().uniqueId};
}

DECLARE_DECL_CONVERTER(Var, VarDecl)
{
    VarDecl::VariableDeclKind varKind = VarDecl::VariableDeclKind::UNDEFINED;

    if (clangNode->isStaticDataMember()) {
        varKind = VarDecl::VariableDeclKind::STATIC_DATA_MEMBER;
    } else if (clangNode->isCXXClassMember()) {
        varKind = VarDecl::VariableDeclKind::CXX_CLASS_MEMBER;  // LCOV_EXCL_LINE: HCAT-2779
    } else if (clangNode->isStaticLocal()) {
        varKind = VarDecl::VariableDeclKind::STATIC_LOCAL;
    } else if (clangNode->isLocalVarDecl()) {
        varKind = VarDecl::VariableDeclKind::LOCAL_DECLARATION;
    }

    auto qualifierLoc = clangNode->getQualifierLoc();
    DeclQualifier* declQualifier =
        qualifierLoc.getBeginLoc().isValid() ? visitor->CreateNode<DeclQualifier>() : nullptr;
    DeclName* declName = visitor->CreateNode<DeclName>();

    // FIXME: Calculate correct qualifier for VarDecl (reference to "parent" name owner)
    VarDecl::Attributes attributes;
    attributes[VarDecl::IS_VA_ARG] = clangNode->getType().getAsString() == "va_list";
    attributes[VarDecl::IS_CONST_EXPR] = clangNode->isUsableInConstantExpressions(visitor->GetContext());
    attributes[VarDecl::IS_EXTERNAL] = clangNode->hasExternalStorage();

    const Node* arraySizeExpr = nullptr;
    InitCommonValuesOfVarDecl(attributes, arraySizeExpr, clangNode->getTypeSourceInfo(), *visitor);

    auto [type, uniqueId] = visitor->GetVarDeclInfo(*clangNode);
    new (node) VarDecl(nullptr, varKind, clangNode->getNameAsString(), type, declQualifier, declName,
                       visitor->GetNode(clangNode->getAnyInitializer()), attributes, arraySizeExpr, uniqueId);
    node->SetTypeSpecSourceRange(
        ConvertSourceRange(visitor->GetSourceManager(), visitor->GetContext().getLangOpts(),
                           clang::SourceRange(clangNode->getTypeSpecStartLoc(), clangNode->getTypeSpecEndLoc())));
    if (declQualifier != nullptr) {
        visitor->AddNode(*node, *declQualifier, {qualifierLoc.getBeginLoc(), qualifierLoc.getEndLoc()});
    }
    auto nameLoc = clangNode->getLocation();
    visitor->AddNode(*node, *declName, {nameLoc, nameLoc.getLocWithOffset(clangNode->getName().size())});
}

DECLARE_DECL_CONVERTER(ParmVar, ParamVarDecl)
{
    VarDecl::Attributes attributes;
    const Node* arraySizeExpr = nullptr;
    InitCommonValuesOfVarDecl(attributes, arraySizeExpr, clangNode->getTypeSourceInfo(), *visitor);
    new (node)
        ParamVarDecl(clangNode->getNameAsString(), visitor->ConvertType(clangNode->getType(), clangNode->getAttrs()),
                     ConvertTypeName(clangNode->getType()), arraySizeExpr);
}

DECLARE_STMT_CONVERTER(IntegerLiteral, IntLiteralExpression)
{
    llvm::APInt apInt = clangNode->getValue();
    uint64_t value = apInt.getLimitedValue();

    new (node) IntLiteralExpression(visitor->ConvertType(clangNode->getType()), value);
}

DECLARE_STMT_CONVERTER(CharacterLiteral, CharLiteralExpression)
{
    new (node) CharLiteralExpression(visitor->ConvertType(clangNode->getType()), clangNode->getValue(),
                                     static_cast<CharLiteralExpression::CharKind>(clangNode->getKind()));
}

DECLARE_STMT_CONVERTER(StringLiteral, StringLiteralExpression)
{
    new (node) StringLiteralExpression(visitor->ConvertType(clangNode->getType()),
                                       // FIXME: provide adequate conversion for non-1-byte strings
                                       clangNode->getCharByteWidth() == 1
                                           ? std::string(clangNode->getString().data(), clangNode->getLength())
                                           // just to prevent errors in clangNode->getString()
                                           : std::move("?"));
}

DECLARE_STMT_CONVERTER(FloatingLiteral, FloatLiteralExpression)
{
    new (node)
        FloatLiteralExpression(visitor->ConvertType(clangNode->getType()), clangNode->getValueAsApproximateDouble());
}

DECLARE_STMT_CONVERTER(CXXNullPtrLiteralExpr, LiteralExpression)
{
    new (node) LiteralExpression(visitor->ConvertType(clangNode->getType()));
}

DECLARE_STMT_CONVERTER(CXXBoolLiteralExpr, BoolLiteralExpression)
{
    new (node) BoolLiteralExpression(visitor->ConvertType(clangNode->getType()), clangNode->getValue());
}

CallExpression ConvertCallExpression(ASTVisitor& visitor, const clang::CallExpr& call)
{
    std::vector<const Node*> arguments;
    auto numArgs = call.getNumArgs();
    arguments.reserve(numArgs);
    for (uint32_t i = 0; i < numArgs; ++i) {
        arguments.push_back(visitor.GetNode(call.getArg(i)));
    }

    Node* callee = visitor.GetNode(call.getCallee());
    Node* decl = visitor.GetNode(call.getCalleeDecl());

    Type returnType = {};
    if (!call.isInstantiationDependent()) {
        // Calculate return type, but only if call is not templated (clang just gives up)
        returnType = visitor.ConvertType(call.getCallReturnType(visitor.GetContext()));
    }

    return CallExpression(callee, decl, std::move(arguments), returnType);
}

DECLARE_STMT_CONVERTER(CallExpr, CallExpression)
{
    new (node) CallExpression(ConvertCallExpression(*visitor, *clangNode));
}

DECLARE_STMT_CONVERTER(CXXTryStmt, TryExpression)
{
    auto numHandlers = clangNode->getNumHandlers();
    std::vector<const CatchExpression*> handlers(numHandlers);
    for (auto i = 0; i < numHandlers; ++i) {
        handlers[i] = static_cast<const CatchExpression*>(visitor->GetNode(clangNode->getHandler(i)));
    }
    new (node) TryExpression(visitor->GetNode(clangNode->getTryBlock()), std::move(handlers));
}

DECLARE_STMT_CONVERTER(CXXNewExpr, CxxNewExpression)
{
    std::vector<const Node*> placementArgs;
    auto numPlacementArgs = clangNode->getNumPlacementArgs();
    placementArgs.reserve(numPlacementArgs);
    for (unsigned i = 0; i < numPlacementArgs; i++) {
        placementArgs.push_back(visitor->GetNode(clangNode->getPlacementArg(i)));
    }
    auto optSize = clangNode->getArraySize();
    Node* arraySize = nullptr;
    if (optSize.hasValue()) {
        arraySize = visitor->GetNode(optSize.getValue());
    }
    new (node) CxxNewExpression(visitor->GetNode(clangNode->getConstructExpr()), std::move(placementArgs),
                                visitor->GetNode(clangNode->getOperatorNew()), arraySize,
                                visitor->ConvertType(clangNode->getAllocatedTypeSourceInfo()->getType()));
}

DECLARE_STMT_CONVERTER(CXXThrowExpr, ThrowExpression)
{
    new (node) ThrowExpression();
}

DECLARE_STMT_CONVERTER(CXXCatchStmt, CatchExpression)
{
    new (node) CatchExpression();
}

DECLARE_STMT_CONVERTER(ReturnStmt, ReturnStatement)
{
    new (node) ReturnStatement(visitor->GetNode(clangNode->getRetValue()));
}

DECLARE_STMT_CONVERTER(ConditionalOperator, ConditionalExpression)
{
    new (node) ConditionalExpression(visitor->GetNode(clangNode->getCond()), visitor->GetNode(clangNode->getTrueExpr()),
                                     visitor->GetNode(clangNode->getFalseExpr()));
}

// LCOV_EXCL_START
DECLARE_STMT_CONVERTER(BinaryConditionalOperator, ConditionalExpression)
{
    new (node) ConditionalExpression(visitor->GetNode(clangNode->getCond()), visitor->GetNode(clangNode->getTrueExpr()),
                                     visitor->GetNode(clangNode->getFalseExpr()));
}
// LCOV_EXCL_STOP

DECLARE_DECL_CONVERTER(StaticAssert, StaticAssertDecl)
{
    const clang::StringLiteral* message = clangNode->getMessage();
    std::string strMsg;
    if (message != nullptr) {
        llvm::StringRef ref = message->getString();
        strMsg.assign(ref.data(), ref.size());
    }
    new (node) StaticAssertDecl(visitor->GetNode(clangNode->getAssertExpr()), std::move(strMsg), clangNode->isFailed());
}

Node* GetQualifier(ASTVisitor& visitor, const clang::Decl& decl)
{
    auto parentFunctionOrMethod = decl.getParentFunctionOrMethod();
    if (parentFunctionOrMethod) {
        // when declared inside function FQN is pointless, no actual qualifier
        return nullptr;
    }

    const clang::DeclContext* parent = decl.getDeclContext();
    // TODO: support other parent kinds, such as CXXRecordDecl
    if (parent != nullptr && parent->getDeclKind() == clang::Decl::Kind::Namespace) {
        return visitor.GetNode(static_cast<const clang::NamespaceDecl*>(parent));
    }

    return nullptr;
}

DECLARE_DECL_CONVERTER(Namespace, NamespaceDecl)
{
    new (node) NamespaceDecl(GetQualifier(*visitor, *clangNode), clangNode->getName().str());
}

DECLARE_STMT_CONVERTER(DeclRefExpr, RefExpression)
{
    new (node) RefExpression(visitor->GetNode(clangNode->getDecl()),
                             visitor->ConvertType(clangNode->getType(), clangNode->getDecl()->getAttrs()));
}

DECLARE_DECL_CONVERTER(CXXRecord, CxxRecordDecl)
{
    new (node) CxxRecordDecl(ConvertCxxRecord(*visitor, *clangNode));
}

DECLARE_DECL_CONVERTER(ClassTemplateSpecialization, CxxRecordDecl)
{
    new (node) CxxRecordDecl(ConvertCxxRecord(*visitor, *clangNode));
}

void ConvertCastExpr(ASTVisitor& visitor, const clang::CastExpr& clangNode, CastExpression::TypeKind typeKind,
                     CastExpression* node)
{
    CastExpression::Kind kind = CastExpression::Kind::UNKNOWN;

    switch (clangNode.getCastKind()) {
#define CAST_OPERATION(Name)                                        \
    case clang::CK_##Name:                                          \
        kind = static_cast<CastExpression::Kind>(clang::CK_##Name); \
        break;
        // LCOV_EXCL_START: HCAT-2779
#include "clang/AST/OperationKinds.def"
        default:
            break;
            // LCOV_EXCL_STOP
    }

    new (node) CastExpression(visitor.GetNode(clangNode.getSubExpr()), kind, visitor.ConvertType(clangNode.getType()),
                              typeKind);
}

DECLARE_STMT_CONVERTER(CStyleCastExpr, CastExpression)
{
    ConvertCastExpr(*visitor, *clangNode, CastExpression::TypeKind::CSTYLE_CAST, node);
}

DECLARE_STMT_CONVERTER(CXXReinterpretCastExpr, CastExpression)
{
    ConvertCastExpr(*visitor, *clangNode, CastExpression::TypeKind::REINTERPRET_CAST, node);
}

DECLARE_STMT_CONVERTER(CXXConstCastExpr, CastExpression)
{
    ConvertCastExpr(*visitor, *clangNode, CastExpression::TypeKind::CONST_CAST, node);
}

DECLARE_STMT_CONVERTER(CXXDynamicCastExpr, CastExpression)
{
    ConvertCastExpr(*visitor, *clangNode, CastExpression::TypeKind::DYNAMIC_CAST, node);
}

DECLARE_STMT_CONVERTER(CXXStaticCastExpr, CastExpression)
{
    ConvertCastExpr(*visitor, *clangNode, CastExpression::TypeKind::STATIC_CAST, node);
}

DECLARE_STMT_CONVERTER(ImplicitCastExpr, CastExpression)
{
    ConvertCastExpr(*visitor, *clangNode, CastExpression::TypeKind::IMPLICIT_CAST, node);
}

DECLARE_STMT_CONVERTER(NullStmt, EmptyStmt)
{
    new (node) EmptyStmt();
}

DeclarationNameInfo CreateDeclarationNameInfo(ASTVisitor& visitor, const clang::FunctionDecl& func)
{
    auto declNameSourceRange = func.getNameInfo().getSourceRange();
    auto sourceRange =
        ConvertSourceRange(visitor.GetSourceManager(), visitor.GetContext().getLangOpts(), declNameSourceRange);
    return DeclarationNameInfo(func.getDeclName().getAsString(), sourceRange);
}

FunctionQualifiers MakeQualifiers(const clang::FunctionDecl& function)
{
    FunctionQualifiers qualifiers;
    qualifiers.reset();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::DEFINED)] = function.isDefined();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::DELETED)] = function.isDeleted();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::DEFAULTED)] = function.isDefaulted();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::PURE)] = function.isPure();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::IMPLICIT)] = function.isImplicit();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::VARIADIC)] = function.isVariadic();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::TEMPLATED)] = function.isTemplated();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::NO_RETURN)] = function.isNoReturn();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::INLINE)] = function.isInlineSpecified();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::STATIC)] = function.isStatic();
    qualifiers[static_cast<int>(FunctionQualifiersIndex::INTRINSIC)] = (function.getBuiltinID() == 0) ? 0 : 1;

    // When marked by a special macro, a function is considered a security risk by company convention.
    // When a related CLI argument is passed, such functions are treated as having tainted arguments.
    qualifiers[static_cast<int>(FunctionQualifiersIndex::ANNOTATED_TAINTED)] = false;
    for (auto& attr : function.getAttrs()) {
        if (attr->getKind() == clang::attr::Kind::Annotate &&
            static_cast<clang::AnnotateAttr*>(attr)->getAnnotation() == "__cooddy_security_risk") {
            qualifiers[static_cast<int>(FunctionQualifiersIndex::ANNOTATED_TAINTED)] = true;
            break;
        }
    }

    if (function.getKind() == clang::Decl::CXXConstructor) {
        const clang::CXXConstructorDecl* ctor = static_cast<const clang::CXXConstructorDecl*>(&function);
        qualifiers[static_cast<int>(FunctionQualifiersIndex::COPY)] = ctor->isCopyConstructor();
        qualifiers[static_cast<int>(FunctionQualifiersIndex::MOVE)] = ctor->isMoveConstructor();
        qualifiers[static_cast<int>(FunctionQualifiersIndex::EXPLICIT)] = ctor->isExplicit();
        qualifiers[static_cast<int>(FunctionQualifiersIndex::DEFAULT)] = ctor->isDefaultConstructor();
    } else if (function.getKind() == clang::Decl::CXXMethod) {
        const clang::CXXMethodDecl* method = static_cast<const clang::CXXMethodDecl*>(&function);
        qualifiers[static_cast<int>(FunctionQualifiersIndex::COPY)] = method->isCopyAssignmentOperator();
        qualifiers[static_cast<int>(FunctionQualifiersIndex::MOVE)] = method->isMoveAssignmentOperator();
        auto pos = std::find_if(method->attr_begin(), method->attr_end(),
                                [](clang::Attr* a) { return (a->getKind() == clang::attr::Kind::Override); });
        if (pos != method->attr_end()) {
            qualifiers[static_cast<int>(FunctionQualifiersIndex::OVERRIDE)] = true;
        }
        qualifiers[static_cast<int>(FunctionQualifiersIndex::CONST)] = method->isConst();
        qualifiers[static_cast<int>(FunctionQualifiersIndex::VIRTUAL)] = method->isVirtual();
        qualifiers[static_cast<int>(FunctionQualifiersIndex::STATIC)] = method->isStatic();
    }
    return qualifiers;
}

std::string MakeQualifiedName(const clang::FunctionDecl& func)
{
    std::string result = func.getQualifiedNameAsString();
    auto* templateArgs = func.getTemplateSpecializationArgs();

    if (templateArgs) {
        result += "<";

        std::string buffer;
        auto sep = "";

        for (auto& arg : templateArgs->asArray()) {
            buffer.clear();
            llvm::raw_string_ostream bufferStream(buffer);
            arg.print(CLANG_PRINT_POLICY, bufferStream, true);

            result += sep;
            result += buffer;

            sep = ", ";
        }

        result += ">";
    }

    return result;
}

std::string GetMangledName(ASTVisitor& visitor, const clang::FunctionDecl& func, FunctionDecl::FunctionKind funcKind)
{
    std::string mangledName = clang::ASTNameGenerator(visitor.GetContext()).getName(&func);
    auto defFunc = func.getDefinition();
    if (defFunc == nullptr) {
        defFunc = &func;
    }
    if (!defFunc->isTemplated() && funcKind == FunctionDecl::FunctionKind::FUNCTION) {
        if (defFunc->isStatic()) {
            auto& sm = visitor.GetContext().getSourceManager();
            if (auto entry = sm.getFileEntryForID(sm.getFileID(defFunc->getBeginLoc())); entry != nullptr) {
                mangledName += "/" + std::filesystem::path(entry->getName().str()).filename().string();
            }
        }
        auto crossContext = visitor.GetTranslationUnit().GetCrossTUContext();
        auto funcInfoArr =
            crossContext != nullptr ? crossContext->FindFunctionDef(defFunc->getQualifiedNameAsString()) : nullptr;
        if (funcInfoArr != nullptr && funcInfoArr->size() > 1) {
            mangledName += "/" + std::to_string(uintptr_t(FindLinkageInfo(visitor, *funcInfoArr)->unit));
        }
    }
    if (visitor.GetTranslationUnit().GetSizeOfPointer() == 32) {
        mangledName += "/32";
    }
    return mangledName;
}

FunctionDecl ConvertFunctionDecl(ASTVisitor& visitor, const clang::FunctionDecl& func,
                                 FunctionDecl::FunctionKind funcKind, FunctionDecl* definition = nullptr)
{
    std::vector<NodePtr<ParamVarDecl>> params;
    const auto& funcParameters = func.parameters();
    params.reserve(funcParameters.size());
    for (auto param : funcParameters) {
        params.emplace_back(static_cast<ParamVarDecl*>(visitor.GetNode(param)));
    }

    auto qualifiers = MakeQualifiers(func);

    std::vector<Type> templateTypes;
    auto templateArgs = func.getTemplateSpecializationArgs();
    if (templateArgs != nullptr) {
        for (auto i = 0; i < templateArgs->size(); ++i) {
            auto& arg = templateArgs->get(i);
            if (arg.getKind() == clang::TemplateArgument::Pack) {
                qualifiers[static_cast<int>(FunctionQualifiersIndex::VARIADIC_TEMPLATE)] = true;
            } else if (arg.getKind() == clang::TemplateArgument::Type) {
                templateTypes.emplace_back(visitor.ConvertType(arg.getAsType()));
            }
        }
    }

    std::vector<SourceRange> commentBlocks;
    auto comment = func.getASTContext().getLocalCommentForDeclUncached(&func);
    if (comment != nullptr) {
        for (auto it = comment->child_begin(); it != comment->child_end(); ++it) {
            auto clangRange = (*it)->getSourceRange();

            SourceRange range;
            if (clangRange.isValid()) {
                range = ConvertSourceRange(visitor.GetSourceManager(), visitor.GetContext().getLangOpts(), clangRange);
                commentBlocks.emplace_back(range);
            }
        }
    }

    auto templateInstPattern = static_cast<FunctionDecl*>(visitor.GetNode(func.getTemplateInstantiationPattern()));
    auto declNameInfo = CreateDeclarationNameInfo(visitor, func);
    Node* body = func.doesThisDeclarationHaveABody() && !func.isWeak() ? visitor.GetNode(func.getBody()) : nullptr;
    return FunctionDecl(std::move(declNameInfo), visitor.ConvertType(func.getReturnType()),
                        ConvertTypeName(func.getReturnType()), std::move(params), body, funcKind, qualifiers,
                        GetMangledName(visitor, func, funcKind), MakeQualifiedName(func),
                        static_cast<FunctionDecl::AccessKind>(func.getAccess()), definition, templateInstPattern,
                        std::move(commentBlocks), std::move(templateTypes));
}

DECLARE_DECL_CONVERTER(Function, FunctionDecl)
{
    new (node)
        FunctionDecl(ConvertFunctionDecl(*visitor, *clangNode->getAsFunction(), FunctionDecl::FunctionKind::FUNCTION));
}

DECLARE_DECL_CONVERTER(CXXMethod, CxxMethodDecl)
{
    new (node) CxxMethodDecl(
        ConvertFunctionDecl(*visitor, *clangNode->getAsFunction(), FunctionDecl::FunctionKind::METHOD, nullptr),
        static_cast<const CxxRecordDecl*>(visitor->GetNode(clangNode->getParent())));
}

DECLARE_DECL_CONVERTER(CXXConversion, CxxConversionDecl)
{
    new (node) CxxConversionDecl(
        ConvertFunctionDecl(*visitor, *clangNode->getAsFunction(), FunctionDecl::FunctionKind::CONVERSION),
        static_cast<const CxxRecordDecl*>(visitor->GetNode(clangNode->getParent())));
}

CxxCtorDecl::Initializers CreateInitList(const clang::CXXConstructorDecl& clangNode, ASTVisitor& visitor,
                                         CxxCtorDecl& node)
{
    CxxCtorDecl::Initializers initList;
    llvm::iterator_range<clang::CXXCtorInitializer* const*> inits = clangNode.inits();
    if (inits.empty()) {
        inits = clangNode.getCanonicalDecl()->inits();
    }
    if (inits.empty()) {
        inits = static_cast<const clang::CXXConstructorDecl*>(clangNode.getMostRecentDecl())->inits();
    }

    // FIXME: change to usage of convertors HCAT-1404
    for (auto init : inits) {
        if (init->getAnyMember() != nullptr) {
            auto fieldDecl = init->getMember();
            auto initExpr = init->getInit();
            if (init->isInClassMemberInitializer() && fieldDecl != nullptr &&
                (initExpr == nullptr || clang::isa<clang::CXXDefaultInitExpr>(initExpr))) {
                initExpr = fieldDecl->getInClassInitializer();
            }
            auto initNode = visitor.CreateNode<CxxCtorInitializerDecl>(
                &node, static_cast<const FieldDecl*>(visitor.GetNode(fieldDecl)), visitor.GetNode(initExpr),
                init->getAnyMember()->getNameAsString());

            initList.emplace_back(initNode);
        } else if (init->getInit() != nullptr && (init->isBaseInitializer() || init->isDelegatingInitializer())) {
            // FIXME: remove copy-paste, change to usage of convertors HCAT-1404
            auto constr = init->getInit();
            if (clang::isa<clang::ExprWithCleanups>(constr)) {
                constr = constr->IgnoreUnlessSpelledInSource();  // LCOV_EXCL_LINE
            }
            if (clang::isa<clang::CXXConstructExpr>(constr)) {
                auto constrExpr = clang::cast<clang::CXXConstructExpr>(constr);
                std::vector<const Node*> arguments;
                auto numArgs = constrExpr->getNumArgs();
                arguments.reserve(numArgs);
                for (uint32_t i = 0; i < numArgs; ++i) {
                    arguments.push_back(visitor.GetNode(constrExpr->getArg(i)));
                }
                auto initNode = visitor.CreateNode<CxxConstructExpression>(
                    visitor.GetNode(constrExpr->getConstructor()), std::move(arguments),
                    visitor.ConvertType(constr->getType()));
                initList.emplace_back(initNode);
            }
        }
    }
    return initList;
}

DECLARE_DECL_CONVERTER(CXXConstructor, CxxCtorDecl)
{
    FunctionDecl* definition = nullptr;
    if (clangNode->getMostRecentDecl() != clangNode) {
        definition = static_cast<FunctionDecl*>(visitor->GetNode(clangNode->getMostRecentDecl()));
    }
    auto recordDecl = static_cast<const CxxRecordDecl*>(visitor->GetNode(clangNode->getParent()));
    auto initList = CreateInitList(*clangNode, *visitor, *node);
    new (node) CxxCtorDecl(
        ConvertFunctionDecl(*visitor, *clangNode->getAsFunction(), FunctionDecl::FunctionKind::CONSTRUCTOR, definition),
        recordDecl, std::move(initList));

    for (auto& it : node->GetInitList()) {
        // COODDY_SUPPRESS
        visitor->AddNode(*node, const_cast<Node&>(*it), {});
    }
}

DECLARE_DECL_CONVERTER(CXXDestructor, CxxDtorDecl)
{
    auto recordDecl = static_cast<const CxxRecordDecl*>(visitor->GetNode(clangNode->getParent()));
    new (node) CxxDtorDecl(
        ConvertFunctionDecl(*visitor, *clangNode->getAsFunction(), FunctionDecl::FunctionKind::DESTRUCTOR), recordDecl);
}

DECLARE_STMT_CONVERTER(CXXDeleteExpr, DeleteExpression)
{
    new (node)
        DeleteExpression(visitor->GetNode(clangNode->getArgument()), visitor->GetNode(clangNode->getOperatorDelete()));
}

DECLARE_STMT_CONVERTER(CXXThisExpr, ThisExpression)
{
    new (node) ThisExpression(visitor->ConvertType(clangNode->getType()));
}

DECLARE_STMT_CONVERTER(IfStmt, IfStatement)
{
    auto elseLoc = clangNode->getElseLoc();
    ElseKeyword* elseNode = elseLoc.isValid() ? visitor->CreateNode<ElseKeyword>() : nullptr;
    new (node) IfStatement(visitor->GetNode(clangNode->getCond()), visitor->GetNode(clangNode->getThen()), elseNode,
                           visitor->GetNode(clangNode->getElse()), visitor->GetNode(clangNode->getInit()),
                           visitor->GetNode(clangNode->getConditionVariableDeclStmt()));
    if (elseNode != nullptr) {
        visitor->AddNode(*node, *elseNode, {elseLoc, elseLoc.getLocWithOffset(4)});
    }
}

DECLARE_STMT_CONVERTER(ForStmt, LoopStatement)
{
    new (node)
        LoopStatement(LoopStatement::LoopKind::FOR, visitor->GetNode(clangNode->getInit()),
                      visitor->GetNode(clangNode->getCond()), visitor->GetNode(clangNode->getBody()),
                      visitor->GetNode(clangNode->getConditionVariable()), visitor->GetNode(clangNode->getInc()));
}

DECLARE_STMT_CONVERTER(CXXForRangeStmt, LoopStatement)
{
    new (node) LoopStatement(LoopStatement::LoopKind::RANGED_FOR, visitor->GetNode(clangNode->getInit()),
                             visitor->GetNode(clangNode->getCond()), visitor->GetNode(clangNode->getBody()),
                             visitor->GetNode(clangNode->getLoopVariable()), visitor->GetNode(clangNode->getInc()),
                             visitor->GetNode(clangNode->getRangeInit()));
}

DECLARE_STMT_CONVERTER(WhileStmt, LoopStatement)
{
    new (node) LoopStatement(LoopStatement::LoopKind::WHILE, nullptr, visitor->GetNode(clangNode->getCond()),
                             visitor->GetNode(clangNode->getBody()),
                             visitor->GetNode(clangNode->getConditionVariable()), nullptr);
}

DECLARE_STMT_CONVERTER(DoStmt, LoopStatement)
{
    new (node) LoopStatement(LoopStatement::LoopKind::DO_WHILE, nullptr, visitor->GetNode(clangNode->getCond()),
                             visitor->GetNode(clangNode->getBody()), nullptr, nullptr);
}

DECLARE_STMT_CONVERTER(CompoundStmt, CompoundStatement)
{
    new (node) CompoundStatement();
}

DECLARE_STMT_CONVERTER(DeclStmt, DeclStatement)
{
    vector<NodePtr<Node>> decls;
    for (auto it = clangNode->decl_begin(); it != clangNode->decl_end(); ++it) {
        decls.emplace_back(visitor->GetNode(*it));
    }
    new (node) DeclStatement(std::move(decls));
}

DECLARE_STMT_CONVERTER(SwitchStmt, SwitchStatement)
{
    SwitchStatement::CaseChildren caseChildren;

    auto caseChild = clangNode->getSwitchCaseList();
    while (caseChild != nullptr) {
        if (auto caseStmt = clang::dyn_cast<clang::CaseStmt>(caseChild); caseStmt != nullptr) {
            caseChildren.emplace_back(static_cast<CaseStatement*>(visitor->GetNode(caseChild)));
        } else if (auto defaultStmt = clang::dyn_cast<clang::DefaultStmt>(caseChild); defaultStmt != nullptr) {
            caseChildren.emplace_back(static_cast<DefaultStatement*>(visitor->GetNode(caseChild)));
        }
        caseChild = caseChild->getNextSwitchCase();
    }

    new (node) SwitchStatement(visitor->GetNode(clangNode->getCond()), visitor->GetNode(clangNode->getBody()),
                               std::move(caseChildren));
}

DECLARE_STMT_CONVERTER(CaseStmt, CaseStatement)
{
    new (node) CaseStatement(visitor->GetNode(clangNode->getSubStmt()), visitor->GetNode(clangNode->getLHS()),
                             visitor->GetNode(clangNode->getRHS()));
}

DECLARE_STMT_CONVERTER(DefaultStmt, DefaultStatement)
{
    new (node) DefaultStatement(visitor->GetNode(clangNode->getSubStmt()));
}

DECLARE_STMT_CONVERTER(CXXDefaultArgExpr, CxxDefaultArgExpression)
{
    new (node) CxxDefaultArgExpression(visitor->GetNode(clangNode->getParam()), visitor->GetNode(clangNode->getExpr()));
}

DECLARE_DECL_CONVERTER(Typedef, TypedefDecl)
{
    clangNode->isImplicit();
    llvm::StringRef sr = clangNode->getName();
    std::string_view sv(sr.data(), sr.size());
    new (node) TypedefDecl(GetQualifier(*visitor, *clangNode), visitor->ConvertType(clangNode->getUnderlyingType()),
                           std::move(sv));
}

DECLARE_DECL_CONVERTER(TypeAlias, TypeAliasDecl)
{
    llvm::StringRef sr = clangNode->getName();
    std::string_view sv(sr.data(), sr.size());
    new (node) TypeAliasDecl(visitor->ConvertType(clangNode->getUnderlyingType()), std::move(sv));
}

DECLARE_DECL_CONVERTER(TypeAliasTemplate, TypeAliasDecl)
{
    new (node) TypeAliasDecl(visitor->ConvertType(clangNode->getTemplatedDecl()->getUnderlyingType()),
                             clangNode->getName().data());
}

DECLARE_DECL_CONVERTER(Enum, EnumDecl)
{
    EnumDecl::EnumMembers members;

    for (auto it = clangNode->enumerator_begin(); it != clangNode->enumerator_end(); ++it) {
        members.emplace_back(static_cast<EnumConstantDecl*>(visitor->GetNode(it->getCanonicalDecl())));
    }

    new (node) EnumDecl(GetQualifier(*visitor, *clangNode), clangNode->getName().data(),
                        visitor->ConvertType(clangNode->getIntegerType()), std::move(members));
}

DECLARE_DECL_CONVERTER(EnumConstant, EnumConstantDecl)
{
    llvm::StringRef strRef = clangNode->getName();
    std::string_view name(strRef.data(), strRef.size());
    new (node) EnumConstantDecl(GetQualifier(*visitor, *clangNode), std::move(name),
                                visitor->ConvertType(clangNode->getType()), clangNode->getInitVal().getExtValue());
}

DECLARE_DECL_CONVERTER(AccessSpec, AccessSpecDecl)
{
    new (node) AccessSpecDecl(static_cast<AccessSpecDecl::AccessKind>(clangNode->getAccess()));
}

DECLARE_STMT_CONVERTER(LabelStmt, LabelStatement)
{
    new (node) LabelStatement(clangNode->getName(), visitor->GetNode(clangNode->getSubStmt()));
}

DECLARE_STMT_CONVERTER(GotoStmt, GotoStatement)
{
    new (node) GotoStatement(visitor->GetNode(clangNode->getLabel()));
}

// LCOV_EXCL_START
DECLARE_STMT_CONVERTER(IndirectGotoStmt, GotoStatement)
{
    new (node) GotoStatement(visitor->GetNode(clangNode->getConstantTarget()));
}
// LCOV_EXCL_STOP

DECLARE_STMT_CONVERTER(BreakStmt, BreakStatement)
{
    new (node) BreakStatement();
}

DECLARE_STMT_CONVERTER(ContinueStmt, ContinueStatement)
{
    new (node) ContinueStatement();
}

DECLARE_STMT_CONVERTER(InitListExpr, InitListExpression)
{
    std::vector<const Node*> initList;
    initList.reserve(std::distance(clangNode->begin(), clangNode->end()));
    for (auto& it : *clangNode) {
        initList.emplace_back(visitor->GetNode(it));
    }
    new (node) InitListExpression(std::move(initList), clangNode->getType()->isArrayType());
}

DECLARE_STMT_CONVERTER(ImplicitValueInitExpr, ImplicitValueInitExpression)
{
    new (node) ImplicitValueInitExpression(visitor->ConvertType(clangNode->getType()));
}

DECLARE_STMT_CONVERTER(CXXMemberCallExpr, CxxMemberCallExpression)
{
    bool isVirtualCall = false;
    auto methodDecl = clangNode->getMethodDecl();
    if (methodDecl != nullptr && methodDecl->isVirtual() && clang::isa<clang::MemberExpr>(clangNode->getCallee())) {
        isVirtualCall = !clang::cast<clang::MemberExpr>(clangNode->getCallee())->hasQualifier();
    }
    new (node) CxxMemberCallExpression(ConvertCallExpression(*visitor, *clangNode),
                                       visitor->GetNode(clangNode->getImplicitObjectArgument()), isVirtualCall);
}

DECLARE_STMT_CONVERTER(CXXOperatorCallExpr, CxxOperatorCallExpression)
{
    auto opTokenLoc = clangNode->getOperatorLoc();
    CxxOperatorToken* opToken = opTokenLoc.isValid() ? visitor->CreateNode<CxxOperatorToken>() : nullptr;

    new (node) CxxOperatorCallExpression(ConvertCallExpression(*visitor, *clangNode), opToken,
                                         static_cast<CxxOperatorCallExpression::OperatorKind>(clangNode->getOperator()),
                                         clangNode->isAssignmentOp());

    if (opToken != nullptr) {
        visitor->AddNode(*node, *opToken, {opTokenLoc, opTokenLoc});
    }
}

DECLARE_STMT_CONVERTER(MaterializeTemporaryExpr, TemporaryExpression)
{
    new (node) TemporaryExpression(visitor->GetNode(clangNode->getSubExpr()));
}

DECLARE_STMT_CONVERTER(CXXStdInitializerListExpr, TemporaryExpression)
{
    new (node) TemporaryExpression(visitor->GetNode(clangNode->getSubExpr()));
}

DECLARE_STMT_CONVERTER(CXXBindTemporaryExpr, TemporaryExpression)
{
    new (node) TemporaryExpression(visitor->GetNode(clangNode->getSubExpr()));
}

DECLARE_STMT_CONVERTER(CXXFunctionalCastExpr, CastExpression)
{
    ConvertCastExpr(*visitor, *clangNode, CastExpression::TypeKind::FUNCTIONAL_CAST, node);
}

DECLARE_STMT_CONVERTER(ConstantExpr, TemporaryExpression)
{
    new (node) TemporaryExpression(visitor->GetNode(clangNode->getSubExpr()));
}

DECLARE_STMT_CONVERTER(ExprWithCleanups, TemporaryExpression)
{
    new (node) TemporaryExpression(visitor->GetNode(clangNode->getSubExpr()));
}

DECLARE_STMT_CONVERTER(StmtExpr, TemporaryExpression)
{
    new (node) TemporaryExpression(visitor->GetNode(clangNode->getSubStmt()));
}

void ConvertConstructExpression(ASTVisitor& visitor, const clang::CXXConstructExpr& clangNode, Node& node)
{
    std::vector<const Node*> arguments;
    auto numArgs = clangNode.getNumArgs();
    arguments.reserve(numArgs);
    for (uint32_t i = 0; i < numArgs; ++i) {
        arguments.push_back(visitor.GetNode(clangNode.getArg(i)));
    }
    new (&node) CxxConstructExpression(visitor.GetNode(clangNode.getConstructor()), std::move(arguments),
                                       visitor.ConvertType(clangNode.getType()));
}

DECLARE_STMT_CONVERTER(CXXConstructExpr, CxxConstructExpression)
{
    ConvertConstructExpression(*visitor, *clangNode, *node);
}

DECLARE_STMT_CONVERTER(CXXTemporaryObjectExpr, CxxConstructExpression)
{
    ConvertConstructExpression(*visitor, *clangNode, *node);
}

DECLARE_STMT_CONVERTER(LambdaExpr, LambdaExpression)
{
    new (node) LambdaExpression();
}

RecordDecl ConvertRecord(ASTVisitor& visitor, const clang::RecordDecl& clangNode)
{
    RecordDecl::Fields fields;
    const auto& fieldsRange = clangNode.fields();
    fields.reserve(std::distance(fieldsRange.begin(), fieldsRange.end()));
    for (clang::FieldDecl* it : fieldsRange) {
        FieldDecl* ptr = TypeSafeAddNode(visitor, it);
        fields.push_back(ptr);
    }

    RecordDecl::RecordType recordType(RecordDecl::RecordType::UNKNOWN);
    if (clangNode.isClass()) {
        recordType = RecordDecl::RecordType::CLASS;
    } else if (clangNode.isStruct()) {
        recordType = RecordDecl::RecordType::STRUCT;
    } else if (clangNode.isUnion()) {
        recordType = RecordDecl::RecordType::UNION;
    }

    std::vector<SourceRange> commentBlocks;
    auto comment = clangNode.getASTContext().getLocalCommentForDeclUncached(&clangNode);
    if (comment != nullptr) {
        for (auto it = comment->child_begin(); it != comment->child_end(); ++it) {
            auto clangRange = (*it)->getSourceRange();

            SourceRange range;
            if (clangRange.isValid()) {
                range = ConvertSourceRange(visitor.GetSourceManager(), visitor.GetContext().getLangOpts(), clangRange);
                commentBlocks.emplace_back(range);
            }
        }
    }

    Type type;
    if (clangNode.getTypeForDecl() != nullptr) {
        type = visitor.ConvertType(clangNode.getTypeForDecl()->getCanonicalTypeInternal(), clangNode.getAttrs());
    }

    return RecordDecl(GetQualifier(visitor, clangNode), clangNode.getNameAsString(), std::move(fields), recordType,
                      clangNode.isFirstDecl(), clangNode.isImplicit(), type.GetSizeInBits(), std::move(commentBlocks),
                      type.IsSensitiveData());
}

DECLARE_DECL_CONVERTER(Record, RecordDecl)
{
    new (node) RecordDecl(ConvertRecord(*visitor, *clangNode));
}

std::vector<clang::CXXRecordDecl*> ASTVisitor::GetBaseRecords(const clang::CXXRecordDecl& clangNode)
{
    std::vector<clang::CXXRecordDecl*> result;
    if (clangNode.getDefinition() == nullptr || clangNode.getNumBases() == 0) {
        return result;
    }
    for (auto base : clangNode.bases()) {
        auto type = base.getType()->getAs<clang::RecordType>();
        if (type == nullptr) {
            continue;  // LCOV_EXCL_LINE
        }
        auto baseDecl = clang::cast<clang::CXXRecordDecl>(type->getDecl()->getDefinition());
        if (baseDecl != nullptr && !baseDecl->isDependentContext()) {
            result.emplace_back(baseDecl);
        }
    }
    return result;
}

CxxRecordDecl ConvertCxxRecord(ASTVisitor& visitor, const clang::CXXRecordDecl& clangNode)
{
    std::vector<const CxxCtorDecl*> constructors;
    const auto& ctorsRange = clangNode.ctors();
    constructors.reserve(std::distance(ctorsRange.begin(), ctorsRange.end()));
    for (auto it : ctorsRange) {
        const CxxCtorDecl* ctor = TypeSafeAddNode(visitor, it);
        constructors.push_back(ctor);
    }
    CxxRecordDecl::BaseRecords baseRecords;
    for (auto base : visitor.GetBaseRecords(clangNode)) {
        if (base != nullptr) {
            baseRecords.push_back(TypeSafeAddNode(visitor, base));
        }
    }
    CxxDtorDecl* dtor = TypeSafeAddNode(visitor, clangNode.getDestructor());
    return CxxRecordDecl(ConvertRecord(visitor, clangNode), dtor, constructors, std::move(baseRecords));
}

DECLARE_STMT_CONVERTER(GCCAsmStmt, AsmStatement)
{
    new (node) AsmStatement();
}

};  // namespace HCXX
