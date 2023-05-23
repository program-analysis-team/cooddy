/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
//
// Declaration of the AST visitor.
// Converts CLang AST-representation to Cooddy representation.
//

#ifndef COODDY_ASTVISITOR_H
#define COODDY_ASTVISITOR_H

#include <Parser.h>
#include <TranslationUnit.h>
#include <ast/CxxCtorDecl.h>
#include <clang/AST/Mangle.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Analysis/CFG.h>

#include "Convertors.h"

namespace HCXX {

class ASTVisitor : public clang::RecursiveASTVisitor<ASTVisitor> {
    TranslationUnit& myTranslationUnit;
    clang::ASTContext& myContext;
    clang::SourceManager& mySM;
    std::vector<Node*> myParents;
    HCXX::Parser::ParserStatistics& myParserStatistics;

    static const uint32_t CLANG_BITS_FOR_NODE_TYPE = 5;  // the average size of clang node is 32 (5 bits per type)
    struct ClangNodeAddressHash {
        std::size_t operator()(const void* node) const noexcept
        {
            //  COODDY_SUPPRESS HastBasedChecker
            return (reinterpret_cast<uintptr_t>(node)) >> CLANG_BITS_FOR_NODE_TYPE;
        }
    };
    std::unordered_map<const void*, Node*, ClangNodeAddressHash> myNodes;

public:
    explicit ASTVisitor(clang::ASTContext& context, TranslationUnit& translationUnit,
                        HCXX::Parser::ParserStatistics& parserStatistics)
        : myTranslationUnit(translationUnit),
          myContext(context),
          mySM(context.getSourceManager()),
          myParserStatistics(parserStatistics)
    {
        myNodes.max_load_factor(2);
        myNodes.reserve(myContext.getASTAllocatedMemory() >> CLANG_BITS_FOR_NODE_TYPE);
        myNodes[myContext.getTranslationUnitDecl()] = &myTranslationUnit;
        myParents.push_back(&myTranslationUnit);
    }

    ~ASTVisitor()
    {
        for (auto& [clangNode, node] : myNodes) {
            auto function = Node::Cast<HCXX::FunctionDecl>(node);
            auto clangFunc = static_cast<const clang::FunctionDecl*>(clangNode);
            if (function != nullptr && clang::isa<clang::FunctionDecl>(clangFunc)) {
                ConvertCfg(function, clangFunc);
            }
        }
        for (auto& [clangNode, node] : myNodes) {
            if (node != nullptr && node->GetTranslationUnit() == nullptr) {
                myTranslationUnit.AddNode(*node, myTranslationUnit, true, {0, 0});
            }
        }
        myTranslationUnit.InitIndices();
    }

    void Traverse();

    Node* GetNode(const clang::Stmt* clangNode);

    Node* GetNode(const clang::Decl* clangNode);

    Node* GetNode(const CxxCtorDecl& ctor, const clang::CXXCtorInitializer* clangNode);

    void AddNode(Node& parent, Node& node, clang::SourceRange range);

    template <typename TNode, typename... Args>
    TNode* CreateNode(Args... args)
    {
        TNode* node = static_cast<TNode*>(myTranslationUnit.AllocateNode(sizeof(TNode)));
        new (node) TNode(std::forward<Args>(args)...);
        return node;
    }

    clang::SourceManager& GetSourceManager() const
    {
        return mySM;
    }

    clang::ASTContext& GetContext() const
    {
        return myContext;
    }

    TranslationUnit& GetTranslationUnit() const
    {
        return myTranslationUnit;
    }

    Type ConvertType(const clang::QualType& qualType);
    Type ConvertType(const clang::QualType& qualType, const clang::AttrVec& attrs);

    std::pair<Type, uint32_t> GetVarDeclInfo(const clang::VarDecl& varDecl);

    static std::vector<clang::CXXRecordDecl*> GetBaseRecords(const clang::CXXRecordDecl& recordDecl);

private:
    friend class clang::RecursiveASTVisitor<ASTVisitor>;

    void ConvertCfg(FunctionDecl* funcNode, const clang::FunctionDecl* funcDecl);

    // TODO HCAT-513
    // COODDY_SUPPRESS CamelCaseChecker
    bool shouldVisitTemplateInstantiations() const
    {
        return true;
    }

    bool TraverseInitListExpr(clang::InitListExpr* initList, DataRecursionQueue* queue = nullptr)
    {
        for (clang::Stmt* subStmt : initList->children()) {
            TraverseStmt(subStmt, queue);
        }
        return false;
    }
    // COODDY_SUPPRESS
    bool TraverseStmt(clang::Stmt* clangNode, DataRecursionQueue* queue = nullptr)
    {
        if (clangNode != nullptr && TraverseBegin(clangNode)) {
            clang::RecursiveASTVisitor<ASTVisitor>::TraverseStmt(clangNode, nullptr);
            TraverseEnd();
        }
        return true;
    }

    bool TraverseDecl(clang::Decl* clangNode)
    {
        if (clangNode != nullptr && clangNode->getKind() == clang::Decl::TranslationUnit) {
            clang::RecursiveASTVisitor<ASTVisitor>::TraverseDecl(clangNode);
        } else if (clangNode != nullptr && TraverseBegin(clangNode)) {
            clang::RecursiveASTVisitor<ASTVisitor>::TraverseDecl(clangNode);
            TraverseEnd();
        }
        return true;
    }

    template <class TNode>
    bool TraverseBegin(const TNode* clangNode)
    {
        Node* node = GetNode(clangNode);
        if (node == nullptr || node->GetTranslationUnit() != nullptr) {
            return false;
        }

        Node* parent = myParents.back();
        clang::SourceRange range = clangNode->getSourceRange();
        bool system = parent->IsSystem() || parent == &myTranslationUnit && mySM.isInSystemHeader(range.getBegin());
        if (system) {
            return false;
        }
        if (!ShouldConvertNode(clangNode)) {
            return false;
        }
        AddNode(*parent, *node, range);
        myParents.emplace_back(node);
        return true;
    }

    void TraverseEnd()
    {
        myParents.pop_back();
    }

    template <class TNode>
    // COODDY_SUPPRESS
    bool ShouldConvertNode(const TNode* clangNode)
    {
        return true;
    }

    bool ShouldConvertNode(const clang::Decl* clangNode)
    {
        if (clang::isa<clang::TypedefDecl>(clangNode)) {
            if (static_cast<const clang::TypedefDecl*>(clangNode)->isImplicit()) {
                //  We want to ignore implicit typedefs added by clang:
                //  TranslationUnitDecl 0x27bf01b7148 <<invalid sloc>> <invalid sloc>
                //  |-TypedefDecl 0x27bf01b7a20 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
                //  | `-BuiltinType 0x27bf01b76e0 '__int128'
                //  |-TypedefDecl 0x27bf01b7a90 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
                //  | `-BuiltinType 0x27bf01b7700 'unsigned __int128'
                //  ...
                return false;
            }
        }

        if (clang::isa<clang::CXXRecordDecl>(clangNode)) {
            if (static_cast<const clang::CXXRecordDecl*>(clangNode)->isImplicit()) {
                //  Clang adds implicit CXXRecordDecl for each class, for example,
                //  'class A {};' is converted to
                //  TranslationUnitDecl 0x29e3bcd00d8 <<invalid sloc>> <invalid sloc>
                //  |-...
                //  `-CXXRecordDecl 0x29e3bd2e890 <a.cpp:1:1, col:10> col:7 class A definition
                //    |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod ...
                //    | |-ctors and dtor...
                //    `-CXXRecordDecl 0x29e3bd2e9a8 <col:1, col:7> col:7 implicit class A
                //  We want to ignore such nodes.
                return false;
            }
        }

        return true;
    }
    Node* AddImplicitDtorCallNode(const clang::CFGImplicitDtor* cfgDtor, const clang::FunctionDecl* funcDecl,
                                  const clang::Stmt* prevStmt);

    void ProcessCFGBlock(clang::CFGBlock& block, std::vector<HCXX::CfgBlock>& blocks,
                         const clang::FunctionDecl* funcDecl);
};

};  // namespace HCXX

#endif  // COODDY_ASTVISITOR_H
