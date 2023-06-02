/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_JAVA_PARSER_PARSEERRORLISTENER_H_
#define COODDY_ANALYZER_SOURCE_JAVA_PARSER_PARSEERRORLISTENER_H_
#include <utils/Log.h>

#include "ANTLRErrorListener.h"
namespace antlr4 {
class Recognizer;
class Token;
class Parser;
}  // namespace antlr4

namespace HCXX {
class ParseErrorListener : public antlr4::ANTLRErrorListener {
public:
    explicit ParseErrorListener(const std::string& sourceFile) : mySourceFile(sourceFile){};

    // LCOV_EXCL_START
    void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                     size_t charPositionInLine, const std::string& msg, std::exception_ptr e) override
    {
        Log(LogLevel::FATAL) << mySourceFile << ":" << line << ":" << charPositionInLine << " " << msg << "\n";
    };
    // LCOV_EXCL_STOP

    void reportAmbiguity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex,
                         bool exact, const antlrcpp::BitSet& ambigAlts, antlr4::atn::ATNConfigSet* configs) override{};
    void reportAttemptingFullContext(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
                                     size_t stopIndex, const antlrcpp::BitSet& conflictingAlts,
                                     antlr4::atn::ATNConfigSet* configs) override
    {}
    void reportContextSensitivity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
                                  size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet* configs) override
    {}

private:
    const std::string& mySourceFile;
};
}  // namespace HCXX

#endif
