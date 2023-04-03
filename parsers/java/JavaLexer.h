
// Generated from ./java_grammar/JavaLexer.g4 by ANTLR 4.10.1

#pragma once

#include "antlr4-runtime.h"

class JavaLexer : public antlr4::Lexer {
public:
    enum {
        ABSTRACT = 1,
        ASSERT = 2,
        BOOLEAN = 3,
        BREAK = 4,
        BYTE = 5,
        CASE = 6,
        CATCH = 7,
        CHAR = 8,
        CLASS = 9,
        CONST = 10,
        CONTINUE = 11,
        DEFAULT = 12,
        DO = 13,
        DOUBLE = 14,
        ELSE = 15,
        ENUM = 16,
        EXTENDS = 17,
        FINAL = 18,
        FINALLY = 19,
        FLOAT = 20,
        FOR = 21,
        IF = 22,
        GOTO = 23,
        IMPLEMENTS = 24,
        IMPORT = 25,
        INSTANCEOF = 26,
        INT = 27,
        INTERFACE = 28,
        LONG = 29,
        NATIVE = 30,
        NEW = 31,
        PACKAGE = 32,
        PRIVATE = 33,
        PROTECTED = 34,
        PUBLIC = 35,
        RETURN = 36,
        SHORT = 37,
        STATIC = 38,
        STRICTFP = 39,
        SUPER = 40,
        SWITCH = 41,
        SYNCHRONIZED = 42,
        THIS = 43,
        THROW = 44,
        THROWS = 45,
        TRANSIENT = 46,
        TRY = 47,
        VOID = 48,
        VOLATILE = 49,
        WHILE = 50,
        MODULE = 51,
        OPEN = 52,
        REQUIRES = 53,
        EXPORTS = 54,
        OPENS = 55,
        TO = 56,
        USES = 57,
        PROVIDES = 58,
        WITH = 59,
        TRANSITIVE = 60,
        VAR = 61,
        YIELD = 62,
        RECORD = 63,
        SEALED = 64,
        PERMITS = 65,
        NON_SEALED = 66,
        DECIMAL_LITERAL = 67,
        HEX_LITERAL = 68,
        OCT_LITERAL = 69,
        BINARY_LITERAL = 70,
        FLOAT_LITERAL = 71,
        HEX_FLOAT_LITERAL = 72,
        BOOL_LITERAL = 73,
        CHAR_LITERAL = 74,
        STRING_LITERAL = 75,
        TEXT_BLOCK = 76,
        NULL_LITERAL = 77,
        LPAREN = 78,
        RPAREN = 79,
        LBRACE = 80,
        RBRACE = 81,
        LBRACK = 82,
        RBRACK = 83,
        SEMI = 84,
        COMMA = 85,
        DOT = 86,
        ASSIGN = 87,
        GT = 88,
        LT = 89,
        BANG = 90,
        TILDE = 91,
        QUESTION = 92,
        COLON = 93,
        EQUAL = 94,
        LE = 95,
        GE = 96,
        NOTEQUAL = 97,
        AND = 98,
        OR = 99,
        INC = 100,
        DEC = 101,
        ADD = 102,
        SUB = 103,
        MUL = 104,
        DIV = 105,
        BITAND = 106,
        BITOR = 107,
        CARET = 108,
        MOD = 109,
        ADD_ASSIGN = 110,
        SUB_ASSIGN = 111,
        MUL_ASSIGN = 112,
        DIV_ASSIGN = 113,
        AND_ASSIGN = 114,
        OR_ASSIGN = 115,
        XOR_ASSIGN = 116,
        MOD_ASSIGN = 117,
        LSHIFT_ASSIGN = 118,
        RSHIFT_ASSIGN = 119,
        URSHIFT_ASSIGN = 120,
        ARROW = 121,
        COLONCOLON = 122,
        AT = 123,
        ELLIPSIS = 124,
        WS = 125,
        COMMENT = 126,
        LINE_COMMENT = 127,
        IDENTIFIER = 128
    };

    explicit JavaLexer(antlr4::CharStream* input);

    ~JavaLexer() override;

    std::string getGrammarFileName() const override;

    const std::vector<std::string>& getRuleNames() const override;

    const std::vector<std::string>& getChannelNames() const override;

    const std::vector<std::string>& getModeNames() const override;

    const antlr4::dfa::Vocabulary& getVocabulary() const override;

    antlr4::atn::SerializedATNView getSerializedATN() const override;

    const antlr4::atn::ATN& getATN() const override;

    // By default the static state used to implement the lexer is lazily initialized during the first
    // call to the constructor. You can call this function if you wish to initialize the static state
    // ahead of time.
    static void initialize();

private:
    // Individual action functions triggered by action() above.

    // Individual semantic predicate functions triggered by sempred() above.
};
