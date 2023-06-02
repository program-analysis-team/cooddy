/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CLANG_FACADE_PARSERCONTEXT_H_
#define COODDY_ANALYZER_SOURCE_CLANG_FACADE_PARSERCONTEXT_H_

#include <clang/Frontend/FrontendActions.h>

class ParserContext : public HCXX::Parser::Context {
    struct ParsedFunctions {
        const clang::FileEntry* entry = nullptr;
        std::unordered_set<uint32_t> offsets;  // already parsed functions' offsets in the source file
    };
    std::unordered_map<std::string, ParsedFunctions> myParsedFunctions[2];  // key: filename, [0]:64-bits, [1]:32-bits
    std::mutex myMutex;

    bool ShouldSkipFunctionBody(const clang::FileEntry* entry, uint32_t functionOffset, bool is32Bit)
    {
        std::unique_lock<std::mutex> lock(myMutex);
        auto& parsedFunctions = myParsedFunctions[is32Bit][entry->getName().str()];
        if (parsedFunctions.entry == nullptr) {
            parsedFunctions.entry = entry;
        }
        // don't skip functions from the first source file entry
        // and functions which are not parsed yet (f.e. under some #ifdef)
        return !parsedFunctions.offsets.emplace(functionOffset).second && parsedFunctions.entry != entry;
    }

    class Consumer : public clang::ASTConsumer {
    public:
        Consumer(ParserContext& owner, clang::SourceManager& sm, bool is32Bit)
            : myOwner(owner), mySM(sm), myIs32Bit(is32Bit)
        {}
        ~Consumer() = default;

        bool shouldSkipFunctionBody(clang::Decl* decl) override
        {
            auto loc = mySM.getExpansionLoc(decl->getBeginLoc());
            if (mySM.isInSystemHeader(loc)) {
                return true;
            }
            if (decl->isTemplated() || mySM.isInMainFile(loc)) {
                return false;
            }
            auto entry = mySM.getFileEntryForID(mySM.getFileID(loc));
            return myOwner.ShouldSkipFunctionBody(entry, mySM.getFileOffset(loc), myIs32Bit);
        }

    private:
        ParserContext& myOwner;
        clang::SourceManager& mySM;
        bool myIs32Bit;
    };

public:
    class Action : public clang::SyntaxOnlyAction {
    public:
        explicit Action(ParserContext& owner) : myOwner(owner) {}
        ~Action() = default;

        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& ci,
                                                              clang::StringRef inFile) override
        {
            bool is32bit = ci.getASTContext().getTypeInfo(ci.getASTContext().getIntPtrType().getTypePtr()).Width == 32;
            return std::make_unique<Consumer>(myOwner, ci.getSourceManager(), is32bit);
        }

    private:
        ParserContext& myOwner;
    };
};

#endif  // COODDY_ANALYZER_SOURCE_CLANG_FACADE_PARSERCONTEXT_H_
