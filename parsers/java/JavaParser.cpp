
// Generated from ./java_grammar/JavaParser.g4 by ANTLR 4.10.1

#include "JavaParser.h"

using namespace antlrcpp;

using namespace antlr4;

namespace {

struct JavaParserStaticData final {
    JavaParserStaticData(std::vector<std::string> ruleNames, std::vector<std::string> literalNames,
                         std::vector<std::string> symbolicNames)
        : ruleNames(std::move(ruleNames)),
          literalNames(std::move(literalNames)),
          symbolicNames(std::move(symbolicNames)),
          vocabulary(this->literalNames, this->symbolicNames)
    {}

    JavaParserStaticData(const JavaParserStaticData&) = delete;
    JavaParserStaticData(JavaParserStaticData&&) = delete;
    JavaParserStaticData& operator=(const JavaParserStaticData&) = delete;
    JavaParserStaticData& operator=(JavaParserStaticData&&) = delete;

    std::vector<antlr4::dfa::DFA> decisionToDFA;
    antlr4::atn::PredictionContextCache sharedContextCache;
    const std::vector<std::string> ruleNames;
    const std::vector<std::string> literalNames;
    const std::vector<std::string> symbolicNames;
    const antlr4::dfa::Vocabulary vocabulary;
    antlr4::atn::SerializedATNView serializedATN;
    std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag javaparserParserOnceFlag;
JavaParserStaticData* javaparserParserStaticData = nullptr;

void javaparserParserInitialize()
{
    assert(javaparserParserStaticData == nullptr);
    auto staticData =
        std::make_unique<JavaParserStaticData>(std::vector<std::string>{"compilationUnit",
                                                                        "packageDeclaration",
                                                                        "importDeclaration",
                                                                        "typeDeclaration",
                                                                        "modifier",
                                                                        "classOrInterfaceModifier",
                                                                        "variableModifier",
                                                                        "classDeclaration",
                                                                        "typeParameters",
                                                                        "typeParameter",
                                                                        "typeBound",
                                                                        "enumDeclaration",
                                                                        "enumConstants",
                                                                        "enumConstant",
                                                                        "enumBodyDeclarations",
                                                                        "interfaceDeclaration",
                                                                        "classBody",
                                                                        "interfaceBody",
                                                                        "classBodyDeclaration",
                                                                        "memberDeclaration",
                                                                        "methodDeclaration",
                                                                        "methodBody",
                                                                        "typeTypeOrVoid",
                                                                        "genericMethodDeclaration",
                                                                        "genericConstructorDeclaration",
                                                                        "constructorDeclaration",
                                                                        "fieldDeclaration",
                                                                        "interfaceBodyDeclaration",
                                                                        "interfaceMemberDeclaration",
                                                                        "constDeclaration",
                                                                        "constantDeclarator",
                                                                        "interfaceMethodDeclaration",
                                                                        "interfaceMethodModifier",
                                                                        "genericInterfaceMethodDeclaration",
                                                                        "interfaceCommonBodyDeclaration",
                                                                        "variableDeclarators",
                                                                        "variableDeclarator",
                                                                        "variableDeclaratorId",
                                                                        "variableInitializer",
                                                                        "arrayInitializer",
                                                                        "classOrInterfaceType",
                                                                        "typeArgument",
                                                                        "qualifiedNameList",
                                                                        "formalParameters",
                                                                        "receiverParameter",
                                                                        "formalParameterList",
                                                                        "formalParameter",
                                                                        "lastFormalParameter",
                                                                        "lambdaLVTIList",
                                                                        "lambdaLVTIParameter",
                                                                        "qualifiedName",
                                                                        "literal",
                                                                        "integerLiteral",
                                                                        "floatLiteral",
                                                                        "altAnnotationQualifiedName",
                                                                        "annotation",
                                                                        "elementValuePairs",
                                                                        "elementValuePair",
                                                                        "elementValue",
                                                                        "elementValueArrayInitializer",
                                                                        "annotationTypeDeclaration",
                                                                        "annotationTypeBody",
                                                                        "annotationTypeElementDeclaration",
                                                                        "annotationTypeElementRest",
                                                                        "annotationMethodOrConstantRest",
                                                                        "annotationMethodRest",
                                                                        "annotationConstantRest",
                                                                        "defaultValue",
                                                                        "moduleDeclaration",
                                                                        "moduleBody",
                                                                        "moduleDirective",
                                                                        "requiresModifier",
                                                                        "recordDeclaration",
                                                                        "recordHeader",
                                                                        "recordComponentList",
                                                                        "recordComponent",
                                                                        "recordBody",
                                                                        "block",
                                                                        "blockStatement",
                                                                        "localVariableDeclaration",
                                                                        "identifier",
                                                                        "localTypeDeclaration",
                                                                        "statement",
                                                                        "catchClause",
                                                                        "catchType",
                                                                        "finallyBlock",
                                                                        "resourceSpecification",
                                                                        "resources",
                                                                        "resource",
                                                                        "switchBlockStatementGroup",
                                                                        "switchLabel",
                                                                        "forControl",
                                                                        "forInit",
                                                                        "enhancedForControl",
                                                                        "parExpression",
                                                                        "expressionList",
                                                                        "methodCall",
                                                                        "expression",
                                                                        "pattern",
                                                                        "lambdaExpression",
                                                                        "lambdaParameters",
                                                                        "lambdaBody",
                                                                        "primary",
                                                                        "switchExpression",
                                                                        "switchLabeledRule",
                                                                        "guardedPattern",
                                                                        "switchRuleOutcome",
                                                                        "classType",
                                                                        "creator",
                                                                        "createdName",
                                                                        "innerCreator",
                                                                        "arrayCreatorRest",
                                                                        "classCreatorRest",
                                                                        "explicitGenericInvocation",
                                                                        "typeArgumentsOrDiamond",
                                                                        "nonWildcardTypeArgumentsOrDiamond",
                                                                        "nonWildcardTypeArguments",
                                                                        "typeList",
                                                                        "typeType",
                                                                        "primitiveType",
                                                                        "typeArguments",
                                                                        "superSuffix",
                                                                        "explicitGenericInvocationSuffix",
                                                                        "arguments"},
                                               std::vector<std::string>{"",
                                                                        "'abstract'",
                                                                        "'assert'",
                                                                        "'boolean'",
                                                                        "'break'",
                                                                        "'byte'",
                                                                        "'case'",
                                                                        "'catch'",
                                                                        "'char'",
                                                                        "'class'",
                                                                        "'const'",
                                                                        "'continue'",
                                                                        "'default'",
                                                                        "'do'",
                                                                        "'double'",
                                                                        "'else'",
                                                                        "'enum'",
                                                                        "'extends'",
                                                                        "'final'",
                                                                        "'finally'",
                                                                        "'float'",
                                                                        "'for'",
                                                                        "'if'",
                                                                        "'goto'",
                                                                        "'implements'",
                                                                        "'import'",
                                                                        "'instanceof'",
                                                                        "'int'",
                                                                        "'interface'",
                                                                        "'long'",
                                                                        "'native'",
                                                                        "'new'",
                                                                        "'package'",
                                                                        "'private'",
                                                                        "'protected'",
                                                                        "'public'",
                                                                        "'return'",
                                                                        "'short'",
                                                                        "'static'",
                                                                        "'strictfp'",
                                                                        "'super'",
                                                                        "'switch'",
                                                                        "'synchronized'",
                                                                        "'this'",
                                                                        "'throw'",
                                                                        "'throws'",
                                                                        "'transient'",
                                                                        "'try'",
                                                                        "'void'",
                                                                        "'volatile'",
                                                                        "'while'",
                                                                        "'module'",
                                                                        "'open'",
                                                                        "'requires'",
                                                                        "'exports'",
                                                                        "'opens'",
                                                                        "'to'",
                                                                        "'uses'",
                                                                        "'provides'",
                                                                        "'with'",
                                                                        "'transitive'",
                                                                        "'var'",
                                                                        "'yield'",
                                                                        "'record'",
                                                                        "'sealed'",
                                                                        "'permits'",
                                                                        "'non-sealed'",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "",
                                                                        "'null'",
                                                                        "'('",
                                                                        "')'",
                                                                        "'{'",
                                                                        "'}'",
                                                                        "'['",
                                                                        "']'",
                                                                        "';'",
                                                                        "','",
                                                                        "'.'",
                                                                        "'='",
                                                                        "'>'",
                                                                        "'<'",
                                                                        "'!'",
                                                                        "'~'",
                                                                        "'\\u003F'",
                                                                        "':'",
                                                                        "'=='",
                                                                        "'<='",
                                                                        "'>='",
                                                                        "'!='",
                                                                        "'&&'",
                                                                        "'||'",
                                                                        "'++'",
                                                                        "'--'",
                                                                        "'+'",
                                                                        "'-'",
                                                                        "'*'",
                                                                        "'/'",
                                                                        "'&'",
                                                                        "'|'",
                                                                        "'^'",
                                                                        "'%'",
                                                                        "'+='",
                                                                        "'-='",
                                                                        "'*='",
                                                                        "'/='",
                                                                        "'&='",
                                                                        "'|='",
                                                                        "'^='",
                                                                        "'%='",
                                                                        "'<<='",
                                                                        "'>>='",
                                                                        "'>>>='",
                                                                        "'->'",
                                                                        "'::'",
                                                                        "'@'",
                                                                        "'...'"},
                                               std::vector<std::string>{"",
                                                                        "ABSTRACT",
                                                                        "ASSERT",
                                                                        "BOOLEAN",
                                                                        "BREAK",
                                                                        "BYTE",
                                                                        "CASE",
                                                                        "CATCH",
                                                                        "CHAR",
                                                                        "CLASS",
                                                                        "CONST",
                                                                        "CONTINUE",
                                                                        "DEFAULT",
                                                                        "DO",
                                                                        "DOUBLE",
                                                                        "ELSE",
                                                                        "ENUM",
                                                                        "EXTENDS",
                                                                        "FINAL",
                                                                        "FINALLY",
                                                                        "FLOAT",
                                                                        "FOR",
                                                                        "IF",
                                                                        "GOTO",
                                                                        "IMPLEMENTS",
                                                                        "IMPORT",
                                                                        "INSTANCEOF",
                                                                        "INT",
                                                                        "INTERFACE",
                                                                        "LONG",
                                                                        "NATIVE",
                                                                        "NEW",
                                                                        "PACKAGE",
                                                                        "PRIVATE",
                                                                        "PROTECTED",
                                                                        "PUBLIC",
                                                                        "RETURN",
                                                                        "SHORT",
                                                                        "STATIC",
                                                                        "STRICTFP",
                                                                        "SUPER",
                                                                        "SWITCH",
                                                                        "SYNCHRONIZED",
                                                                        "THIS",
                                                                        "THROW",
                                                                        "THROWS",
                                                                        "TRANSIENT",
                                                                        "TRY",
                                                                        "VOID",
                                                                        "VOLATILE",
                                                                        "WHILE",
                                                                        "MODULE",
                                                                        "OPEN",
                                                                        "REQUIRES",
                                                                        "EXPORTS",
                                                                        "OPENS",
                                                                        "TO",
                                                                        "USES",
                                                                        "PROVIDES",
                                                                        "WITH",
                                                                        "TRANSITIVE",
                                                                        "VAR",
                                                                        "YIELD",
                                                                        "RECORD",
                                                                        "SEALED",
                                                                        "PERMITS",
                                                                        "NON_SEALED",
                                                                        "DECIMAL_LITERAL",
                                                                        "HEX_LITERAL",
                                                                        "OCT_LITERAL",
                                                                        "BINARY_LITERAL",
                                                                        "FLOAT_LITERAL",
                                                                        "HEX_FLOAT_LITERAL",
                                                                        "BOOL_LITERAL",
                                                                        "CHAR_LITERAL",
                                                                        "STRING_LITERAL",
                                                                        "TEXT_BLOCK",
                                                                        "NULL_LITERAL",
                                                                        "LPAREN",
                                                                        "RPAREN",
                                                                        "LBRACE",
                                                                        "RBRACE",
                                                                        "LBRACK",
                                                                        "RBRACK",
                                                                        "SEMI",
                                                                        "COMMA",
                                                                        "DOT",
                                                                        "ASSIGN",
                                                                        "GT",
                                                                        "LT",
                                                                        "BANG",
                                                                        "TILDE",
                                                                        "QUESTION",
                                                                        "COLON",
                                                                        "EQUAL",
                                                                        "LE",
                                                                        "GE",
                                                                        "NOTEQUAL",
                                                                        "AND",
                                                                        "OR",
                                                                        "INC",
                                                                        "DEC",
                                                                        "ADD",
                                                                        "SUB",
                                                                        "MUL",
                                                                        "DIV",
                                                                        "BITAND",
                                                                        "BITOR",
                                                                        "CARET",
                                                                        "MOD",
                                                                        "ADD_ASSIGN",
                                                                        "SUB_ASSIGN",
                                                                        "MUL_ASSIGN",
                                                                        "DIV_ASSIGN",
                                                                        "AND_ASSIGN",
                                                                        "OR_ASSIGN",
                                                                        "XOR_ASSIGN",
                                                                        "MOD_ASSIGN",
                                                                        "LSHIFT_ASSIGN",
                                                                        "RSHIFT_ASSIGN",
                                                                        "URSHIFT_ASSIGN",
                                                                        "ARROW",
                                                                        "COLONCOLON",
                                                                        "AT",
                                                                        "ELLIPSIS",
                                                                        "WS",
                                                                        "COMMENT",
                                                                        "LINE_COMMENT",
                                                                        "IDENTIFIER"});
    static const int32_t serializedATNSegment[] = {
        4,    1,    128,  1764, 2,    0,    7,    0,    2,    1,    7,    1,    2,    2,    7,    2,    2,    3,
        7,    3,    2,    4,    7,    4,    2,    5,    7,    5,    2,    6,    7,    6,    2,    7,    7,    7,
        2,    8,    7,    8,    2,    9,    7,    9,    2,    10,   7,    10,   2,    11,   7,    11,   2,    12,
        7,    12,   2,    13,   7,    13,   2,    14,   7,    14,   2,    15,   7,    15,   2,    16,   7,    16,
        2,    17,   7,    17,   2,    18,   7,    18,   2,    19,   7,    19,   2,    20,   7,    20,   2,    21,
        7,    21,   2,    22,   7,    22,   2,    23,   7,    23,   2,    24,   7,    24,   2,    25,   7,    25,
        2,    26,   7,    26,   2,    27,   7,    27,   2,    28,   7,    28,   2,    29,   7,    29,   2,    30,
        7,    30,   2,    31,   7,    31,   2,    32,   7,    32,   2,    33,   7,    33,   2,    34,   7,    34,
        2,    35,   7,    35,   2,    36,   7,    36,   2,    37,   7,    37,   2,    38,   7,    38,   2,    39,
        7,    39,   2,    40,   7,    40,   2,    41,   7,    41,   2,    42,   7,    42,   2,    43,   7,    43,
        2,    44,   7,    44,   2,    45,   7,    45,   2,    46,   7,    46,   2,    47,   7,    47,   2,    48,
        7,    48,   2,    49,   7,    49,   2,    50,   7,    50,   2,    51,   7,    51,   2,    52,   7,    52,
        2,    53,   7,    53,   2,    54,   7,    54,   2,    55,   7,    55,   2,    56,   7,    56,   2,    57,
        7,    57,   2,    58,   7,    58,   2,    59,   7,    59,   2,    60,   7,    60,   2,    61,   7,    61,
        2,    62,   7,    62,   2,    63,   7,    63,   2,    64,   7,    64,   2,    65,   7,    65,   2,    66,
        7,    66,   2,    67,   7,    67,   2,    68,   7,    68,   2,    69,   7,    69,   2,    70,   7,    70,
        2,    71,   7,    71,   2,    72,   7,    72,   2,    73,   7,    73,   2,    74,   7,    74,   2,    75,
        7,    75,   2,    76,   7,    76,   2,    77,   7,    77,   2,    78,   7,    78,   2,    79,   7,    79,
        2,    80,   7,    80,   2,    81,   7,    81,   2,    82,   7,    82,   2,    83,   7,    83,   2,    84,
        7,    84,   2,    85,   7,    85,   2,    86,   7,    86,   2,    87,   7,    87,   2,    88,   7,    88,
        2,    89,   7,    89,   2,    90,   7,    90,   2,    91,   7,    91,   2,    92,   7,    92,   2,    93,
        7,    93,   2,    94,   7,    94,   2,    95,   7,    95,   2,    96,   7,    96,   2,    97,   7,    97,
        2,    98,   7,    98,   2,    99,   7,    99,   2,    100,  7,    100,  2,    101,  7,    101,  2,    102,
        7,    102,  2,    103,  7,    103,  2,    104,  7,    104,  2,    105,  7,    105,  2,    106,  7,    106,
        2,    107,  7,    107,  2,    108,  7,    108,  2,    109,  7,    109,  2,    110,  7,    110,  2,    111,
        7,    111,  2,    112,  7,    112,  2,    113,  7,    113,  2,    114,  7,    114,  2,    115,  7,    115,
        2,    116,  7,    116,  2,    117,  7,    117,  2,    118,  7,    118,  2,    119,  7,    119,  2,    120,
        7,    120,  2,    121,  7,    121,  2,    122,  7,    122,  2,    123,  7,    123,  1,    0,    3,    0,
        250,  8,    0,    1,    0,    5,    0,    253,  8,    0,    10,   0,    12,   0,    256,  9,    0,    1,
        0,    5,    0,    259,  8,    0,    10,   0,    12,   0,    262,  9,    0,    1,    0,    1,    0,    1,
        0,    3,    0,    267,  8,    0,    1,    1,    5,    1,    270,  8,    1,    10,   1,    12,   1,    273,
        9,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,    1,    2,    3,    2,    281,  8,
        2,    1,    2,    1,    2,    1,    2,    3,    2,    286,  8,    2,    1,    2,    1,    2,    1,    3,
        5,    3,    291,  8,    3,    10,   3,    12,   3,    294,  9,    3,    1,    3,    1,    3,    1,    3,
        1,    3,    1,    3,    3,    3,    301,  8,    3,    1,    3,    3,    3,    304,  8,    3,    1,    4,
        1,    4,    1,    4,    1,    4,    1,    4,    3,    4,    311,  8,    4,    1,    5,    1,    5,    1,
        5,    1,    5,    1,    5,    1,    5,    1,    5,    1,    5,    1,    5,    1,    5,    3,    5,    323,
        8,    5,    1,    6,    1,    6,    3,    6,    327,  8,    6,    1,    7,    1,    7,    1,    7,    3,
        7,    332,  8,    7,    1,    7,    1,    7,    3,    7,    336,  8,    7,    1,    7,    1,    7,    3,
        7,    340,  8,    7,    1,    7,    1,    7,    3,    7,    344,  8,    7,    1,    7,    1,    7,    1,
        8,    1,    8,    1,    8,    1,    8,    5,    8,    352,  8,    8,    10,   8,    12,   8,    355,  9,
        8,    1,    8,    1,    8,    1,    9,    5,    9,    360,  8,    9,    10,   9,    12,   9,    363,  9,
        9,    1,    9,    1,    9,    1,    9,    5,    9,    368,  8,    9,    10,   9,    12,   9,    371,  9,
        9,    1,    9,    3,    9,    374,  8,    9,    1,    10,   1,    10,   1,    10,   5,    10,   379,  8,
        10,   10,   10,   12,   10,   382,  9,    10,   1,    11,   1,    11,   1,    11,   1,    11,   3,    11,
        388,  8,    11,   1,    11,   1,    11,   3,    11,   392,  8,    11,   1,    11,   3,    11,   395,  8,
        11,   1,    11,   3,    11,   398,  8,    11,   1,    11,   1,    11,   1,    12,   1,    12,   1,    12,
        5,    12,   405,  8,    12,   10,   12,   12,   12,   408,  9,    12,   1,    13,   5,    13,   411,  8,
        13,   10,   13,   12,   13,   414,  9,    13,   1,    13,   1,    13,   3,    13,   418,  8,    13,   1,
        13,   3,    13,   421,  8,    13,   1,    14,   1,    14,   5,    14,   425,  8,    14,   10,   14,   12,
        14,   428,  9,    14,   1,    15,   1,    15,   1,    15,   3,    15,   433,  8,    15,   1,    15,   1,
        15,   3,    15,   437,  8,    15,   1,    15,   1,    15,   3,    15,   441,  8,    15,   1,    15,   1,
        15,   1,    16,   1,    16,   5,    16,   447,  8,    16,   10,   16,   12,   16,   450,  9,    16,   1,
        16,   1,    16,   1,    17,   1,    17,   5,    17,   456,  8,    17,   10,   17,   12,   17,   459,  9,
        17,   1,    17,   1,    17,   1,    18,   1,    18,   3,    18,   465,  8,    18,   1,    18,   1,    18,
        5,    18,   469,  8,    18,   10,   18,   12,   18,   472,  9,    18,   1,    18,   3,    18,   475,  8,
        18,   1,    19,   1,    19,   1,    19,   1,    19,   1,    19,   1,    19,   1,    19,   1,    19,   1,
        19,   1,    19,   3,    19,   487,  8,    19,   1,    20,   1,    20,   1,    20,   1,    20,   1,    20,
        5,    20,   494,  8,    20,   10,   20,   12,   20,   497,  9,    20,   1,    20,   1,    20,   3,    20,
        501,  8,    20,   1,    20,   1,    20,   1,    21,   1,    21,   3,    21,   507,  8,    21,   1,    22,
        1,    22,   3,    22,   511,  8,    22,   1,    23,   1,    23,   1,    23,   1,    24,   1,    24,   1,
        24,   1,    25,   1,    25,   1,    25,   1,    25,   3,    25,   523,  8,    25,   1,    25,   1,    25,
        1,    26,   1,    26,   1,    26,   1,    26,   1,    27,   5,    27,   532,  8,    27,   10,   27,   12,
        27,   535,  9,    27,   1,    27,   1,    27,   3,    27,   539,  8,    27,   1,    28,   1,    28,   1,
        28,   1,    28,   1,    28,   1,    28,   1,    28,   1,    28,   3,    28,   549,  8,    28,   1,    29,
        1,    29,   1,    29,   1,    29,   5,    29,   555,  8,    29,   10,   29,   12,   29,   558,  9,    29,
        1,    29,   1,    29,   1,    30,   1,    30,   1,    30,   5,    30,   565,  8,    30,   10,   30,   12,
        30,   568,  9,    30,   1,    30,   1,    30,   1,    30,   1,    31,   5,    31,   574,  8,    31,   10,
        31,   12,   31,   577,  9,    31,   1,    31,   1,    31,   1,    32,   1,    32,   1,    32,   1,    32,
        1,    32,   1,    32,   3,    32,   587,  8,    32,   1,    33,   5,    33,   590,  8,    33,   10,   33,
        12,   33,   593,  9,    33,   1,    33,   1,    33,   1,    33,   1,    34,   5,    34,   599,  8,    34,
        10,   34,   12,   34,   602,  9,    34,   1,    34,   1,    34,   1,    34,   1,    34,   1,    34,   5,
        34,   609,  8,    34,   10,   34,   12,   34,   612,  9,    34,   1,    34,   1,    34,   3,    34,   616,
        8,    34,   1,    34,   1,    34,   1,    35,   1,    35,   1,    35,   5,    35,   623,  8,    35,   10,
        35,   12,   35,   626,  9,    35,   1,    36,   1,    36,   1,    36,   3,    36,   631,  8,    36,   1,
        37,   1,    37,   1,    37,   5,    37,   636,  8,    37,   10,   37,   12,   37,   639,  9,    37,   1,
        38,   1,    38,   3,    38,   643,  8,    38,   1,    39,   1,    39,   1,    39,   1,    39,   5,    39,
        649,  8,    39,   10,   39,   12,   39,   652,  9,    39,   1,    39,   3,    39,   655,  8,    39,   3,
        39,   657,  8,    39,   1,    39,   1,    39,   1,    40,   1,    40,   3,    40,   663,  8,    40,   1,
        40,   1,    40,   1,    40,   3,    40,   668,  8,    40,   5,    40,   670,  8,    40,   10,   40,   12,
        40,   673,  9,    40,   1,    41,   1,    41,   5,    41,   677,  8,    41,   10,   41,   12,   41,   680,
        9,    41,   1,    41,   1,    41,   1,    41,   3,    41,   685,  8,    41,   3,    41,   687,  8,    41,
        1,    42,   1,    42,   1,    42,   5,    42,   692,  8,    42,   10,   42,   12,   42,   695,  9,    42,
        1,    43,   1,    43,   3,    43,   699,  8,    43,   1,    43,   1,    43,   1,    43,   3,    43,   704,
        8,    43,   1,    43,   3,    43,   707,  8,    43,   3,    43,   709,  8,    43,   1,    43,   1,    43,
        1,    44,   1,    44,   1,    44,   1,    44,   5,    44,   717,  8,    44,   10,   44,   12,   44,   720,
        9,    44,   1,    44,   1,    44,   1,    45,   1,    45,   1,    45,   5,    45,   727,  8,    45,   10,
        45,   12,   45,   730,  9,    45,   1,    45,   1,    45,   3,    45,   734,  8,    45,   1,    45,   3,
        45,   737,  8,    45,   1,    46,   5,    46,   740,  8,    46,   10,   46,   12,   46,   743,  9,    46,
        1,    46,   1,    46,   1,    46,   1,    47,   5,    47,   749,  8,    47,   10,   47,   12,   47,   752,
        9,    47,   1,    47,   1,    47,   5,    47,   756,  8,    47,   10,   47,   12,   47,   759,  9,    47,
        1,    47,   1,    47,   1,    47,   1,    48,   1,    48,   1,    48,   5,    48,   767,  8,    48,   10,
        48,   12,   48,   770,  9,    48,   1,    49,   5,    49,   773,  8,    49,   10,   49,   12,   49,   776,
        9,    49,   1,    49,   1,    49,   1,    49,   1,    50,   1,    50,   1,    50,   5,    50,   784,  8,
        50,   10,   50,   12,   50,   787,  9,    50,   1,    51,   1,    51,   1,    51,   1,    51,   1,    51,
        1,    51,   1,    51,   3,    51,   796,  8,    51,   1,    52,   1,    52,   1,    53,   1,    53,   1,
        54,   1,    54,   1,    54,   5,    54,   805,  8,    54,   10,   54,   12,   54,   808,  9,    54,   1,
        54,   1,    54,   1,    54,   1,    55,   1,    55,   1,    55,   3,    55,   816,  8,    55,   1,    55,
        1,    55,   1,    55,   3,    55,   821,  8,    55,   1,    55,   3,    55,   824,  8,    55,   1,    56,
        1,    56,   1,    56,   5,    56,   829,  8,    56,   10,   56,   12,   56,   832,  9,    56,   1,    57,
        1,    57,   1,    57,   1,    57,   1,    58,   1,    58,   1,    58,   3,    58,   841,  8,    58,   1,
        59,   1,    59,   1,    59,   1,    59,   5,    59,   847,  8,    59,   10,   59,   12,   59,   850,  9,
        59,   3,    59,   852,  8,    59,   1,    59,   3,    59,   855,  8,    59,   1,    59,   1,    59,   1,
        60,   1,    60,   1,    60,   1,    60,   1,    60,   1,    61,   1,    61,   5,    61,   866,  8,    61,
        10,   61,   12,   61,   869,  9,    61,   1,    61,   1,    61,   1,    62,   5,    62,   874,  8,    62,
        10,   62,   12,   62,   877,  9,    62,   1,    62,   1,    62,   3,    62,   881,  8,    62,   1,    63,
        1,    63,   1,    63,   1,    63,   1,    63,   1,    63,   3,    63,   889,  8,    63,   1,    63,   1,
        63,   3,    63,   893,  8,    63,   1,    63,   1,    63,   3,    63,   897,  8,    63,   1,    63,   1,
        63,   3,    63,   901,  8,    63,   1,    63,   1,    63,   3,    63,   905,  8,    63,   3,    63,   907,
        8,    63,   1,    64,   1,    64,   3,    64,   911,  8,    64,   1,    65,   1,    65,   1,    65,   1,
        65,   3,    65,   917,  8,    65,   1,    66,   1,    66,   1,    67,   1,    67,   1,    67,   1,    68,
        3,    68,   925,  8,    68,   1,    68,   1,    68,   1,    68,   1,    68,   1,    69,   1,    69,   5,
        69,   933,  8,    69,   10,   69,   12,   69,   936,  9,    69,   1,    69,   1,    69,   1,    70,   1,
        70,   5,    70,   942,  8,    70,   10,   70,   12,   70,   945,  9,    70,   1,    70,   1,    70,   1,
        70,   1,    70,   1,    70,   1,    70,   1,    70,   3,    70,   954,  8,    70,   1,    70,   1,    70,
        1,    70,   1,    70,   1,    70,   1,    70,   3,    70,   962,  8,    70,   1,    70,   1,    70,   1,
        70,   1,    70,   1,    70,   1,    70,   1,    70,   1,    70,   1,    70,   1,    70,   1,    70,   1,
        70,   3,    70,   976,  8,    70,   1,    71,   1,    71,   1,    72,   1,    72,   1,    72,   3,    72,
        983,  8,    72,   1,    72,   1,    72,   1,    72,   3,    72,   988,  8,    72,   1,    72,   1,    72,
        1,    73,   1,    73,   3,    73,   994,  8,    73,   1,    73,   1,    73,   1,    74,   1,    74,   1,
        74,   5,    74,   1001, 8,    74,   10,   74,   12,   74,   1004, 9,    74,   1,    75,   1,    75,   1,
        75,   1,    76,   1,    76,   5,    76,   1011, 8,    76,   10,   76,   12,   76,   1014, 9,    76,   1,
        76,   1,    76,   1,    77,   1,    77,   5,    77,   1020, 8,    77,   10,   77,   12,   77,   1023, 9,
        77,   1,    77,   1,    77,   1,    78,   1,    78,   1,    78,   1,    78,   1,    78,   3,    78,   1032,
        8,    78,   1,    79,   5,    79,   1035, 8,    79,   10,   79,   12,   79,   1038, 9,    79,   1,    79,
        1,    79,   1,    79,   1,    79,   1,    79,   1,    79,   1,    79,   1,    79,   3,    79,   1048, 8,
        79,   1,    80,   1,    80,   1,    81,   5,    81,   1053, 8,    81,   10,   81,   12,   81,   1056, 9,
        81,   1,    81,   1,    81,   1,    81,   3,    81,   1061, 8,    81,   1,    81,   3,    81,   1064, 8,
        81,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   3,    82,   1071, 8,    82,   1,    82,
        1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   3,    82,   1080, 8,    82,   1,
        82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,
        82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,
        82,   4,    82,   1101, 8,    82,   11,   82,   12,   82,   1102, 1,    82,   3,    82,   1106, 8,    82,
        1,    82,   3,    82,   1109, 8,    82,   1,    82,   1,    82,   1,    82,   1,    82,   5,    82,   1115,
        8,    82,   10,   82,   12,   82,   1118, 9,    82,   1,    82,   3,    82,   1121, 8,    82,   1,    82,
        1,    82,   1,    82,   1,    82,   5,    82,   1127, 8,    82,   10,   82,   12,   82,   1130, 9,    82,
        1,    82,   5,    82,   1133, 8,    82,   10,   82,   12,   82,   1136, 9,    82,   1,    82,   1,    82,
        1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   3,    82,   1146, 8,    82,   1,
        82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   3,    82,   1155, 8,    82,
        1,    82,   1,    82,   1,    82,   3,    82,   1160, 8,    82,   1,    82,   1,    82,   1,    82,   1,
        82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   1,    82,   3,    82,   1173,
        8,    82,   1,    82,   1,    82,   1,    82,   1,    82,   3,    82,   1179, 8,    82,   1,    83,   1,
        83,   1,    83,   5,    83,   1184, 8,    83,   10,   83,   12,   83,   1187, 9,    83,   1,    83,   1,
        83,   1,    83,   1,    83,   1,    83,   1,    84,   1,    84,   1,    84,   5,    84,   1197, 8,    84,
        10,   84,   12,   84,   1200, 9,    84,   1,    85,   1,    85,   1,    85,   1,    86,   1,    86,   1,
        86,   3,    86,   1208, 8,    86,   1,    86,   1,    86,   1,    87,   1,    87,   1,    87,   5,    87,
        1215, 8,    87,   10,   87,   12,   87,   1218, 9,    87,   1,    88,   5,    88,   1221, 8,    88,   10,
        88,   12,   88,   1224, 9,    88,   1,    88,   1,    88,   1,    88,   1,    88,   1,    88,   3,    88,
        1231, 8,    88,   1,    88,   1,    88,   1,    88,   1,    88,   3,    88,   1237, 8,    88,   1,    89,
        4,    89,   1240, 8,    89,   11,   89,   12,   89,   1241, 1,    89,   4,    89,   1245, 8,    89,   11,
        89,   12,   89,   1246, 1,    90,   1,    90,   1,    90,   1,    90,   1,    90,   1,    90,   3,    90,
        1255, 8,    90,   1,    90,   1,    90,   1,    90,   3,    90,   1260, 8,    90,   1,    91,   1,    91,
        3,    91,   1264, 8,    91,   1,    91,   1,    91,   3,    91,   1268, 8,    91,   1,    91,   1,    91,
        3,    91,   1272, 8,    91,   3,    91,   1274, 8,    91,   1,    92,   1,    92,   3,    92,   1278, 8,
        92,   1,    93,   5,    93,   1281, 8,    93,   10,   93,   12,   93,   1284, 9,    93,   1,    93,   1,
        93,   3,    93,   1288, 8,    93,   1,    93,   1,    93,   1,    93,   1,    93,   1,    94,   1,    94,
        1,    94,   1,    94,   1,    95,   1,    95,   1,    95,   5,    95,   1301, 8,    95,   10,   95,   12,
        95,   1304, 9,    95,   1,    96,   1,    96,   1,    96,   3,    96,   1309, 8,    96,   1,    96,   1,
        96,   1,    96,   1,    96,   1,    96,   3,    96,   1316, 8,    96,   1,    96,   1,    96,   1,    96,
        1,    96,   3,    96,   1322, 8,    96,   1,    96,   3,    96,   1325, 8,    96,   1,    97,   1,    97,
        1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   5,    97,   1334, 8,    97,   10,   97,   12,
        97,   1337, 9,    97,   1,    97,   1,    97,   1,    97,   5,    97,   1342, 8,    97,   10,   97,   12,
        97,   1345, 9,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,
        1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   3,    97,   1359, 8,    97,   1,    97,   1,
        97,   3,    97,   1363, 8,    97,   1,    97,   1,    97,   1,    97,   3,    97,   1368, 8,    97,   1,
        97,   1,    97,   3,    97,   1372, 8,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,
        1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,
        3,    97,   1388, 8,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,
        97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,
        97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,
        97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,
        97,   1,    97,   1,    97,   1,    97,   1,    97,   3,    97,   1428, 8,    97,   1,    97,   1,    97,
        1,    97,   1,    97,   3,    97,   1434, 8,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,
        97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   1,    97,   3,    97,   1447, 8,    97,
        1,    97,   1,    97,   1,    97,   3,    97,   1452, 8,    97,   1,    97,   5,    97,   1455, 8,    97,
        10,   97,   12,   97,   1458, 9,    97,   1,    98,   5,    98,   1461, 8,    98,   10,   98,   12,   98,
        1464, 9,    98,   1,    98,   1,    98,   5,    98,   1468, 8,    98,   10,   98,   12,   98,   1471, 9,
        98,   1,    98,   1,    98,   1,    99,   1,    99,   1,    99,   1,    99,   1,    100,  1,    100,  1,
        100,  3,    100,  1482, 8,    100,  1,    100,  1,    100,  1,    100,  1,    100,  1,    100,  5,    100,
        1489, 8,    100,  10,   100,  12,   100,  1492, 9,    100,  1,    100,  1,    100,  1,    100,  1,    100,
        3,    100,  1498, 8,    100,  1,    100,  3,    100,  1501, 8,    100,  1,    101,  1,    101,  3,    101,
        1505, 8,    101,  1,    102,  1,    102,  1,    102,  1,    102,  1,    102,  1,    102,  1,    102,  1,
        102,  1,    102,  1,    102,  1,    102,  1,    102,  1,    102,  1,    102,  1,    102,  1,    102,  3,
        102,  1523, 8,    102,  3,    102,  1525, 8,    102,  1,    103,  1,    103,  1,    103,  1,    103,  5,
        103,  1531, 8,    103,  10,   103,  12,   103,  1534, 9,    103,  1,    103,  1,    103,  1,    104,  1,
        104,  1,    104,  1,    104,  3,    104,  1542, 8,    104,  1,    104,  1,    104,  1,    104,  1,    104,
        1,    104,  3,    104,  1549, 8,    104,  1,    105,  1,    105,  1,    105,  1,    105,  1,    105,  1,
        105,  5,    105,  1557, 8,    105,  10,   105,  12,   105,  1560, 9,    105,  1,    105,  1,    105,  5,
        105,  1564, 8,    105,  10,   105,  12,   105,  1567, 9,    105,  1,    105,  1,    105,  1,    105,  5,
        105,  1572, 8,    105,  10,   105,  12,   105,  1575, 9,    105,  3,    105,  1577, 8,    105,  1,    105,
        1,    105,  1,    105,  5,    105,  1582, 8,    105,  10,   105,  12,   105,  1585, 9,    105,  1,    106,
        1,    106,  5,    106,  1589, 8,    106,  10,   106,  12,   106,  1592, 9,    106,  3,    106,  1594, 8,
        106,  1,    107,  1,    107,  1,    107,  3,    107,  1599, 8,    107,  1,    107,  5,    107,  1602, 8,
        107,  10,   107,  12,   107,  1605, 9,    107,  1,    107,  1,    107,  3,    107,  1609, 8,    107,  1,
        108,  1,    108,  1,    108,  1,    108,  1,    108,  1,    108,  1,    108,  3,    108,  1618, 8,    108,
        3,    108,  1620, 8,    108,  1,    109,  1,    109,  3,    109,  1624, 8,    109,  1,    109,  1,    109,
        1,    109,  3,    109,  1629, 8,    109,  5,    109,  1631, 8,    109,  10,   109,  12,   109,  1634, 9,
        109,  1,    109,  3,    109,  1637, 8,    109,  1,    110,  1,    110,  3,    110,  1641, 8,    110,  1,
        110,  1,    110,  1,    111,  1,    111,  1,    111,  1,    111,  5,    111,  1649, 8,    111,  10,   111,
        12,   111,  1652, 9,    111,  1,    111,  1,    111,  1,    111,  1,    111,  1,    111,  1,    111,  1,
        111,  5,    111,  1661, 8,    111,  10,   111,  12,   111,  1664, 9,    111,  1,    111,  1,    111,  5,
        111,  1668, 8,    111,  10,   111,  12,   111,  1671, 9,    111,  3,    111,  1673, 8,    111,  1,    112,
        1,    112,  3,    112,  1677, 8,    112,  1,    113,  1,    113,  1,    113,  1,    114,  1,    114,  1,
        114,  3,    114,  1685, 8,    114,  1,    115,  1,    115,  1,    115,  3,    115,  1690, 8,    115,  1,
        116,  1,    116,  1,    116,  1,    116,  1,    117,  1,    117,  1,    117,  5,    117,  1699, 8,    117,
        10,   117,  12,   117,  1702, 9,    117,  1,    118,  5,    118,  1705, 8,    118,  10,   118,  12,   118,
        1708, 9,    118,  1,    118,  1,    118,  3,    118,  1712, 8,    118,  1,    118,  5,    118,  1715, 8,
        118,  10,   118,  12,   118,  1718, 9,    118,  1,    118,  1,    118,  5,    118,  1722, 8,    118,  10,
        118,  12,   118,  1725, 9,    118,  1,    119,  1,    119,  1,    120,  1,    120,  1,    120,  1,    120,
        5,    120,  1733, 8,    120,  10,   120,  12,   120,  1736, 9,    120,  1,    120,  1,    120,  1,    121,
        1,    121,  1,    121,  3,    121,  1743, 8,    121,  1,    121,  1,    121,  3,    121,  1747, 8,    121,
        3,    121,  1749, 8,    121,  1,    122,  1,    122,  1,    122,  1,    122,  1,    122,  3,    122,  1756,
        8,    122,  1,    123,  1,    123,  3,    123,  1760, 8,    123,  1,    123,  1,    123,  1,    123,  0,
        2,    194,  210,  124,  0,    2,    4,    6,    8,    10,   12,   14,   16,   18,   20,   22,   24,   26,
        28,   30,   32,   34,   36,   38,   40,   42,   44,   46,   48,   50,   52,   54,   56,   58,   60,   62,
        64,   66,   68,   70,   72,   74,   76,   78,   80,   82,   84,   86,   88,   90,   92,   94,   96,   98,
        100,  102,  104,  106,  108,  110,  112,  114,  116,  118,  120,  122,  124,  126,  128,  130,  132,  134,
        136,  138,  140,  142,  144,  146,  148,  150,  152,  154,  156,  158,  160,  162,  164,  166,  168,  170,
        172,  174,  176,  178,  180,  182,  184,  186,  188,  190,  192,  194,  196,  198,  200,  202,  204,  206,
        208,  210,  212,  214,  216,  218,  220,  222,  224,  226,  228,  230,  232,  234,  236,  238,  240,  242,
        244,  246,  0,    15,   2,    0,    17,   17,   40,   40,   1,    0,    67,   70,   1,    0,    71,   72,
        2,    0,    38,   38,   60,   60,   2,    0,    51,   65,   128,  128,  1,    0,    100,  103,  1,    0,
        90,   91,   2,    0,    104,  105,  109,  109,  1,    0,    102,  103,  2,    0,    88,   89,   95,   96,
        2,    0,    94,   94,   97,   97,   2,    0,    87,   87,   110,  120,  1,    0,    100,  101,  2,    0,
        93,   93,   121,  121,  8,    0,    3,    3,    5,    5,    8,    8,    14,   14,   20,   20,   27,   27,
        29,   29,   37,   37,   1964, 0,    266,  1,    0,    0,    0,    2,    271,  1,    0,    0,    0,    4,
        278,  1,    0,    0,    0,    6,    303,  1,    0,    0,    0,    8,    310,  1,    0,    0,    0,    10,
        322,  1,    0,    0,    0,    12,   326,  1,    0,    0,    0,    14,   328,  1,    0,    0,    0,    16,
        347,  1,    0,    0,    0,    18,   361,  1,    0,    0,    0,    20,   375,  1,    0,    0,    0,    22,
        383,  1,    0,    0,    0,    24,   401,  1,    0,    0,    0,    26,   412,  1,    0,    0,    0,    28,
        422,  1,    0,    0,    0,    30,   429,  1,    0,    0,    0,    32,   444,  1,    0,    0,    0,    34,
        453,  1,    0,    0,    0,    36,   474,  1,    0,    0,    0,    38,   486,  1,    0,    0,    0,    40,
        488,  1,    0,    0,    0,    42,   506,  1,    0,    0,    0,    44,   510,  1,    0,    0,    0,    46,
        512,  1,    0,    0,    0,    48,   515,  1,    0,    0,    0,    50,   518,  1,    0,    0,    0,    52,
        526,  1,    0,    0,    0,    54,   538,  1,    0,    0,    0,    56,   548,  1,    0,    0,    0,    58,
        550,  1,    0,    0,    0,    60,   561,  1,    0,    0,    0,    62,   575,  1,    0,    0,    0,    64,
        586,  1,    0,    0,    0,    66,   591,  1,    0,    0,    0,    68,   600,  1,    0,    0,    0,    70,
        619,  1,    0,    0,    0,    72,   627,  1,    0,    0,    0,    74,   632,  1,    0,    0,    0,    76,
        642,  1,    0,    0,    0,    78,   644,  1,    0,    0,    0,    80,   660,  1,    0,    0,    0,    82,
        686,  1,    0,    0,    0,    84,   688,  1,    0,    0,    0,    86,   696,  1,    0,    0,    0,    88,
        712,  1,    0,    0,    0,    90,   736,  1,    0,    0,    0,    92,   741,  1,    0,    0,    0,    94,
        750,  1,    0,    0,    0,    96,   763,  1,    0,    0,    0,    98,   774,  1,    0,    0,    0,    100,
        780,  1,    0,    0,    0,    102,  795,  1,    0,    0,    0,    104,  797,  1,    0,    0,    0,    106,
        799,  1,    0,    0,    0,    108,  806,  1,    0,    0,    0,    110,  815,  1,    0,    0,    0,    112,
        825,  1,    0,    0,    0,    114,  833,  1,    0,    0,    0,    116,  840,  1,    0,    0,    0,    118,
        842,  1,    0,    0,    0,    120,  858,  1,    0,    0,    0,    122,  863,  1,    0,    0,    0,    124,
        880,  1,    0,    0,    0,    126,  906,  1,    0,    0,    0,    128,  910,  1,    0,    0,    0,    130,
        912,  1,    0,    0,    0,    132,  918,  1,    0,    0,    0,    134,  920,  1,    0,    0,    0,    136,
        924,  1,    0,    0,    0,    138,  930,  1,    0,    0,    0,    140,  975,  1,    0,    0,    0,    142,
        977,  1,    0,    0,    0,    144,  979,  1,    0,    0,    0,    146,  991,  1,    0,    0,    0,    148,
        997,  1,    0,    0,    0,    150,  1005, 1,    0,    0,    0,    152,  1008, 1,    0,    0,    0,    154,
        1017, 1,    0,    0,    0,    156,  1031, 1,    0,    0,    0,    158,  1036, 1,    0,    0,    0,    160,
        1049, 1,    0,    0,    0,    162,  1063, 1,    0,    0,    0,    164,  1178, 1,    0,    0,    0,    166,
        1180, 1,    0,    0,    0,    168,  1193, 1,    0,    0,    0,    170,  1201, 1,    0,    0,    0,    172,
        1204, 1,    0,    0,    0,    174,  1211, 1,    0,    0,    0,    176,  1236, 1,    0,    0,    0,    178,
        1239, 1,    0,    0,    0,    180,  1259, 1,    0,    0,    0,    182,  1273, 1,    0,    0,    0,    184,
        1277, 1,    0,    0,    0,    186,  1282, 1,    0,    0,    0,    188,  1293, 1,    0,    0,    0,    190,
        1297, 1,    0,    0,    0,    192,  1324, 1,    0,    0,    0,    194,  1371, 1,    0,    0,    0,    196,
        1462, 1,    0,    0,    0,    198,  1474, 1,    0,    0,    0,    200,  1500, 1,    0,    0,    0,    202,
        1504, 1,    0,    0,    0,    204,  1524, 1,    0,    0,    0,    206,  1526, 1,    0,    0,    0,    208,
        1548, 1,    0,    0,    0,    210,  1576, 1,    0,    0,    0,    212,  1593, 1,    0,    0,    0,    214,
        1598, 1,    0,    0,    0,    216,  1619, 1,    0,    0,    0,    218,  1636, 1,    0,    0,    0,    220,
        1638, 1,    0,    0,    0,    222,  1644, 1,    0,    0,    0,    224,  1674, 1,    0,    0,    0,    226,
        1678, 1,    0,    0,    0,    228,  1684, 1,    0,    0,    0,    230,  1689, 1,    0,    0,    0,    232,
        1691, 1,    0,    0,    0,    234,  1695, 1,    0,    0,    0,    236,  1706, 1,    0,    0,    0,    238,
        1726, 1,    0,    0,    0,    240,  1728, 1,    0,    0,    0,    242,  1748, 1,    0,    0,    0,    244,
        1755, 1,    0,    0,    0,    246,  1757, 1,    0,    0,    0,    248,  250,  3,    2,    1,    0,    249,
        248,  1,    0,    0,    0,    249,  250,  1,    0,    0,    0,    250,  254,  1,    0,    0,    0,    251,
        253,  3,    4,    2,    0,    252,  251,  1,    0,    0,    0,    253,  256,  1,    0,    0,    0,    254,
        252,  1,    0,    0,    0,    254,  255,  1,    0,    0,    0,    255,  260,  1,    0,    0,    0,    256,
        254,  1,    0,    0,    0,    257,  259,  3,    6,    3,    0,    258,  257,  1,    0,    0,    0,    259,
        262,  1,    0,    0,    0,    260,  258,  1,    0,    0,    0,    260,  261,  1,    0,    0,    0,    261,
        267,  1,    0,    0,    0,    262,  260,  1,    0,    0,    0,    263,  264,  3,    136,  68,   0,    264,
        265,  5,    0,    0,    1,    265,  267,  1,    0,    0,    0,    266,  249,  1,    0,    0,    0,    266,
        263,  1,    0,    0,    0,    267,  1,    1,    0,    0,    0,    268,  270,  3,    110,  55,   0,    269,
        268,  1,    0,    0,    0,    270,  273,  1,    0,    0,    0,    271,  269,  1,    0,    0,    0,    271,
        272,  1,    0,    0,    0,    272,  274,  1,    0,    0,    0,    273,  271,  1,    0,    0,    0,    274,
        275,  5,    32,   0,    0,    275,  276,  3,    100,  50,   0,    276,  277,  5,    84,   0,    0,    277,
        3,    1,    0,    0,    0,    278,  280,  5,    25,   0,    0,    279,  281,  5,    38,   0,    0,    280,
        279,  1,    0,    0,    0,    280,  281,  1,    0,    0,    0,    281,  282,  1,    0,    0,    0,    282,
        285,  3,    100,  50,   0,    283,  284,  5,    86,   0,    0,    284,  286,  5,    104,  0,    0,    285,
        283,  1,    0,    0,    0,    285,  286,  1,    0,    0,    0,    286,  287,  1,    0,    0,    0,    287,
        288,  5,    84,   0,    0,    288,  5,    1,    0,    0,    0,    289,  291,  3,    10,   5,    0,    290,
        289,  1,    0,    0,    0,    291,  294,  1,    0,    0,    0,    292,  290,  1,    0,    0,    0,    292,
        293,  1,    0,    0,    0,    293,  300,  1,    0,    0,    0,    294,  292,  1,    0,    0,    0,    295,
        301,  3,    14,   7,    0,    296,  301,  3,    22,   11,   0,    297,  301,  3,    30,   15,   0,    298,
        301,  3,    120,  60,   0,    299,  301,  3,    144,  72,   0,    300,  295,  1,    0,    0,    0,    300,
        296,  1,    0,    0,    0,    300,  297,  1,    0,    0,    0,    300,  298,  1,    0,    0,    0,    300,
        299,  1,    0,    0,    0,    301,  304,  1,    0,    0,    0,    302,  304,  5,    84,   0,    0,    303,
        292,  1,    0,    0,    0,    303,  302,  1,    0,    0,    0,    304,  7,    1,    0,    0,    0,    305,
        311,  3,    10,   5,    0,    306,  311,  5,    30,   0,    0,    307,  311,  5,    42,   0,    0,    308,
        311,  5,    46,   0,    0,    309,  311,  5,    49,   0,    0,    310,  305,  1,    0,    0,    0,    310,
        306,  1,    0,    0,    0,    310,  307,  1,    0,    0,    0,    310,  308,  1,    0,    0,    0,    310,
        309,  1,    0,    0,    0,    311,  9,    1,    0,    0,    0,    312,  323,  3,    110,  55,   0,    313,
        323,  5,    35,   0,    0,    314,  323,  5,    34,   0,    0,    315,  323,  5,    33,   0,    0,    316,
        323,  5,    38,   0,    0,    317,  323,  5,    1,    0,    0,    318,  323,  5,    18,   0,    0,    319,
        323,  5,    39,   0,    0,    320,  323,  5,    64,   0,    0,    321,  323,  5,    66,   0,    0,    322,
        312,  1,    0,    0,    0,    322,  313,  1,    0,    0,    0,    322,  314,  1,    0,    0,    0,    322,
        315,  1,    0,    0,    0,    322,  316,  1,    0,    0,    0,    322,  317,  1,    0,    0,    0,    322,
        318,  1,    0,    0,    0,    322,  319,  1,    0,    0,    0,    322,  320,  1,    0,    0,    0,    322,
        321,  1,    0,    0,    0,    323,  11,   1,    0,    0,    0,    324,  327,  5,    18,   0,    0,    325,
        327,  3,    110,  55,   0,    326,  324,  1,    0,    0,    0,    326,  325,  1,    0,    0,    0,    327,
        13,   1,    0,    0,    0,    328,  329,  5,    9,    0,    0,    329,  331,  3,    160,  80,   0,    330,
        332,  3,    16,   8,    0,    331,  330,  1,    0,    0,    0,    331,  332,  1,    0,    0,    0,    332,
        335,  1,    0,    0,    0,    333,  334,  5,    17,   0,    0,    334,  336,  3,    236,  118,  0,    335,
        333,  1,    0,    0,    0,    335,  336,  1,    0,    0,    0,    336,  339,  1,    0,    0,    0,    337,
        338,  5,    24,   0,    0,    338,  340,  3,    234,  117,  0,    339,  337,  1,    0,    0,    0,    339,
        340,  1,    0,    0,    0,    340,  343,  1,    0,    0,    0,    341,  342,  5,    65,   0,    0,    342,
        344,  3,    234,  117,  0,    343,  341,  1,    0,    0,    0,    343,  344,  1,    0,    0,    0,    344,
        345,  1,    0,    0,    0,    345,  346,  3,    32,   16,   0,    346,  15,   1,    0,    0,    0,    347,
        348,  5,    89,   0,    0,    348,  353,  3,    18,   9,    0,    349,  350,  5,    85,   0,    0,    350,
        352,  3,    18,   9,    0,    351,  349,  1,    0,    0,    0,    352,  355,  1,    0,    0,    0,    353,
        351,  1,    0,    0,    0,    353,  354,  1,    0,    0,    0,    354,  356,  1,    0,    0,    0,    355,
        353,  1,    0,    0,    0,    356,  357,  5,    88,   0,    0,    357,  17,   1,    0,    0,    0,    358,
        360,  3,    110,  55,   0,    359,  358,  1,    0,    0,    0,    360,  363,  1,    0,    0,    0,    361,
        359,  1,    0,    0,    0,    361,  362,  1,    0,    0,    0,    362,  364,  1,    0,    0,    0,    363,
        361,  1,    0,    0,    0,    364,  373,  3,    160,  80,   0,    365,  369,  5,    17,   0,    0,    366,
        368,  3,    110,  55,   0,    367,  366,  1,    0,    0,    0,    368,  371,  1,    0,    0,    0,    369,
        367,  1,    0,    0,    0,    369,  370,  1,    0,    0,    0,    370,  372,  1,    0,    0,    0,    371,
        369,  1,    0,    0,    0,    372,  374,  3,    20,   10,   0,    373,  365,  1,    0,    0,    0,    373,
        374,  1,    0,    0,    0,    374,  19,   1,    0,    0,    0,    375,  380,  3,    236,  118,  0,    376,
        377,  5,    106,  0,    0,    377,  379,  3,    236,  118,  0,    378,  376,  1,    0,    0,    0,    379,
        382,  1,    0,    0,    0,    380,  378,  1,    0,    0,    0,    380,  381,  1,    0,    0,    0,    381,
        21,   1,    0,    0,    0,    382,  380,  1,    0,    0,    0,    383,  384,  5,    16,   0,    0,    384,
        387,  3,    160,  80,   0,    385,  386,  5,    24,   0,    0,    386,  388,  3,    234,  117,  0,    387,
        385,  1,    0,    0,    0,    387,  388,  1,    0,    0,    0,    388,  389,  1,    0,    0,    0,    389,
        391,  5,    80,   0,    0,    390,  392,  3,    24,   12,   0,    391,  390,  1,    0,    0,    0,    391,
        392,  1,    0,    0,    0,    392,  394,  1,    0,    0,    0,    393,  395,  5,    85,   0,    0,    394,
        393,  1,    0,    0,    0,    394,  395,  1,    0,    0,    0,    395,  397,  1,    0,    0,    0,    396,
        398,  3,    28,   14,   0,    397,  396,  1,    0,    0,    0,    397,  398,  1,    0,    0,    0,    398,
        399,  1,    0,    0,    0,    399,  400,  5,    81,   0,    0,    400,  23,   1,    0,    0,    0,    401,
        406,  3,    26,   13,   0,    402,  403,  5,    85,   0,    0,    403,  405,  3,    26,   13,   0,    404,
        402,  1,    0,    0,    0,    405,  408,  1,    0,    0,    0,    406,  404,  1,    0,    0,    0,    406,
        407,  1,    0,    0,    0,    407,  25,   1,    0,    0,    0,    408,  406,  1,    0,    0,    0,    409,
        411,  3,    110,  55,   0,    410,  409,  1,    0,    0,    0,    411,  414,  1,    0,    0,    0,    412,
        410,  1,    0,    0,    0,    412,  413,  1,    0,    0,    0,    413,  415,  1,    0,    0,    0,    414,
        412,  1,    0,    0,    0,    415,  417,  3,    160,  80,   0,    416,  418,  3,    246,  123,  0,    417,
        416,  1,    0,    0,    0,    417,  418,  1,    0,    0,    0,    418,  420,  1,    0,    0,    0,    419,
        421,  3,    32,   16,   0,    420,  419,  1,    0,    0,    0,    420,  421,  1,    0,    0,    0,    421,
        27,   1,    0,    0,    0,    422,  426,  5,    84,   0,    0,    423,  425,  3,    36,   18,   0,    424,
        423,  1,    0,    0,    0,    425,  428,  1,    0,    0,    0,    426,  424,  1,    0,    0,    0,    426,
        427,  1,    0,    0,    0,    427,  29,   1,    0,    0,    0,    428,  426,  1,    0,    0,    0,    429,
        430,  5,    28,   0,    0,    430,  432,  3,    160,  80,   0,    431,  433,  3,    16,   8,    0,    432,
        431,  1,    0,    0,    0,    432,  433,  1,    0,    0,    0,    433,  436,  1,    0,    0,    0,    434,
        435,  5,    17,   0,    0,    435,  437,  3,    234,  117,  0,    436,  434,  1,    0,    0,    0,    436,
        437,  1,    0,    0,    0,    437,  440,  1,    0,    0,    0,    438,  439,  5,    65,   0,    0,    439,
        441,  3,    234,  117,  0,    440,  438,  1,    0,    0,    0,    440,  441,  1,    0,    0,    0,    441,
        442,  1,    0,    0,    0,    442,  443,  3,    34,   17,   0,    443,  31,   1,    0,    0,    0,    444,
        448,  5,    80,   0,    0,    445,  447,  3,    36,   18,   0,    446,  445,  1,    0,    0,    0,    447,
        450,  1,    0,    0,    0,    448,  446,  1,    0,    0,    0,    448,  449,  1,    0,    0,    0,    449,
        451,  1,    0,    0,    0,    450,  448,  1,    0,    0,    0,    451,  452,  5,    81,   0,    0,    452,
        33,   1,    0,    0,    0,    453,  457,  5,    80,   0,    0,    454,  456,  3,    54,   27,   0,    455,
        454,  1,    0,    0,    0,    456,  459,  1,    0,    0,    0,    457,  455,  1,    0,    0,    0,    457,
        458,  1,    0,    0,    0,    458,  460,  1,    0,    0,    0,    459,  457,  1,    0,    0,    0,    460,
        461,  5,    81,   0,    0,    461,  35,   1,    0,    0,    0,    462,  475,  5,    84,   0,    0,    463,
        465,  5,    38,   0,    0,    464,  463,  1,    0,    0,    0,    464,  465,  1,    0,    0,    0,    465,
        466,  1,    0,    0,    0,    466,  475,  3,    154,  77,   0,    467,  469,  3,    8,    4,    0,    468,
        467,  1,    0,    0,    0,    469,  472,  1,    0,    0,    0,    470,  468,  1,    0,    0,    0,    470,
        471,  1,    0,    0,    0,    471,  473,  1,    0,    0,    0,    472,  470,  1,    0,    0,    0,    473,
        475,  3,    38,   19,   0,    474,  462,  1,    0,    0,    0,    474,  464,  1,    0,    0,    0,    474,
        470,  1,    0,    0,    0,    475,  37,   1,    0,    0,    0,    476,  487,  3,    40,   20,   0,    477,
        487,  3,    46,   23,   0,    478,  487,  3,    52,   26,   0,    479,  487,  3,    50,   25,   0,    480,
        487,  3,    48,   24,   0,    481,  487,  3,    30,   15,   0,    482,  487,  3,    120,  60,   0,    483,
        487,  3,    14,   7,    0,    484,  487,  3,    22,   11,   0,    485,  487,  3,    144,  72,   0,    486,
        476,  1,    0,    0,    0,    486,  477,  1,    0,    0,    0,    486,  478,  1,    0,    0,    0,    486,
        479,  1,    0,    0,    0,    486,  480,  1,    0,    0,    0,    486,  481,  1,    0,    0,    0,    486,
        482,  1,    0,    0,    0,    486,  483,  1,    0,    0,    0,    486,  484,  1,    0,    0,    0,    486,
        485,  1,    0,    0,    0,    487,  39,   1,    0,    0,    0,    488,  489,  3,    44,   22,   0,    489,
        490,  3,    160,  80,   0,    490,  495,  3,    86,   43,   0,    491,  492,  5,    82,   0,    0,    492,
        494,  5,    83,   0,    0,    493,  491,  1,    0,    0,    0,    494,  497,  1,    0,    0,    0,    495,
        493,  1,    0,    0,    0,    495,  496,  1,    0,    0,    0,    496,  500,  1,    0,    0,    0,    497,
        495,  1,    0,    0,    0,    498,  499,  5,    45,   0,    0,    499,  501,  3,    84,   42,   0,    500,
        498,  1,    0,    0,    0,    500,  501,  1,    0,    0,    0,    501,  502,  1,    0,    0,    0,    502,
        503,  3,    42,   21,   0,    503,  41,   1,    0,    0,    0,    504,  507,  3,    154,  77,   0,    505,
        507,  5,    84,   0,    0,    506,  504,  1,    0,    0,    0,    506,  505,  1,    0,    0,    0,    507,
        43,   1,    0,    0,    0,    508,  511,  3,    236,  118,  0,    509,  511,  5,    48,   0,    0,    510,
        508,  1,    0,    0,    0,    510,  509,  1,    0,    0,    0,    511,  45,   1,    0,    0,    0,    512,
        513,  3,    16,   8,    0,    513,  514,  3,    40,   20,   0,    514,  47,   1,    0,    0,    0,    515,
        516,  3,    16,   8,    0,    516,  517,  3,    50,   25,   0,    517,  49,   1,    0,    0,    0,    518,
        519,  3,    160,  80,   0,    519,  522,  3,    86,   43,   0,    520,  521,  5,    45,   0,    0,    521,
        523,  3,    84,   42,   0,    522,  520,  1,    0,    0,    0,    522,  523,  1,    0,    0,    0,    523,
        524,  1,    0,    0,    0,    524,  525,  3,    154,  77,   0,    525,  51,   1,    0,    0,    0,    526,
        527,  3,    236,  118,  0,    527,  528,  3,    70,   35,   0,    528,  529,  5,    84,   0,    0,    529,
        53,   1,    0,    0,    0,    530,  532,  3,    8,    4,    0,    531,  530,  1,    0,    0,    0,    532,
        535,  1,    0,    0,    0,    533,  531,  1,    0,    0,    0,    533,  534,  1,    0,    0,    0,    534,
        536,  1,    0,    0,    0,    535,  533,  1,    0,    0,    0,    536,  539,  3,    56,   28,   0,    537,
        539,  5,    84,   0,    0,    538,  533,  1,    0,    0,    0,    538,  537,  1,    0,    0,    0,    539,
        55,   1,    0,    0,    0,    540,  549,  3,    58,   29,   0,    541,  549,  3,    62,   31,   0,    542,
        549,  3,    66,   33,   0,    543,  549,  3,    30,   15,   0,    544,  549,  3,    120,  60,   0,    545,
        549,  3,    14,   7,    0,    546,  549,  3,    22,   11,   0,    547,  549,  3,    144,  72,   0,    548,
        540,  1,    0,    0,    0,    548,  541,  1,    0,    0,    0,    548,  542,  1,    0,    0,    0,    548,
        543,  1,    0,    0,    0,    548,  544,  1,    0,    0,    0,    548,  545,  1,    0,    0,    0,    548,
        546,  1,    0,    0,    0,    548,  547,  1,    0,    0,    0,    549,  57,   1,    0,    0,    0,    550,
        551,  3,    236,  118,  0,    551,  556,  3,    60,   30,   0,    552,  553,  5,    85,   0,    0,    553,
        555,  3,    60,   30,   0,    554,  552,  1,    0,    0,    0,    555,  558,  1,    0,    0,    0,    556,
        554,  1,    0,    0,    0,    556,  557,  1,    0,    0,    0,    557,  559,  1,    0,    0,    0,    558,
        556,  1,    0,    0,    0,    559,  560,  5,    84,   0,    0,    560,  59,   1,    0,    0,    0,    561,
        566,  3,    160,  80,   0,    562,  563,  5,    82,   0,    0,    563,  565,  5,    83,   0,    0,    564,
        562,  1,    0,    0,    0,    565,  568,  1,    0,    0,    0,    566,  564,  1,    0,    0,    0,    566,
        567,  1,    0,    0,    0,    567,  569,  1,    0,    0,    0,    568,  566,  1,    0,    0,    0,    569,
        570,  5,    87,   0,    0,    570,  571,  3,    76,   38,   0,    571,  61,   1,    0,    0,    0,    572,
        574,  3,    64,   32,   0,    573,  572,  1,    0,    0,    0,    574,  577,  1,    0,    0,    0,    575,
        573,  1,    0,    0,    0,    575,  576,  1,    0,    0,    0,    576,  578,  1,    0,    0,    0,    577,
        575,  1,    0,    0,    0,    578,  579,  3,    68,   34,   0,    579,  63,   1,    0,    0,    0,    580,
        587,  3,    110,  55,   0,    581,  587,  5,    35,   0,    0,    582,  587,  5,    1,    0,    0,    583,
        587,  5,    12,   0,    0,    584,  587,  5,    38,   0,    0,    585,  587,  5,    39,   0,    0,    586,
        580,  1,    0,    0,    0,    586,  581,  1,    0,    0,    0,    586,  582,  1,    0,    0,    0,    586,
        583,  1,    0,    0,    0,    586,  584,  1,    0,    0,    0,    586,  585,  1,    0,    0,    0,    587,
        65,   1,    0,    0,    0,    588,  590,  3,    64,   32,   0,    589,  588,  1,    0,    0,    0,    590,
        593,  1,    0,    0,    0,    591,  589,  1,    0,    0,    0,    591,  592,  1,    0,    0,    0,    592,
        594,  1,    0,    0,    0,    593,  591,  1,    0,    0,    0,    594,  595,  3,    16,   8,    0,    595,
        596,  3,    68,   34,   0,    596,  67,   1,    0,    0,    0,    597,  599,  3,    110,  55,   0,    598,
        597,  1,    0,    0,    0,    599,  602,  1,    0,    0,    0,    600,  598,  1,    0,    0,    0,    600,
        601,  1,    0,    0,    0,    601,  603,  1,    0,    0,    0,    602,  600,  1,    0,    0,    0,    603,
        604,  3,    44,   22,   0,    604,  605,  3,    160,  80,   0,    605,  610,  3,    86,   43,   0,    606,
        607,  5,    82,   0,    0,    607,  609,  5,    83,   0,    0,    608,  606,  1,    0,    0,    0,    609,
        612,  1,    0,    0,    0,    610,  608,  1,    0,    0,    0,    610,  611,  1,    0,    0,    0,    611,
        615,  1,    0,    0,    0,    612,  610,  1,    0,    0,    0,    613,  614,  5,    45,   0,    0,    614,
        616,  3,    84,   42,   0,    615,  613,  1,    0,    0,    0,    615,  616,  1,    0,    0,    0,    616,
        617,  1,    0,    0,    0,    617,  618,  3,    42,   21,   0,    618,  69,   1,    0,    0,    0,    619,
        624,  3,    72,   36,   0,    620,  621,  5,    85,   0,    0,    621,  623,  3,    72,   36,   0,    622,
        620,  1,    0,    0,    0,    623,  626,  1,    0,    0,    0,    624,  622,  1,    0,    0,    0,    624,
        625,  1,    0,    0,    0,    625,  71,   1,    0,    0,    0,    626,  624,  1,    0,    0,    0,    627,
        630,  3,    74,   37,   0,    628,  629,  5,    87,   0,    0,    629,  631,  3,    76,   38,   0,    630,
        628,  1,    0,    0,    0,    630,  631,  1,    0,    0,    0,    631,  73,   1,    0,    0,    0,    632,
        637,  3,    160,  80,   0,    633,  634,  5,    82,   0,    0,    634,  636,  5,    83,   0,    0,    635,
        633,  1,    0,    0,    0,    636,  639,  1,    0,    0,    0,    637,  635,  1,    0,    0,    0,    637,
        638,  1,    0,    0,    0,    638,  75,   1,    0,    0,    0,    639,  637,  1,    0,    0,    0,    640,
        643,  3,    78,   39,   0,    641,  643,  3,    194,  97,   0,    642,  640,  1,    0,    0,    0,    642,
        641,  1,    0,    0,    0,    643,  77,   1,    0,    0,    0,    644,  656,  5,    80,   0,    0,    645,
        650,  3,    76,   38,   0,    646,  647,  5,    85,   0,    0,    647,  649,  3,    76,   38,   0,    648,
        646,  1,    0,    0,    0,    649,  652,  1,    0,    0,    0,    650,  648,  1,    0,    0,    0,    650,
        651,  1,    0,    0,    0,    651,  654,  1,    0,    0,    0,    652,  650,  1,    0,    0,    0,    653,
        655,  5,    85,   0,    0,    654,  653,  1,    0,    0,    0,    654,  655,  1,    0,    0,    0,    655,
        657,  1,    0,    0,    0,    656,  645,  1,    0,    0,    0,    656,  657,  1,    0,    0,    0,    657,
        658,  1,    0,    0,    0,    658,  659,  5,    81,   0,    0,    659,  79,   1,    0,    0,    0,    660,
        662,  3,    160,  80,   0,    661,  663,  3,    240,  120,  0,    662,  661,  1,    0,    0,    0,    662,
        663,  1,    0,    0,    0,    663,  671,  1,    0,    0,    0,    664,  665,  5,    86,   0,    0,    665,
        667,  3,    160,  80,   0,    666,  668,  3,    240,  120,  0,    667,  666,  1,    0,    0,    0,    667,
        668,  1,    0,    0,    0,    668,  670,  1,    0,    0,    0,    669,  664,  1,    0,    0,    0,    670,
        673,  1,    0,    0,    0,    671,  669,  1,    0,    0,    0,    671,  672,  1,    0,    0,    0,    672,
        81,   1,    0,    0,    0,    673,  671,  1,    0,    0,    0,    674,  687,  3,    236,  118,  0,    675,
        677,  3,    110,  55,   0,    676,  675,  1,    0,    0,    0,    677,  680,  1,    0,    0,    0,    678,
        676,  1,    0,    0,    0,    678,  679,  1,    0,    0,    0,    679,  681,  1,    0,    0,    0,    680,
        678,  1,    0,    0,    0,    681,  684,  5,    92,   0,    0,    682,  683,  7,    0,    0,    0,    683,
        685,  3,    236,  118,  0,    684,  682,  1,    0,    0,    0,    684,  685,  1,    0,    0,    0,    685,
        687,  1,    0,    0,    0,    686,  674,  1,    0,    0,    0,    686,  678,  1,    0,    0,    0,    687,
        83,   1,    0,    0,    0,    688,  693,  3,    100,  50,   0,    689,  690,  5,    85,   0,    0,    690,
        692,  3,    100,  50,   0,    691,  689,  1,    0,    0,    0,    692,  695,  1,    0,    0,    0,    693,
        691,  1,    0,    0,    0,    693,  694,  1,    0,    0,    0,    694,  85,   1,    0,    0,    0,    695,
        693,  1,    0,    0,    0,    696,  708,  5,    78,   0,    0,    697,  699,  3,    88,   44,   0,    698,
        697,  1,    0,    0,    0,    698,  699,  1,    0,    0,    0,    699,  709,  1,    0,    0,    0,    700,
        703,  3,    88,   44,   0,    701,  702,  5,    85,   0,    0,    702,  704,  3,    90,   45,   0,    703,
        701,  1,    0,    0,    0,    703,  704,  1,    0,    0,    0,    704,  709,  1,    0,    0,    0,    705,
        707,  3,    90,   45,   0,    706,  705,  1,    0,    0,    0,    706,  707,  1,    0,    0,    0,    707,
        709,  1,    0,    0,    0,    708,  698,  1,    0,    0,    0,    708,  700,  1,    0,    0,    0,    708,
        706,  1,    0,    0,    0,    709,  710,  1,    0,    0,    0,    710,  711,  5,    79,   0,    0,    711,
        87,   1,    0,    0,    0,    712,  718,  3,    236,  118,  0,    713,  714,  3,    160,  80,   0,    714,
        715,  5,    86,   0,    0,    715,  717,  1,    0,    0,    0,    716,  713,  1,    0,    0,    0,    717,
        720,  1,    0,    0,    0,    718,  716,  1,    0,    0,    0,    718,  719,  1,    0,    0,    0,    719,
        721,  1,    0,    0,    0,    720,  718,  1,    0,    0,    0,    721,  722,  5,    43,   0,    0,    722,
        89,   1,    0,    0,    0,    723,  728,  3,    92,   46,   0,    724,  725,  5,    85,   0,    0,    725,
        727,  3,    92,   46,   0,    726,  724,  1,    0,    0,    0,    727,  730,  1,    0,    0,    0,    728,
        726,  1,    0,    0,    0,    728,  729,  1,    0,    0,    0,    729,  733,  1,    0,    0,    0,    730,
        728,  1,    0,    0,    0,    731,  732,  5,    85,   0,    0,    732,  734,  3,    94,   47,   0,    733,
        731,  1,    0,    0,    0,    733,  734,  1,    0,    0,    0,    734,  737,  1,    0,    0,    0,    735,
        737,  3,    94,   47,   0,    736,  723,  1,    0,    0,    0,    736,  735,  1,    0,    0,    0,    737,
        91,   1,    0,    0,    0,    738,  740,  3,    12,   6,    0,    739,  738,  1,    0,    0,    0,    740,
        743,  1,    0,    0,    0,    741,  739,  1,    0,    0,    0,    741,  742,  1,    0,    0,    0,    742,
        744,  1,    0,    0,    0,    743,  741,  1,    0,    0,    0,    744,  745,  3,    236,  118,  0,    745,
        746,  3,    74,   37,   0,    746,  93,   1,    0,    0,    0,    747,  749,  3,    12,   6,    0,    748,
        747,  1,    0,    0,    0,    749,  752,  1,    0,    0,    0,    750,  748,  1,    0,    0,    0,    750,
        751,  1,    0,    0,    0,    751,  753,  1,    0,    0,    0,    752,  750,  1,    0,    0,    0,    753,
        757,  3,    236,  118,  0,    754,  756,  3,    110,  55,   0,    755,  754,  1,    0,    0,    0,    756,
        759,  1,    0,    0,    0,    757,  755,  1,    0,    0,    0,    757,  758,  1,    0,    0,    0,    758,
        760,  1,    0,    0,    0,    759,  757,  1,    0,    0,    0,    760,  761,  5,    124,  0,    0,    761,
        762,  3,    74,   37,   0,    762,  95,   1,    0,    0,    0,    763,  768,  3,    98,   49,   0,    764,
        765,  5,    85,   0,    0,    765,  767,  3,    98,   49,   0,    766,  764,  1,    0,    0,    0,    767,
        770,  1,    0,    0,    0,    768,  766,  1,    0,    0,    0,    768,  769,  1,    0,    0,    0,    769,
        97,   1,    0,    0,    0,    770,  768,  1,    0,    0,    0,    771,  773,  3,    12,   6,    0,    772,
        771,  1,    0,    0,    0,    773,  776,  1,    0,    0,    0,    774,  772,  1,    0,    0,    0,    774,
        775,  1,    0,    0,    0,    775,  777,  1,    0,    0,    0,    776,  774,  1,    0,    0,    0,    777,
        778,  5,    61,   0,    0,    778,  779,  3,    160,  80,   0,    779,  99,   1,    0,    0,    0,    780,
        785,  3,    160,  80,   0,    781,  782,  5,    86,   0,    0,    782,  784,  3,    160,  80,   0,    783,
        781,  1,    0,    0,    0,    784,  787,  1,    0,    0,    0,    785,  783,  1,    0,    0,    0,    785,
        786,  1,    0,    0,    0,    786,  101,  1,    0,    0,    0,    787,  785,  1,    0,    0,    0,    788,
        796,  3,    104,  52,   0,    789,  796,  3,    106,  53,   0,    790,  796,  5,    74,   0,    0,    791,
        796,  5,    75,   0,    0,    792,  796,  5,    73,   0,    0,    793,  796,  5,    77,   0,    0,    794,
        796,  5,    76,   0,    0,    795,  788,  1,    0,    0,    0,    795,  789,  1,    0,    0,    0,    795,
        790,  1,    0,    0,    0,    795,  791,  1,    0,    0,    0,    795,  792,  1,    0,    0,    0,    795,
        793,  1,    0,    0,    0,    795,  794,  1,    0,    0,    0,    796,  103,  1,    0,    0,    0,    797,
        798,  7,    1,    0,    0,    798,  105,  1,    0,    0,    0,    799,  800,  7,    2,    0,    0,    800,
        107,  1,    0,    0,    0,    801,  802,  3,    160,  80,   0,    802,  803,  5,    86,   0,    0,    803,
        805,  1,    0,    0,    0,    804,  801,  1,    0,    0,    0,    805,  808,  1,    0,    0,    0,    806,
        804,  1,    0,    0,    0,    806,  807,  1,    0,    0,    0,    807,  809,  1,    0,    0,    0,    808,
        806,  1,    0,    0,    0,    809,  810,  5,    123,  0,    0,    810,  811,  3,    160,  80,   0,    811,
        109,  1,    0,    0,    0,    812,  813,  5,    123,  0,    0,    813,  816,  3,    100,  50,   0,    814,
        816,  3,    108,  54,   0,    815,  812,  1,    0,    0,    0,    815,  814,  1,    0,    0,    0,    816,
        823,  1,    0,    0,    0,    817,  820,  5,    78,   0,    0,    818,  821,  3,    112,  56,   0,    819,
        821,  3,    116,  58,   0,    820,  818,  1,    0,    0,    0,    820,  819,  1,    0,    0,    0,    820,
        821,  1,    0,    0,    0,    821,  822,  1,    0,    0,    0,    822,  824,  5,    79,   0,    0,    823,
        817,  1,    0,    0,    0,    823,  824,  1,    0,    0,    0,    824,  111,  1,    0,    0,    0,    825,
        830,  3,    114,  57,   0,    826,  827,  5,    85,   0,    0,    827,  829,  3,    114,  57,   0,    828,
        826,  1,    0,    0,    0,    829,  832,  1,    0,    0,    0,    830,  828,  1,    0,    0,    0,    830,
        831,  1,    0,    0,    0,    831,  113,  1,    0,    0,    0,    832,  830,  1,    0,    0,    0,    833,
        834,  3,    160,  80,   0,    834,  835,  5,    87,   0,    0,    835,  836,  3,    116,  58,   0,    836,
        115,  1,    0,    0,    0,    837,  841,  3,    194,  97,   0,    838,  841,  3,    110,  55,   0,    839,
        841,  3,    118,  59,   0,    840,  837,  1,    0,    0,    0,    840,  838,  1,    0,    0,    0,    840,
        839,  1,    0,    0,    0,    841,  117,  1,    0,    0,    0,    842,  851,  5,    80,   0,    0,    843,
        848,  3,    116,  58,   0,    844,  845,  5,    85,   0,    0,    845,  847,  3,    116,  58,   0,    846,
        844,  1,    0,    0,    0,    847,  850,  1,    0,    0,    0,    848,  846,  1,    0,    0,    0,    848,
        849,  1,    0,    0,    0,    849,  852,  1,    0,    0,    0,    850,  848,  1,    0,    0,    0,    851,
        843,  1,    0,    0,    0,    851,  852,  1,    0,    0,    0,    852,  854,  1,    0,    0,    0,    853,
        855,  5,    85,   0,    0,    854,  853,  1,    0,    0,    0,    854,  855,  1,    0,    0,    0,    855,
        856,  1,    0,    0,    0,    856,  857,  5,    81,   0,    0,    857,  119,  1,    0,    0,    0,    858,
        859,  5,    123,  0,    0,    859,  860,  5,    28,   0,    0,    860,  861,  3,    160,  80,   0,    861,
        862,  3,    122,  61,   0,    862,  121,  1,    0,    0,    0,    863,  867,  5,    80,   0,    0,    864,
        866,  3,    124,  62,   0,    865,  864,  1,    0,    0,    0,    866,  869,  1,    0,    0,    0,    867,
        865,  1,    0,    0,    0,    867,  868,  1,    0,    0,    0,    868,  870,  1,    0,    0,    0,    869,
        867,  1,    0,    0,    0,    870,  871,  5,    81,   0,    0,    871,  123,  1,    0,    0,    0,    872,
        874,  3,    8,    4,    0,    873,  872,  1,    0,    0,    0,    874,  877,  1,    0,    0,    0,    875,
        873,  1,    0,    0,    0,    875,  876,  1,    0,    0,    0,    876,  878,  1,    0,    0,    0,    877,
        875,  1,    0,    0,    0,    878,  881,  3,    126,  63,   0,    879,  881,  5,    84,   0,    0,    880,
        875,  1,    0,    0,    0,    880,  879,  1,    0,    0,    0,    881,  125,  1,    0,    0,    0,    882,
        883,  3,    236,  118,  0,    883,  884,  3,    128,  64,   0,    884,  885,  5,    84,   0,    0,    885,
        907,  1,    0,    0,    0,    886,  888,  3,    14,   7,    0,    887,  889,  5,    84,   0,    0,    888,
        887,  1,    0,    0,    0,    888,  889,  1,    0,    0,    0,    889,  907,  1,    0,    0,    0,    890,
        892,  3,    30,   15,   0,    891,  893,  5,    84,   0,    0,    892,  891,  1,    0,    0,    0,    892,
        893,  1,    0,    0,    0,    893,  907,  1,    0,    0,    0,    894,  896,  3,    22,   11,   0,    895,
        897,  5,    84,   0,    0,    896,  895,  1,    0,    0,    0,    896,  897,  1,    0,    0,    0,    897,
        907,  1,    0,    0,    0,    898,  900,  3,    120,  60,   0,    899,  901,  5,    84,   0,    0,    900,
        899,  1,    0,    0,    0,    900,  901,  1,    0,    0,    0,    901,  907,  1,    0,    0,    0,    902,
        904,  3,    144,  72,   0,    903,  905,  5,    84,   0,    0,    904,  903,  1,    0,    0,    0,    904,
        905,  1,    0,    0,    0,    905,  907,  1,    0,    0,    0,    906,  882,  1,    0,    0,    0,    906,
        886,  1,    0,    0,    0,    906,  890,  1,    0,    0,    0,    906,  894,  1,    0,    0,    0,    906,
        898,  1,    0,    0,    0,    906,  902,  1,    0,    0,    0,    907,  127,  1,    0,    0,    0,    908,
        911,  3,    130,  65,   0,    909,  911,  3,    132,  66,   0,    910,  908,  1,    0,    0,    0,    910,
        909,  1,    0,    0,    0,    911,  129,  1,    0,    0,    0,    912,  913,  3,    160,  80,   0,    913,
        914,  5,    78,   0,    0,    914,  916,  5,    79,   0,    0,    915,  917,  3,    134,  67,   0,    916,
        915,  1,    0,    0,    0,    916,  917,  1,    0,    0,    0,    917,  131,  1,    0,    0,    0,    918,
        919,  3,    70,   35,   0,    919,  133,  1,    0,    0,    0,    920,  921,  5,    12,   0,    0,    921,
        922,  3,    116,  58,   0,    922,  135,  1,    0,    0,    0,    923,  925,  5,    52,   0,    0,    924,
        923,  1,    0,    0,    0,    924,  925,  1,    0,    0,    0,    925,  926,  1,    0,    0,    0,    926,
        927,  5,    51,   0,    0,    927,  928,  3,    100,  50,   0,    928,  929,  3,    138,  69,   0,    929,
        137,  1,    0,    0,    0,    930,  934,  5,    80,   0,    0,    931,  933,  3,    140,  70,   0,    932,
        931,  1,    0,    0,    0,    933,  936,  1,    0,    0,    0,    934,  932,  1,    0,    0,    0,    934,
        935,  1,    0,    0,    0,    935,  937,  1,    0,    0,    0,    936,  934,  1,    0,    0,    0,    937,
        938,  5,    81,   0,    0,    938,  139,  1,    0,    0,    0,    939,  943,  5,    53,   0,    0,    940,
        942,  3,    142,  71,   0,    941,  940,  1,    0,    0,    0,    942,  945,  1,    0,    0,    0,    943,
        941,  1,    0,    0,    0,    943,  944,  1,    0,    0,    0,    944,  946,  1,    0,    0,    0,    945,
        943,  1,    0,    0,    0,    946,  947,  3,    100,  50,   0,    947,  948,  5,    84,   0,    0,    948,
        976,  1,    0,    0,    0,    949,  950,  5,    54,   0,    0,    950,  953,  3,    100,  50,   0,    951,
        952,  5,    56,   0,    0,    952,  954,  3,    100,  50,   0,    953,  951,  1,    0,    0,    0,    953,
        954,  1,    0,    0,    0,    954,  955,  1,    0,    0,    0,    955,  956,  5,    84,   0,    0,    956,
        976,  1,    0,    0,    0,    957,  958,  5,    55,   0,    0,    958,  961,  3,    100,  50,   0,    959,
        960,  5,    56,   0,    0,    960,  962,  3,    100,  50,   0,    961,  959,  1,    0,    0,    0,    961,
        962,  1,    0,    0,    0,    962,  963,  1,    0,    0,    0,    963,  964,  5,    84,   0,    0,    964,
        976,  1,    0,    0,    0,    965,  966,  5,    57,   0,    0,    966,  967,  3,    100,  50,   0,    967,
        968,  5,    84,   0,    0,    968,  976,  1,    0,    0,    0,    969,  970,  5,    58,   0,    0,    970,
        971,  3,    100,  50,   0,    971,  972,  5,    59,   0,    0,    972,  973,  3,    100,  50,   0,    973,
        974,  5,    84,   0,    0,    974,  976,  1,    0,    0,    0,    975,  939,  1,    0,    0,    0,    975,
        949,  1,    0,    0,    0,    975,  957,  1,    0,    0,    0,    975,  965,  1,    0,    0,    0,    975,
        969,  1,    0,    0,    0,    976,  141,  1,    0,    0,    0,    977,  978,  7,    3,    0,    0,    978,
        143,  1,    0,    0,    0,    979,  980,  5,    63,   0,    0,    980,  982,  3,    160,  80,   0,    981,
        983,  3,    16,   8,    0,    982,  981,  1,    0,    0,    0,    982,  983,  1,    0,    0,    0,    983,
        984,  1,    0,    0,    0,    984,  987,  3,    146,  73,   0,    985,  986,  5,    24,   0,    0,    986,
        988,  3,    234,  117,  0,    987,  985,  1,    0,    0,    0,    987,  988,  1,    0,    0,    0,    988,
        989,  1,    0,    0,    0,    989,  990,  3,    152,  76,   0,    990,  145,  1,    0,    0,    0,    991,
        993,  5,    78,   0,    0,    992,  994,  3,    148,  74,   0,    993,  992,  1,    0,    0,    0,    993,
        994,  1,    0,    0,    0,    994,  995,  1,    0,    0,    0,    995,  996,  5,    79,   0,    0,    996,
        147,  1,    0,    0,    0,    997,  1002, 3,    150,  75,   0,    998,  999,  5,    85,   0,    0,    999,
        1001, 3,    150,  75,   0,    1000, 998,  1,    0,    0,    0,    1001, 1004, 1,    0,    0,    0,    1002,
        1000, 1,    0,    0,    0,    1002, 1003, 1,    0,    0,    0,    1003, 149,  1,    0,    0,    0,    1004,
        1002, 1,    0,    0,    0,    1005, 1006, 3,    236,  118,  0,    1006, 1007, 3,    160,  80,   0,    1007,
        151,  1,    0,    0,    0,    1008, 1012, 5,    80,   0,    0,    1009, 1011, 3,    36,   18,   0,    1010,
        1009, 1,    0,    0,    0,    1011, 1014, 1,    0,    0,    0,    1012, 1010, 1,    0,    0,    0,    1012,
        1013, 1,    0,    0,    0,    1013, 1015, 1,    0,    0,    0,    1014, 1012, 1,    0,    0,    0,    1015,
        1016, 5,    81,   0,    0,    1016, 153,  1,    0,    0,    0,    1017, 1021, 5,    80,   0,    0,    1018,
        1020, 3,    156,  78,   0,    1019, 1018, 1,    0,    0,    0,    1020, 1023, 1,    0,    0,    0,    1021,
        1019, 1,    0,    0,    0,    1021, 1022, 1,    0,    0,    0,    1022, 1024, 1,    0,    0,    0,    1023,
        1021, 1,    0,    0,    0,    1024, 1025, 5,    81,   0,    0,    1025, 155,  1,    0,    0,    0,    1026,
        1027, 3,    158,  79,   0,    1027, 1028, 5,    84,   0,    0,    1028, 1032, 1,    0,    0,    0,    1029,
        1032, 3,    164,  82,   0,    1030, 1032, 3,    162,  81,   0,    1031, 1026, 1,    0,    0,    0,    1031,
        1029, 1,    0,    0,    0,    1031, 1030, 1,    0,    0,    0,    1032, 157,  1,    0,    0,    0,    1033,
        1035, 3,    12,   6,    0,    1034, 1033, 1,    0,    0,    0,    1035, 1038, 1,    0,    0,    0,    1036,
        1034, 1,    0,    0,    0,    1036, 1037, 1,    0,    0,    0,    1037, 1047, 1,    0,    0,    0,    1038,
        1036, 1,    0,    0,    0,    1039, 1040, 3,    236,  118,  0,    1040, 1041, 3,    70,   35,   0,    1041,
        1048, 1,    0,    0,    0,    1042, 1043, 5,    61,   0,    0,    1043, 1044, 3,    160,  80,   0,    1044,
        1045, 5,    87,   0,    0,    1045, 1046, 3,    194,  97,   0,    1046, 1048, 1,    0,    0,    0,    1047,
        1039, 1,    0,    0,    0,    1047, 1042, 1,    0,    0,    0,    1048, 159,  1,    0,    0,    0,    1049,
        1050, 7,    4,    0,    0,    1050, 161,  1,    0,    0,    0,    1051, 1053, 3,    10,   5,    0,    1052,
        1051, 1,    0,    0,    0,    1053, 1056, 1,    0,    0,    0,    1054, 1052, 1,    0,    0,    0,    1054,
        1055, 1,    0,    0,    0,    1055, 1060, 1,    0,    0,    0,    1056, 1054, 1,    0,    0,    0,    1057,
        1061, 3,    14,   7,    0,    1058, 1061, 3,    30,   15,   0,    1059, 1061, 3,    144,  72,   0,    1060,
        1057, 1,    0,    0,    0,    1060, 1058, 1,    0,    0,    0,    1060, 1059, 1,    0,    0,    0,    1061,
        1064, 1,    0,    0,    0,    1062, 1064, 5,    84,   0,    0,    1063, 1054, 1,    0,    0,    0,    1063,
        1062, 1,    0,    0,    0,    1064, 163,  1,    0,    0,    0,    1065, 1179, 3,    154,  77,   0,    1066,
        1067, 5,    2,    0,    0,    1067, 1070, 3,    194,  97,   0,    1068, 1069, 5,    93,   0,    0,    1069,
        1071, 3,    194,  97,   0,    1070, 1068, 1,    0,    0,    0,    1070, 1071, 1,    0,    0,    0,    1071,
        1072, 1,    0,    0,    0,    1072, 1073, 5,    84,   0,    0,    1073, 1179, 1,    0,    0,    0,    1074,
        1075, 5,    22,   0,    0,    1075, 1076, 3,    188,  94,   0,    1076, 1079, 3,    164,  82,   0,    1077,
        1078, 5,    15,   0,    0,    1078, 1080, 3,    164,  82,   0,    1079, 1077, 1,    0,    0,    0,    1079,
        1080, 1,    0,    0,    0,    1080, 1179, 1,    0,    0,    0,    1081, 1082, 5,    21,   0,    0,    1082,
        1083, 5,    78,   0,    0,    1083, 1084, 3,    182,  91,   0,    1084, 1085, 5,    79,   0,    0,    1085,
        1086, 3,    164,  82,   0,    1086, 1179, 1,    0,    0,    0,    1087, 1088, 5,    50,   0,    0,    1088,
        1089, 3,    188,  94,   0,    1089, 1090, 3,    164,  82,   0,    1090, 1179, 1,    0,    0,    0,    1091,
        1092, 5,    13,   0,    0,    1092, 1093, 3,    164,  82,   0,    1093, 1094, 5,    50,   0,    0,    1094,
        1095, 3,    188,  94,   0,    1095, 1096, 5,    84,   0,    0,    1096, 1179, 1,    0,    0,    0,    1097,
        1098, 5,    47,   0,    0,    1098, 1108, 3,    154,  77,   0,    1099, 1101, 3,    166,  83,   0,    1100,
        1099, 1,    0,    0,    0,    1101, 1102, 1,    0,    0,    0,    1102, 1100, 1,    0,    0,    0,    1102,
        1103, 1,    0,    0,    0,    1103, 1105, 1,    0,    0,    0,    1104, 1106, 3,    170,  85,   0,    1105,
        1104, 1,    0,    0,    0,    1105, 1106, 1,    0,    0,    0,    1106, 1109, 1,    0,    0,    0,    1107,
        1109, 3,    170,  85,   0,    1108, 1100, 1,    0,    0,    0,    1108, 1107, 1,    0,    0,    0,    1109,
        1179, 1,    0,    0,    0,    1110, 1111, 5,    47,   0,    0,    1111, 1112, 3,    172,  86,   0,    1112,
        1116, 3,    154,  77,   0,    1113, 1115, 3,    166,  83,   0,    1114, 1113, 1,    0,    0,    0,    1115,
        1118, 1,    0,    0,    0,    1116, 1114, 1,    0,    0,    0,    1116, 1117, 1,    0,    0,    0,    1117,
        1120, 1,    0,    0,    0,    1118, 1116, 1,    0,    0,    0,    1119, 1121, 3,    170,  85,   0,    1120,
        1119, 1,    0,    0,    0,    1120, 1121, 1,    0,    0,    0,    1121, 1179, 1,    0,    0,    0,    1122,
        1123, 5,    41,   0,    0,    1123, 1124, 3,    188,  94,   0,    1124, 1128, 5,    80,   0,    0,    1125,
        1127, 3,    178,  89,   0,    1126, 1125, 1,    0,    0,    0,    1127, 1130, 1,    0,    0,    0,    1128,
        1126, 1,    0,    0,    0,    1128, 1129, 1,    0,    0,    0,    1129, 1134, 1,    0,    0,    0,    1130,
        1128, 1,    0,    0,    0,    1131, 1133, 3,    180,  90,   0,    1132, 1131, 1,    0,    0,    0,    1133,
        1136, 1,    0,    0,    0,    1134, 1132, 1,    0,    0,    0,    1134, 1135, 1,    0,    0,    0,    1135,
        1137, 1,    0,    0,    0,    1136, 1134, 1,    0,    0,    0,    1137, 1138, 5,    81,   0,    0,    1138,
        1179, 1,    0,    0,    0,    1139, 1140, 5,    42,   0,    0,    1140, 1141, 3,    188,  94,   0,    1141,
        1142, 3,    154,  77,   0,    1142, 1179, 1,    0,    0,    0,    1143, 1145, 5,    36,   0,    0,    1144,
        1146, 3,    194,  97,   0,    1145, 1144, 1,    0,    0,    0,    1145, 1146, 1,    0,    0,    0,    1146,
        1147, 1,    0,    0,    0,    1147, 1179, 5,    84,   0,    0,    1148, 1149, 5,    44,   0,    0,    1149,
        1150, 3,    194,  97,   0,    1150, 1151, 5,    84,   0,    0,    1151, 1179, 1,    0,    0,    0,    1152,
        1154, 5,    4,    0,    0,    1153, 1155, 3,    160,  80,   0,    1154, 1153, 1,    0,    0,    0,    1154,
        1155, 1,    0,    0,    0,    1155, 1156, 1,    0,    0,    0,    1156, 1179, 5,    84,   0,    0,    1157,
        1159, 5,    11,   0,    0,    1158, 1160, 3,    160,  80,   0,    1159, 1158, 1,    0,    0,    0,    1159,
        1160, 1,    0,    0,    0,    1160, 1161, 1,    0,    0,    0,    1161, 1179, 5,    84,   0,    0,    1162,
        1163, 5,    62,   0,    0,    1163, 1164, 3,    194,  97,   0,    1164, 1165, 5,    84,   0,    0,    1165,
        1179, 1,    0,    0,    0,    1166, 1179, 5,    84,   0,    0,    1167, 1168, 3,    194,  97,   0,    1168,
        1169, 5,    84,   0,    0,    1169, 1179, 1,    0,    0,    0,    1170, 1172, 3,    206,  103,  0,    1171,
        1173, 5,    84,   0,    0,    1172, 1171, 1,    0,    0,    0,    1172, 1173, 1,    0,    0,    0,    1173,
        1179, 1,    0,    0,    0,    1174, 1175, 3,    160,  80,   0,    1175, 1176, 5,    93,   0,    0,    1176,
        1177, 3,    164,  82,   0,    1177, 1179, 1,    0,    0,    0,    1178, 1065, 1,    0,    0,    0,    1178,
        1066, 1,    0,    0,    0,    1178, 1074, 1,    0,    0,    0,    1178, 1081, 1,    0,    0,    0,    1178,
        1087, 1,    0,    0,    0,    1178, 1091, 1,    0,    0,    0,    1178, 1097, 1,    0,    0,    0,    1178,
        1110, 1,    0,    0,    0,    1178, 1122, 1,    0,    0,    0,    1178, 1139, 1,    0,    0,    0,    1178,
        1143, 1,    0,    0,    0,    1178, 1148, 1,    0,    0,    0,    1178, 1152, 1,    0,    0,    0,    1178,
        1157, 1,    0,    0,    0,    1178, 1162, 1,    0,    0,    0,    1178, 1166, 1,    0,    0,    0,    1178,
        1167, 1,    0,    0,    0,    1178, 1170, 1,    0,    0,    0,    1178, 1174, 1,    0,    0,    0,    1179,
        165,  1,    0,    0,    0,    1180, 1181, 5,    7,    0,    0,    1181, 1185, 5,    78,   0,    0,    1182,
        1184, 3,    12,   6,    0,    1183, 1182, 1,    0,    0,    0,    1184, 1187, 1,    0,    0,    0,    1185,
        1183, 1,    0,    0,    0,    1185, 1186, 1,    0,    0,    0,    1186, 1188, 1,    0,    0,    0,    1187,
        1185, 1,    0,    0,    0,    1188, 1189, 3,    168,  84,   0,    1189, 1190, 3,    160,  80,   0,    1190,
        1191, 5,    79,   0,    0,    1191, 1192, 3,    154,  77,   0,    1192, 167,  1,    0,    0,    0,    1193,
        1198, 3,    100,  50,   0,    1194, 1195, 5,    107,  0,    0,    1195, 1197, 3,    100,  50,   0,    1196,
        1194, 1,    0,    0,    0,    1197, 1200, 1,    0,    0,    0,    1198, 1196, 1,    0,    0,    0,    1198,
        1199, 1,    0,    0,    0,    1199, 169,  1,    0,    0,    0,    1200, 1198, 1,    0,    0,    0,    1201,
        1202, 5,    19,   0,    0,    1202, 1203, 3,    154,  77,   0,    1203, 171,  1,    0,    0,    0,    1204,
        1205, 5,    78,   0,    0,    1205, 1207, 3,    174,  87,   0,    1206, 1208, 5,    84,   0,    0,    1207,
        1206, 1,    0,    0,    0,    1207, 1208, 1,    0,    0,    0,    1208, 1209, 1,    0,    0,    0,    1209,
        1210, 5,    79,   0,    0,    1210, 173,  1,    0,    0,    0,    1211, 1216, 3,    176,  88,   0,    1212,
        1213, 5,    84,   0,    0,    1213, 1215, 3,    176,  88,   0,    1214, 1212, 1,    0,    0,    0,    1215,
        1218, 1,    0,    0,    0,    1216, 1214, 1,    0,    0,    0,    1216, 1217, 1,    0,    0,    0,    1217,
        175,  1,    0,    0,    0,    1218, 1216, 1,    0,    0,    0,    1219, 1221, 3,    12,   6,    0,    1220,
        1219, 1,    0,    0,    0,    1221, 1224, 1,    0,    0,    0,    1222, 1220, 1,    0,    0,    0,    1222,
        1223, 1,    0,    0,    0,    1223, 1230, 1,    0,    0,    0,    1224, 1222, 1,    0,    0,    0,    1225,
        1226, 3,    80,   40,   0,    1226, 1227, 3,    74,   37,   0,    1227, 1231, 1,    0,    0,    0,    1228,
        1229, 5,    61,   0,    0,    1229, 1231, 3,    160,  80,   0,    1230, 1225, 1,    0,    0,    0,    1230,
        1228, 1,    0,    0,    0,    1231, 1232, 1,    0,    0,    0,    1232, 1233, 5,    87,   0,    0,    1233,
        1234, 3,    194,  97,   0,    1234, 1237, 1,    0,    0,    0,    1235, 1237, 3,    160,  80,   0,    1236,
        1222, 1,    0,    0,    0,    1236, 1235, 1,    0,    0,    0,    1237, 177,  1,    0,    0,    0,    1238,
        1240, 3,    180,  90,   0,    1239, 1238, 1,    0,    0,    0,    1240, 1241, 1,    0,    0,    0,    1241,
        1239, 1,    0,    0,    0,    1241, 1242, 1,    0,    0,    0,    1242, 1244, 1,    0,    0,    0,    1243,
        1245, 3,    156,  78,   0,    1244, 1243, 1,    0,    0,    0,    1245, 1246, 1,    0,    0,    0,    1246,
        1244, 1,    0,    0,    0,    1246, 1247, 1,    0,    0,    0,    1247, 179,  1,    0,    0,    0,    1248,
        1254, 5,    6,    0,    0,    1249, 1255, 3,    194,  97,   0,    1250, 1255, 5,    128,  0,    0,    1251,
        1252, 3,    236,  118,  0,    1252, 1253, 3,    160,  80,   0,    1253, 1255, 1,    0,    0,    0,    1254,
        1249, 1,    0,    0,    0,    1254, 1250, 1,    0,    0,    0,    1254, 1251, 1,    0,    0,    0,    1255,
        1256, 1,    0,    0,    0,    1256, 1260, 5,    93,   0,    0,    1257, 1258, 5,    12,   0,    0,    1258,
        1260, 5,    93,   0,    0,    1259, 1248, 1,    0,    0,    0,    1259, 1257, 1,    0,    0,    0,    1260,
        181,  1,    0,    0,    0,    1261, 1274, 3,    186,  93,   0,    1262, 1264, 3,    184,  92,   0,    1263,
        1262, 1,    0,    0,    0,    1263, 1264, 1,    0,    0,    0,    1264, 1265, 1,    0,    0,    0,    1265,
        1267, 5,    84,   0,    0,    1266, 1268, 3,    194,  97,   0,    1267, 1266, 1,    0,    0,    0,    1267,
        1268, 1,    0,    0,    0,    1268, 1269, 1,    0,    0,    0,    1269, 1271, 5,    84,   0,    0,    1270,
        1272, 3,    190,  95,   0,    1271, 1270, 1,    0,    0,    0,    1271, 1272, 1,    0,    0,    0,    1272,
        1274, 1,    0,    0,    0,    1273, 1261, 1,    0,    0,    0,    1273, 1263, 1,    0,    0,    0,    1274,
        183,  1,    0,    0,    0,    1275, 1278, 3,    158,  79,   0,    1276, 1278, 3,    190,  95,   0,    1277,
        1275, 1,    0,    0,    0,    1277, 1276, 1,    0,    0,    0,    1278, 185,  1,    0,    0,    0,    1279,
        1281, 3,    12,   6,    0,    1280, 1279, 1,    0,    0,    0,    1281, 1284, 1,    0,    0,    0,    1282,
        1280, 1,    0,    0,    0,    1282, 1283, 1,    0,    0,    0,    1283, 1287, 1,    0,    0,    0,    1284,
        1282, 1,    0,    0,    0,    1285, 1288, 3,    236,  118,  0,    1286, 1288, 5,    61,   0,    0,    1287,
        1285, 1,    0,    0,    0,    1287, 1286, 1,    0,    0,    0,    1288, 1289, 1,    0,    0,    0,    1289,
        1290, 3,    74,   37,   0,    1290, 1291, 5,    93,   0,    0,    1291, 1292, 3,    194,  97,   0,    1292,
        187,  1,    0,    0,    0,    1293, 1294, 5,    78,   0,    0,    1294, 1295, 3,    194,  97,   0,    1295,
        1296, 5,    79,   0,    0,    1296, 189,  1,    0,    0,    0,    1297, 1302, 3,    194,  97,   0,    1298,
        1299, 5,    85,   0,    0,    1299, 1301, 3,    194,  97,   0,    1300, 1298, 1,    0,    0,    0,    1301,
        1304, 1,    0,    0,    0,    1302, 1300, 1,    0,    0,    0,    1302, 1303, 1,    0,    0,    0,    1303,
        191,  1,    0,    0,    0,    1304, 1302, 1,    0,    0,    0,    1305, 1306, 3,    160,  80,   0,    1306,
        1308, 5,    78,   0,    0,    1307, 1309, 3,    190,  95,   0,    1308, 1307, 1,    0,    0,    0,    1308,
        1309, 1,    0,    0,    0,    1309, 1310, 1,    0,    0,    0,    1310, 1311, 5,    79,   0,    0,    1311,
        1325, 1,    0,    0,    0,    1312, 1313, 5,    43,   0,    0,    1313, 1315, 5,    78,   0,    0,    1314,
        1316, 3,    190,  95,   0,    1315, 1314, 1,    0,    0,    0,    1315, 1316, 1,    0,    0,    0,    1316,
        1317, 1,    0,    0,    0,    1317, 1325, 5,    79,   0,    0,    1318, 1319, 5,    40,   0,    0,    1319,
        1321, 5,    78,   0,    0,    1320, 1322, 3,    190,  95,   0,    1321, 1320, 1,    0,    0,    0,    1321,
        1322, 1,    0,    0,    0,    1322, 1323, 1,    0,    0,    0,    1323, 1325, 5,    79,   0,    0,    1324,
        1305, 1,    0,    0,    0,    1324, 1312, 1,    0,    0,    0,    1324, 1318, 1,    0,    0,    0,    1325,
        193,  1,    0,    0,    0,    1326, 1327, 6,    97,   -1,   0,    1327, 1372, 3,    204,  102,  0,    1328,
        1372, 3,    192,  96,   0,    1329, 1330, 5,    31,   0,    0,    1330, 1372, 3,    216,  108,  0,    1331,
        1335, 5,    78,   0,    0,    1332, 1334, 3,    110,  55,   0,    1333, 1332, 1,    0,    0,    0,    1334,
        1337, 1,    0,    0,    0,    1335, 1333, 1,    0,    0,    0,    1335, 1336, 1,    0,    0,    0,    1336,
        1338, 1,    0,    0,    0,    1337, 1335, 1,    0,    0,    0,    1338, 1343, 3,    236,  118,  0,    1339,
        1340, 5,    106,  0,    0,    1340, 1342, 3,    236,  118,  0,    1341, 1339, 1,    0,    0,    0,    1342,
        1345, 1,    0,    0,    0,    1343, 1341, 1,    0,    0,    0,    1343, 1344, 1,    0,    0,    0,    1344,
        1346, 1,    0,    0,    0,    1345, 1343, 1,    0,    0,    0,    1346, 1347, 5,    79,   0,    0,    1347,
        1348, 3,    194,  97,   22,   1348, 1372, 1,    0,    0,    0,    1349, 1350, 7,    5,    0,    0,    1350,
        1372, 3,    194,  97,   20,   1351, 1352, 7,    6,    0,    0,    1352, 1372, 3,    194,  97,   19,   1353,
        1372, 3,    198,  99,   0,    1354, 1372, 3,    206,  103,  0,    1355, 1356, 3,    236,  118,  0,    1356,
        1362, 5,    122,  0,    0,    1357, 1359, 3,    240,  120,  0,    1358, 1357, 1,    0,    0,    0,    1358,
        1359, 1,    0,    0,    0,    1359, 1360, 1,    0,    0,    0,    1360, 1363, 3,    160,  80,   0,    1361,
        1363, 5,    31,   0,    0,    1362, 1358, 1,    0,    0,    0,    1362, 1361, 1,    0,    0,    0,    1363,
        1372, 1,    0,    0,    0,    1364, 1365, 3,    214,  107,  0,    1365, 1367, 5,    122,  0,    0,    1366,
        1368, 3,    240,  120,  0,    1367, 1366, 1,    0,    0,    0,    1367, 1368, 1,    0,    0,    0,    1368,
        1369, 1,    0,    0,    0,    1369, 1370, 5,    31,   0,    0,    1370, 1372, 1,    0,    0,    0,    1371,
        1326, 1,    0,    0,    0,    1371, 1328, 1,    0,    0,    0,    1371, 1329, 1,    0,    0,    0,    1371,
        1331, 1,    0,    0,    0,    1371, 1349, 1,    0,    0,    0,    1371, 1351, 1,    0,    0,    0,    1371,
        1353, 1,    0,    0,    0,    1371, 1354, 1,    0,    0,    0,    1371, 1355, 1,    0,    0,    0,    1371,
        1364, 1,    0,    0,    0,    1372, 1456, 1,    0,    0,    0,    1373, 1374, 10,   18,   0,    0,    1374,
        1375, 7,    7,    0,    0,    1375, 1455, 3,    194,  97,   19,   1376, 1377, 10,   17,   0,    0,    1377,
        1378, 7,    8,    0,    0,    1378, 1455, 3,    194,  97,   18,   1379, 1387, 10,   16,   0,    0,    1380,
        1381, 5,    89,   0,    0,    1381, 1388, 5,    89,   0,    0,    1382, 1383, 5,    88,   0,    0,    1383,
        1384, 5,    88,   0,    0,    1384, 1388, 5,    88,   0,    0,    1385, 1386, 5,    88,   0,    0,    1386,
        1388, 5,    88,   0,    0,    1387, 1380, 1,    0,    0,    0,    1387, 1382, 1,    0,    0,    0,    1387,
        1385, 1,    0,    0,    0,    1388, 1389, 1,    0,    0,    0,    1389, 1455, 3,    194,  97,   17,   1390,
        1391, 10,   15,   0,    0,    1391, 1392, 7,    9,    0,    0,    1392, 1455, 3,    194,  97,   16,   1393,
        1394, 10,   13,   0,    0,    1394, 1395, 7,    10,   0,    0,    1395, 1455, 3,    194,  97,   14,   1396,
        1397, 10,   12,   0,    0,    1397, 1398, 5,    106,  0,    0,    1398, 1455, 3,    194,  97,   13,   1399,
        1400, 10,   11,   0,    0,    1400, 1401, 5,    108,  0,    0,    1401, 1455, 3,    194,  97,   12,   1402,
        1403, 10,   10,   0,    0,    1403, 1404, 5,    107,  0,    0,    1404, 1455, 3,    194,  97,   11,   1405,
        1406, 10,   9,    0,    0,    1406, 1407, 5,    98,   0,    0,    1407, 1455, 3,    194,  97,   10,   1408,
        1409, 10,   8,    0,    0,    1409, 1410, 5,    99,   0,    0,    1410, 1455, 3,    194,  97,   9,    1411,
        1412, 10,   7,    0,    0,    1412, 1413, 5,    92,   0,    0,    1413, 1414, 3,    194,  97,   0,    1414,
        1415, 5,    93,   0,    0,    1415, 1416, 3,    194,  97,   7,    1416, 1455, 1,    0,    0,    0,    1417,
        1418, 10,   6,    0,    0,    1418, 1419, 7,    11,   0,    0,    1419, 1455, 3,    194,  97,   6,    1420,
        1421, 10,   26,   0,    0,    1421, 1433, 5,    86,   0,    0,    1422, 1434, 3,    160,  80,   0,    1423,
        1434, 3,    192,  96,   0,    1424, 1434, 5,    43,   0,    0,    1425, 1427, 5,    31,   0,    0,    1426,
        1428, 3,    232,  116,  0,    1427, 1426, 1,    0,    0,    0,    1427, 1428, 1,    0,    0,    0,    1428,
        1429, 1,    0,    0,    0,    1429, 1434, 3,    220,  110,  0,    1430, 1431, 5,    40,   0,    0,    1431,
        1434, 3,    242,  121,  0,    1432, 1434, 3,    226,  113,  0,    1433, 1422, 1,    0,    0,    0,    1433,
        1423, 1,    0,    0,    0,    1433, 1424, 1,    0,    0,    0,    1433, 1425, 1,    0,    0,    0,    1433,
        1430, 1,    0,    0,    0,    1433, 1432, 1,    0,    0,    0,    1434, 1455, 1,    0,    0,    0,    1435,
        1436, 10,   25,   0,    0,    1436, 1437, 5,    82,   0,    0,    1437, 1438, 3,    194,  97,   0,    1438,
        1439, 5,    83,   0,    0,    1439, 1455, 1,    0,    0,    0,    1440, 1441, 10,   21,   0,    0,    1441,
        1455, 7,    12,   0,    0,    1442, 1443, 10,   14,   0,    0,    1443, 1446, 5,    26,   0,    0,    1444,
        1447, 3,    236,  118,  0,    1445, 1447, 3,    196,  98,   0,    1446, 1444, 1,    0,    0,    0,    1446,
        1445, 1,    0,    0,    0,    1447, 1455, 1,    0,    0,    0,    1448, 1449, 10,   3,    0,    0,    1449,
        1451, 5,    122,  0,    0,    1450, 1452, 3,    240,  120,  0,    1451, 1450, 1,    0,    0,    0,    1451,
        1452, 1,    0,    0,    0,    1452, 1453, 1,    0,    0,    0,    1453, 1455, 3,    160,  80,   0,    1454,
        1373, 1,    0,    0,    0,    1454, 1376, 1,    0,    0,    0,    1454, 1379, 1,    0,    0,    0,    1454,
        1390, 1,    0,    0,    0,    1454, 1393, 1,    0,    0,    0,    1454, 1396, 1,    0,    0,    0,    1454,
        1399, 1,    0,    0,    0,    1454, 1402, 1,    0,    0,    0,    1454, 1405, 1,    0,    0,    0,    1454,
        1408, 1,    0,    0,    0,    1454, 1411, 1,    0,    0,    0,    1454, 1417, 1,    0,    0,    0,    1454,
        1420, 1,    0,    0,    0,    1454, 1435, 1,    0,    0,    0,    1454, 1440, 1,    0,    0,    0,    1454,
        1442, 1,    0,    0,    0,    1454, 1448, 1,    0,    0,    0,    1455, 1458, 1,    0,    0,    0,    1456,
        1454, 1,    0,    0,    0,    1456, 1457, 1,    0,    0,    0,    1457, 195,  1,    0,    0,    0,    1458,
        1456, 1,    0,    0,    0,    1459, 1461, 3,    12,   6,    0,    1460, 1459, 1,    0,    0,    0,    1461,
        1464, 1,    0,    0,    0,    1462, 1460, 1,    0,    0,    0,    1462, 1463, 1,    0,    0,    0,    1463,
        1465, 1,    0,    0,    0,    1464, 1462, 1,    0,    0,    0,    1465, 1469, 3,    236,  118,  0,    1466,
        1468, 3,    110,  55,   0,    1467, 1466, 1,    0,    0,    0,    1468, 1471, 1,    0,    0,    0,    1469,
        1467, 1,    0,    0,    0,    1469, 1470, 1,    0,    0,    0,    1470, 1472, 1,    0,    0,    0,    1471,
        1469, 1,    0,    0,    0,    1472, 1473, 3,    160,  80,   0,    1473, 197,  1,    0,    0,    0,    1474,
        1475, 3,    200,  100,  0,    1475, 1476, 5,    121,  0,    0,    1476, 1477, 3,    202,  101,  0,    1477,
        199,  1,    0,    0,    0,    1478, 1501, 3,    160,  80,   0,    1479, 1481, 5,    78,   0,    0,    1480,
        1482, 3,    90,   45,   0,    1481, 1480, 1,    0,    0,    0,    1481, 1482, 1,    0,    0,    0,    1482,
        1483, 1,    0,    0,    0,    1483, 1501, 5,    79,   0,    0,    1484, 1485, 5,    78,   0,    0,    1485,
        1490, 3,    160,  80,   0,    1486, 1487, 5,    85,   0,    0,    1487, 1489, 3,    160,  80,   0,    1488,
        1486, 1,    0,    0,    0,    1489, 1492, 1,    0,    0,    0,    1490, 1488, 1,    0,    0,    0,    1490,
        1491, 1,    0,    0,    0,    1491, 1493, 1,    0,    0,    0,    1492, 1490, 1,    0,    0,    0,    1493,
        1494, 5,    79,   0,    0,    1494, 1501, 1,    0,    0,    0,    1495, 1497, 5,    78,   0,    0,    1496,
        1498, 3,    96,   48,   0,    1497, 1496, 1,    0,    0,    0,    1497, 1498, 1,    0,    0,    0,    1498,
        1499, 1,    0,    0,    0,    1499, 1501, 5,    79,   0,    0,    1500, 1478, 1,    0,    0,    0,    1500,
        1479, 1,    0,    0,    0,    1500, 1484, 1,    0,    0,    0,    1500, 1495, 1,    0,    0,    0,    1501,
        201,  1,    0,    0,    0,    1502, 1505, 3,    194,  97,   0,    1503, 1505, 3,    154,  77,   0,    1504,
        1502, 1,    0,    0,    0,    1504, 1503, 1,    0,    0,    0,    1505, 203,  1,    0,    0,    0,    1506,
        1507, 5,    78,   0,    0,    1507, 1508, 3,    194,  97,   0,    1508, 1509, 5,    79,   0,    0,    1509,
        1525, 1,    0,    0,    0,    1510, 1525, 5,    43,   0,    0,    1511, 1525, 5,    40,   0,    0,    1512,
        1525, 3,    102,  51,   0,    1513, 1525, 3,    160,  80,   0,    1514, 1515, 3,    44,   22,   0,    1515,
        1516, 5,    86,   0,    0,    1516, 1517, 5,    9,    0,    0,    1517, 1525, 1,    0,    0,    0,    1518,
        1522, 3,    232,  116,  0,    1519, 1523, 3,    244,  122,  0,    1520, 1521, 5,    43,   0,    0,    1521,
        1523, 3,    246,  123,  0,    1522, 1519, 1,    0,    0,    0,    1522, 1520, 1,    0,    0,    0,    1523,
        1525, 1,    0,    0,    0,    1524, 1506, 1,    0,    0,    0,    1524, 1510, 1,    0,    0,    0,    1524,
        1511, 1,    0,    0,    0,    1524, 1512, 1,    0,    0,    0,    1524, 1513, 1,    0,    0,    0,    1524,
        1514, 1,    0,    0,    0,    1524, 1518, 1,    0,    0,    0,    1525, 205,  1,    0,    0,    0,    1526,
        1527, 5,    41,   0,    0,    1527, 1528, 3,    188,  94,   0,    1528, 1532, 5,    80,   0,    0,    1529,
        1531, 3,    208,  104,  0,    1530, 1529, 1,    0,    0,    0,    1531, 1534, 1,    0,    0,    0,    1532,
        1530, 1,    0,    0,    0,    1532, 1533, 1,    0,    0,    0,    1533, 1535, 1,    0,    0,    0,    1534,
        1532, 1,    0,    0,    0,    1535, 1536, 5,    81,   0,    0,    1536, 207,  1,    0,    0,    0,    1537,
        1541, 5,    6,    0,    0,    1538, 1542, 3,    190,  95,   0,    1539, 1542, 5,    77,   0,    0,    1540,
        1542, 3,    210,  105,  0,    1541, 1538, 1,    0,    0,    0,    1541, 1539, 1,    0,    0,    0,    1541,
        1540, 1,    0,    0,    0,    1542, 1543, 1,    0,    0,    0,    1543, 1544, 7,    13,   0,    0,    1544,
        1549, 3,    212,  106,  0,    1545, 1546, 5,    12,   0,    0,    1546, 1547, 7,    13,   0,    0,    1547,
        1549, 3,    212,  106,  0,    1548, 1537, 1,    0,    0,    0,    1548, 1545, 1,    0,    0,    0,    1549,
        209,  1,    0,    0,    0,    1550, 1551, 6,    105,  -1,   0,    1551, 1552, 5,    78,   0,    0,    1552,
        1553, 3,    210,  105,  0,    1553, 1554, 5,    79,   0,    0,    1554, 1577, 1,    0,    0,    0,    1555,
        1557, 3,    12,   6,    0,    1556, 1555, 1,    0,    0,    0,    1557, 1560, 1,    0,    0,    0,    1558,
        1556, 1,    0,    0,    0,    1558, 1559, 1,    0,    0,    0,    1559, 1561, 1,    0,    0,    0,    1560,
        1558, 1,    0,    0,    0,    1561, 1565, 3,    236,  118,  0,    1562, 1564, 3,    110,  55,   0,    1563,
        1562, 1,    0,    0,    0,    1564, 1567, 1,    0,    0,    0,    1565, 1563, 1,    0,    0,    0,    1565,
        1566, 1,    0,    0,    0,    1566, 1568, 1,    0,    0,    0,    1567, 1565, 1,    0,    0,    0,    1568,
        1573, 3,    160,  80,   0,    1569, 1570, 5,    98,   0,    0,    1570, 1572, 3,    194,  97,   0,    1571,
        1569, 1,    0,    0,    0,    1572, 1575, 1,    0,    0,    0,    1573, 1571, 1,    0,    0,    0,    1573,
        1574, 1,    0,    0,    0,    1574, 1577, 1,    0,    0,    0,    1575, 1573, 1,    0,    0,    0,    1576,
        1550, 1,    0,    0,    0,    1576, 1558, 1,    0,    0,    0,    1577, 1583, 1,    0,    0,    0,    1578,
        1579, 10,   1,    0,    0,    1579, 1580, 5,    98,   0,    0,    1580, 1582, 3,    194,  97,   0,    1581,
        1578, 1,    0,    0,    0,    1582, 1585, 1,    0,    0,    0,    1583, 1581, 1,    0,    0,    0,    1583,
        1584, 1,    0,    0,    0,    1584, 211,  1,    0,    0,    0,    1585, 1583, 1,    0,    0,    0,    1586,
        1594, 3,    154,  77,   0,    1587, 1589, 3,    156,  78,   0,    1588, 1587, 1,    0,    0,    0,    1589,
        1592, 1,    0,    0,    0,    1590, 1588, 1,    0,    0,    0,    1590, 1591, 1,    0,    0,    0,    1591,
        1594, 1,    0,    0,    0,    1592, 1590, 1,    0,    0,    0,    1593, 1586, 1,    0,    0,    0,    1593,
        1590, 1,    0,    0,    0,    1594, 213,  1,    0,    0,    0,    1595, 1596, 3,    80,   40,   0,    1596,
        1597, 5,    86,   0,    0,    1597, 1599, 1,    0,    0,    0,    1598, 1595, 1,    0,    0,    0,    1598,
        1599, 1,    0,    0,    0,    1599, 1603, 1,    0,    0,    0,    1600, 1602, 3,    110,  55,   0,    1601,
        1600, 1,    0,    0,    0,    1602, 1605, 1,    0,    0,    0,    1603, 1601, 1,    0,    0,    0,    1603,
        1604, 1,    0,    0,    0,    1604, 1606, 1,    0,    0,    0,    1605, 1603, 1,    0,    0,    0,    1606,
        1608, 3,    160,  80,   0,    1607, 1609, 3,    240,  120,  0,    1608, 1607, 1,    0,    0,    0,    1608,
        1609, 1,    0,    0,    0,    1609, 215,  1,    0,    0,    0,    1610, 1611, 3,    232,  116,  0,    1611,
        1612, 3,    218,  109,  0,    1612, 1613, 3,    224,  112,  0,    1613, 1620, 1,    0,    0,    0,    1614,
        1617, 3,    218,  109,  0,    1615, 1618, 3,    222,  111,  0,    1616, 1618, 3,    224,  112,  0,    1617,
        1615, 1,    0,    0,    0,    1617, 1616, 1,    0,    0,    0,    1618, 1620, 1,    0,    0,    0,    1619,
        1610, 1,    0,    0,    0,    1619, 1614, 1,    0,    0,    0,    1620, 217,  1,    0,    0,    0,    1621,
        1623, 3,    160,  80,   0,    1622, 1624, 3,    228,  114,  0,    1623, 1622, 1,    0,    0,    0,    1623,
        1624, 1,    0,    0,    0,    1624, 1632, 1,    0,    0,    0,    1625, 1626, 5,    86,   0,    0,    1626,
        1628, 3,    160,  80,   0,    1627, 1629, 3,    228,  114,  0,    1628, 1627, 1,    0,    0,    0,    1628,
        1629, 1,    0,    0,    0,    1629, 1631, 1,    0,    0,    0,    1630, 1625, 1,    0,    0,    0,    1631,
        1634, 1,    0,    0,    0,    1632, 1630, 1,    0,    0,    0,    1632, 1633, 1,    0,    0,    0,    1633,
        1637, 1,    0,    0,    0,    1634, 1632, 1,    0,    0,    0,    1635, 1637, 3,    238,  119,  0,    1636,
        1621, 1,    0,    0,    0,    1636, 1635, 1,    0,    0,    0,    1637, 219,  1,    0,    0,    0,    1638,
        1640, 3,    160,  80,   0,    1639, 1641, 3,    230,  115,  0,    1640, 1639, 1,    0,    0,    0,    1640,
        1641, 1,    0,    0,    0,    1641, 1642, 1,    0,    0,    0,    1642, 1643, 3,    224,  112,  0,    1643,
        221,  1,    0,    0,    0,    1644, 1672, 5,    82,   0,    0,    1645, 1650, 5,    83,   0,    0,    1646,
        1647, 5,    82,   0,    0,    1647, 1649, 5,    83,   0,    0,    1648, 1646, 1,    0,    0,    0,    1649,
        1652, 1,    0,    0,    0,    1650, 1648, 1,    0,    0,    0,    1650, 1651, 1,    0,    0,    0,    1651,
        1653, 1,    0,    0,    0,    1652, 1650, 1,    0,    0,    0,    1653, 1673, 3,    78,   39,   0,    1654,
        1655, 3,    194,  97,   0,    1655, 1662, 5,    83,   0,    0,    1656, 1657, 5,    82,   0,    0,    1657,
        1658, 3,    194,  97,   0,    1658, 1659, 5,    83,   0,    0,    1659, 1661, 1,    0,    0,    0,    1660,
        1656, 1,    0,    0,    0,    1661, 1664, 1,    0,    0,    0,    1662, 1660, 1,    0,    0,    0,    1662,
        1663, 1,    0,    0,    0,    1663, 1669, 1,    0,    0,    0,    1664, 1662, 1,    0,    0,    0,    1665,
        1666, 5,    82,   0,    0,    1666, 1668, 5,    83,   0,    0,    1667, 1665, 1,    0,    0,    0,    1668,
        1671, 1,    0,    0,    0,    1669, 1667, 1,    0,    0,    0,    1669, 1670, 1,    0,    0,    0,    1670,
        1673, 1,    0,    0,    0,    1671, 1669, 1,    0,    0,    0,    1672, 1645, 1,    0,    0,    0,    1672,
        1654, 1,    0,    0,    0,    1673, 223,  1,    0,    0,    0,    1674, 1676, 3,    246,  123,  0,    1675,
        1677, 3,    32,   16,   0,    1676, 1675, 1,    0,    0,    0,    1676, 1677, 1,    0,    0,    0,    1677,
        225,  1,    0,    0,    0,    1678, 1679, 3,    232,  116,  0,    1679, 1680, 3,    244,  122,  0,    1680,
        227,  1,    0,    0,    0,    1681, 1682, 5,    89,   0,    0,    1682, 1685, 5,    88,   0,    0,    1683,
        1685, 3,    240,  120,  0,    1684, 1681, 1,    0,    0,    0,    1684, 1683, 1,    0,    0,    0,    1685,
        229,  1,    0,    0,    0,    1686, 1687, 5,    89,   0,    0,    1687, 1690, 5,    88,   0,    0,    1688,
        1690, 3,    232,  116,  0,    1689, 1686, 1,    0,    0,    0,    1689, 1688, 1,    0,    0,    0,    1690,
        231,  1,    0,    0,    0,    1691, 1692, 5,    89,   0,    0,    1692, 1693, 3,    234,  117,  0,    1693,
        1694, 5,    88,   0,    0,    1694, 233,  1,    0,    0,    0,    1695, 1700, 3,    236,  118,  0,    1696,
        1697, 5,    85,   0,    0,    1697, 1699, 3,    236,  118,  0,    1698, 1696, 1,    0,    0,    0,    1699,
        1702, 1,    0,    0,    0,    1700, 1698, 1,    0,    0,    0,    1700, 1701, 1,    0,    0,    0,    1701,
        235,  1,    0,    0,    0,    1702, 1700, 1,    0,    0,    0,    1703, 1705, 3,    110,  55,   0,    1704,
        1703, 1,    0,    0,    0,    1705, 1708, 1,    0,    0,    0,    1706, 1704, 1,    0,    0,    0,    1706,
        1707, 1,    0,    0,    0,    1707, 1711, 1,    0,    0,    0,    1708, 1706, 1,    0,    0,    0,    1709,
        1712, 3,    80,   40,   0,    1710, 1712, 3,    238,  119,  0,    1711, 1709, 1,    0,    0,    0,    1711,
        1710, 1,    0,    0,    0,    1712, 1723, 1,    0,    0,    0,    1713, 1715, 3,    110,  55,   0,    1714,
        1713, 1,    0,    0,    0,    1715, 1718, 1,    0,    0,    0,    1716, 1714, 1,    0,    0,    0,    1716,
        1717, 1,    0,    0,    0,    1717, 1719, 1,    0,    0,    0,    1718, 1716, 1,    0,    0,    0,    1719,
        1720, 5,    82,   0,    0,    1720, 1722, 5,    83,   0,    0,    1721, 1716, 1,    0,    0,    0,    1722,
        1725, 1,    0,    0,    0,    1723, 1721, 1,    0,    0,    0,    1723, 1724, 1,    0,    0,    0,    1724,
        237,  1,    0,    0,    0,    1725, 1723, 1,    0,    0,    0,    1726, 1727, 7,    14,   0,    0,    1727,
        239,  1,    0,    0,    0,    1728, 1729, 5,    89,   0,    0,    1729, 1734, 3,    82,   41,   0,    1730,
        1731, 5,    85,   0,    0,    1731, 1733, 3,    82,   41,   0,    1732, 1730, 1,    0,    0,    0,    1733,
        1736, 1,    0,    0,    0,    1734, 1732, 1,    0,    0,    0,    1734, 1735, 1,    0,    0,    0,    1735,
        1737, 1,    0,    0,    0,    1736, 1734, 1,    0,    0,    0,    1737, 1738, 5,    88,   0,    0,    1738,
        241,  1,    0,    0,    0,    1739, 1749, 3,    246,  123,  0,    1740, 1742, 5,    86,   0,    0,    1741,
        1743, 3,    240,  120,  0,    1742, 1741, 1,    0,    0,    0,    1742, 1743, 1,    0,    0,    0,    1743,
        1744, 1,    0,    0,    0,    1744, 1746, 3,    160,  80,   0,    1745, 1747, 3,    246,  123,  0,    1746,
        1745, 1,    0,    0,    0,    1746, 1747, 1,    0,    0,    0,    1747, 1749, 1,    0,    0,    0,    1748,
        1739, 1,    0,    0,    0,    1748, 1740, 1,    0,    0,    0,    1749, 243,  1,    0,    0,    0,    1750,
        1751, 5,    40,   0,    0,    1751, 1756, 3,    242,  121,  0,    1752, 1753, 3,    160,  80,   0,    1753,
        1754, 3,    246,  123,  0,    1754, 1756, 1,    0,    0,    0,    1755, 1750, 1,    0,    0,    0,    1755,
        1752, 1,    0,    0,    0,    1756, 245,  1,    0,    0,    0,    1757, 1759, 5,    78,   0,    0,    1758,
        1760, 3,    190,  95,   0,    1759, 1758, 1,    0,    0,    0,    1759, 1760, 1,    0,    0,    0,    1760,
        1761, 1,    0,    0,    0,    1761, 1762, 5,    79,   0,    0,    1762, 247,  1,    0,    0,    0,    220,
        249,  254,  260,  266,  271,  280,  285,  292,  300,  303,  310,  322,  326,  331,  335,  339,  343,  353,
        361,  369,  373,  380,  387,  391,  394,  397,  406,  412,  417,  420,  426,  432,  436,  440,  448,  457,
        464,  470,  474,  486,  495,  500,  506,  510,  522,  533,  538,  548,  556,  566,  575,  586,  591,  600,
        610,  615,  624,  630,  637,  642,  650,  654,  656,  662,  667,  671,  678,  684,  686,  693,  698,  703,
        706,  708,  718,  728,  733,  736,  741,  750,  757,  768,  774,  785,  795,  806,  815,  820,  823,  830,
        840,  848,  851,  854,  867,  875,  880,  888,  892,  896,  900,  904,  906,  910,  916,  924,  934,  943,
        953,  961,  975,  982,  987,  993,  1002, 1012, 1021, 1031, 1036, 1047, 1054, 1060, 1063, 1070, 1079, 1102,
        1105, 1108, 1116, 1120, 1128, 1134, 1145, 1154, 1159, 1172, 1178, 1185, 1198, 1207, 1216, 1222, 1230, 1236,
        1241, 1246, 1254, 1259, 1263, 1267, 1271, 1273, 1277, 1282, 1287, 1302, 1308, 1315, 1321, 1324, 1335, 1343,
        1358, 1362, 1367, 1371, 1387, 1427, 1433, 1446, 1451, 1454, 1456, 1462, 1469, 1481, 1490, 1497, 1500, 1504,
        1522, 1524, 1532, 1541, 1548, 1558, 1565, 1573, 1576, 1583, 1590, 1593, 1598, 1603, 1608, 1617, 1619, 1623,
        1628, 1632, 1636, 1640, 1650, 1662, 1669, 1672, 1676, 1684, 1689, 1700, 1706, 1711, 1716, 1723, 1734, 1742,
        1746, 1748, 1755, 1759};
    staticData->serializedATN = antlr4::atn::SerializedATNView(
        serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

    antlr4::atn::ATNDeserializer deserializer;
    staticData->atn = deserializer.deserialize(staticData->serializedATN);

    const size_t count = staticData->atn->getNumberOfDecisions();
    staticData->decisionToDFA.reserve(count);
    for (size_t i = 0; i < count; i++) {
        staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
    }
    javaparserParserStaticData = staticData.release();
}

}  // namespace

JavaParser::JavaParser(TokenStream* input) : JavaParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

JavaParser::JavaParser(TokenStream* input, const antlr4::atn::ParserATNSimulatorOptions& options) : Parser(input)
{
    JavaParser::initialize();
    _interpreter =
        new atn::ParserATNSimulator(this, *javaparserParserStaticData->atn, javaparserParserStaticData->decisionToDFA,
                                    javaparserParserStaticData->sharedContextCache, options);
}

JavaParser::~JavaParser()
{
    delete _interpreter;
}

const atn::ATN& JavaParser::getATN() const
{
    return *javaparserParserStaticData->atn;
}

std::string JavaParser::getGrammarFileName() const
{
    return "JavaParser.g4";
}

const std::vector<std::string>& JavaParser::getRuleNames() const
{
    return javaparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& JavaParser::getVocabulary() const
{
    return javaparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView JavaParser::getSerializedATN() const
{
    return javaparserParserStaticData->serializedATN;
}

//----------------- CompilationUnitContext ------------------------------------------------------------------

JavaParser::CompilationUnitContext::CompilationUnitContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::PackageDeclarationContext* JavaParser::CompilationUnitContext::packageDeclaration()
{
    return getRuleContext<JavaParser::PackageDeclarationContext>(0);
}

std::vector<JavaParser::ImportDeclarationContext*> JavaParser::CompilationUnitContext::importDeclaration()
{
    return getRuleContexts<JavaParser::ImportDeclarationContext>();
}

JavaParser::ImportDeclarationContext* JavaParser::CompilationUnitContext::importDeclaration(size_t i)
{
    return getRuleContext<JavaParser::ImportDeclarationContext>(i);
}

std::vector<JavaParser::TypeDeclarationContext*> JavaParser::CompilationUnitContext::typeDeclaration()
{
    return getRuleContexts<JavaParser::TypeDeclarationContext>();
}

JavaParser::TypeDeclarationContext* JavaParser::CompilationUnitContext::typeDeclaration(size_t i)
{
    return getRuleContext<JavaParser::TypeDeclarationContext>(i);
}

JavaParser::ModuleDeclarationContext* JavaParser::CompilationUnitContext::moduleDeclaration()
{
    return getRuleContext<JavaParser::ModuleDeclarationContext>(0);
}

tree::TerminalNode* JavaParser::CompilationUnitContext::EOF()
{
    return getToken(JavaParser::EOF, 0);
}

size_t JavaParser::CompilationUnitContext::getRuleIndex() const
{
    return JavaParser::RuleCompilationUnit;
}

JavaParser::CompilationUnitContext* JavaParser::compilationUnit()
{
    CompilationUnitContext* _localctx = _tracker.createInstance<CompilationUnitContext>(_ctx, getState());
    enterRule(_localctx, 0, JavaParser::RuleCompilationUnit);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(266);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(249);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
                    case 1: {
                        setState(248);
                        packageDeclaration();
                        break;
                    }

                    default:
                        break;
                }
                setState(254);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == JavaParser::IMPORT) {
                    setState(251);
                    importDeclaration();
                    setState(256);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(260);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (((((_la - 1) & ~0x3fULL) == 0) &&
                        ((1ULL << (_la - 1)) &
                         ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::CLASS - 1)) |
                          (1ULL << (JavaParser::ENUM - 1)) | (1ULL << (JavaParser::FINAL - 1)) |
                          (1ULL << (JavaParser::INTERFACE - 1)) | (1ULL << (JavaParser::PRIVATE - 1)) |
                          (1ULL << (JavaParser::PROTECTED - 1)) | (1ULL << (JavaParser::PUBLIC - 1)) |
                          (1ULL << (JavaParser::STATIC - 1)) | (1ULL << (JavaParser::STRICTFP - 1)) |
                          (1ULL << (JavaParser::MODULE - 1)) | (1ULL << (JavaParser::OPEN - 1)) |
                          (1ULL << (JavaParser::REQUIRES - 1)) | (1ULL << (JavaParser::EXPORTS - 1)) |
                          (1ULL << (JavaParser::OPENS - 1)) | (1ULL << (JavaParser::TO - 1)) |
                          (1ULL << (JavaParser::USES - 1)) | (1ULL << (JavaParser::PROVIDES - 1)) |
                          (1ULL << (JavaParser::WITH - 1)) | (1ULL << (JavaParser::TRANSITIVE - 1)) |
                          (1ULL << (JavaParser::VAR - 1)) | (1ULL << (JavaParser::YIELD - 1)) |
                          (1ULL << (JavaParser::RECORD - 1)) | (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
                       ((((_la - 65) & ~0x3fULL) == 0) &&
                        ((1ULL << (_la - 65)) &
                         ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
                          (1ULL << (JavaParser::SEMI - 65)) | (1ULL << (JavaParser::AT - 65)) |
                          (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
                    setState(257);
                    typeDeclaration();
                    setState(262);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(263);
                moduleDeclaration();
                setState(264);
                match(JavaParser::EOF);
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- PackageDeclarationContext ------------------------------------------------------------------

JavaParser::PackageDeclarationContext::PackageDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::PackageDeclarationContext::PACKAGE()
{
    return getToken(JavaParser::PACKAGE, 0);
}

JavaParser::QualifiedNameContext* JavaParser::PackageDeclarationContext::qualifiedName()
{
    return getRuleContext<JavaParser::QualifiedNameContext>(0);
}

tree::TerminalNode* JavaParser::PackageDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::PackageDeclarationContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::PackageDeclarationContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

size_t JavaParser::PackageDeclarationContext::getRuleIndex() const
{
    return JavaParser::RulePackageDeclaration;
}

JavaParser::PackageDeclarationContext* JavaParser::packageDeclaration()
{
    PackageDeclarationContext* _localctx = _tracker.createInstance<PackageDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 2, JavaParser::RulePackageDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(271);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 51) & ~0x3fULL) == 0) &&
                ((1ULL << (_la - 51)) & ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                                         (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                                         (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                                         (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                                         (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                                         (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                                         (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                                         (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
               _la == JavaParser::AT

               || _la == JavaParser::IDENTIFIER) {
            setState(268);
            annotation();
            setState(273);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(274);
        match(JavaParser::PACKAGE);
        setState(275);
        qualifiedName();
        setState(276);
        match(JavaParser::SEMI);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ImportDeclarationContext ------------------------------------------------------------------

JavaParser::ImportDeclarationContext::ImportDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ImportDeclarationContext::IMPORT()
{
    return getToken(JavaParser::IMPORT, 0);
}

JavaParser::QualifiedNameContext* JavaParser::ImportDeclarationContext::qualifiedName()
{
    return getRuleContext<JavaParser::QualifiedNameContext>(0);
}

tree::TerminalNode* JavaParser::ImportDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

tree::TerminalNode* JavaParser::ImportDeclarationContext::STATIC()
{
    return getToken(JavaParser::STATIC, 0);
}

tree::TerminalNode* JavaParser::ImportDeclarationContext::DOT()
{
    return getToken(JavaParser::DOT, 0);
}

tree::TerminalNode* JavaParser::ImportDeclarationContext::MUL()
{
    return getToken(JavaParser::MUL, 0);
}

size_t JavaParser::ImportDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleImportDeclaration;
}

JavaParser::ImportDeclarationContext* JavaParser::importDeclaration()
{
    ImportDeclarationContext* _localctx = _tracker.createInstance<ImportDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 4, JavaParser::RuleImportDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(278);
        match(JavaParser::IMPORT);
        setState(280);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::STATIC) {
            setState(279);
            match(JavaParser::STATIC);
        }
        setState(282);
        qualifiedName();
        setState(285);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::DOT) {
            setState(283);
            match(JavaParser::DOT);
            setState(284);
            match(JavaParser::MUL);
        }
        setState(287);
        match(JavaParser::SEMI);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeDeclarationContext ------------------------------------------------------------------

JavaParser::TypeDeclarationContext::TypeDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ClassDeclarationContext* JavaParser::TypeDeclarationContext::classDeclaration()
{
    return getRuleContext<JavaParser::ClassDeclarationContext>(0);
}

JavaParser::EnumDeclarationContext* JavaParser::TypeDeclarationContext::enumDeclaration()
{
    return getRuleContext<JavaParser::EnumDeclarationContext>(0);
}

JavaParser::InterfaceDeclarationContext* JavaParser::TypeDeclarationContext::interfaceDeclaration()
{
    return getRuleContext<JavaParser::InterfaceDeclarationContext>(0);
}

JavaParser::AnnotationTypeDeclarationContext* JavaParser::TypeDeclarationContext::annotationTypeDeclaration()
{
    return getRuleContext<JavaParser::AnnotationTypeDeclarationContext>(0);
}

JavaParser::RecordDeclarationContext* JavaParser::TypeDeclarationContext::recordDeclaration()
{
    return getRuleContext<JavaParser::RecordDeclarationContext>(0);
}

std::vector<JavaParser::ClassOrInterfaceModifierContext*> JavaParser::TypeDeclarationContext::classOrInterfaceModifier()
{
    return getRuleContexts<JavaParser::ClassOrInterfaceModifierContext>();
}

JavaParser::ClassOrInterfaceModifierContext* JavaParser::TypeDeclarationContext::classOrInterfaceModifier(size_t i)
{
    return getRuleContext<JavaParser::ClassOrInterfaceModifierContext>(i);
}

tree::TerminalNode* JavaParser::TypeDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

size_t JavaParser::TypeDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleTypeDeclaration;
}

JavaParser::TypeDeclarationContext* JavaParser::typeDeclaration()
{
    TypeDeclarationContext* _localctx = _tracker.createInstance<TypeDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 6, JavaParser::RuleTypeDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(303);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::ABSTRACT:
            case JavaParser::CLASS:
            case JavaParser::ENUM:
            case JavaParser::FINAL:
            case JavaParser::INTERFACE:
            case JavaParser::PRIVATE:
            case JavaParser::PROTECTED:
            case JavaParser::PUBLIC:
            case JavaParser::STATIC:
            case JavaParser::STRICTFP:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::NON_SEALED:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(292);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(289);
                        classOrInterfaceModifier();
                    }
                    setState(294);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
                }
                setState(300);
                _errHandler->sync(this);
                switch (_input->LA(1)) {
                    case JavaParser::CLASS: {
                        setState(295);
                        classDeclaration();
                        break;
                    }

                    case JavaParser::ENUM: {
                        setState(296);
                        enumDeclaration();
                        break;
                    }

                    case JavaParser::INTERFACE: {
                        setState(297);
                        interfaceDeclaration();
                        break;
                    }

                    case JavaParser::AT: {
                        setState(298);
                        annotationTypeDeclaration();
                        break;
                    }

                    case JavaParser::RECORD: {
                        setState(299);
                        recordDeclaration();
                        break;
                    }

                    default:
                        throw NoViableAltException(this);
                }
                break;
            }

            case JavaParser::SEMI: {
                enterOuterAlt(_localctx, 2);
                setState(302);
                match(JavaParser::SEMI);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ModifierContext ------------------------------------------------------------------

JavaParser::ModifierContext::ModifierContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ClassOrInterfaceModifierContext* JavaParser::ModifierContext::classOrInterfaceModifier()
{
    return getRuleContext<JavaParser::ClassOrInterfaceModifierContext>(0);
}

tree::TerminalNode* JavaParser::ModifierContext::NATIVE()
{
    return getToken(JavaParser::NATIVE, 0);
}

tree::TerminalNode* JavaParser::ModifierContext::SYNCHRONIZED()
{
    return getToken(JavaParser::SYNCHRONIZED, 0);
}

tree::TerminalNode* JavaParser::ModifierContext::TRANSIENT()
{
    return getToken(JavaParser::TRANSIENT, 0);
}

tree::TerminalNode* JavaParser::ModifierContext::VOLATILE()
{
    return getToken(JavaParser::VOLATILE, 0);
}

size_t JavaParser::ModifierContext::getRuleIndex() const
{
    return JavaParser::RuleModifier;
}

JavaParser::ModifierContext* JavaParser::modifier()
{
    ModifierContext* _localctx = _tracker.createInstance<ModifierContext>(_ctx, getState());
    enterRule(_localctx, 8, JavaParser::RuleModifier);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(310);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::ABSTRACT:
            case JavaParser::FINAL:
            case JavaParser::PRIVATE:
            case JavaParser::PROTECTED:
            case JavaParser::PUBLIC:
            case JavaParser::STATIC:
            case JavaParser::STRICTFP:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::NON_SEALED:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(305);
                classOrInterfaceModifier();
                break;
            }

            case JavaParser::NATIVE: {
                enterOuterAlt(_localctx, 2);
                setState(306);
                match(JavaParser::NATIVE);
                break;
            }

            case JavaParser::SYNCHRONIZED: {
                enterOuterAlt(_localctx, 3);
                setState(307);
                match(JavaParser::SYNCHRONIZED);
                break;
            }

            case JavaParser::TRANSIENT: {
                enterOuterAlt(_localctx, 4);
                setState(308);
                match(JavaParser::TRANSIENT);
                break;
            }

            case JavaParser::VOLATILE: {
                enterOuterAlt(_localctx, 5);
                setState(309);
                match(JavaParser::VOLATILE);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ClassOrInterfaceModifierContext ------------------------------------------------------------------

JavaParser::ClassOrInterfaceModifierContext::ClassOrInterfaceModifierContext(ParserRuleContext* parent,
                                                                             size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::AnnotationContext* JavaParser::ClassOrInterfaceModifierContext::annotation()
{
    return getRuleContext<JavaParser::AnnotationContext>(0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::PUBLIC()
{
    return getToken(JavaParser::PUBLIC, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::PROTECTED()
{
    return getToken(JavaParser::PROTECTED, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::PRIVATE()
{
    return getToken(JavaParser::PRIVATE, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::STATIC()
{
    return getToken(JavaParser::STATIC, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::ABSTRACT()
{
    return getToken(JavaParser::ABSTRACT, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::FINAL()
{
    return getToken(JavaParser::FINAL, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::STRICTFP()
{
    return getToken(JavaParser::STRICTFP, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::SEALED()
{
    return getToken(JavaParser::SEALED, 0);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceModifierContext::NON_SEALED()
{
    return getToken(JavaParser::NON_SEALED, 0);
}

size_t JavaParser::ClassOrInterfaceModifierContext::getRuleIndex() const
{
    return JavaParser::RuleClassOrInterfaceModifier;
}

JavaParser::ClassOrInterfaceModifierContext* JavaParser::classOrInterfaceModifier()
{
    ClassOrInterfaceModifierContext* _localctx =
        _tracker.createInstance<ClassOrInterfaceModifierContext>(_ctx, getState());
    enterRule(_localctx, 10, JavaParser::RuleClassOrInterfaceModifier);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(322);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(312);
                annotation();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(313);
                match(JavaParser::PUBLIC);
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(314);
                match(JavaParser::PROTECTED);
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(315);
                match(JavaParser::PRIVATE);
                break;
            }

            case 5: {
                enterOuterAlt(_localctx, 5);
                setState(316);
                match(JavaParser::STATIC);
                break;
            }

            case 6: {
                enterOuterAlt(_localctx, 6);
                setState(317);
                match(JavaParser::ABSTRACT);
                break;
            }

            case 7: {
                enterOuterAlt(_localctx, 7);
                setState(318);
                match(JavaParser::FINAL);
                break;
            }

            case 8: {
                enterOuterAlt(_localctx, 8);
                setState(319);
                match(JavaParser::STRICTFP);
                break;
            }

            case 9: {
                enterOuterAlt(_localctx, 9);
                setState(320);
                match(JavaParser::SEALED);
                break;
            }

            case 10: {
                enterOuterAlt(_localctx, 10);
                setState(321);
                match(JavaParser::NON_SEALED);
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- VariableModifierContext ------------------------------------------------------------------

JavaParser::VariableModifierContext::VariableModifierContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::VariableModifierContext::FINAL()
{
    return getToken(JavaParser::FINAL, 0);
}

JavaParser::AnnotationContext* JavaParser::VariableModifierContext::annotation()
{
    return getRuleContext<JavaParser::AnnotationContext>(0);
}

size_t JavaParser::VariableModifierContext::getRuleIndex() const
{
    return JavaParser::RuleVariableModifier;
}

JavaParser::VariableModifierContext* JavaParser::variableModifier()
{
    VariableModifierContext* _localctx = _tracker.createInstance<VariableModifierContext>(_ctx, getState());
    enterRule(_localctx, 12, JavaParser::RuleVariableModifier);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(326);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::FINAL: {
                enterOuterAlt(_localctx, 1);
                setState(324);
                match(JavaParser::FINAL);
                break;
            }

            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 2);
                setState(325);
                annotation();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ClassDeclarationContext ------------------------------------------------------------------

JavaParser::ClassDeclarationContext::ClassDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ClassDeclarationContext::CLASS()
{
    return getToken(JavaParser::CLASS, 0);
}

JavaParser::IdentifierContext* JavaParser::ClassDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::ClassBodyContext* JavaParser::ClassDeclarationContext::classBody()
{
    return getRuleContext<JavaParser::ClassBodyContext>(0);
}

JavaParser::TypeParametersContext* JavaParser::ClassDeclarationContext::typeParameters()
{
    return getRuleContext<JavaParser::TypeParametersContext>(0);
}

tree::TerminalNode* JavaParser::ClassDeclarationContext::EXTENDS()
{
    return getToken(JavaParser::EXTENDS, 0);
}

JavaParser::TypeTypeContext* JavaParser::ClassDeclarationContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

tree::TerminalNode* JavaParser::ClassDeclarationContext::IMPLEMENTS()
{
    return getToken(JavaParser::IMPLEMENTS, 0);
}

std::vector<JavaParser::TypeListContext*> JavaParser::ClassDeclarationContext::typeList()
{
    return getRuleContexts<JavaParser::TypeListContext>();
}

JavaParser::TypeListContext* JavaParser::ClassDeclarationContext::typeList(size_t i)
{
    return getRuleContext<JavaParser::TypeListContext>(i);
}

tree::TerminalNode* JavaParser::ClassDeclarationContext::PERMITS()
{
    return getToken(JavaParser::PERMITS, 0);
}

size_t JavaParser::ClassDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleClassDeclaration;
}

JavaParser::ClassDeclarationContext* JavaParser::classDeclaration()
{
    ClassDeclarationContext* _localctx = _tracker.createInstance<ClassDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 14, JavaParser::RuleClassDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(328);
        match(JavaParser::CLASS);
        setState(329);
        identifier();
        setState(331);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LT) {
            setState(330);
            typeParameters();
        }
        setState(335);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::EXTENDS) {
            setState(333);
            match(JavaParser::EXTENDS);
            setState(334);
            typeType();
        }
        setState(339);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::IMPLEMENTS) {
            setState(337);
            match(JavaParser::IMPLEMENTS);
            setState(338);
            typeList();
        }
        setState(343);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::PERMITS) {
            setState(341);
            match(JavaParser::PERMITS);
            setState(342);
            typeList();
        }
        setState(345);
        classBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeParametersContext ------------------------------------------------------------------

JavaParser::TypeParametersContext::TypeParametersContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::TypeParametersContext::LT()
{
    return getToken(JavaParser::LT, 0);
}

std::vector<JavaParser::TypeParameterContext*> JavaParser::TypeParametersContext::typeParameter()
{
    return getRuleContexts<JavaParser::TypeParameterContext>();
}

JavaParser::TypeParameterContext* JavaParser::TypeParametersContext::typeParameter(size_t i)
{
    return getRuleContext<JavaParser::TypeParameterContext>(i);
}

tree::TerminalNode* JavaParser::TypeParametersContext::GT()
{
    return getToken(JavaParser::GT, 0);
}

std::vector<tree::TerminalNode*> JavaParser::TypeParametersContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::TypeParametersContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::TypeParametersContext::getRuleIndex() const
{
    return JavaParser::RuleTypeParameters;
}

JavaParser::TypeParametersContext* JavaParser::typeParameters()
{
    TypeParametersContext* _localctx = _tracker.createInstance<TypeParametersContext>(_ctx, getState());
    enterRule(_localctx, 16, JavaParser::RuleTypeParameters);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(347);
        match(JavaParser::LT);
        setState(348);
        typeParameter();
        setState(353);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(349);
            match(JavaParser::COMMA);
            setState(350);
            typeParameter();
            setState(355);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(356);
        match(JavaParser::GT);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeParameterContext ------------------------------------------------------------------

JavaParser::TypeParameterContext::TypeParameterContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::TypeParameterContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::TypeParameterContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::TypeParameterContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

tree::TerminalNode* JavaParser::TypeParameterContext::EXTENDS()
{
    return getToken(JavaParser::EXTENDS, 0);
}

JavaParser::TypeBoundContext* JavaParser::TypeParameterContext::typeBound()
{
    return getRuleContext<JavaParser::TypeBoundContext>(0);
}

size_t JavaParser::TypeParameterContext::getRuleIndex() const
{
    return JavaParser::RuleTypeParameter;
}

JavaParser::TypeParameterContext* JavaParser::typeParameter()
{
    TypeParameterContext* _localctx = _tracker.createInstance<TypeParameterContext>(_ctx, getState());
    enterRule(_localctx, 18, JavaParser::RuleTypeParameter);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(361);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(358);
                annotation();
            }
            setState(363);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
        }
        setState(364);
        identifier();
        setState(373);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::EXTENDS) {
            setState(365);
            match(JavaParser::EXTENDS);
            setState(369);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
            while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                if (alt == 1) {
                    setState(366);
                    annotation();
                }
                setState(371);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
            }
            setState(372);
            typeBound();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeBoundContext ------------------------------------------------------------------

JavaParser::TypeBoundContext::TypeBoundContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::TypeTypeContext*> JavaParser::TypeBoundContext::typeType()
{
    return getRuleContexts<JavaParser::TypeTypeContext>();
}

JavaParser::TypeTypeContext* JavaParser::TypeBoundContext::typeType(size_t i)
{
    return getRuleContext<JavaParser::TypeTypeContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::TypeBoundContext::BITAND()
{
    return getTokens(JavaParser::BITAND);
}

tree::TerminalNode* JavaParser::TypeBoundContext::BITAND(size_t i)
{
    return getToken(JavaParser::BITAND, i);
}

size_t JavaParser::TypeBoundContext::getRuleIndex() const
{
    return JavaParser::RuleTypeBound;
}

JavaParser::TypeBoundContext* JavaParser::typeBound()
{
    TypeBoundContext* _localctx = _tracker.createInstance<TypeBoundContext>(_ctx, getState());
    enterRule(_localctx, 20, JavaParser::RuleTypeBound);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(375);
        typeType();
        setState(380);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::BITAND) {
            setState(376);
            match(JavaParser::BITAND);
            setState(377);
            typeType();
            setState(382);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- EnumDeclarationContext ------------------------------------------------------------------

JavaParser::EnumDeclarationContext::EnumDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::EnumDeclarationContext::ENUM()
{
    return getToken(JavaParser::ENUM, 0);
}

JavaParser::IdentifierContext* JavaParser::EnumDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::EnumDeclarationContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::EnumDeclarationContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

tree::TerminalNode* JavaParser::EnumDeclarationContext::IMPLEMENTS()
{
    return getToken(JavaParser::IMPLEMENTS, 0);
}

JavaParser::TypeListContext* JavaParser::EnumDeclarationContext::typeList()
{
    return getRuleContext<JavaParser::TypeListContext>(0);
}

JavaParser::EnumConstantsContext* JavaParser::EnumDeclarationContext::enumConstants()
{
    return getRuleContext<JavaParser::EnumConstantsContext>(0);
}

tree::TerminalNode* JavaParser::EnumDeclarationContext::COMMA()
{
    return getToken(JavaParser::COMMA, 0);
}

JavaParser::EnumBodyDeclarationsContext* JavaParser::EnumDeclarationContext::enumBodyDeclarations()
{
    return getRuleContext<JavaParser::EnumBodyDeclarationsContext>(0);
}

size_t JavaParser::EnumDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleEnumDeclaration;
}

JavaParser::EnumDeclarationContext* JavaParser::enumDeclaration()
{
    EnumDeclarationContext* _localctx = _tracker.createInstance<EnumDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 22, JavaParser::RuleEnumDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(383);
        match(JavaParser::ENUM);
        setState(384);
        identifier();
        setState(387);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::IMPLEMENTS) {
            setState(385);
            match(JavaParser::IMPLEMENTS);
            setState(386);
            typeList();
        }
        setState(389);
        match(JavaParser::LBRACE);
        setState(391);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 51) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 51)) & ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                                      (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                                      (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                                      (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                                      (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                                      (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                                      (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                                      (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
            _la == JavaParser::AT

            || _la == JavaParser::IDENTIFIER) {
            setState(390);
            enumConstants();
        }
        setState(394);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::COMMA) {
            setState(393);
            match(JavaParser::COMMA);
        }
        setState(397);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::SEMI) {
            setState(396);
            enumBodyDeclarations();
        }
        setState(399);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- EnumConstantsContext ------------------------------------------------------------------

JavaParser::EnumConstantsContext::EnumConstantsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::EnumConstantContext*> JavaParser::EnumConstantsContext::enumConstant()
{
    return getRuleContexts<JavaParser::EnumConstantContext>();
}

JavaParser::EnumConstantContext* JavaParser::EnumConstantsContext::enumConstant(size_t i)
{
    return getRuleContext<JavaParser::EnumConstantContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::EnumConstantsContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::EnumConstantsContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::EnumConstantsContext::getRuleIndex() const
{
    return JavaParser::RuleEnumConstants;
}

JavaParser::EnumConstantsContext* JavaParser::enumConstants()
{
    EnumConstantsContext* _localctx = _tracker.createInstance<EnumConstantsContext>(_ctx, getState());
    enterRule(_localctx, 24, JavaParser::RuleEnumConstants);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(401);
        enumConstant();
        setState(406);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(402);
                match(JavaParser::COMMA);
                setState(403);
                enumConstant();
            }
            setState(408);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- EnumConstantContext ------------------------------------------------------------------

JavaParser::EnumConstantContext::EnumConstantContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::EnumConstantContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::EnumConstantContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::EnumConstantContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

JavaParser::ArgumentsContext* JavaParser::EnumConstantContext::arguments()
{
    return getRuleContext<JavaParser::ArgumentsContext>(0);
}

JavaParser::ClassBodyContext* JavaParser::EnumConstantContext::classBody()
{
    return getRuleContext<JavaParser::ClassBodyContext>(0);
}

size_t JavaParser::EnumConstantContext::getRuleIndex() const
{
    return JavaParser::RuleEnumConstant;
}

JavaParser::EnumConstantContext* JavaParser::enumConstant()
{
    EnumConstantContext* _localctx = _tracker.createInstance<EnumConstantContext>(_ctx, getState());
    enterRule(_localctx, 26, JavaParser::RuleEnumConstant);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(412);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(409);
                annotation();
            }
            setState(414);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx);
        }
        setState(415);
        identifier();
        setState(417);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LPAREN) {
            setState(416);
            arguments();
        }
        setState(420);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LBRACE) {
            setState(419);
            classBody();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- EnumBodyDeclarationsContext ------------------------------------------------------------------

JavaParser::EnumBodyDeclarationsContext::EnumBodyDeclarationsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::EnumBodyDeclarationsContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

std::vector<JavaParser::ClassBodyDeclarationContext*> JavaParser::EnumBodyDeclarationsContext::classBodyDeclaration()
{
    return getRuleContexts<JavaParser::ClassBodyDeclarationContext>();
}

JavaParser::ClassBodyDeclarationContext* JavaParser::EnumBodyDeclarationsContext::classBodyDeclaration(size_t i)
{
    return getRuleContext<JavaParser::ClassBodyDeclarationContext>(i);
}

size_t JavaParser::EnumBodyDeclarationsContext::getRuleIndex() const
{
    return JavaParser::RuleEnumBodyDeclarations;
}

JavaParser::EnumBodyDeclarationsContext* JavaParser::enumBodyDeclarations()
{
    EnumBodyDeclarationsContext* _localctx = _tracker.createInstance<EnumBodyDeclarationsContext>(_ctx, getState());
    enterRule(_localctx, 28, JavaParser::RuleEnumBodyDeclarations);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(422);
        match(JavaParser::SEMI);
        setState(426);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (
            ((((_la - 1) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 1)) &
              ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::BOOLEAN - 1)) |
               (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) | (1ULL << (JavaParser::CLASS - 1)) |
               (1ULL << (JavaParser::DOUBLE - 1)) | (1ULL << (JavaParser::ENUM - 1)) |
               (1ULL << (JavaParser::FINAL - 1)) | (1ULL << (JavaParser::FLOAT - 1)) | (1ULL << (JavaParser::INT - 1)) |
               (1ULL << (JavaParser::INTERFACE - 1)) | (1ULL << (JavaParser::LONG - 1)) |
               (1ULL << (JavaParser::NATIVE - 1)) | (1ULL << (JavaParser::PRIVATE - 1)) |
               (1ULL << (JavaParser::PROTECTED - 1)) | (1ULL << (JavaParser::PUBLIC - 1)) |
               (1ULL << (JavaParser::SHORT - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
               (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::SYNCHRONIZED - 1)) |
               (1ULL << (JavaParser::TRANSIENT - 1)) | (1ULL << (JavaParser::VOID - 1)) |
               (1ULL << (JavaParser::VOLATILE - 1)) | (1ULL << (JavaParser::MODULE - 1)) |
               (1ULL << (JavaParser::OPEN - 1)) | (1ULL << (JavaParser::REQUIRES - 1)) |
               (1ULL << (JavaParser::EXPORTS - 1)) | (1ULL << (JavaParser::OPENS - 1)) |
               (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
               (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
               (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
               (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
               (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
            ((((_la - 65) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 65)) & ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
                                      (1ULL << (JavaParser::LBRACE - 65)) | (1ULL << (JavaParser::SEMI - 65)) |
                                      (1ULL << (JavaParser::LT - 65)) | (1ULL << (JavaParser::AT - 65)) |
                                      (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
            setState(423);
            classBodyDeclaration();
            setState(428);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InterfaceDeclarationContext ------------------------------------------------------------------

JavaParser::InterfaceDeclarationContext::InterfaceDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::InterfaceDeclarationContext::INTERFACE()
{
    return getToken(JavaParser::INTERFACE, 0);
}

JavaParser::IdentifierContext* JavaParser::InterfaceDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::InterfaceBodyContext* JavaParser::InterfaceDeclarationContext::interfaceBody()
{
    return getRuleContext<JavaParser::InterfaceBodyContext>(0);
}

JavaParser::TypeParametersContext* JavaParser::InterfaceDeclarationContext::typeParameters()
{
    return getRuleContext<JavaParser::TypeParametersContext>(0);
}

tree::TerminalNode* JavaParser::InterfaceDeclarationContext::EXTENDS()
{
    return getToken(JavaParser::EXTENDS, 0);
}

std::vector<JavaParser::TypeListContext*> JavaParser::InterfaceDeclarationContext::typeList()
{
    return getRuleContexts<JavaParser::TypeListContext>();
}

JavaParser::TypeListContext* JavaParser::InterfaceDeclarationContext::typeList(size_t i)
{
    return getRuleContext<JavaParser::TypeListContext>(i);
}

tree::TerminalNode* JavaParser::InterfaceDeclarationContext::PERMITS()
{
    return getToken(JavaParser::PERMITS, 0);
}

size_t JavaParser::InterfaceDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleInterfaceDeclaration;
}

JavaParser::InterfaceDeclarationContext* JavaParser::interfaceDeclaration()
{
    InterfaceDeclarationContext* _localctx = _tracker.createInstance<InterfaceDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 30, JavaParser::RuleInterfaceDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(429);
        match(JavaParser::INTERFACE);
        setState(430);
        identifier();
        setState(432);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LT) {
            setState(431);
            typeParameters();
        }
        setState(436);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::EXTENDS) {
            setState(434);
            match(JavaParser::EXTENDS);
            setState(435);
            typeList();
        }
        setState(440);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::PERMITS) {
            setState(438);
            match(JavaParser::PERMITS);
            setState(439);
            typeList();
        }
        setState(442);
        interfaceBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ClassBodyContext ------------------------------------------------------------------

JavaParser::ClassBodyContext::ClassBodyContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ClassBodyContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::ClassBodyContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::ClassBodyDeclarationContext*> JavaParser::ClassBodyContext::classBodyDeclaration()
{
    return getRuleContexts<JavaParser::ClassBodyDeclarationContext>();
}

JavaParser::ClassBodyDeclarationContext* JavaParser::ClassBodyContext::classBodyDeclaration(size_t i)
{
    return getRuleContext<JavaParser::ClassBodyDeclarationContext>(i);
}

size_t JavaParser::ClassBodyContext::getRuleIndex() const
{
    return JavaParser::RuleClassBody;
}

JavaParser::ClassBodyContext* JavaParser::classBody()
{
    ClassBodyContext* _localctx = _tracker.createInstance<ClassBodyContext>(_ctx, getState());
    enterRule(_localctx, 32, JavaParser::RuleClassBody);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(444);
        match(JavaParser::LBRACE);
        setState(448);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (
            ((((_la - 1) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 1)) &
              ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::BOOLEAN - 1)) |
               (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) | (1ULL << (JavaParser::CLASS - 1)) |
               (1ULL << (JavaParser::DOUBLE - 1)) | (1ULL << (JavaParser::ENUM - 1)) |
               (1ULL << (JavaParser::FINAL - 1)) | (1ULL << (JavaParser::FLOAT - 1)) | (1ULL << (JavaParser::INT - 1)) |
               (1ULL << (JavaParser::INTERFACE - 1)) | (1ULL << (JavaParser::LONG - 1)) |
               (1ULL << (JavaParser::NATIVE - 1)) | (1ULL << (JavaParser::PRIVATE - 1)) |
               (1ULL << (JavaParser::PROTECTED - 1)) | (1ULL << (JavaParser::PUBLIC - 1)) |
               (1ULL << (JavaParser::SHORT - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
               (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::SYNCHRONIZED - 1)) |
               (1ULL << (JavaParser::TRANSIENT - 1)) | (1ULL << (JavaParser::VOID - 1)) |
               (1ULL << (JavaParser::VOLATILE - 1)) | (1ULL << (JavaParser::MODULE - 1)) |
               (1ULL << (JavaParser::OPEN - 1)) | (1ULL << (JavaParser::REQUIRES - 1)) |
               (1ULL << (JavaParser::EXPORTS - 1)) | (1ULL << (JavaParser::OPENS - 1)) |
               (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
               (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
               (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
               (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
               (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
            ((((_la - 65) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 65)) & ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
                                      (1ULL << (JavaParser::LBRACE - 65)) | (1ULL << (JavaParser::SEMI - 65)) |
                                      (1ULL << (JavaParser::LT - 65)) | (1ULL << (JavaParser::AT - 65)) |
                                      (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
            setState(445);
            classBodyDeclaration();
            setState(450);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(451);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InterfaceBodyContext ------------------------------------------------------------------

JavaParser::InterfaceBodyContext::InterfaceBodyContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::InterfaceBodyContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::InterfaceBodyContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::InterfaceBodyDeclarationContext*> JavaParser::InterfaceBodyContext::interfaceBodyDeclaration()
{
    return getRuleContexts<JavaParser::InterfaceBodyDeclarationContext>();
}

JavaParser::InterfaceBodyDeclarationContext* JavaParser::InterfaceBodyContext::interfaceBodyDeclaration(size_t i)
{
    return getRuleContext<JavaParser::InterfaceBodyDeclarationContext>(i);
}

size_t JavaParser::InterfaceBodyContext::getRuleIndex() const
{
    return JavaParser::RuleInterfaceBody;
}

JavaParser::InterfaceBodyContext* JavaParser::interfaceBody()
{
    InterfaceBodyContext* _localctx = _tracker.createInstance<InterfaceBodyContext>(_ctx, getState());
    enterRule(_localctx, 34, JavaParser::RuleInterfaceBody);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(453);
        match(JavaParser::LBRACE);
        setState(457);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (
            ((((_la - 1) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 1)) & ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::BOOLEAN - 1)) |
                                     (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) |
                                     (1ULL << (JavaParser::CLASS - 1)) | (1ULL << (JavaParser::DEFAULT - 1)) |
                                     (1ULL << (JavaParser::DOUBLE - 1)) | (1ULL << (JavaParser::ENUM - 1)) |
                                     (1ULL << (JavaParser::FINAL - 1)) | (1ULL << (JavaParser::FLOAT - 1)) |
                                     (1ULL << (JavaParser::INT - 1)) | (1ULL << (JavaParser::INTERFACE - 1)) |
                                     (1ULL << (JavaParser::LONG - 1)) | (1ULL << (JavaParser::NATIVE - 1)) |
                                     (1ULL << (JavaParser::PRIVATE - 1)) | (1ULL << (JavaParser::PROTECTED - 1)) |
                                     (1ULL << (JavaParser::PUBLIC - 1)) | (1ULL << (JavaParser::SHORT - 1)) |
                                     (1ULL << (JavaParser::STATIC - 1)) | (1ULL << (JavaParser::STRICTFP - 1)) |
                                     (1ULL << (JavaParser::SYNCHRONIZED - 1)) | (1ULL << (JavaParser::TRANSIENT - 1)) |
                                     (1ULL << (JavaParser::VOID - 1)) | (1ULL << (JavaParser::VOLATILE - 1)) |
                                     (1ULL << (JavaParser::MODULE - 1)) | (1ULL << (JavaParser::OPEN - 1)) |
                                     (1ULL << (JavaParser::REQUIRES - 1)) | (1ULL << (JavaParser::EXPORTS - 1)) |
                                     (1ULL << (JavaParser::OPENS - 1)) | (1ULL << (JavaParser::TO - 1)) |
                                     (1ULL << (JavaParser::USES - 1)) | (1ULL << (JavaParser::PROVIDES - 1)) |
                                     (1ULL << (JavaParser::WITH - 1)) | (1ULL << (JavaParser::TRANSITIVE - 1)) |
                                     (1ULL << (JavaParser::VAR - 1)) | (1ULL << (JavaParser::YIELD - 1)) |
                                     (1ULL << (JavaParser::RECORD - 1)) | (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
            ((((_la - 65) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 65)) & ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
                                      (1ULL << (JavaParser::SEMI - 65)) | (1ULL << (JavaParser::LT - 65)) |
                                      (1ULL << (JavaParser::AT - 65)) | (1ULL << (JavaParser::IDENTIFIER - 65)))) !=
                 0)) {
            setState(454);
            interfaceBodyDeclaration();
            setState(459);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(460);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ClassBodyDeclarationContext ------------------------------------------------------------------

JavaParser::ClassBodyDeclarationContext::ClassBodyDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ClassBodyDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

JavaParser::BlockContext* JavaParser::ClassBodyDeclarationContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

tree::TerminalNode* JavaParser::ClassBodyDeclarationContext::STATIC()
{
    return getToken(JavaParser::STATIC, 0);
}

JavaParser::MemberDeclarationContext* JavaParser::ClassBodyDeclarationContext::memberDeclaration()
{
    return getRuleContext<JavaParser::MemberDeclarationContext>(0);
}

std::vector<JavaParser::ModifierContext*> JavaParser::ClassBodyDeclarationContext::modifier()
{
    return getRuleContexts<JavaParser::ModifierContext>();
}

JavaParser::ModifierContext* JavaParser::ClassBodyDeclarationContext::modifier(size_t i)
{
    return getRuleContext<JavaParser::ModifierContext>(i);
}

size_t JavaParser::ClassBodyDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleClassBodyDeclaration;
}

JavaParser::ClassBodyDeclarationContext* JavaParser::classBodyDeclaration()
{
    ClassBodyDeclarationContext* _localctx = _tracker.createInstance<ClassBodyDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 36, JavaParser::RuleClassBodyDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(474);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(462);
                match(JavaParser::SEMI);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(464);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::STATIC) {
                    setState(463);
                    match(JavaParser::STATIC);
                }
                setState(466);
                block();
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(470);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(467);
                        modifier();
                    }
                    setState(472);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
                }
                setState(473);
                memberDeclaration();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- MemberDeclarationContext ------------------------------------------------------------------

JavaParser::MemberDeclarationContext::MemberDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::MethodDeclarationContext* JavaParser::MemberDeclarationContext::methodDeclaration()
{
    return getRuleContext<JavaParser::MethodDeclarationContext>(0);
}

JavaParser::GenericMethodDeclarationContext* JavaParser::MemberDeclarationContext::genericMethodDeclaration()
{
    return getRuleContext<JavaParser::GenericMethodDeclarationContext>(0);
}

JavaParser::FieldDeclarationContext* JavaParser::MemberDeclarationContext::fieldDeclaration()
{
    return getRuleContext<JavaParser::FieldDeclarationContext>(0);
}

JavaParser::ConstructorDeclarationContext* JavaParser::MemberDeclarationContext::constructorDeclaration()
{
    return getRuleContext<JavaParser::ConstructorDeclarationContext>(0);
}

JavaParser::GenericConstructorDeclarationContext* JavaParser::MemberDeclarationContext::genericConstructorDeclaration()
{
    return getRuleContext<JavaParser::GenericConstructorDeclarationContext>(0);
}

JavaParser::InterfaceDeclarationContext* JavaParser::MemberDeclarationContext::interfaceDeclaration()
{
    return getRuleContext<JavaParser::InterfaceDeclarationContext>(0);
}

JavaParser::AnnotationTypeDeclarationContext* JavaParser::MemberDeclarationContext::annotationTypeDeclaration()
{
    return getRuleContext<JavaParser::AnnotationTypeDeclarationContext>(0);
}

JavaParser::ClassDeclarationContext* JavaParser::MemberDeclarationContext::classDeclaration()
{
    return getRuleContext<JavaParser::ClassDeclarationContext>(0);
}

JavaParser::EnumDeclarationContext* JavaParser::MemberDeclarationContext::enumDeclaration()
{
    return getRuleContext<JavaParser::EnumDeclarationContext>(0);
}

JavaParser::RecordDeclarationContext* JavaParser::MemberDeclarationContext::recordDeclaration()
{
    return getRuleContext<JavaParser::RecordDeclarationContext>(0);
}

size_t JavaParser::MemberDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleMemberDeclaration;
}

JavaParser::MemberDeclarationContext* JavaParser::memberDeclaration()
{
    MemberDeclarationContext* _localctx = _tracker.createInstance<MemberDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 38, JavaParser::RuleMemberDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(486);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(476);
                methodDeclaration();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(477);
                genericMethodDeclaration();
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(478);
                fieldDeclaration();
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(479);
                constructorDeclaration();
                break;
            }

            case 5: {
                enterOuterAlt(_localctx, 5);
                setState(480);
                genericConstructorDeclaration();
                break;
            }

            case 6: {
                enterOuterAlt(_localctx, 6);
                setState(481);
                interfaceDeclaration();
                break;
            }

            case 7: {
                enterOuterAlt(_localctx, 7);
                setState(482);
                annotationTypeDeclaration();
                break;
            }

            case 8: {
                enterOuterAlt(_localctx, 8);
                setState(483);
                classDeclaration();
                break;
            }

            case 9: {
                enterOuterAlt(_localctx, 9);
                setState(484);
                enumDeclaration();
                break;
            }

            case 10: {
                enterOuterAlt(_localctx, 10);
                setState(485);
                recordDeclaration();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- MethodDeclarationContext ------------------------------------------------------------------

JavaParser::MethodDeclarationContext::MethodDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeOrVoidContext* JavaParser::MethodDeclarationContext::typeTypeOrVoid()
{
    return getRuleContext<JavaParser::TypeTypeOrVoidContext>(0);
}

JavaParser::IdentifierContext* JavaParser::MethodDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::FormalParametersContext* JavaParser::MethodDeclarationContext::formalParameters()
{
    return getRuleContext<JavaParser::FormalParametersContext>(0);
}

JavaParser::MethodBodyContext* JavaParser::MethodDeclarationContext::methodBody()
{
    return getRuleContext<JavaParser::MethodBodyContext>(0);
}

std::vector<tree::TerminalNode*> JavaParser::MethodDeclarationContext::LBRACK()
{
    return getTokens(JavaParser::LBRACK);
}

tree::TerminalNode* JavaParser::MethodDeclarationContext::LBRACK(size_t i)
{
    return getToken(JavaParser::LBRACK, i);
}

std::vector<tree::TerminalNode*> JavaParser::MethodDeclarationContext::RBRACK()
{
    return getTokens(JavaParser::RBRACK);
}

tree::TerminalNode* JavaParser::MethodDeclarationContext::RBRACK(size_t i)
{
    return getToken(JavaParser::RBRACK, i);
}

tree::TerminalNode* JavaParser::MethodDeclarationContext::THROWS()
{
    return getToken(JavaParser::THROWS, 0);
}

JavaParser::QualifiedNameListContext* JavaParser::MethodDeclarationContext::qualifiedNameList()
{
    return getRuleContext<JavaParser::QualifiedNameListContext>(0);
}

size_t JavaParser::MethodDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleMethodDeclaration;
}

JavaParser::MethodDeclarationContext* JavaParser::methodDeclaration()
{
    MethodDeclarationContext* _localctx = _tracker.createInstance<MethodDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 40, JavaParser::RuleMethodDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(488);
        typeTypeOrVoid();
        setState(489);
        identifier();
        setState(490);
        formalParameters();
        setState(495);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::LBRACK) {
            setState(491);
            match(JavaParser::LBRACK);
            setState(492);
            match(JavaParser::RBRACK);
            setState(497);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(500);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::THROWS) {
            setState(498);
            match(JavaParser::THROWS);
            setState(499);
            qualifiedNameList();
        }
        setState(502);
        methodBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- MethodBodyContext ------------------------------------------------------------------

JavaParser::MethodBodyContext::MethodBodyContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::BlockContext* JavaParser::MethodBodyContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

tree::TerminalNode* JavaParser::MethodBodyContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

size_t JavaParser::MethodBodyContext::getRuleIndex() const
{
    return JavaParser::RuleMethodBody;
}

JavaParser::MethodBodyContext* JavaParser::methodBody()
{
    MethodBodyContext* _localctx = _tracker.createInstance<MethodBodyContext>(_ctx, getState());
    enterRule(_localctx, 42, JavaParser::RuleMethodBody);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(506);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::LBRACE: {
                enterOuterAlt(_localctx, 1);
                setState(504);
                block();
                break;
            }

            case JavaParser::SEMI: {
                enterOuterAlt(_localctx, 2);
                setState(505);
                match(JavaParser::SEMI);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeTypeOrVoidContext ------------------------------------------------------------------

JavaParser::TypeTypeOrVoidContext::TypeTypeOrVoidContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::TypeTypeOrVoidContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

tree::TerminalNode* JavaParser::TypeTypeOrVoidContext::VOID()
{
    return getToken(JavaParser::VOID, 0);
}

size_t JavaParser::TypeTypeOrVoidContext::getRuleIndex() const
{
    return JavaParser::RuleTypeTypeOrVoid;
}

JavaParser::TypeTypeOrVoidContext* JavaParser::typeTypeOrVoid()
{
    TypeTypeOrVoidContext* _localctx = _tracker.createInstance<TypeTypeOrVoidContext>(_ctx, getState());
    enterRule(_localctx, 44, JavaParser::RuleTypeTypeOrVoid);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(510);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::SHORT:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(508);
                typeType();
                break;
            }

            case JavaParser::VOID: {
                enterOuterAlt(_localctx, 2);
                setState(509);
                match(JavaParser::VOID);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- GenericMethodDeclarationContext ------------------------------------------------------------------

JavaParser::GenericMethodDeclarationContext::GenericMethodDeclarationContext(ParserRuleContext* parent,
                                                                             size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeParametersContext* JavaParser::GenericMethodDeclarationContext::typeParameters()
{
    return getRuleContext<JavaParser::TypeParametersContext>(0);
}

JavaParser::MethodDeclarationContext* JavaParser::GenericMethodDeclarationContext::methodDeclaration()
{
    return getRuleContext<JavaParser::MethodDeclarationContext>(0);
}

size_t JavaParser::GenericMethodDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleGenericMethodDeclaration;
}

JavaParser::GenericMethodDeclarationContext* JavaParser::genericMethodDeclaration()
{
    GenericMethodDeclarationContext* _localctx =
        _tracker.createInstance<GenericMethodDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 46, JavaParser::RuleGenericMethodDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(512);
        typeParameters();
        setState(513);
        methodDeclaration();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- GenericConstructorDeclarationContext
//------------------------------------------------------------------

JavaParser::GenericConstructorDeclarationContext::GenericConstructorDeclarationContext(ParserRuleContext* parent,
                                                                                       size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeParametersContext* JavaParser::GenericConstructorDeclarationContext::typeParameters()
{
    return getRuleContext<JavaParser::TypeParametersContext>(0);
}

JavaParser::ConstructorDeclarationContext* JavaParser::GenericConstructorDeclarationContext::constructorDeclaration()
{
    return getRuleContext<JavaParser::ConstructorDeclarationContext>(0);
}

size_t JavaParser::GenericConstructorDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleGenericConstructorDeclaration;
}

JavaParser::GenericConstructorDeclarationContext* JavaParser::genericConstructorDeclaration()
{
    GenericConstructorDeclarationContext* _localctx =
        _tracker.createInstance<GenericConstructorDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 48, JavaParser::RuleGenericConstructorDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(515);
        typeParameters();
        setState(516);
        constructorDeclaration();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ConstructorDeclarationContext ------------------------------------------------------------------

JavaParser::ConstructorDeclarationContext::ConstructorDeclarationContext(ParserRuleContext* parent,
                                                                         size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::ConstructorDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::FormalParametersContext* JavaParser::ConstructorDeclarationContext::formalParameters()
{
    return getRuleContext<JavaParser::FormalParametersContext>(0);
}

JavaParser::BlockContext* JavaParser::ConstructorDeclarationContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

tree::TerminalNode* JavaParser::ConstructorDeclarationContext::THROWS()
{
    return getToken(JavaParser::THROWS, 0);
}

JavaParser::QualifiedNameListContext* JavaParser::ConstructorDeclarationContext::qualifiedNameList()
{
    return getRuleContext<JavaParser::QualifiedNameListContext>(0);
}

size_t JavaParser::ConstructorDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleConstructorDeclaration;
}

JavaParser::ConstructorDeclarationContext* JavaParser::constructorDeclaration()
{
    ConstructorDeclarationContext* _localctx = _tracker.createInstance<ConstructorDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 50, JavaParser::RuleConstructorDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(518);
        identifier();
        setState(519);
        formalParameters();
        setState(522);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::THROWS) {
            setState(520);
            match(JavaParser::THROWS);
            setState(521);
            qualifiedNameList();
        }
        setState(524);
        antlrcpp::downCast<ConstructorDeclarationContext*>(_localctx)->constructorBody = block();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FieldDeclarationContext ------------------------------------------------------------------

JavaParser::FieldDeclarationContext::FieldDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::FieldDeclarationContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::VariableDeclaratorsContext* JavaParser::FieldDeclarationContext::variableDeclarators()
{
    return getRuleContext<JavaParser::VariableDeclaratorsContext>(0);
}

tree::TerminalNode* JavaParser::FieldDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

size_t JavaParser::FieldDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleFieldDeclaration;
}

JavaParser::FieldDeclarationContext* JavaParser::fieldDeclaration()
{
    FieldDeclarationContext* _localctx = _tracker.createInstance<FieldDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 52, JavaParser::RuleFieldDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(526);
        typeType();
        setState(527);
        variableDeclarators();
        setState(528);
        match(JavaParser::SEMI);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InterfaceBodyDeclarationContext ------------------------------------------------------------------

JavaParser::InterfaceBodyDeclarationContext::InterfaceBodyDeclarationContext(ParserRuleContext* parent,
                                                                             size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::InterfaceMemberDeclarationContext* JavaParser::InterfaceBodyDeclarationContext::interfaceMemberDeclaration()
{
    return getRuleContext<JavaParser::InterfaceMemberDeclarationContext>(0);
}

std::vector<JavaParser::ModifierContext*> JavaParser::InterfaceBodyDeclarationContext::modifier()
{
    return getRuleContexts<JavaParser::ModifierContext>();
}

JavaParser::ModifierContext* JavaParser::InterfaceBodyDeclarationContext::modifier(size_t i)
{
    return getRuleContext<JavaParser::ModifierContext>(i);
}

tree::TerminalNode* JavaParser::InterfaceBodyDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

size_t JavaParser::InterfaceBodyDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleInterfaceBodyDeclaration;
}

JavaParser::InterfaceBodyDeclarationContext* JavaParser::interfaceBodyDeclaration()
{
    InterfaceBodyDeclarationContext* _localctx =
        _tracker.createInstance<InterfaceBodyDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 54, JavaParser::RuleInterfaceBodyDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(538);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::ABSTRACT:
            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::CLASS:
            case JavaParser::DEFAULT:
            case JavaParser::DOUBLE:
            case JavaParser::ENUM:
            case JavaParser::FINAL:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::INTERFACE:
            case JavaParser::LONG:
            case JavaParser::NATIVE:
            case JavaParser::PRIVATE:
            case JavaParser::PROTECTED:
            case JavaParser::PUBLIC:
            case JavaParser::SHORT:
            case JavaParser::STATIC:
            case JavaParser::STRICTFP:
            case JavaParser::SYNCHRONIZED:
            case JavaParser::TRANSIENT:
            case JavaParser::VOID:
            case JavaParser::VOLATILE:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::NON_SEALED:
            case JavaParser::LT:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(533);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(530);
                        modifier();
                    }
                    setState(535);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx);
                }
                setState(536);
                interfaceMemberDeclaration();
                break;
            }

            case JavaParser::SEMI: {
                enterOuterAlt(_localctx, 2);
                setState(537);
                match(JavaParser::SEMI);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InterfaceMemberDeclarationContext ------------------------------------------------------------------

JavaParser::InterfaceMemberDeclarationContext::InterfaceMemberDeclarationContext(ParserRuleContext* parent,
                                                                                 size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ConstDeclarationContext* JavaParser::InterfaceMemberDeclarationContext::constDeclaration()
{
    return getRuleContext<JavaParser::ConstDeclarationContext>(0);
}

JavaParser::InterfaceMethodDeclarationContext*
JavaParser::InterfaceMemberDeclarationContext::interfaceMethodDeclaration()
{
    return getRuleContext<JavaParser::InterfaceMethodDeclarationContext>(0);
}

JavaParser::GenericInterfaceMethodDeclarationContext*
JavaParser::InterfaceMemberDeclarationContext::genericInterfaceMethodDeclaration()
{
    return getRuleContext<JavaParser::GenericInterfaceMethodDeclarationContext>(0);
}

JavaParser::InterfaceDeclarationContext* JavaParser::InterfaceMemberDeclarationContext::interfaceDeclaration()
{
    return getRuleContext<JavaParser::InterfaceDeclarationContext>(0);
}

JavaParser::AnnotationTypeDeclarationContext* JavaParser::InterfaceMemberDeclarationContext::annotationTypeDeclaration()
{
    return getRuleContext<JavaParser::AnnotationTypeDeclarationContext>(0);
}

JavaParser::ClassDeclarationContext* JavaParser::InterfaceMemberDeclarationContext::classDeclaration()
{
    return getRuleContext<JavaParser::ClassDeclarationContext>(0);
}

JavaParser::EnumDeclarationContext* JavaParser::InterfaceMemberDeclarationContext::enumDeclaration()
{
    return getRuleContext<JavaParser::EnumDeclarationContext>(0);
}

JavaParser::RecordDeclarationContext* JavaParser::InterfaceMemberDeclarationContext::recordDeclaration()
{
    return getRuleContext<JavaParser::RecordDeclarationContext>(0);
}

size_t JavaParser::InterfaceMemberDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleInterfaceMemberDeclaration;
}

JavaParser::InterfaceMemberDeclarationContext* JavaParser::interfaceMemberDeclaration()
{
    InterfaceMemberDeclarationContext* _localctx =
        _tracker.createInstance<InterfaceMemberDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 56, JavaParser::RuleInterfaceMemberDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(548);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(540);
                constDeclaration();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(541);
                interfaceMethodDeclaration();
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(542);
                genericInterfaceMethodDeclaration();
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(543);
                interfaceDeclaration();
                break;
            }

            case 5: {
                enterOuterAlt(_localctx, 5);
                setState(544);
                annotationTypeDeclaration();
                break;
            }

            case 6: {
                enterOuterAlt(_localctx, 6);
                setState(545);
                classDeclaration();
                break;
            }

            case 7: {
                enterOuterAlt(_localctx, 7);
                setState(546);
                enumDeclaration();
                break;
            }

            case 8: {
                enterOuterAlt(_localctx, 8);
                setState(547);
                recordDeclaration();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ConstDeclarationContext ------------------------------------------------------------------

JavaParser::ConstDeclarationContext::ConstDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::ConstDeclarationContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

std::vector<JavaParser::ConstantDeclaratorContext*> JavaParser::ConstDeclarationContext::constantDeclarator()
{
    return getRuleContexts<JavaParser::ConstantDeclaratorContext>();
}

JavaParser::ConstantDeclaratorContext* JavaParser::ConstDeclarationContext::constantDeclarator(size_t i)
{
    return getRuleContext<JavaParser::ConstantDeclaratorContext>(i);
}

tree::TerminalNode* JavaParser::ConstDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

std::vector<tree::TerminalNode*> JavaParser::ConstDeclarationContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::ConstDeclarationContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::ConstDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleConstDeclaration;
}

JavaParser::ConstDeclarationContext* JavaParser::constDeclaration()
{
    ConstDeclarationContext* _localctx = _tracker.createInstance<ConstDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 58, JavaParser::RuleConstDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(550);
        typeType();
        setState(551);
        constantDeclarator();
        setState(556);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(552);
            match(JavaParser::COMMA);
            setState(553);
            constantDeclarator();
            setState(558);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(559);
        match(JavaParser::SEMI);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ConstantDeclaratorContext ------------------------------------------------------------------

JavaParser::ConstantDeclaratorContext::ConstantDeclaratorContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::ConstantDeclaratorContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::ConstantDeclaratorContext::ASSIGN()
{
    return getToken(JavaParser::ASSIGN, 0);
}

JavaParser::VariableInitializerContext* JavaParser::ConstantDeclaratorContext::variableInitializer()
{
    return getRuleContext<JavaParser::VariableInitializerContext>(0);
}

std::vector<tree::TerminalNode*> JavaParser::ConstantDeclaratorContext::LBRACK()
{
    return getTokens(JavaParser::LBRACK);
}

tree::TerminalNode* JavaParser::ConstantDeclaratorContext::LBRACK(size_t i)
{
    return getToken(JavaParser::LBRACK, i);
}

std::vector<tree::TerminalNode*> JavaParser::ConstantDeclaratorContext::RBRACK()
{
    return getTokens(JavaParser::RBRACK);
}

tree::TerminalNode* JavaParser::ConstantDeclaratorContext::RBRACK(size_t i)
{
    return getToken(JavaParser::RBRACK, i);
}

size_t JavaParser::ConstantDeclaratorContext::getRuleIndex() const
{
    return JavaParser::RuleConstantDeclarator;
}

JavaParser::ConstantDeclaratorContext* JavaParser::constantDeclarator()
{
    ConstantDeclaratorContext* _localctx = _tracker.createInstance<ConstantDeclaratorContext>(_ctx, getState());
    enterRule(_localctx, 60, JavaParser::RuleConstantDeclarator);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(561);
        identifier();
        setState(566);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::LBRACK) {
            setState(562);
            match(JavaParser::LBRACK);
            setState(563);
            match(JavaParser::RBRACK);
            setState(568);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(569);
        match(JavaParser::ASSIGN);
        setState(570);
        variableInitializer();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InterfaceMethodDeclarationContext ------------------------------------------------------------------

JavaParser::InterfaceMethodDeclarationContext::InterfaceMethodDeclarationContext(ParserRuleContext* parent,
                                                                                 size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::InterfaceCommonBodyDeclarationContext*
JavaParser::InterfaceMethodDeclarationContext::interfaceCommonBodyDeclaration()
{
    return getRuleContext<JavaParser::InterfaceCommonBodyDeclarationContext>(0);
}

std::vector<JavaParser::InterfaceMethodModifierContext*>
JavaParser::InterfaceMethodDeclarationContext::interfaceMethodModifier()
{
    return getRuleContexts<JavaParser::InterfaceMethodModifierContext>();
}

JavaParser::InterfaceMethodModifierContext* JavaParser::InterfaceMethodDeclarationContext::interfaceMethodModifier(
    size_t i)
{
    return getRuleContext<JavaParser::InterfaceMethodModifierContext>(i);
}

size_t JavaParser::InterfaceMethodDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleInterfaceMethodDeclaration;
}

JavaParser::InterfaceMethodDeclarationContext* JavaParser::interfaceMethodDeclaration()
{
    InterfaceMethodDeclarationContext* _localctx =
        _tracker.createInstance<InterfaceMethodDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 62, JavaParser::RuleInterfaceMethodDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(575);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(572);
                interfaceMethodModifier();
            }
            setState(577);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
        }
        setState(578);
        interfaceCommonBodyDeclaration();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InterfaceMethodModifierContext ------------------------------------------------------------------

JavaParser::InterfaceMethodModifierContext::InterfaceMethodModifierContext(ParserRuleContext* parent,
                                                                           size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::AnnotationContext* JavaParser::InterfaceMethodModifierContext::annotation()
{
    return getRuleContext<JavaParser::AnnotationContext>(0);
}

tree::TerminalNode* JavaParser::InterfaceMethodModifierContext::PUBLIC()
{
    return getToken(JavaParser::PUBLIC, 0);
}

tree::TerminalNode* JavaParser::InterfaceMethodModifierContext::ABSTRACT()
{
    return getToken(JavaParser::ABSTRACT, 0);
}

tree::TerminalNode* JavaParser::InterfaceMethodModifierContext::DEFAULT()
{
    return getToken(JavaParser::DEFAULT, 0);
}

tree::TerminalNode* JavaParser::InterfaceMethodModifierContext::STATIC()
{
    return getToken(JavaParser::STATIC, 0);
}

tree::TerminalNode* JavaParser::InterfaceMethodModifierContext::STRICTFP()
{
    return getToken(JavaParser::STRICTFP, 0);
}

size_t JavaParser::InterfaceMethodModifierContext::getRuleIndex() const
{
    return JavaParser::RuleInterfaceMethodModifier;
}

JavaParser::InterfaceMethodModifierContext* JavaParser::interfaceMethodModifier()
{
    InterfaceMethodModifierContext* _localctx =
        _tracker.createInstance<InterfaceMethodModifierContext>(_ctx, getState());
    enterRule(_localctx, 64, JavaParser::RuleInterfaceMethodModifier);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(586);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(580);
                annotation();
                break;
            }

            case JavaParser::PUBLIC: {
                enterOuterAlt(_localctx, 2);
                setState(581);
                match(JavaParser::PUBLIC);
                break;
            }

            case JavaParser::ABSTRACT: {
                enterOuterAlt(_localctx, 3);
                setState(582);
                match(JavaParser::ABSTRACT);
                break;
            }

            case JavaParser::DEFAULT: {
                enterOuterAlt(_localctx, 4);
                setState(583);
                match(JavaParser::DEFAULT);
                break;
            }

            case JavaParser::STATIC: {
                enterOuterAlt(_localctx, 5);
                setState(584);
                match(JavaParser::STATIC);
                break;
            }

            case JavaParser::STRICTFP: {
                enterOuterAlt(_localctx, 6);
                setState(585);
                match(JavaParser::STRICTFP);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- GenericInterfaceMethodDeclarationContext
//------------------------------------------------------------------

JavaParser::GenericInterfaceMethodDeclarationContext::GenericInterfaceMethodDeclarationContext(
    ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeParametersContext* JavaParser::GenericInterfaceMethodDeclarationContext::typeParameters()
{
    return getRuleContext<JavaParser::TypeParametersContext>(0);
}

JavaParser::InterfaceCommonBodyDeclarationContext*
JavaParser::GenericInterfaceMethodDeclarationContext::interfaceCommonBodyDeclaration()
{
    return getRuleContext<JavaParser::InterfaceCommonBodyDeclarationContext>(0);
}

std::vector<JavaParser::InterfaceMethodModifierContext*>
JavaParser::GenericInterfaceMethodDeclarationContext::interfaceMethodModifier()
{
    return getRuleContexts<JavaParser::InterfaceMethodModifierContext>();
}

JavaParser::InterfaceMethodModifierContext*
JavaParser::GenericInterfaceMethodDeclarationContext::interfaceMethodModifier(size_t i)
{
    return getRuleContext<JavaParser::InterfaceMethodModifierContext>(i);
}

size_t JavaParser::GenericInterfaceMethodDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleGenericInterfaceMethodDeclaration;
}

JavaParser::GenericInterfaceMethodDeclarationContext* JavaParser::genericInterfaceMethodDeclaration()
{
    GenericInterfaceMethodDeclarationContext* _localctx =
        _tracker.createInstance<GenericInterfaceMethodDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 66, JavaParser::RuleGenericInterfaceMethodDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(591);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 1) & ~0x3fULL) == 0) &&
                ((1ULL << (_la - 1)) & ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::DEFAULT - 1)) |
                                        (1ULL << (JavaParser::PUBLIC - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
                                        (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::MODULE - 1)) |
                                        (1ULL << (JavaParser::OPEN - 1)) | (1ULL << (JavaParser::REQUIRES - 1)) |
                                        (1ULL << (JavaParser::EXPORTS - 1)) | (1ULL << (JavaParser::OPENS - 1)) |
                                        (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
                                        (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
                                        (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
                                        (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
                                        (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
               ((((_la - 65) & ~0x3fULL) == 0) &&
                ((1ULL << (_la - 65)) & ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::AT - 65)) |
                                         (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
            setState(588);
            interfaceMethodModifier();
            setState(593);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(594);
        typeParameters();
        setState(595);
        interfaceCommonBodyDeclaration();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InterfaceCommonBodyDeclarationContext
//------------------------------------------------------------------

JavaParser::InterfaceCommonBodyDeclarationContext::InterfaceCommonBodyDeclarationContext(ParserRuleContext* parent,
                                                                                         size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeOrVoidContext* JavaParser::InterfaceCommonBodyDeclarationContext::typeTypeOrVoid()
{
    return getRuleContext<JavaParser::TypeTypeOrVoidContext>(0);
}

JavaParser::IdentifierContext* JavaParser::InterfaceCommonBodyDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::FormalParametersContext* JavaParser::InterfaceCommonBodyDeclarationContext::formalParameters()
{
    return getRuleContext<JavaParser::FormalParametersContext>(0);
}

JavaParser::MethodBodyContext* JavaParser::InterfaceCommonBodyDeclarationContext::methodBody()
{
    return getRuleContext<JavaParser::MethodBodyContext>(0);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::InterfaceCommonBodyDeclarationContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::InterfaceCommonBodyDeclarationContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::InterfaceCommonBodyDeclarationContext::LBRACK()
{
    return getTokens(JavaParser::LBRACK);
}

tree::TerminalNode* JavaParser::InterfaceCommonBodyDeclarationContext::LBRACK(size_t i)
{
    return getToken(JavaParser::LBRACK, i);
}

std::vector<tree::TerminalNode*> JavaParser::InterfaceCommonBodyDeclarationContext::RBRACK()
{
    return getTokens(JavaParser::RBRACK);
}

tree::TerminalNode* JavaParser::InterfaceCommonBodyDeclarationContext::RBRACK(size_t i)
{
    return getToken(JavaParser::RBRACK, i);
}

tree::TerminalNode* JavaParser::InterfaceCommonBodyDeclarationContext::THROWS()
{
    return getToken(JavaParser::THROWS, 0);
}

JavaParser::QualifiedNameListContext* JavaParser::InterfaceCommonBodyDeclarationContext::qualifiedNameList()
{
    return getRuleContext<JavaParser::QualifiedNameListContext>(0);
}

size_t JavaParser::InterfaceCommonBodyDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleInterfaceCommonBodyDeclaration;
}

JavaParser::InterfaceCommonBodyDeclarationContext* JavaParser::interfaceCommonBodyDeclaration()
{
    InterfaceCommonBodyDeclarationContext* _localctx =
        _tracker.createInstance<InterfaceCommonBodyDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 68, JavaParser::RuleInterfaceCommonBodyDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(600);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(597);
                annotation();
            }
            setState(602);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
        }
        setState(603);
        typeTypeOrVoid();
        setState(604);
        identifier();
        setState(605);
        formalParameters();
        setState(610);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::LBRACK) {
            setState(606);
            match(JavaParser::LBRACK);
            setState(607);
            match(JavaParser::RBRACK);
            setState(612);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(615);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::THROWS) {
            setState(613);
            match(JavaParser::THROWS);
            setState(614);
            qualifiedNameList();
        }
        setState(617);
        methodBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- VariableDeclaratorsContext ------------------------------------------------------------------

JavaParser::VariableDeclaratorsContext::VariableDeclaratorsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::VariableDeclaratorContext*> JavaParser::VariableDeclaratorsContext::variableDeclarator()
{
    return getRuleContexts<JavaParser::VariableDeclaratorContext>();
}

JavaParser::VariableDeclaratorContext* JavaParser::VariableDeclaratorsContext::variableDeclarator(size_t i)
{
    return getRuleContext<JavaParser::VariableDeclaratorContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::VariableDeclaratorsContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::VariableDeclaratorsContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::VariableDeclaratorsContext::getRuleIndex() const
{
    return JavaParser::RuleVariableDeclarators;
}

JavaParser::VariableDeclaratorsContext* JavaParser::variableDeclarators()
{
    VariableDeclaratorsContext* _localctx = _tracker.createInstance<VariableDeclaratorsContext>(_ctx, getState());
    enterRule(_localctx, 70, JavaParser::RuleVariableDeclarators);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(619);
        variableDeclarator();
        setState(624);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(620);
            match(JavaParser::COMMA);
            setState(621);
            variableDeclarator();
            setState(626);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- VariableDeclaratorContext ------------------------------------------------------------------

JavaParser::VariableDeclaratorContext::VariableDeclaratorContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::VariableDeclaratorIdContext* JavaParser::VariableDeclaratorContext::variableDeclaratorId()
{
    return getRuleContext<JavaParser::VariableDeclaratorIdContext>(0);
}

tree::TerminalNode* JavaParser::VariableDeclaratorContext::ASSIGN()
{
    return getToken(JavaParser::ASSIGN, 0);
}

JavaParser::VariableInitializerContext* JavaParser::VariableDeclaratorContext::variableInitializer()
{
    return getRuleContext<JavaParser::VariableInitializerContext>(0);
}

size_t JavaParser::VariableDeclaratorContext::getRuleIndex() const
{
    return JavaParser::RuleVariableDeclarator;
}

JavaParser::VariableDeclaratorContext* JavaParser::variableDeclarator()
{
    VariableDeclaratorContext* _localctx = _tracker.createInstance<VariableDeclaratorContext>(_ctx, getState());
    enterRule(_localctx, 72, JavaParser::RuleVariableDeclarator);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(627);
        variableDeclaratorId();
        setState(630);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::ASSIGN) {
            setState(628);
            match(JavaParser::ASSIGN);
            setState(629);
            variableInitializer();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- VariableDeclaratorIdContext ------------------------------------------------------------------

JavaParser::VariableDeclaratorIdContext::VariableDeclaratorIdContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::VariableDeclaratorIdContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

std::vector<tree::TerminalNode*> JavaParser::VariableDeclaratorIdContext::LBRACK()
{
    return getTokens(JavaParser::LBRACK);
}

tree::TerminalNode* JavaParser::VariableDeclaratorIdContext::LBRACK(size_t i)
{
    return getToken(JavaParser::LBRACK, i);
}

std::vector<tree::TerminalNode*> JavaParser::VariableDeclaratorIdContext::RBRACK()
{
    return getTokens(JavaParser::RBRACK);
}

tree::TerminalNode* JavaParser::VariableDeclaratorIdContext::RBRACK(size_t i)
{
    return getToken(JavaParser::RBRACK, i);
}

size_t JavaParser::VariableDeclaratorIdContext::getRuleIndex() const
{
    return JavaParser::RuleVariableDeclaratorId;
}

JavaParser::VariableDeclaratorIdContext* JavaParser::variableDeclaratorId()
{
    VariableDeclaratorIdContext* _localctx = _tracker.createInstance<VariableDeclaratorIdContext>(_ctx, getState());
    enterRule(_localctx, 74, JavaParser::RuleVariableDeclaratorId);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(632);
        identifier();
        setState(637);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::LBRACK) {
            setState(633);
            match(JavaParser::LBRACK);
            setState(634);
            match(JavaParser::RBRACK);
            setState(639);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- VariableInitializerContext ------------------------------------------------------------------

JavaParser::VariableInitializerContext::VariableInitializerContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ArrayInitializerContext* JavaParser::VariableInitializerContext::arrayInitializer()
{
    return getRuleContext<JavaParser::ArrayInitializerContext>(0);
}

JavaParser::ExpressionContext* JavaParser::VariableInitializerContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

size_t JavaParser::VariableInitializerContext::getRuleIndex() const
{
    return JavaParser::RuleVariableInitializer;
}

JavaParser::VariableInitializerContext* JavaParser::variableInitializer()
{
    VariableInitializerContext* _localctx = _tracker.createInstance<VariableInitializerContext>(_ctx, getState());
    enterRule(_localctx, 76, JavaParser::RuleVariableInitializer);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(642);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::LBRACE: {
                enterOuterAlt(_localctx, 1);
                setState(640);
                arrayInitializer();
                break;
            }

            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::NEW:
            case JavaParser::SHORT:
            case JavaParser::SUPER:
            case JavaParser::SWITCH:
            case JavaParser::THIS:
            case JavaParser::VOID:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::DECIMAL_LITERAL:
            case JavaParser::HEX_LITERAL:
            case JavaParser::OCT_LITERAL:
            case JavaParser::BINARY_LITERAL:
            case JavaParser::FLOAT_LITERAL:
            case JavaParser::HEX_FLOAT_LITERAL:
            case JavaParser::BOOL_LITERAL:
            case JavaParser::CHAR_LITERAL:
            case JavaParser::STRING_LITERAL:
            case JavaParser::TEXT_BLOCK:
            case JavaParser::NULL_LITERAL:
            case JavaParser::LPAREN:
            case JavaParser::LT:
            case JavaParser::BANG:
            case JavaParser::TILDE:
            case JavaParser::INC:
            case JavaParser::DEC:
            case JavaParser::ADD:
            case JavaParser::SUB:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 2);
                setState(641);
                expression(0);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ArrayInitializerContext ------------------------------------------------------------------

JavaParser::ArrayInitializerContext::ArrayInitializerContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ArrayInitializerContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::ArrayInitializerContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::VariableInitializerContext*> JavaParser::ArrayInitializerContext::variableInitializer()
{
    return getRuleContexts<JavaParser::VariableInitializerContext>();
}

JavaParser::VariableInitializerContext* JavaParser::ArrayInitializerContext::variableInitializer(size_t i)
{
    return getRuleContext<JavaParser::VariableInitializerContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ArrayInitializerContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::ArrayInitializerContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::ArrayInitializerContext::getRuleIndex() const
{
    return JavaParser::RuleArrayInitializer;
}

JavaParser::ArrayInitializerContext* JavaParser::arrayInitializer()
{
    ArrayInitializerContext* _localctx = _tracker.createInstance<ArrayInitializerContext>(_ctx, getState());
    enterRule(_localctx, 78, JavaParser::RuleArrayInitializer);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(644);
        match(JavaParser::LBRACE);
        setState(656);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 3) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 3)) &
              ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
               (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
               (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) | (1ULL << (JavaParser::LONG - 3)) |
               (1ULL << (JavaParser::NEW - 3)) | (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
               (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
               (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
               (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
               (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
               (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
               (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
               (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
               (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
               (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
            ((((_la - 67) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 67)) &
              ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
               (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
               (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
               (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
               (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
               (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
               (1ULL << (JavaParser::LBRACE - 67)) | (1ULL << (JavaParser::LT - 67)) |
               (1ULL << (JavaParser::BANG - 67)) | (1ULL << (JavaParser::TILDE - 67)) |
               (1ULL << (JavaParser::INC - 67)) | (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
               (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
               (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
            setState(645);
            variableInitializer();
            setState(650);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 60, _ctx);
            while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                if (alt == 1) {
                    setState(646);
                    match(JavaParser::COMMA);
                    setState(647);
                    variableInitializer();
                }
                setState(652);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 60, _ctx);
            }
            setState(654);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == JavaParser::COMMA) {
                setState(653);
                match(JavaParser::COMMA);
            }
        }
        setState(658);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ClassOrInterfaceTypeContext ------------------------------------------------------------------

JavaParser::ClassOrInterfaceTypeContext::ClassOrInterfaceTypeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::IdentifierContext*> JavaParser::ClassOrInterfaceTypeContext::identifier()
{
    return getRuleContexts<JavaParser::IdentifierContext>();
}

JavaParser::IdentifierContext* JavaParser::ClassOrInterfaceTypeContext::identifier(size_t i)
{
    return getRuleContext<JavaParser::IdentifierContext>(i);
}

std::vector<JavaParser::TypeArgumentsContext*> JavaParser::ClassOrInterfaceTypeContext::typeArguments()
{
    return getRuleContexts<JavaParser::TypeArgumentsContext>();
}

JavaParser::TypeArgumentsContext* JavaParser::ClassOrInterfaceTypeContext::typeArguments(size_t i)
{
    return getRuleContext<JavaParser::TypeArgumentsContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ClassOrInterfaceTypeContext::DOT()
{
    return getTokens(JavaParser::DOT);
}

tree::TerminalNode* JavaParser::ClassOrInterfaceTypeContext::DOT(size_t i)
{
    return getToken(JavaParser::DOT, i);
}

size_t JavaParser::ClassOrInterfaceTypeContext::getRuleIndex() const
{
    return JavaParser::RuleClassOrInterfaceType;
}

JavaParser::ClassOrInterfaceTypeContext* JavaParser::classOrInterfaceType()
{
    ClassOrInterfaceTypeContext* _localctx = _tracker.createInstance<ClassOrInterfaceTypeContext>(_ctx, getState());
    enterRule(_localctx, 80, JavaParser::RuleClassOrInterfaceType);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(660);
        identifier();
        setState(662);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx)) {
            case 1: {
                setState(661);
                typeArguments();
                break;
            }

            default:
                break;
        }
        setState(671);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(664);
                match(JavaParser::DOT);
                setState(665);
                identifier();
                setState(667);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx)) {
                    case 1: {
                        setState(666);
                        typeArguments();
                        break;
                    }

                    default:
                        break;
                }
            }
            setState(673);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeArgumentContext ------------------------------------------------------------------

JavaParser::TypeArgumentContext::TypeArgumentContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::TypeArgumentContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

tree::TerminalNode* JavaParser::TypeArgumentContext::QUESTION()
{
    return getToken(JavaParser::QUESTION, 0);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::TypeArgumentContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::TypeArgumentContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

tree::TerminalNode* JavaParser::TypeArgumentContext::EXTENDS()
{
    return getToken(JavaParser::EXTENDS, 0);
}

tree::TerminalNode* JavaParser::TypeArgumentContext::SUPER()
{
    return getToken(JavaParser::SUPER, 0);
}

size_t JavaParser::TypeArgumentContext::getRuleIndex() const
{
    return JavaParser::RuleTypeArgument;
}

JavaParser::TypeArgumentContext* JavaParser::typeArgument()
{
    TypeArgumentContext* _localctx = _tracker.createInstance<TypeArgumentContext>(_ctx, getState());
    enterRule(_localctx, 82, JavaParser::RuleTypeArgument);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(686);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(674);
                typeType();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(678);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (((((_la - 51) & ~0x3fULL) == 0) &&
                        ((1ULL << (_la - 51)) &
                         ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                          (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                          (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                          (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                          (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                          (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                          (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                          (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
                       _la == JavaParser::AT

                       || _la == JavaParser::IDENTIFIER) {
                    setState(675);
                    annotation();
                    setState(680);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(681);
                match(JavaParser::QUESTION);
                setState(684);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::EXTENDS

                    || _la == JavaParser::SUPER) {
                    setState(682);
                    _la = _input->LA(1);
                    if (!(_la == JavaParser::EXTENDS

                          || _la == JavaParser::SUPER)) {
                        _errHandler->recoverInline(this);
                    } else {
                        _errHandler->reportMatch(this);
                        consume();
                    }
                    setState(683);
                    typeType();
                }
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- QualifiedNameListContext ------------------------------------------------------------------

JavaParser::QualifiedNameListContext::QualifiedNameListContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::QualifiedNameContext*> JavaParser::QualifiedNameListContext::qualifiedName()
{
    return getRuleContexts<JavaParser::QualifiedNameContext>();
}

JavaParser::QualifiedNameContext* JavaParser::QualifiedNameListContext::qualifiedName(size_t i)
{
    return getRuleContext<JavaParser::QualifiedNameContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::QualifiedNameListContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::QualifiedNameListContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::QualifiedNameListContext::getRuleIndex() const
{
    return JavaParser::RuleQualifiedNameList;
}

JavaParser::QualifiedNameListContext* JavaParser::qualifiedNameList()
{
    QualifiedNameListContext* _localctx = _tracker.createInstance<QualifiedNameListContext>(_ctx, getState());
    enterRule(_localctx, 84, JavaParser::RuleQualifiedNameList);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(688);
        qualifiedName();
        setState(693);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(689);
            match(JavaParser::COMMA);
            setState(690);
            qualifiedName();
            setState(695);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FormalParametersContext ------------------------------------------------------------------

JavaParser::FormalParametersContext::FormalParametersContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::FormalParametersContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

tree::TerminalNode* JavaParser::FormalParametersContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::ReceiverParameterContext* JavaParser::FormalParametersContext::receiverParameter()
{
    return getRuleContext<JavaParser::ReceiverParameterContext>(0);
}

tree::TerminalNode* JavaParser::FormalParametersContext::COMMA()
{
    return getToken(JavaParser::COMMA, 0);
}

JavaParser::FormalParameterListContext* JavaParser::FormalParametersContext::formalParameterList()
{
    return getRuleContext<JavaParser::FormalParameterListContext>(0);
}

size_t JavaParser::FormalParametersContext::getRuleIndex() const
{
    return JavaParser::RuleFormalParameters;
}

JavaParser::FormalParametersContext* JavaParser::formalParameters()
{
    FormalParametersContext* _localctx = _tracker.createInstance<FormalParametersContext>(_ctx, getState());
    enterRule(_localctx, 86, JavaParser::RuleFormalParameters);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(696);
        match(JavaParser::LPAREN);
        setState(708);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 73, _ctx)) {
            case 1: {
                setState(698);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) |
                       (1ULL << (JavaParser::LONG - 3)) | (1ULL << (JavaParser::SHORT - 3)) |
                       (1ULL << (JavaParser::MODULE - 3)) | (1ULL << (JavaParser::OPEN - 3)) |
                       (1ULL << (JavaParser::REQUIRES - 3)) | (1ULL << (JavaParser::EXPORTS - 3)) |
                       (1ULL << (JavaParser::OPENS - 3)) | (1ULL << (JavaParser::TO - 3)) |
                       (1ULL << (JavaParser::USES - 3)) | (1ULL << (JavaParser::PROVIDES - 3)) |
                       (1ULL << (JavaParser::WITH - 3)) | (1ULL << (JavaParser::TRANSITIVE - 3)) |
                       (1ULL << (JavaParser::VAR - 3)) | (1ULL << (JavaParser::YIELD - 3)) |
                       (1ULL << (JavaParser::RECORD - 3)) | (1ULL << (JavaParser::SEALED - 3)) |
                       (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    _la == JavaParser::AT

                    || _la == JavaParser::IDENTIFIER) {
                    setState(697);
                    receiverParameter();
                }
                break;
            }

            case 2: {
                setState(700);
                receiverParameter();
                setState(703);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::COMMA) {
                    setState(701);
                    match(JavaParser::COMMA);
                    setState(702);
                    formalParameterList();
                }
                break;
            }

            case 3: {
                setState(706);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FINAL - 3)) | (1ULL << (JavaParser::FLOAT - 3)) |
                       (1ULL << (JavaParser::INT - 3)) | (1ULL << (JavaParser::LONG - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    _la == JavaParser::AT

                    || _la == JavaParser::IDENTIFIER) {
                    setState(705);
                    formalParameterList();
                }
                break;
            }

            default:
                break;
        }
        setState(710);
        match(JavaParser::RPAREN);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ReceiverParameterContext ------------------------------------------------------------------

JavaParser::ReceiverParameterContext::ReceiverParameterContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::ReceiverParameterContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

tree::TerminalNode* JavaParser::ReceiverParameterContext::THIS()
{
    return getToken(JavaParser::THIS, 0);
}

std::vector<JavaParser::IdentifierContext*> JavaParser::ReceiverParameterContext::identifier()
{
    return getRuleContexts<JavaParser::IdentifierContext>();
}

JavaParser::IdentifierContext* JavaParser::ReceiverParameterContext::identifier(size_t i)
{
    return getRuleContext<JavaParser::IdentifierContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ReceiverParameterContext::DOT()
{
    return getTokens(JavaParser::DOT);
}

tree::TerminalNode* JavaParser::ReceiverParameterContext::DOT(size_t i)
{
    return getToken(JavaParser::DOT, i);
}

size_t JavaParser::ReceiverParameterContext::getRuleIndex() const
{
    return JavaParser::RuleReceiverParameter;
}

JavaParser::ReceiverParameterContext* JavaParser::receiverParameter()
{
    ReceiverParameterContext* _localctx = _tracker.createInstance<ReceiverParameterContext>(_ctx, getState());
    enterRule(_localctx, 88, JavaParser::RuleReceiverParameter);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(712);
        typeType();
        setState(718);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 51) & ~0x3fULL) == 0) &&
                ((1ULL << (_la - 51)) & ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                                         (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                                         (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                                         (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                                         (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                                         (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                                         (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                                         (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
               _la == JavaParser::IDENTIFIER) {
            setState(713);
            identifier();
            setState(714);
            match(JavaParser::DOT);
            setState(720);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(721);
        match(JavaParser::THIS);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FormalParameterListContext ------------------------------------------------------------------

JavaParser::FormalParameterListContext::FormalParameterListContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::FormalParameterContext*> JavaParser::FormalParameterListContext::formalParameter()
{
    return getRuleContexts<JavaParser::FormalParameterContext>();
}

JavaParser::FormalParameterContext* JavaParser::FormalParameterListContext::formalParameter(size_t i)
{
    return getRuleContext<JavaParser::FormalParameterContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::FormalParameterListContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::FormalParameterListContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

JavaParser::LastFormalParameterContext* JavaParser::FormalParameterListContext::lastFormalParameter()
{
    return getRuleContext<JavaParser::LastFormalParameterContext>(0);
}

size_t JavaParser::FormalParameterListContext::getRuleIndex() const
{
    return JavaParser::RuleFormalParameterList;
}

JavaParser::FormalParameterListContext* JavaParser::formalParameterList()
{
    FormalParameterListContext* _localctx = _tracker.createInstance<FormalParameterListContext>(_ctx, getState());
    enterRule(_localctx, 90, JavaParser::RuleFormalParameterList);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(736);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 77, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(723);
                formalParameter();
                setState(728);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(724);
                        match(JavaParser::COMMA);
                        setState(725);
                        formalParameter();
                    }
                    setState(730);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx);
                }
                setState(733);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::COMMA) {
                    setState(731);
                    match(JavaParser::COMMA);
                    setState(732);
                    lastFormalParameter();
                }
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(735);
                lastFormalParameter();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FormalParameterContext ------------------------------------------------------------------

JavaParser::FormalParameterContext::FormalParameterContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::FormalParameterContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::VariableDeclaratorIdContext* JavaParser::FormalParameterContext::variableDeclaratorId()
{
    return getRuleContext<JavaParser::VariableDeclaratorIdContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::FormalParameterContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::FormalParameterContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

size_t JavaParser::FormalParameterContext::getRuleIndex() const
{
    return JavaParser::RuleFormalParameter;
}

JavaParser::FormalParameterContext* JavaParser::formalParameter()
{
    FormalParameterContext* _localctx = _tracker.createInstance<FormalParameterContext>(_ctx, getState());
    enterRule(_localctx, 92, JavaParser::RuleFormalParameter);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(741);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(738);
                variableModifier();
            }
            setState(743);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx);
        }
        setState(744);
        typeType();
        setState(745);
        variableDeclaratorId();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LastFormalParameterContext ------------------------------------------------------------------

JavaParser::LastFormalParameterContext::LastFormalParameterContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::LastFormalParameterContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

tree::TerminalNode* JavaParser::LastFormalParameterContext::ELLIPSIS()
{
    return getToken(JavaParser::ELLIPSIS, 0);
}

JavaParser::VariableDeclaratorIdContext* JavaParser::LastFormalParameterContext::variableDeclaratorId()
{
    return getRuleContext<JavaParser::VariableDeclaratorIdContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::LastFormalParameterContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::LastFormalParameterContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::LastFormalParameterContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::LastFormalParameterContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

size_t JavaParser::LastFormalParameterContext::getRuleIndex() const
{
    return JavaParser::RuleLastFormalParameter;
}

JavaParser::LastFormalParameterContext* JavaParser::lastFormalParameter()
{
    LastFormalParameterContext* _localctx = _tracker.createInstance<LastFormalParameterContext>(_ctx, getState());
    enterRule(_localctx, 94, JavaParser::RuleLastFormalParameter);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(750);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(747);
                variableModifier();
            }
            setState(752);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx);
        }
        setState(753);
        typeType();
        setState(757);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 51) & ~0x3fULL) == 0) &&
                ((1ULL << (_la - 51)) & ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                                         (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                                         (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                                         (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                                         (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                                         (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                                         (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                                         (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
               _la == JavaParser::AT

               || _la == JavaParser::IDENTIFIER) {
            setState(754);
            annotation();
            setState(759);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(760);
        match(JavaParser::ELLIPSIS);
        setState(761);
        variableDeclaratorId();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LambdaLVTIListContext ------------------------------------------------------------------

JavaParser::LambdaLVTIListContext::LambdaLVTIListContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::LambdaLVTIParameterContext*> JavaParser::LambdaLVTIListContext::lambdaLVTIParameter()
{
    return getRuleContexts<JavaParser::LambdaLVTIParameterContext>();
}

JavaParser::LambdaLVTIParameterContext* JavaParser::LambdaLVTIListContext::lambdaLVTIParameter(size_t i)
{
    return getRuleContext<JavaParser::LambdaLVTIParameterContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::LambdaLVTIListContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::LambdaLVTIListContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::LambdaLVTIListContext::getRuleIndex() const
{
    return JavaParser::RuleLambdaLVTIList;
}

JavaParser::LambdaLVTIListContext* JavaParser::lambdaLVTIList()
{
    LambdaLVTIListContext* _localctx = _tracker.createInstance<LambdaLVTIListContext>(_ctx, getState());
    enterRule(_localctx, 96, JavaParser::RuleLambdaLVTIList);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(763);
        lambdaLVTIParameter();
        setState(768);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(764);
            match(JavaParser::COMMA);
            setState(765);
            lambdaLVTIParameter();
            setState(770);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LambdaLVTIParameterContext ------------------------------------------------------------------

JavaParser::LambdaLVTIParameterContext::LambdaLVTIParameterContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::LambdaLVTIParameterContext::VAR()
{
    return getToken(JavaParser::VAR, 0);
}

JavaParser::IdentifierContext* JavaParser::LambdaLVTIParameterContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::LambdaLVTIParameterContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::LambdaLVTIParameterContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

size_t JavaParser::LambdaLVTIParameterContext::getRuleIndex() const
{
    return JavaParser::RuleLambdaLVTIParameter;
}

JavaParser::LambdaLVTIParameterContext* JavaParser::lambdaLVTIParameter()
{
    LambdaLVTIParameterContext* _localctx = _tracker.createInstance<LambdaLVTIParameterContext>(_ctx, getState());
    enterRule(_localctx, 98, JavaParser::RuleLambdaLVTIParameter);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(774);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(771);
                variableModifier();
            }
            setState(776);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx);
        }
        setState(777);
        match(JavaParser::VAR);
        setState(778);
        identifier();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- QualifiedNameContext ------------------------------------------------------------------

JavaParser::QualifiedNameContext::QualifiedNameContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::IdentifierContext*> JavaParser::QualifiedNameContext::identifier()
{
    return getRuleContexts<JavaParser::IdentifierContext>();
}

JavaParser::IdentifierContext* JavaParser::QualifiedNameContext::identifier(size_t i)
{
    return getRuleContext<JavaParser::IdentifierContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::QualifiedNameContext::DOT()
{
    return getTokens(JavaParser::DOT);
}

tree::TerminalNode* JavaParser::QualifiedNameContext::DOT(size_t i)
{
    return getToken(JavaParser::DOT, i);
}

size_t JavaParser::QualifiedNameContext::getRuleIndex() const
{
    return JavaParser::RuleQualifiedName;
}

JavaParser::QualifiedNameContext* JavaParser::qualifiedName()
{
    QualifiedNameContext* _localctx = _tracker.createInstance<QualifiedNameContext>(_ctx, getState());
    enterRule(_localctx, 100, JavaParser::RuleQualifiedName);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(780);
        identifier();
        setState(785);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 83, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(781);
                match(JavaParser::DOT);
                setState(782);
                identifier();
            }
            setState(787);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 83, _ctx);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

JavaParser::LiteralContext::LiteralContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IntegerLiteralContext* JavaParser::LiteralContext::integerLiteral()
{
    return getRuleContext<JavaParser::IntegerLiteralContext>(0);
}

JavaParser::FloatLiteralContext* JavaParser::LiteralContext::floatLiteral()
{
    return getRuleContext<JavaParser::FloatLiteralContext>(0);
}

tree::TerminalNode* JavaParser::LiteralContext::CHAR_LITERAL()
{
    return getToken(JavaParser::CHAR_LITERAL, 0);
}

tree::TerminalNode* JavaParser::LiteralContext::STRING_LITERAL()
{
    return getToken(JavaParser::STRING_LITERAL, 0);
}

tree::TerminalNode* JavaParser::LiteralContext::BOOL_LITERAL()
{
    return getToken(JavaParser::BOOL_LITERAL, 0);
}

tree::TerminalNode* JavaParser::LiteralContext::NULL_LITERAL()
{
    return getToken(JavaParser::NULL_LITERAL, 0);
}

tree::TerminalNode* JavaParser::LiteralContext::TEXT_BLOCK()
{
    return getToken(JavaParser::TEXT_BLOCK, 0);
}

size_t JavaParser::LiteralContext::getRuleIndex() const
{
    return JavaParser::RuleLiteral;
}

JavaParser::LiteralContext* JavaParser::literal()
{
    LiteralContext* _localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
    enterRule(_localctx, 102, JavaParser::RuleLiteral);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(795);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::DECIMAL_LITERAL:
            case JavaParser::HEX_LITERAL:
            case JavaParser::OCT_LITERAL:
            case JavaParser::BINARY_LITERAL: {
                enterOuterAlt(_localctx, 1);
                setState(788);
                integerLiteral();
                break;
            }

            case JavaParser::FLOAT_LITERAL:
            case JavaParser::HEX_FLOAT_LITERAL: {
                enterOuterAlt(_localctx, 2);
                setState(789);
                floatLiteral();
                break;
            }

            case JavaParser::CHAR_LITERAL: {
                enterOuterAlt(_localctx, 3);
                setState(790);
                match(JavaParser::CHAR_LITERAL);
                break;
            }

            case JavaParser::STRING_LITERAL: {
                enterOuterAlt(_localctx, 4);
                setState(791);
                match(JavaParser::STRING_LITERAL);
                break;
            }

            case JavaParser::BOOL_LITERAL: {
                enterOuterAlt(_localctx, 5);
                setState(792);
                match(JavaParser::BOOL_LITERAL);
                break;
            }

            case JavaParser::NULL_LITERAL: {
                enterOuterAlt(_localctx, 6);
                setState(793);
                match(JavaParser::NULL_LITERAL);
                break;
            }

            case JavaParser::TEXT_BLOCK: {
                enterOuterAlt(_localctx, 7);
                setState(794);
                match(JavaParser::TEXT_BLOCK);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- IntegerLiteralContext ------------------------------------------------------------------

JavaParser::IntegerLiteralContext::IntegerLiteralContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::IntegerLiteralContext::DECIMAL_LITERAL()
{
    return getToken(JavaParser::DECIMAL_LITERAL, 0);
}

tree::TerminalNode* JavaParser::IntegerLiteralContext::HEX_LITERAL()
{
    return getToken(JavaParser::HEX_LITERAL, 0);
}

tree::TerminalNode* JavaParser::IntegerLiteralContext::OCT_LITERAL()
{
    return getToken(JavaParser::OCT_LITERAL, 0);
}

tree::TerminalNode* JavaParser::IntegerLiteralContext::BINARY_LITERAL()
{
    return getToken(JavaParser::BINARY_LITERAL, 0);
}

size_t JavaParser::IntegerLiteralContext::getRuleIndex() const
{
    return JavaParser::RuleIntegerLiteral;
}

JavaParser::IntegerLiteralContext* JavaParser::integerLiteral()
{
    IntegerLiteralContext* _localctx = _tracker.createInstance<IntegerLiteralContext>(_ctx, getState());
    enterRule(_localctx, 104, JavaParser::RuleIntegerLiteral);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(797);
        _la = _input->LA(1);
        if (!(((((_la - 67) & ~0x3fULL) == 0) &&
               ((1ULL << (_la - 67)) &
                ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                 (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)))) != 0))) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FloatLiteralContext ------------------------------------------------------------------

JavaParser::FloatLiteralContext::FloatLiteralContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::FloatLiteralContext::FLOAT_LITERAL()
{
    return getToken(JavaParser::FLOAT_LITERAL, 0);
}

tree::TerminalNode* JavaParser::FloatLiteralContext::HEX_FLOAT_LITERAL()
{
    return getToken(JavaParser::HEX_FLOAT_LITERAL, 0);
}

size_t JavaParser::FloatLiteralContext::getRuleIndex() const
{
    return JavaParser::RuleFloatLiteral;
}

JavaParser::FloatLiteralContext* JavaParser::floatLiteral()
{
    FloatLiteralContext* _localctx = _tracker.createInstance<FloatLiteralContext>(_ctx, getState());
    enterRule(_localctx, 106, JavaParser::RuleFloatLiteral);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(799);
        _la = _input->LA(1);
        if (!(_la == JavaParser::FLOAT_LITERAL

              || _la == JavaParser::HEX_FLOAT_LITERAL)) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AltAnnotationQualifiedNameContext ------------------------------------------------------------------

JavaParser::AltAnnotationQualifiedNameContext::AltAnnotationQualifiedNameContext(ParserRuleContext* parent,
                                                                                 size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::AltAnnotationQualifiedNameContext::AT()
{
    return getToken(JavaParser::AT, 0);
}

std::vector<JavaParser::IdentifierContext*> JavaParser::AltAnnotationQualifiedNameContext::identifier()
{
    return getRuleContexts<JavaParser::IdentifierContext>();
}

JavaParser::IdentifierContext* JavaParser::AltAnnotationQualifiedNameContext::identifier(size_t i)
{
    return getRuleContext<JavaParser::IdentifierContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::AltAnnotationQualifiedNameContext::DOT()
{
    return getTokens(JavaParser::DOT);
}

tree::TerminalNode* JavaParser::AltAnnotationQualifiedNameContext::DOT(size_t i)
{
    return getToken(JavaParser::DOT, i);
}

size_t JavaParser::AltAnnotationQualifiedNameContext::getRuleIndex() const
{
    return JavaParser::RuleAltAnnotationQualifiedName;
}

JavaParser::AltAnnotationQualifiedNameContext* JavaParser::altAnnotationQualifiedName()
{
    AltAnnotationQualifiedNameContext* _localctx =
        _tracker.createInstance<AltAnnotationQualifiedNameContext>(_ctx, getState());
    enterRule(_localctx, 108, JavaParser::RuleAltAnnotationQualifiedName);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(806);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 51) & ~0x3fULL) == 0) &&
                ((1ULL << (_la - 51)) & ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                                         (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                                         (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                                         (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                                         (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                                         (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                                         (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                                         (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
               _la == JavaParser::IDENTIFIER) {
            setState(801);
            identifier();
            setState(802);
            match(JavaParser::DOT);
            setState(808);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(809);
        match(JavaParser::AT);
        setState(810);
        identifier();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationContext ------------------------------------------------------------------

JavaParser::AnnotationContext::AnnotationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::AnnotationContext::AT()
{
    return getToken(JavaParser::AT, 0);
}

JavaParser::QualifiedNameContext* JavaParser::AnnotationContext::qualifiedName()
{
    return getRuleContext<JavaParser::QualifiedNameContext>(0);
}

JavaParser::AltAnnotationQualifiedNameContext* JavaParser::AnnotationContext::altAnnotationQualifiedName()
{
    return getRuleContext<JavaParser::AltAnnotationQualifiedNameContext>(0);
}

tree::TerminalNode* JavaParser::AnnotationContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

tree::TerminalNode* JavaParser::AnnotationContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::ElementValuePairsContext* JavaParser::AnnotationContext::elementValuePairs()
{
    return getRuleContext<JavaParser::ElementValuePairsContext>(0);
}

JavaParser::ElementValueContext* JavaParser::AnnotationContext::elementValue()
{
    return getRuleContext<JavaParser::ElementValueContext>(0);
}

size_t JavaParser::AnnotationContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotation;
}

JavaParser::AnnotationContext* JavaParser::annotation()
{
    AnnotationContext* _localctx = _tracker.createInstance<AnnotationContext>(_ctx, getState());
    enterRule(_localctx, 110, JavaParser::RuleAnnotation);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(815);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 86, _ctx)) {
            case 1: {
                setState(812);
                match(JavaParser::AT);
                setState(813);
                qualifiedName();
                break;
            }

            case 2: {
                setState(814);
                altAnnotationQualifiedName();
                break;
            }

            default:
                break;
        }
        setState(823);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LPAREN) {
            setState(817);
            match(JavaParser::LPAREN);
            setState(820);
            _errHandler->sync(this);

            switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx)) {
                case 1: {
                    setState(818);
                    elementValuePairs();
                    break;
                }

                case 2: {
                    setState(819);
                    elementValue();
                    break;
                }

                default:
                    break;
            }
            setState(822);
            match(JavaParser::RPAREN);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ElementValuePairsContext ------------------------------------------------------------------

JavaParser::ElementValuePairsContext::ElementValuePairsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::ElementValuePairContext*> JavaParser::ElementValuePairsContext::elementValuePair()
{
    return getRuleContexts<JavaParser::ElementValuePairContext>();
}

JavaParser::ElementValuePairContext* JavaParser::ElementValuePairsContext::elementValuePair(size_t i)
{
    return getRuleContext<JavaParser::ElementValuePairContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ElementValuePairsContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::ElementValuePairsContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::ElementValuePairsContext::getRuleIndex() const
{
    return JavaParser::RuleElementValuePairs;
}

JavaParser::ElementValuePairsContext* JavaParser::elementValuePairs()
{
    ElementValuePairsContext* _localctx = _tracker.createInstance<ElementValuePairsContext>(_ctx, getState());
    enterRule(_localctx, 112, JavaParser::RuleElementValuePairs);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(825);
        elementValuePair();
        setState(830);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(826);
            match(JavaParser::COMMA);
            setState(827);
            elementValuePair();
            setState(832);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ElementValuePairContext ------------------------------------------------------------------

JavaParser::ElementValuePairContext::ElementValuePairContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::ElementValuePairContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::ElementValuePairContext::ASSIGN()
{
    return getToken(JavaParser::ASSIGN, 0);
}

JavaParser::ElementValueContext* JavaParser::ElementValuePairContext::elementValue()
{
    return getRuleContext<JavaParser::ElementValueContext>(0);
}

size_t JavaParser::ElementValuePairContext::getRuleIndex() const
{
    return JavaParser::RuleElementValuePair;
}

JavaParser::ElementValuePairContext* JavaParser::elementValuePair()
{
    ElementValuePairContext* _localctx = _tracker.createInstance<ElementValuePairContext>(_ctx, getState());
    enterRule(_localctx, 114, JavaParser::RuleElementValuePair);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(833);
        identifier();
        setState(834);
        match(JavaParser::ASSIGN);
        setState(835);
        elementValue();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ElementValueContext ------------------------------------------------------------------

JavaParser::ElementValueContext::ElementValueContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ExpressionContext* JavaParser::ElementValueContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

JavaParser::AnnotationContext* JavaParser::ElementValueContext::annotation()
{
    return getRuleContext<JavaParser::AnnotationContext>(0);
}

JavaParser::ElementValueArrayInitializerContext* JavaParser::ElementValueContext::elementValueArrayInitializer()
{
    return getRuleContext<JavaParser::ElementValueArrayInitializerContext>(0);
}

size_t JavaParser::ElementValueContext::getRuleIndex() const
{
    return JavaParser::RuleElementValue;
}

JavaParser::ElementValueContext* JavaParser::elementValue()
{
    ElementValueContext* _localctx = _tracker.createInstance<ElementValueContext>(_ctx, getState());
    enterRule(_localctx, 116, JavaParser::RuleElementValue);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(840);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 90, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(837);
                expression(0);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(838);
                annotation();
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(839);
                elementValueArrayInitializer();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ElementValueArrayInitializerContext
//------------------------------------------------------------------

JavaParser::ElementValueArrayInitializerContext::ElementValueArrayInitializerContext(ParserRuleContext* parent,
                                                                                     size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ElementValueArrayInitializerContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::ElementValueArrayInitializerContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::ElementValueContext*> JavaParser::ElementValueArrayInitializerContext::elementValue()
{
    return getRuleContexts<JavaParser::ElementValueContext>();
}

JavaParser::ElementValueContext* JavaParser::ElementValueArrayInitializerContext::elementValue(size_t i)
{
    return getRuleContext<JavaParser::ElementValueContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ElementValueArrayInitializerContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::ElementValueArrayInitializerContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::ElementValueArrayInitializerContext::getRuleIndex() const
{
    return JavaParser::RuleElementValueArrayInitializer;
}

JavaParser::ElementValueArrayInitializerContext* JavaParser::elementValueArrayInitializer()
{
    ElementValueArrayInitializerContext* _localctx =
        _tracker.createInstance<ElementValueArrayInitializerContext>(_ctx, getState());
    enterRule(_localctx, 118, JavaParser::RuleElementValueArrayInitializer);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(842);
        match(JavaParser::LBRACE);
        setState(851);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 3) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 3)) &
              ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
               (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
               (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) | (1ULL << (JavaParser::LONG - 3)) |
               (1ULL << (JavaParser::NEW - 3)) | (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
               (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
               (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
               (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
               (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
               (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
               (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
               (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
               (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
               (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
            ((((_la - 67) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 67)) &
              ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
               (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
               (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
               (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
               (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
               (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
               (1ULL << (JavaParser::LBRACE - 67)) | (1ULL << (JavaParser::LT - 67)) |
               (1ULL << (JavaParser::BANG - 67)) | (1ULL << (JavaParser::TILDE - 67)) |
               (1ULL << (JavaParser::INC - 67)) | (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
               (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
               (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
            setState(843);
            elementValue();
            setState(848);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 91, _ctx);
            while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                if (alt == 1) {
                    setState(844);
                    match(JavaParser::COMMA);
                    setState(845);
                    elementValue();
                }
                setState(850);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 91, _ctx);
            }
        }
        setState(854);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::COMMA) {
            setState(853);
            match(JavaParser::COMMA);
        }
        setState(856);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationTypeDeclarationContext ------------------------------------------------------------------

JavaParser::AnnotationTypeDeclarationContext::AnnotationTypeDeclarationContext(ParserRuleContext* parent,
                                                                               size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::AnnotationTypeDeclarationContext::AT()
{
    return getToken(JavaParser::AT, 0);
}

tree::TerminalNode* JavaParser::AnnotationTypeDeclarationContext::INTERFACE()
{
    return getToken(JavaParser::INTERFACE, 0);
}

JavaParser::IdentifierContext* JavaParser::AnnotationTypeDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::AnnotationTypeBodyContext* JavaParser::AnnotationTypeDeclarationContext::annotationTypeBody()
{
    return getRuleContext<JavaParser::AnnotationTypeBodyContext>(0);
}

size_t JavaParser::AnnotationTypeDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotationTypeDeclaration;
}

JavaParser::AnnotationTypeDeclarationContext* JavaParser::annotationTypeDeclaration()
{
    AnnotationTypeDeclarationContext* _localctx =
        _tracker.createInstance<AnnotationTypeDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 120, JavaParser::RuleAnnotationTypeDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(858);
        match(JavaParser::AT);
        setState(859);
        match(JavaParser::INTERFACE);
        setState(860);
        identifier();
        setState(861);
        annotationTypeBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationTypeBodyContext ------------------------------------------------------------------

JavaParser::AnnotationTypeBodyContext::AnnotationTypeBodyContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::AnnotationTypeBodyContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::AnnotationTypeBodyContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::AnnotationTypeElementDeclarationContext*>
JavaParser::AnnotationTypeBodyContext::annotationTypeElementDeclaration()
{
    return getRuleContexts<JavaParser::AnnotationTypeElementDeclarationContext>();
}

JavaParser::AnnotationTypeElementDeclarationContext*
JavaParser::AnnotationTypeBodyContext::annotationTypeElementDeclaration(size_t i)
{
    return getRuleContext<JavaParser::AnnotationTypeElementDeclarationContext>(i);
}

size_t JavaParser::AnnotationTypeBodyContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotationTypeBody;
}

JavaParser::AnnotationTypeBodyContext* JavaParser::annotationTypeBody()
{
    AnnotationTypeBodyContext* _localctx = _tracker.createInstance<AnnotationTypeBodyContext>(_ctx, getState());
    enterRule(_localctx, 122, JavaParser::RuleAnnotationTypeBody);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(863);
        match(JavaParser::LBRACE);
        setState(867);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (
            ((((_la - 1) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 1)) &
              ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::BOOLEAN - 1)) |
               (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) | (1ULL << (JavaParser::CLASS - 1)) |
               (1ULL << (JavaParser::DOUBLE - 1)) | (1ULL << (JavaParser::ENUM - 1)) |
               (1ULL << (JavaParser::FINAL - 1)) | (1ULL << (JavaParser::FLOAT - 1)) | (1ULL << (JavaParser::INT - 1)) |
               (1ULL << (JavaParser::INTERFACE - 1)) | (1ULL << (JavaParser::LONG - 1)) |
               (1ULL << (JavaParser::NATIVE - 1)) | (1ULL << (JavaParser::PRIVATE - 1)) |
               (1ULL << (JavaParser::PROTECTED - 1)) | (1ULL << (JavaParser::PUBLIC - 1)) |
               (1ULL << (JavaParser::SHORT - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
               (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::SYNCHRONIZED - 1)) |
               (1ULL << (JavaParser::TRANSIENT - 1)) | (1ULL << (JavaParser::VOLATILE - 1)) |
               (1ULL << (JavaParser::MODULE - 1)) | (1ULL << (JavaParser::OPEN - 1)) |
               (1ULL << (JavaParser::REQUIRES - 1)) | (1ULL << (JavaParser::EXPORTS - 1)) |
               (1ULL << (JavaParser::OPENS - 1)) | (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
               (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
               (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
               (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
               (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
            ((((_la - 65) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 65)) & ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
                                      (1ULL << (JavaParser::SEMI - 65)) | (1ULL << (JavaParser::AT - 65)) |
                                      (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
            setState(864);
            annotationTypeElementDeclaration();
            setState(869);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(870);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationTypeElementDeclarationContext
//------------------------------------------------------------------

JavaParser::AnnotationTypeElementDeclarationContext::AnnotationTypeElementDeclarationContext(ParserRuleContext* parent,
                                                                                             size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::AnnotationTypeElementRestContext*
JavaParser::AnnotationTypeElementDeclarationContext::annotationTypeElementRest()
{
    return getRuleContext<JavaParser::AnnotationTypeElementRestContext>(0);
}

std::vector<JavaParser::ModifierContext*> JavaParser::AnnotationTypeElementDeclarationContext::modifier()
{
    return getRuleContexts<JavaParser::ModifierContext>();
}

JavaParser::ModifierContext* JavaParser::AnnotationTypeElementDeclarationContext::modifier(size_t i)
{
    return getRuleContext<JavaParser::ModifierContext>(i);
}

tree::TerminalNode* JavaParser::AnnotationTypeElementDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

size_t JavaParser::AnnotationTypeElementDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotationTypeElementDeclaration;
}

JavaParser::AnnotationTypeElementDeclarationContext* JavaParser::annotationTypeElementDeclaration()
{
    AnnotationTypeElementDeclarationContext* _localctx =
        _tracker.createInstance<AnnotationTypeElementDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 124, JavaParser::RuleAnnotationTypeElementDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(880);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::ABSTRACT:
            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::CLASS:
            case JavaParser::DOUBLE:
            case JavaParser::ENUM:
            case JavaParser::FINAL:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::INTERFACE:
            case JavaParser::LONG:
            case JavaParser::NATIVE:
            case JavaParser::PRIVATE:
            case JavaParser::PROTECTED:
            case JavaParser::PUBLIC:
            case JavaParser::SHORT:
            case JavaParser::STATIC:
            case JavaParser::STRICTFP:
            case JavaParser::SYNCHRONIZED:
            case JavaParser::TRANSIENT:
            case JavaParser::VOLATILE:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::NON_SEALED:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(875);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 95, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(872);
                        modifier();
                    }
                    setState(877);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 95, _ctx);
                }
                setState(878);
                annotationTypeElementRest();
                break;
            }

            case JavaParser::SEMI: {
                enterOuterAlt(_localctx, 2);
                setState(879);
                match(JavaParser::SEMI);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationTypeElementRestContext ------------------------------------------------------------------

JavaParser::AnnotationTypeElementRestContext::AnnotationTypeElementRestContext(ParserRuleContext* parent,
                                                                               size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::AnnotationTypeElementRestContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::AnnotationMethodOrConstantRestContext*
JavaParser::AnnotationTypeElementRestContext::annotationMethodOrConstantRest()
{
    return getRuleContext<JavaParser::AnnotationMethodOrConstantRestContext>(0);
}

tree::TerminalNode* JavaParser::AnnotationTypeElementRestContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

JavaParser::ClassDeclarationContext* JavaParser::AnnotationTypeElementRestContext::classDeclaration()
{
    return getRuleContext<JavaParser::ClassDeclarationContext>(0);
}

JavaParser::InterfaceDeclarationContext* JavaParser::AnnotationTypeElementRestContext::interfaceDeclaration()
{
    return getRuleContext<JavaParser::InterfaceDeclarationContext>(0);
}

JavaParser::EnumDeclarationContext* JavaParser::AnnotationTypeElementRestContext::enumDeclaration()
{
    return getRuleContext<JavaParser::EnumDeclarationContext>(0);
}

JavaParser::AnnotationTypeDeclarationContext* JavaParser::AnnotationTypeElementRestContext::annotationTypeDeclaration()
{
    return getRuleContext<JavaParser::AnnotationTypeDeclarationContext>(0);
}

JavaParser::RecordDeclarationContext* JavaParser::AnnotationTypeElementRestContext::recordDeclaration()
{
    return getRuleContext<JavaParser::RecordDeclarationContext>(0);
}

size_t JavaParser::AnnotationTypeElementRestContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotationTypeElementRest;
}

JavaParser::AnnotationTypeElementRestContext* JavaParser::annotationTypeElementRest()
{
    AnnotationTypeElementRestContext* _localctx =
        _tracker.createInstance<AnnotationTypeElementRestContext>(_ctx, getState());
    enterRule(_localctx, 126, JavaParser::RuleAnnotationTypeElementRest);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(906);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 102, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(882);
                typeType();
                setState(883);
                annotationMethodOrConstantRest();
                setState(884);
                match(JavaParser::SEMI);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(886);
                classDeclaration();
                setState(888);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 97, _ctx)) {
                    case 1: {
                        setState(887);
                        match(JavaParser::SEMI);
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(890);
                interfaceDeclaration();
                setState(892);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 98, _ctx)) {
                    case 1: {
                        setState(891);
                        match(JavaParser::SEMI);
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(894);
                enumDeclaration();
                setState(896);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 99, _ctx)) {
                    case 1: {
                        setState(895);
                        match(JavaParser::SEMI);
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            case 5: {
                enterOuterAlt(_localctx, 5);
                setState(898);
                annotationTypeDeclaration();
                setState(900);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 100, _ctx)) {
                    case 1: {
                        setState(899);
                        match(JavaParser::SEMI);
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            case 6: {
                enterOuterAlt(_localctx, 6);
                setState(902);
                recordDeclaration();
                setState(904);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 101, _ctx)) {
                    case 1: {
                        setState(903);
                        match(JavaParser::SEMI);
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationMethodOrConstantRestContext
//------------------------------------------------------------------

JavaParser::AnnotationMethodOrConstantRestContext::AnnotationMethodOrConstantRestContext(ParserRuleContext* parent,
                                                                                         size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::AnnotationMethodRestContext* JavaParser::AnnotationMethodOrConstantRestContext::annotationMethodRest()
{
    return getRuleContext<JavaParser::AnnotationMethodRestContext>(0);
}

JavaParser::AnnotationConstantRestContext* JavaParser::AnnotationMethodOrConstantRestContext::annotationConstantRest()
{
    return getRuleContext<JavaParser::AnnotationConstantRestContext>(0);
}

size_t JavaParser::AnnotationMethodOrConstantRestContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotationMethodOrConstantRest;
}

JavaParser::AnnotationMethodOrConstantRestContext* JavaParser::annotationMethodOrConstantRest()
{
    AnnotationMethodOrConstantRestContext* _localctx =
        _tracker.createInstance<AnnotationMethodOrConstantRestContext>(_ctx, getState());
    enterRule(_localctx, 128, JavaParser::RuleAnnotationMethodOrConstantRest);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(910);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 103, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(908);
                annotationMethodRest();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(909);
                annotationConstantRest();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationMethodRestContext ------------------------------------------------------------------

JavaParser::AnnotationMethodRestContext::AnnotationMethodRestContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::AnnotationMethodRestContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::AnnotationMethodRestContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

tree::TerminalNode* JavaParser::AnnotationMethodRestContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::DefaultValueContext* JavaParser::AnnotationMethodRestContext::defaultValue()
{
    return getRuleContext<JavaParser::DefaultValueContext>(0);
}

size_t JavaParser::AnnotationMethodRestContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotationMethodRest;
}

JavaParser::AnnotationMethodRestContext* JavaParser::annotationMethodRest()
{
    AnnotationMethodRestContext* _localctx = _tracker.createInstance<AnnotationMethodRestContext>(_ctx, getState());
    enterRule(_localctx, 130, JavaParser::RuleAnnotationMethodRest);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(912);
        identifier();
        setState(913);
        match(JavaParser::LPAREN);
        setState(914);
        match(JavaParser::RPAREN);
        setState(916);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::DEFAULT) {
            setState(915);
            defaultValue();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AnnotationConstantRestContext ------------------------------------------------------------------

JavaParser::AnnotationConstantRestContext::AnnotationConstantRestContext(ParserRuleContext* parent,
                                                                         size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::VariableDeclaratorsContext* JavaParser::AnnotationConstantRestContext::variableDeclarators()
{
    return getRuleContext<JavaParser::VariableDeclaratorsContext>(0);
}

size_t JavaParser::AnnotationConstantRestContext::getRuleIndex() const
{
    return JavaParser::RuleAnnotationConstantRest;
}

JavaParser::AnnotationConstantRestContext* JavaParser::annotationConstantRest()
{
    AnnotationConstantRestContext* _localctx = _tracker.createInstance<AnnotationConstantRestContext>(_ctx, getState());
    enterRule(_localctx, 132, JavaParser::RuleAnnotationConstantRest);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(918);
        variableDeclarators();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- DefaultValueContext ------------------------------------------------------------------

JavaParser::DefaultValueContext::DefaultValueContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::DefaultValueContext::DEFAULT()
{
    return getToken(JavaParser::DEFAULT, 0);
}

JavaParser::ElementValueContext* JavaParser::DefaultValueContext::elementValue()
{
    return getRuleContext<JavaParser::ElementValueContext>(0);
}

size_t JavaParser::DefaultValueContext::getRuleIndex() const
{
    return JavaParser::RuleDefaultValue;
}

JavaParser::DefaultValueContext* JavaParser::defaultValue()
{
    DefaultValueContext* _localctx = _tracker.createInstance<DefaultValueContext>(_ctx, getState());
    enterRule(_localctx, 134, JavaParser::RuleDefaultValue);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(920);
        match(JavaParser::DEFAULT);
        setState(921);
        elementValue();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ModuleDeclarationContext ------------------------------------------------------------------

JavaParser::ModuleDeclarationContext::ModuleDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ModuleDeclarationContext::MODULE()
{
    return getToken(JavaParser::MODULE, 0);
}

JavaParser::QualifiedNameContext* JavaParser::ModuleDeclarationContext::qualifiedName()
{
    return getRuleContext<JavaParser::QualifiedNameContext>(0);
}

JavaParser::ModuleBodyContext* JavaParser::ModuleDeclarationContext::moduleBody()
{
    return getRuleContext<JavaParser::ModuleBodyContext>(0);
}

tree::TerminalNode* JavaParser::ModuleDeclarationContext::OPEN()
{
    return getToken(JavaParser::OPEN, 0);
}

size_t JavaParser::ModuleDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleModuleDeclaration;
}

JavaParser::ModuleDeclarationContext* JavaParser::moduleDeclaration()
{
    ModuleDeclarationContext* _localctx = _tracker.createInstance<ModuleDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 136, JavaParser::RuleModuleDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(924);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::OPEN) {
            setState(923);
            match(JavaParser::OPEN);
        }
        setState(926);
        match(JavaParser::MODULE);
        setState(927);
        qualifiedName();
        setState(928);
        moduleBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ModuleBodyContext ------------------------------------------------------------------

JavaParser::ModuleBodyContext::ModuleBodyContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ModuleBodyContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::ModuleBodyContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::ModuleDirectiveContext*> JavaParser::ModuleBodyContext::moduleDirective()
{
    return getRuleContexts<JavaParser::ModuleDirectiveContext>();
}

JavaParser::ModuleDirectiveContext* JavaParser::ModuleBodyContext::moduleDirective(size_t i)
{
    return getRuleContext<JavaParser::ModuleDirectiveContext>(i);
}

size_t JavaParser::ModuleBodyContext::getRuleIndex() const
{
    return JavaParser::RuleModuleBody;
}

JavaParser::ModuleBodyContext* JavaParser::moduleBody()
{
    ModuleBodyContext* _localctx = _tracker.createInstance<ModuleBodyContext>(_ctx, getState());
    enterRule(_localctx, 138, JavaParser::RuleModuleBody);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(930);
        match(JavaParser::LBRACE);
        setState(934);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~0x3fULL) == 0) &&
                ((1ULL << _la) &
                 ((1ULL << JavaParser::REQUIRES) | (1ULL << JavaParser::EXPORTS) | (1ULL << JavaParser::OPENS) |
                  (1ULL << JavaParser::USES) | (1ULL << JavaParser::PROVIDES))) != 0)) {
            setState(931);
            moduleDirective();
            setState(936);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(937);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ModuleDirectiveContext ------------------------------------------------------------------

JavaParser::ModuleDirectiveContext::ModuleDirectiveContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::REQUIRES()
{
    return getToken(JavaParser::REQUIRES, 0);
}

std::vector<JavaParser::QualifiedNameContext*> JavaParser::ModuleDirectiveContext::qualifiedName()
{
    return getRuleContexts<JavaParser::QualifiedNameContext>();
}

JavaParser::QualifiedNameContext* JavaParser::ModuleDirectiveContext::qualifiedName(size_t i)
{
    return getRuleContext<JavaParser::QualifiedNameContext>(i);
}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

std::vector<JavaParser::RequiresModifierContext*> JavaParser::ModuleDirectiveContext::requiresModifier()
{
    return getRuleContexts<JavaParser::RequiresModifierContext>();
}

JavaParser::RequiresModifierContext* JavaParser::ModuleDirectiveContext::requiresModifier(size_t i)
{
    return getRuleContext<JavaParser::RequiresModifierContext>(i);
}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::EXPORTS()
{
    return getToken(JavaParser::EXPORTS, 0);
}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::TO()
{
    return getToken(JavaParser::TO, 0);
}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::OPENS()
{
    return getToken(JavaParser::OPENS, 0);
}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::USES()
{
    return getToken(JavaParser::USES, 0);
}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::PROVIDES()
{
    return getToken(JavaParser::PROVIDES, 0);
}

tree::TerminalNode* JavaParser::ModuleDirectiveContext::WITH()
{
    return getToken(JavaParser::WITH, 0);
}

size_t JavaParser::ModuleDirectiveContext::getRuleIndex() const
{
    return JavaParser::RuleModuleDirective;
}

JavaParser::ModuleDirectiveContext* JavaParser::moduleDirective()
{
    ModuleDirectiveContext* _localctx = _tracker.createInstance<ModuleDirectiveContext>(_ctx, getState());
    enterRule(_localctx, 140, JavaParser::RuleModuleDirective);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(975);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::REQUIRES: {
                enterOuterAlt(_localctx, 1);
                setState(939);
                match(JavaParser::REQUIRES);
                setState(943);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 107, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(940);
                        requiresModifier();
                    }
                    setState(945);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 107, _ctx);
                }
                setState(946);
                qualifiedName();
                setState(947);
                match(JavaParser::SEMI);
                break;
            }

            case JavaParser::EXPORTS: {
                enterOuterAlt(_localctx, 2);
                setState(949);
                match(JavaParser::EXPORTS);
                setState(950);
                qualifiedName();
                setState(953);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::TO) {
                    setState(951);
                    match(JavaParser::TO);
                    setState(952);
                    qualifiedName();
                }
                setState(955);
                match(JavaParser::SEMI);
                break;
            }

            case JavaParser::OPENS: {
                enterOuterAlt(_localctx, 3);
                setState(957);
                match(JavaParser::OPENS);
                setState(958);
                qualifiedName();
                setState(961);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::TO) {
                    setState(959);
                    match(JavaParser::TO);
                    setState(960);
                    qualifiedName();
                }
                setState(963);
                match(JavaParser::SEMI);
                break;
            }

            case JavaParser::USES: {
                enterOuterAlt(_localctx, 4);
                setState(965);
                match(JavaParser::USES);
                setState(966);
                qualifiedName();
                setState(967);
                match(JavaParser::SEMI);
                break;
            }

            case JavaParser::PROVIDES: {
                enterOuterAlt(_localctx, 5);
                setState(969);
                match(JavaParser::PROVIDES);
                setState(970);
                qualifiedName();
                setState(971);
                match(JavaParser::WITH);
                setState(972);
                qualifiedName();
                setState(973);
                match(JavaParser::SEMI);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- RequiresModifierContext ------------------------------------------------------------------

JavaParser::RequiresModifierContext::RequiresModifierContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::RequiresModifierContext::TRANSITIVE()
{
    return getToken(JavaParser::TRANSITIVE, 0);
}

tree::TerminalNode* JavaParser::RequiresModifierContext::STATIC()
{
    return getToken(JavaParser::STATIC, 0);
}

size_t JavaParser::RequiresModifierContext::getRuleIndex() const
{
    return JavaParser::RuleRequiresModifier;
}

JavaParser::RequiresModifierContext* JavaParser::requiresModifier()
{
    RequiresModifierContext* _localctx = _tracker.createInstance<RequiresModifierContext>(_ctx, getState());
    enterRule(_localctx, 142, JavaParser::RuleRequiresModifier);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(977);
        _la = _input->LA(1);
        if (!(_la == JavaParser::STATIC

              || _la == JavaParser::TRANSITIVE)) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- RecordDeclarationContext ------------------------------------------------------------------

JavaParser::RecordDeclarationContext::RecordDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::RecordDeclarationContext::RECORD()
{
    return getToken(JavaParser::RECORD, 0);
}

JavaParser::IdentifierContext* JavaParser::RecordDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::RecordHeaderContext* JavaParser::RecordDeclarationContext::recordHeader()
{
    return getRuleContext<JavaParser::RecordHeaderContext>(0);
}

JavaParser::RecordBodyContext* JavaParser::RecordDeclarationContext::recordBody()
{
    return getRuleContext<JavaParser::RecordBodyContext>(0);
}

JavaParser::TypeParametersContext* JavaParser::RecordDeclarationContext::typeParameters()
{
    return getRuleContext<JavaParser::TypeParametersContext>(0);
}

tree::TerminalNode* JavaParser::RecordDeclarationContext::IMPLEMENTS()
{
    return getToken(JavaParser::IMPLEMENTS, 0);
}

JavaParser::TypeListContext* JavaParser::RecordDeclarationContext::typeList()
{
    return getRuleContext<JavaParser::TypeListContext>(0);
}

size_t JavaParser::RecordDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleRecordDeclaration;
}

JavaParser::RecordDeclarationContext* JavaParser::recordDeclaration()
{
    RecordDeclarationContext* _localctx = _tracker.createInstance<RecordDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 144, JavaParser::RuleRecordDeclaration);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(979);
        match(JavaParser::RECORD);
        setState(980);
        identifier();
        setState(982);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LT) {
            setState(981);
            typeParameters();
        }
        setState(984);
        recordHeader();
        setState(987);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::IMPLEMENTS) {
            setState(985);
            match(JavaParser::IMPLEMENTS);
            setState(986);
            typeList();
        }
        setState(989);
        recordBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- RecordHeaderContext ------------------------------------------------------------------

JavaParser::RecordHeaderContext::RecordHeaderContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::RecordHeaderContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

tree::TerminalNode* JavaParser::RecordHeaderContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::RecordComponentListContext* JavaParser::RecordHeaderContext::recordComponentList()
{
    return getRuleContext<JavaParser::RecordComponentListContext>(0);
}

size_t JavaParser::RecordHeaderContext::getRuleIndex() const
{
    return JavaParser::RuleRecordHeader;
}

JavaParser::RecordHeaderContext* JavaParser::recordHeader()
{
    RecordHeaderContext* _localctx = _tracker.createInstance<RecordHeaderContext>(_ctx, getState());
    enterRule(_localctx, 146, JavaParser::RuleRecordHeader);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(991);
        match(JavaParser::LPAREN);
        setState(993);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 3) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 3)) &
              ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
               (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
               (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) | (1ULL << (JavaParser::LONG - 3)) |
               (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
               (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
               (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
               (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
               (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
               (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
               (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
               (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
            _la == JavaParser::AT

            || _la == JavaParser::IDENTIFIER) {
            setState(992);
            recordComponentList();
        }
        setState(995);
        match(JavaParser::RPAREN);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- RecordComponentListContext ------------------------------------------------------------------

JavaParser::RecordComponentListContext::RecordComponentListContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::RecordComponentContext*> JavaParser::RecordComponentListContext::recordComponent()
{
    return getRuleContexts<JavaParser::RecordComponentContext>();
}

JavaParser::RecordComponentContext* JavaParser::RecordComponentListContext::recordComponent(size_t i)
{
    return getRuleContext<JavaParser::RecordComponentContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::RecordComponentListContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::RecordComponentListContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::RecordComponentListContext::getRuleIndex() const
{
    return JavaParser::RuleRecordComponentList;
}

JavaParser::RecordComponentListContext* JavaParser::recordComponentList()
{
    RecordComponentListContext* _localctx = _tracker.createInstance<RecordComponentListContext>(_ctx, getState());
    enterRule(_localctx, 148, JavaParser::RuleRecordComponentList);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(997);
        recordComponent();
        setState(1002);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(998);
            match(JavaParser::COMMA);
            setState(999);
            recordComponent();
            setState(1004);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- RecordComponentContext ------------------------------------------------------------------

JavaParser::RecordComponentContext::RecordComponentContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::RecordComponentContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::IdentifierContext* JavaParser::RecordComponentContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

size_t JavaParser::RecordComponentContext::getRuleIndex() const
{
    return JavaParser::RuleRecordComponent;
}

JavaParser::RecordComponentContext* JavaParser::recordComponent()
{
    RecordComponentContext* _localctx = _tracker.createInstance<RecordComponentContext>(_ctx, getState());
    enterRule(_localctx, 150, JavaParser::RuleRecordComponent);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1005);
        typeType();
        setState(1006);
        identifier();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- RecordBodyContext ------------------------------------------------------------------

JavaParser::RecordBodyContext::RecordBodyContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::RecordBodyContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::RecordBodyContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::ClassBodyDeclarationContext*> JavaParser::RecordBodyContext::classBodyDeclaration()
{
    return getRuleContexts<JavaParser::ClassBodyDeclarationContext>();
}

JavaParser::ClassBodyDeclarationContext* JavaParser::RecordBodyContext::classBodyDeclaration(size_t i)
{
    return getRuleContext<JavaParser::ClassBodyDeclarationContext>(i);
}

size_t JavaParser::RecordBodyContext::getRuleIndex() const
{
    return JavaParser::RuleRecordBody;
}

JavaParser::RecordBodyContext* JavaParser::recordBody()
{
    RecordBodyContext* _localctx = _tracker.createInstance<RecordBodyContext>(_ctx, getState());
    enterRule(_localctx, 152, JavaParser::RuleRecordBody);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1008);
        match(JavaParser::LBRACE);
        setState(1012);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (
            ((((_la - 1) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 1)) &
              ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::BOOLEAN - 1)) |
               (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) | (1ULL << (JavaParser::CLASS - 1)) |
               (1ULL << (JavaParser::DOUBLE - 1)) | (1ULL << (JavaParser::ENUM - 1)) |
               (1ULL << (JavaParser::FINAL - 1)) | (1ULL << (JavaParser::FLOAT - 1)) | (1ULL << (JavaParser::INT - 1)) |
               (1ULL << (JavaParser::INTERFACE - 1)) | (1ULL << (JavaParser::LONG - 1)) |
               (1ULL << (JavaParser::NATIVE - 1)) | (1ULL << (JavaParser::PRIVATE - 1)) |
               (1ULL << (JavaParser::PROTECTED - 1)) | (1ULL << (JavaParser::PUBLIC - 1)) |
               (1ULL << (JavaParser::SHORT - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
               (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::SYNCHRONIZED - 1)) |
               (1ULL << (JavaParser::TRANSIENT - 1)) | (1ULL << (JavaParser::VOID - 1)) |
               (1ULL << (JavaParser::VOLATILE - 1)) | (1ULL << (JavaParser::MODULE - 1)) |
               (1ULL << (JavaParser::OPEN - 1)) | (1ULL << (JavaParser::REQUIRES - 1)) |
               (1ULL << (JavaParser::EXPORTS - 1)) | (1ULL << (JavaParser::OPENS - 1)) |
               (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
               (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
               (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
               (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
               (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
            ((((_la - 65) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 65)) & ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
                                      (1ULL << (JavaParser::LBRACE - 65)) | (1ULL << (JavaParser::SEMI - 65)) |
                                      (1ULL << (JavaParser::LT - 65)) | (1ULL << (JavaParser::AT - 65)) |
                                      (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
            setState(1009);
            classBodyDeclaration();
            setState(1014);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(1015);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

JavaParser::BlockContext::BlockContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::BlockContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::BlockContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::BlockStatementContext*> JavaParser::BlockContext::blockStatement()
{
    return getRuleContexts<JavaParser::BlockStatementContext>();
}

JavaParser::BlockStatementContext* JavaParser::BlockContext::blockStatement(size_t i)
{
    return getRuleContext<JavaParser::BlockStatementContext>(i);
}

size_t JavaParser::BlockContext::getRuleIndex() const
{
    return JavaParser::RuleBlock;
}

JavaParser::BlockContext* JavaParser::block()
{
    BlockContext* _localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
    enterRule(_localctx, 154, JavaParser::RuleBlock);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1017);
        match(JavaParser::LBRACE);
        setState(1021);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (
            ((((_la - 1) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 1)) &
              ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::ASSERT - 1)) |
               (1ULL << (JavaParser::BOOLEAN - 1)) | (1ULL << (JavaParser::BREAK - 1)) |
               (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) | (1ULL << (JavaParser::CLASS - 1)) |
               (1ULL << (JavaParser::CONTINUE - 1)) | (1ULL << (JavaParser::DO - 1)) |
               (1ULL << (JavaParser::DOUBLE - 1)) | (1ULL << (JavaParser::FINAL - 1)) |
               (1ULL << (JavaParser::FLOAT - 1)) | (1ULL << (JavaParser::FOR - 1)) | (1ULL << (JavaParser::IF - 1)) |
               (1ULL << (JavaParser::INT - 1)) | (1ULL << (JavaParser::INTERFACE - 1)) |
               (1ULL << (JavaParser::LONG - 1)) | (1ULL << (JavaParser::NEW - 1)) |
               (1ULL << (JavaParser::PRIVATE - 1)) | (1ULL << (JavaParser::PROTECTED - 1)) |
               (1ULL << (JavaParser::PUBLIC - 1)) | (1ULL << (JavaParser::RETURN - 1)) |
               (1ULL << (JavaParser::SHORT - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
               (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::SUPER - 1)) |
               (1ULL << (JavaParser::SWITCH - 1)) | (1ULL << (JavaParser::SYNCHRONIZED - 1)) |
               (1ULL << (JavaParser::THIS - 1)) | (1ULL << (JavaParser::THROW - 1)) | (1ULL << (JavaParser::TRY - 1)) |
               (1ULL << (JavaParser::VOID - 1)) | (1ULL << (JavaParser::WHILE - 1)) |
               (1ULL << (JavaParser::MODULE - 1)) | (1ULL << (JavaParser::OPEN - 1)) |
               (1ULL << (JavaParser::REQUIRES - 1)) | (1ULL << (JavaParser::EXPORTS - 1)) |
               (1ULL << (JavaParser::OPENS - 1)) | (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
               (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
               (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
               (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
               (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
            ((((_la - 65) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 65)) &
              ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
               (1ULL << (JavaParser::DECIMAL_LITERAL - 65)) | (1ULL << (JavaParser::HEX_LITERAL - 65)) |
               (1ULL << (JavaParser::OCT_LITERAL - 65)) | (1ULL << (JavaParser::BINARY_LITERAL - 65)) |
               (1ULL << (JavaParser::FLOAT_LITERAL - 65)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 65)) |
               (1ULL << (JavaParser::BOOL_LITERAL - 65)) | (1ULL << (JavaParser::CHAR_LITERAL - 65)) |
               (1ULL << (JavaParser::STRING_LITERAL - 65)) | (1ULL << (JavaParser::TEXT_BLOCK - 65)) |
               (1ULL << (JavaParser::NULL_LITERAL - 65)) | (1ULL << (JavaParser::LPAREN - 65)) |
               (1ULL << (JavaParser::LBRACE - 65)) | (1ULL << (JavaParser::SEMI - 65)) |
               (1ULL << (JavaParser::LT - 65)) | (1ULL << (JavaParser::BANG - 65)) |
               (1ULL << (JavaParser::TILDE - 65)) | (1ULL << (JavaParser::INC - 65)) |
               (1ULL << (JavaParser::DEC - 65)) | (1ULL << (JavaParser::ADD - 65)) | (1ULL << (JavaParser::SUB - 65)) |
               (1ULL << (JavaParser::AT - 65)) | (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
            setState(1018);
            blockStatement();
            setState(1023);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(1024);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- BlockStatementContext ------------------------------------------------------------------

JavaParser::BlockStatementContext::BlockStatementContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::LocalVariableDeclarationContext* JavaParser::BlockStatementContext::localVariableDeclaration()
{
    return getRuleContext<JavaParser::LocalVariableDeclarationContext>(0);
}

tree::TerminalNode* JavaParser::BlockStatementContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

JavaParser::StatementContext* JavaParser::BlockStatementContext::statement()
{
    return getRuleContext<JavaParser::StatementContext>(0);
}

JavaParser::LocalTypeDeclarationContext* JavaParser::BlockStatementContext::localTypeDeclaration()
{
    return getRuleContext<JavaParser::LocalTypeDeclarationContext>(0);
}

size_t JavaParser::BlockStatementContext::getRuleIndex() const
{
    return JavaParser::RuleBlockStatement;
}

JavaParser::BlockStatementContext* JavaParser::blockStatement()
{
    BlockStatementContext* _localctx = _tracker.createInstance<BlockStatementContext>(_ctx, getState());
    enterRule(_localctx, 156, JavaParser::RuleBlockStatement);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1031);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 117, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1026);
                localVariableDeclaration();
                setState(1027);
                match(JavaParser::SEMI);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1029);
                statement();
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(1030);
                localTypeDeclaration();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LocalVariableDeclarationContext ------------------------------------------------------------------

JavaParser::LocalVariableDeclarationContext::LocalVariableDeclarationContext(ParserRuleContext* parent,
                                                                             size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::LocalVariableDeclarationContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::VariableDeclaratorsContext* JavaParser::LocalVariableDeclarationContext::variableDeclarators()
{
    return getRuleContext<JavaParser::VariableDeclaratorsContext>(0);
}

tree::TerminalNode* JavaParser::LocalVariableDeclarationContext::VAR()
{
    return getToken(JavaParser::VAR, 0);
}

JavaParser::IdentifierContext* JavaParser::LocalVariableDeclarationContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::LocalVariableDeclarationContext::ASSIGN()
{
    return getToken(JavaParser::ASSIGN, 0);
}

JavaParser::ExpressionContext* JavaParser::LocalVariableDeclarationContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::LocalVariableDeclarationContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::LocalVariableDeclarationContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

size_t JavaParser::LocalVariableDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleLocalVariableDeclaration;
}

JavaParser::LocalVariableDeclarationContext* JavaParser::localVariableDeclaration()
{
    LocalVariableDeclarationContext* _localctx =
        _tracker.createInstance<LocalVariableDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 158, JavaParser::RuleLocalVariableDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1036);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 118, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1033);
                variableModifier();
            }
            setState(1038);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 118, _ctx);
        }
        setState(1047);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 119, _ctx)) {
            case 1: {
                setState(1039);
                typeType();
                setState(1040);
                variableDeclarators();
                break;
            }

            case 2: {
                setState(1042);
                match(JavaParser::VAR);
                setState(1043);
                identifier();
                setState(1044);
                match(JavaParser::ASSIGN);
                setState(1045);
                expression(0);
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- IdentifierContext ------------------------------------------------------------------

JavaParser::IdentifierContext::IdentifierContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::IdentifierContext::IDENTIFIER()
{
    return getToken(JavaParser::IDENTIFIER, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::MODULE()
{
    return getToken(JavaParser::MODULE, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::OPEN()
{
    return getToken(JavaParser::OPEN, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::REQUIRES()
{
    return getToken(JavaParser::REQUIRES, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::EXPORTS()
{
    return getToken(JavaParser::EXPORTS, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::OPENS()
{
    return getToken(JavaParser::OPENS, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::TO()
{
    return getToken(JavaParser::TO, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::USES()
{
    return getToken(JavaParser::USES, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::PROVIDES()
{
    return getToken(JavaParser::PROVIDES, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::WITH()
{
    return getToken(JavaParser::WITH, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::TRANSITIVE()
{
    return getToken(JavaParser::TRANSITIVE, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::YIELD()
{
    return getToken(JavaParser::YIELD, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::SEALED()
{
    return getToken(JavaParser::SEALED, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::PERMITS()
{
    return getToken(JavaParser::PERMITS, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::RECORD()
{
    return getToken(JavaParser::RECORD, 0);
}

tree::TerminalNode* JavaParser::IdentifierContext::VAR()
{
    return getToken(JavaParser::VAR, 0);
}

size_t JavaParser::IdentifierContext::getRuleIndex() const
{
    return JavaParser::RuleIdentifier;
}

JavaParser::IdentifierContext* JavaParser::identifier()
{
    IdentifierContext* _localctx = _tracker.createInstance<IdentifierContext>(_ctx, getState());
    enterRule(_localctx, 160, JavaParser::RuleIdentifier);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1049);
        _la = _input->LA(1);
        if (!(((((_la - 51) & ~0x3fULL) == 0) &&
               ((1ULL << (_la - 51)) & ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                                        (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                                        (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                                        (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                                        (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                                        (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                                        (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                                        (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
              _la == JavaParser::IDENTIFIER)) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LocalTypeDeclarationContext ------------------------------------------------------------------

JavaParser::LocalTypeDeclarationContext::LocalTypeDeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ClassDeclarationContext* JavaParser::LocalTypeDeclarationContext::classDeclaration()
{
    return getRuleContext<JavaParser::ClassDeclarationContext>(0);
}

JavaParser::InterfaceDeclarationContext* JavaParser::LocalTypeDeclarationContext::interfaceDeclaration()
{
    return getRuleContext<JavaParser::InterfaceDeclarationContext>(0);
}

JavaParser::RecordDeclarationContext* JavaParser::LocalTypeDeclarationContext::recordDeclaration()
{
    return getRuleContext<JavaParser::RecordDeclarationContext>(0);
}

std::vector<JavaParser::ClassOrInterfaceModifierContext*>
JavaParser::LocalTypeDeclarationContext::classOrInterfaceModifier()
{
    return getRuleContexts<JavaParser::ClassOrInterfaceModifierContext>();
}

JavaParser::ClassOrInterfaceModifierContext* JavaParser::LocalTypeDeclarationContext::classOrInterfaceModifier(size_t i)
{
    return getRuleContext<JavaParser::ClassOrInterfaceModifierContext>(i);
}

tree::TerminalNode* JavaParser::LocalTypeDeclarationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

size_t JavaParser::LocalTypeDeclarationContext::getRuleIndex() const
{
    return JavaParser::RuleLocalTypeDeclaration;
}

JavaParser::LocalTypeDeclarationContext* JavaParser::localTypeDeclaration()
{
    LocalTypeDeclarationContext* _localctx = _tracker.createInstance<LocalTypeDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 162, JavaParser::RuleLocalTypeDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(1063);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::ABSTRACT:
            case JavaParser::CLASS:
            case JavaParser::FINAL:
            case JavaParser::INTERFACE:
            case JavaParser::PRIVATE:
            case JavaParser::PROTECTED:
            case JavaParser::PUBLIC:
            case JavaParser::STATIC:
            case JavaParser::STRICTFP:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::NON_SEALED:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(1054);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 120, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1051);
                        classOrInterfaceModifier();
                    }
                    setState(1056);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 120, _ctx);
                }
                setState(1060);
                _errHandler->sync(this);
                switch (_input->LA(1)) {
                    case JavaParser::CLASS: {
                        setState(1057);
                        classDeclaration();
                        break;
                    }

                    case JavaParser::INTERFACE: {
                        setState(1058);
                        interfaceDeclaration();
                        break;
                    }

                    case JavaParser::RECORD: {
                        setState(1059);
                        recordDeclaration();
                        break;
                    }

                    default:
                        throw NoViableAltException(this);
                }
                break;
            }

            case JavaParser::SEMI: {
                enterOuterAlt(_localctx, 2);
                setState(1062);
                match(JavaParser::SEMI);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

JavaParser::StatementContext::StatementContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::BlockContext* JavaParser::StatementContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

tree::TerminalNode* JavaParser::StatementContext::ASSERT()
{
    return getToken(JavaParser::ASSERT, 0);
}

std::vector<JavaParser::ExpressionContext*> JavaParser::StatementContext::expression()
{
    return getRuleContexts<JavaParser::ExpressionContext>();
}

JavaParser::ExpressionContext* JavaParser::StatementContext::expression(size_t i)
{
    return getRuleContext<JavaParser::ExpressionContext>(i);
}

tree::TerminalNode* JavaParser::StatementContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

tree::TerminalNode* JavaParser::StatementContext::COLON()
{
    return getToken(JavaParser::COLON, 0);
}

tree::TerminalNode* JavaParser::StatementContext::IF()
{
    return getToken(JavaParser::IF, 0);
}

JavaParser::ParExpressionContext* JavaParser::StatementContext::parExpression()
{
    return getRuleContext<JavaParser::ParExpressionContext>(0);
}

std::vector<JavaParser::StatementContext*> JavaParser::StatementContext::statement()
{
    return getRuleContexts<JavaParser::StatementContext>();
}

JavaParser::StatementContext* JavaParser::StatementContext::statement(size_t i)
{
    return getRuleContext<JavaParser::StatementContext>(i);
}

tree::TerminalNode* JavaParser::StatementContext::ELSE()
{
    return getToken(JavaParser::ELSE, 0);
}

tree::TerminalNode* JavaParser::StatementContext::FOR()
{
    return getToken(JavaParser::FOR, 0);
}

tree::TerminalNode* JavaParser::StatementContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

JavaParser::ForControlContext* JavaParser::StatementContext::forControl()
{
    return getRuleContext<JavaParser::ForControlContext>(0);
}

tree::TerminalNode* JavaParser::StatementContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

tree::TerminalNode* JavaParser::StatementContext::WHILE()
{
    return getToken(JavaParser::WHILE, 0);
}

tree::TerminalNode* JavaParser::StatementContext::DO()
{
    return getToken(JavaParser::DO, 0);
}

tree::TerminalNode* JavaParser::StatementContext::TRY()
{
    return getToken(JavaParser::TRY, 0);
}

JavaParser::FinallyBlockContext* JavaParser::StatementContext::finallyBlock()
{
    return getRuleContext<JavaParser::FinallyBlockContext>(0);
}

std::vector<JavaParser::CatchClauseContext*> JavaParser::StatementContext::catchClause()
{
    return getRuleContexts<JavaParser::CatchClauseContext>();
}

JavaParser::CatchClauseContext* JavaParser::StatementContext::catchClause(size_t i)
{
    return getRuleContext<JavaParser::CatchClauseContext>(i);
}

JavaParser::ResourceSpecificationContext* JavaParser::StatementContext::resourceSpecification()
{
    return getRuleContext<JavaParser::ResourceSpecificationContext>(0);
}

tree::TerminalNode* JavaParser::StatementContext::SWITCH()
{
    return getToken(JavaParser::SWITCH, 0);
}

tree::TerminalNode* JavaParser::StatementContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::StatementContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::SwitchBlockStatementGroupContext*> JavaParser::StatementContext::switchBlockStatementGroup()
{
    return getRuleContexts<JavaParser::SwitchBlockStatementGroupContext>();
}

JavaParser::SwitchBlockStatementGroupContext* JavaParser::StatementContext::switchBlockStatementGroup(size_t i)
{
    return getRuleContext<JavaParser::SwitchBlockStatementGroupContext>(i);
}

std::vector<JavaParser::SwitchLabelContext*> JavaParser::StatementContext::switchLabel()
{
    return getRuleContexts<JavaParser::SwitchLabelContext>();
}

JavaParser::SwitchLabelContext* JavaParser::StatementContext::switchLabel(size_t i)
{
    return getRuleContext<JavaParser::SwitchLabelContext>(i);
}

tree::TerminalNode* JavaParser::StatementContext::SYNCHRONIZED()
{
    return getToken(JavaParser::SYNCHRONIZED, 0);
}

tree::TerminalNode* JavaParser::StatementContext::RETURN()
{
    return getToken(JavaParser::RETURN, 0);
}

tree::TerminalNode* JavaParser::StatementContext::THROW()
{
    return getToken(JavaParser::THROW, 0);
}

tree::TerminalNode* JavaParser::StatementContext::BREAK()
{
    return getToken(JavaParser::BREAK, 0);
}

JavaParser::IdentifierContext* JavaParser::StatementContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::StatementContext::CONTINUE()
{
    return getToken(JavaParser::CONTINUE, 0);
}

tree::TerminalNode* JavaParser::StatementContext::YIELD()
{
    return getToken(JavaParser::YIELD, 0);
}

JavaParser::SwitchExpressionContext* JavaParser::StatementContext::switchExpression()
{
    return getRuleContext<JavaParser::SwitchExpressionContext>(0);
}

size_t JavaParser::StatementContext::getRuleIndex() const
{
    return JavaParser::RuleStatement;
}

JavaParser::StatementContext* JavaParser::statement()
{
    StatementContext* _localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
    enterRule(_localctx, 164, JavaParser::RuleStatement);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(1178);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 136, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1065);
                antlrcpp::downCast<StatementContext*>(_localctx)->blockLabel = block();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1066);
                match(JavaParser::ASSERT);
                setState(1067);
                expression(0);
                setState(1070);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::COLON) {
                    setState(1068);
                    match(JavaParser::COLON);
                    setState(1069);
                    expression(0);
                }
                setState(1072);
                match(JavaParser::SEMI);
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(1074);
                match(JavaParser::IF);
                setState(1075);
                parExpression();
                setState(1076);
                statement();
                setState(1079);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 124, _ctx)) {
                    case 1: {
                        setState(1077);
                        match(JavaParser::ELSE);
                        setState(1078);
                        statement();
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(1081);
                match(JavaParser::FOR);
                setState(1082);
                match(JavaParser::LPAREN);
                setState(1083);
                forControl();
                setState(1084);
                match(JavaParser::RPAREN);
                setState(1085);
                statement();
                break;
            }

            case 5: {
                enterOuterAlt(_localctx, 5);
                setState(1087);
                match(JavaParser::WHILE);
                setState(1088);
                parExpression();
                setState(1089);
                statement();
                break;
            }

            case 6: {
                enterOuterAlt(_localctx, 6);
                setState(1091);
                match(JavaParser::DO);
                setState(1092);
                statement();
                setState(1093);
                match(JavaParser::WHILE);
                setState(1094);
                parExpression();
                setState(1095);
                match(JavaParser::SEMI);
                break;
            }

            case 7: {
                enterOuterAlt(_localctx, 7);
                setState(1097);
                match(JavaParser::TRY);
                setState(1098);
                block();
                setState(1108);
                _errHandler->sync(this);
                switch (_input->LA(1)) {
                    case JavaParser::CATCH: {
                        setState(1100);
                        _errHandler->sync(this);
                        _la = _input->LA(1);
                        do {
                            setState(1099);
                            catchClause();
                            setState(1102);
                            _errHandler->sync(this);
                            _la = _input->LA(1);
                        } while (_la == JavaParser::CATCH);
                        setState(1105);
                        _errHandler->sync(this);

                        _la = _input->LA(1);
                        if (_la == JavaParser::FINALLY) {
                            setState(1104);
                            finallyBlock();
                        }
                        break;
                    }

                    case JavaParser::FINALLY: {
                        setState(1107);
                        finallyBlock();
                        break;
                    }

                    default:
                        throw NoViableAltException(this);
                }
                break;
            }

            case 8: {
                enterOuterAlt(_localctx, 8);
                setState(1110);
                match(JavaParser::TRY);
                setState(1111);
                resourceSpecification();
                setState(1112);
                block();
                setState(1116);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == JavaParser::CATCH) {
                    setState(1113);
                    catchClause();
                    setState(1118);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(1120);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::FINALLY) {
                    setState(1119);
                    finallyBlock();
                }
                break;
            }

            case 9: {
                enterOuterAlt(_localctx, 9);
                setState(1122);
                match(JavaParser::SWITCH);
                setState(1123);
                parExpression();
                setState(1124);
                match(JavaParser::LBRACE);
                setState(1128);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 130, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1125);
                        switchBlockStatementGroup();
                    }
                    setState(1130);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 130, _ctx);
                }
                setState(1134);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == JavaParser::CASE

                       || _la == JavaParser::DEFAULT) {
                    setState(1131);
                    switchLabel();
                    setState(1136);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(1137);
                match(JavaParser::RBRACE);
                break;
            }

            case 10: {
                enterOuterAlt(_localctx, 10);
                setState(1139);
                match(JavaParser::SYNCHRONIZED);
                setState(1140);
                parExpression();
                setState(1141);
                block();
                break;
            }

            case 11: {
                enterOuterAlt(_localctx, 11);
                setState(1143);
                match(JavaParser::RETURN);
                setState(1145);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) |
                       (1ULL << (JavaParser::LONG - 3)) | (1ULL << (JavaParser::NEW - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
                       (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
                       (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    ((((_la - 67) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 67)) &
                      ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                       (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
                       (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
                       (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
                       (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
                       (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
                       (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
                       (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
                       (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
                       (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
                       (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
                    setState(1144);
                    expression(0);
                }
                setState(1147);
                match(JavaParser::SEMI);
                break;
            }

            case 12: {
                enterOuterAlt(_localctx, 12);
                setState(1148);
                match(JavaParser::THROW);
                setState(1149);
                expression(0);
                setState(1150);
                match(JavaParser::SEMI);
                break;
            }

            case 13: {
                enterOuterAlt(_localctx, 13);
                setState(1152);
                match(JavaParser::BREAK);
                setState(1154);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 51) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 51)) &
                      ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                       (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                       (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                       (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                       (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                       (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                       (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                       (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
                    _la == JavaParser::IDENTIFIER) {
                    setState(1153);
                    identifier();
                }
                setState(1156);
                match(JavaParser::SEMI);
                break;
            }

            case 14: {
                enterOuterAlt(_localctx, 14);
                setState(1157);
                match(JavaParser::CONTINUE);
                setState(1159);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 51) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 51)) &
                      ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                       (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                       (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                       (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                       (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                       (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                       (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                       (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
                    _la == JavaParser::IDENTIFIER) {
                    setState(1158);
                    identifier();
                }
                setState(1161);
                match(JavaParser::SEMI);
                break;
            }

            case 15: {
                enterOuterAlt(_localctx, 15);
                setState(1162);
                match(JavaParser::YIELD);
                setState(1163);
                expression(0);
                setState(1164);
                match(JavaParser::SEMI);
                break;
            }

            case 16: {
                enterOuterAlt(_localctx, 16);
                setState(1166);
                match(JavaParser::SEMI);
                break;
            }

            case 17: {
                enterOuterAlt(_localctx, 17);
                setState(1167);
                antlrcpp::downCast<StatementContext*>(_localctx)->statementExpression = expression(0);
                setState(1168);
                match(JavaParser::SEMI);
                break;
            }

            case 18: {
                enterOuterAlt(_localctx, 18);
                setState(1170);
                switchExpression();
                setState(1172);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 135, _ctx)) {
                    case 1: {
                        setState(1171);
                        match(JavaParser::SEMI);
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            case 19: {
                enterOuterAlt(_localctx, 19);
                setState(1174);
                antlrcpp::downCast<StatementContext*>(_localctx)->identifierLabel = identifier();
                setState(1175);
                match(JavaParser::COLON);
                setState(1176);
                statement();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- CatchClauseContext ------------------------------------------------------------------

JavaParser::CatchClauseContext::CatchClauseContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::CatchClauseContext::CATCH()
{
    return getToken(JavaParser::CATCH, 0);
}

tree::TerminalNode* JavaParser::CatchClauseContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

JavaParser::CatchTypeContext* JavaParser::CatchClauseContext::catchType()
{
    return getRuleContext<JavaParser::CatchTypeContext>(0);
}

JavaParser::IdentifierContext* JavaParser::CatchClauseContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::CatchClauseContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::BlockContext* JavaParser::CatchClauseContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::CatchClauseContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::CatchClauseContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

size_t JavaParser::CatchClauseContext::getRuleIndex() const
{
    return JavaParser::RuleCatchClause;
}

JavaParser::CatchClauseContext* JavaParser::catchClause()
{
    CatchClauseContext* _localctx = _tracker.createInstance<CatchClauseContext>(_ctx, getState());
    enterRule(_localctx, 166, JavaParser::RuleCatchClause);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1180);
        match(JavaParser::CATCH);
        setState(1181);
        match(JavaParser::LPAREN);
        setState(1185);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 137, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1182);
                variableModifier();
            }
            setState(1187);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 137, _ctx);
        }
        setState(1188);
        catchType();
        setState(1189);
        identifier();
        setState(1190);
        match(JavaParser::RPAREN);
        setState(1191);
        block();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- CatchTypeContext ------------------------------------------------------------------

JavaParser::CatchTypeContext::CatchTypeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::QualifiedNameContext*> JavaParser::CatchTypeContext::qualifiedName()
{
    return getRuleContexts<JavaParser::QualifiedNameContext>();
}

JavaParser::QualifiedNameContext* JavaParser::CatchTypeContext::qualifiedName(size_t i)
{
    return getRuleContext<JavaParser::QualifiedNameContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::CatchTypeContext::BITOR()
{
    return getTokens(JavaParser::BITOR);
}

tree::TerminalNode* JavaParser::CatchTypeContext::BITOR(size_t i)
{
    return getToken(JavaParser::BITOR, i);
}

size_t JavaParser::CatchTypeContext::getRuleIndex() const
{
    return JavaParser::RuleCatchType;
}

JavaParser::CatchTypeContext* JavaParser::catchType()
{
    CatchTypeContext* _localctx = _tracker.createInstance<CatchTypeContext>(_ctx, getState());
    enterRule(_localctx, 168, JavaParser::RuleCatchType);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1193);
        qualifiedName();
        setState(1198);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::BITOR) {
            setState(1194);
            match(JavaParser::BITOR);
            setState(1195);
            qualifiedName();
            setState(1200);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FinallyBlockContext ------------------------------------------------------------------

JavaParser::FinallyBlockContext::FinallyBlockContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::FinallyBlockContext::FINALLY()
{
    return getToken(JavaParser::FINALLY, 0);
}

JavaParser::BlockContext* JavaParser::FinallyBlockContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

size_t JavaParser::FinallyBlockContext::getRuleIndex() const
{
    return JavaParser::RuleFinallyBlock;
}

JavaParser::FinallyBlockContext* JavaParser::finallyBlock()
{
    FinallyBlockContext* _localctx = _tracker.createInstance<FinallyBlockContext>(_ctx, getState());
    enterRule(_localctx, 170, JavaParser::RuleFinallyBlock);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1201);
        match(JavaParser::FINALLY);
        setState(1202);
        block();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ResourceSpecificationContext ------------------------------------------------------------------

JavaParser::ResourceSpecificationContext::ResourceSpecificationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ResourceSpecificationContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

JavaParser::ResourcesContext* JavaParser::ResourceSpecificationContext::resources()
{
    return getRuleContext<JavaParser::ResourcesContext>(0);
}

tree::TerminalNode* JavaParser::ResourceSpecificationContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

tree::TerminalNode* JavaParser::ResourceSpecificationContext::SEMI()
{
    return getToken(JavaParser::SEMI, 0);
}

size_t JavaParser::ResourceSpecificationContext::getRuleIndex() const
{
    return JavaParser::RuleResourceSpecification;
}

JavaParser::ResourceSpecificationContext* JavaParser::resourceSpecification()
{
    ResourceSpecificationContext* _localctx = _tracker.createInstance<ResourceSpecificationContext>(_ctx, getState());
    enterRule(_localctx, 172, JavaParser::RuleResourceSpecification);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1204);
        match(JavaParser::LPAREN);
        setState(1205);
        resources();
        setState(1207);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::SEMI) {
            setState(1206);
            match(JavaParser::SEMI);
        }
        setState(1209);
        match(JavaParser::RPAREN);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ResourcesContext ------------------------------------------------------------------

JavaParser::ResourcesContext::ResourcesContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::ResourceContext*> JavaParser::ResourcesContext::resource()
{
    return getRuleContexts<JavaParser::ResourceContext>();
}

JavaParser::ResourceContext* JavaParser::ResourcesContext::resource(size_t i)
{
    return getRuleContext<JavaParser::ResourceContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ResourcesContext::SEMI()
{
    return getTokens(JavaParser::SEMI);
}

tree::TerminalNode* JavaParser::ResourcesContext::SEMI(size_t i)
{
    return getToken(JavaParser::SEMI, i);
}

size_t JavaParser::ResourcesContext::getRuleIndex() const
{
    return JavaParser::RuleResources;
}

JavaParser::ResourcesContext* JavaParser::resources()
{
    ResourcesContext* _localctx = _tracker.createInstance<ResourcesContext>(_ctx, getState());
    enterRule(_localctx, 174, JavaParser::RuleResources);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1211);
        resource();
        setState(1216);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 140, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1212);
                match(JavaParser::SEMI);
                setState(1213);
                resource();
            }
            setState(1218);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 140, _ctx);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ResourceContext ------------------------------------------------------------------

JavaParser::ResourceContext::ResourceContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ResourceContext::ASSIGN()
{
    return getToken(JavaParser::ASSIGN, 0);
}

JavaParser::ExpressionContext* JavaParser::ResourceContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

JavaParser::ClassOrInterfaceTypeContext* JavaParser::ResourceContext::classOrInterfaceType()
{
    return getRuleContext<JavaParser::ClassOrInterfaceTypeContext>(0);
}

JavaParser::VariableDeclaratorIdContext* JavaParser::ResourceContext::variableDeclaratorId()
{
    return getRuleContext<JavaParser::VariableDeclaratorIdContext>(0);
}

tree::TerminalNode* JavaParser::ResourceContext::VAR()
{
    return getToken(JavaParser::VAR, 0);
}

JavaParser::IdentifierContext* JavaParser::ResourceContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::ResourceContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::ResourceContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

size_t JavaParser::ResourceContext::getRuleIndex() const
{
    return JavaParser::RuleResource;
}

JavaParser::ResourceContext* JavaParser::resource()
{
    ResourceContext* _localctx = _tracker.createInstance<ResourceContext>(_ctx, getState());
    enterRule(_localctx, 176, JavaParser::RuleResource);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        setState(1236);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 143, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1222);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 141, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1219);
                        variableModifier();
                    }
                    setState(1224);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 141, _ctx);
                }
                setState(1230);
                _errHandler->sync(this);
                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 142, _ctx)) {
                    case 1: {
                        setState(1225);
                        classOrInterfaceType();
                        setState(1226);
                        variableDeclaratorId();
                        break;
                    }

                    case 2: {
                        setState(1228);
                        match(JavaParser::VAR);
                        setState(1229);
                        identifier();
                        break;
                    }

                    default:
                        break;
                }
                setState(1232);
                match(JavaParser::ASSIGN);
                setState(1233);
                expression(0);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1235);
                identifier();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- SwitchBlockStatementGroupContext ------------------------------------------------------------------

JavaParser::SwitchBlockStatementGroupContext::SwitchBlockStatementGroupContext(ParserRuleContext* parent,
                                                                               size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::SwitchLabelContext*> JavaParser::SwitchBlockStatementGroupContext::switchLabel()
{
    return getRuleContexts<JavaParser::SwitchLabelContext>();
}

JavaParser::SwitchLabelContext* JavaParser::SwitchBlockStatementGroupContext::switchLabel(size_t i)
{
    return getRuleContext<JavaParser::SwitchLabelContext>(i);
}

std::vector<JavaParser::BlockStatementContext*> JavaParser::SwitchBlockStatementGroupContext::blockStatement()
{
    return getRuleContexts<JavaParser::BlockStatementContext>();
}

JavaParser::BlockStatementContext* JavaParser::SwitchBlockStatementGroupContext::blockStatement(size_t i)
{
    return getRuleContext<JavaParser::BlockStatementContext>(i);
}

size_t JavaParser::SwitchBlockStatementGroupContext::getRuleIndex() const
{
    return JavaParser::RuleSwitchBlockStatementGroup;
}

JavaParser::SwitchBlockStatementGroupContext* JavaParser::switchBlockStatementGroup()
{
    SwitchBlockStatementGroupContext* _localctx =
        _tracker.createInstance<SwitchBlockStatementGroupContext>(_ctx, getState());
    enterRule(_localctx, 178, JavaParser::RuleSwitchBlockStatementGroup);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1239);
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
            setState(1238);
            switchLabel();
            setState(1241);
            _errHandler->sync(this);
            _la = _input->LA(1);
        } while (_la == JavaParser::CASE

                 || _la == JavaParser::DEFAULT);
        setState(1244);
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
            setState(1243);
            blockStatement();
            setState(1246);
            _errHandler->sync(this);
            _la = _input->LA(1);
        } while (
            ((((_la - 1) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 1)) &
              ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::ASSERT - 1)) |
               (1ULL << (JavaParser::BOOLEAN - 1)) | (1ULL << (JavaParser::BREAK - 1)) |
               (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) | (1ULL << (JavaParser::CLASS - 1)) |
               (1ULL << (JavaParser::CONTINUE - 1)) | (1ULL << (JavaParser::DO - 1)) |
               (1ULL << (JavaParser::DOUBLE - 1)) | (1ULL << (JavaParser::FINAL - 1)) |
               (1ULL << (JavaParser::FLOAT - 1)) | (1ULL << (JavaParser::FOR - 1)) | (1ULL << (JavaParser::IF - 1)) |
               (1ULL << (JavaParser::INT - 1)) | (1ULL << (JavaParser::INTERFACE - 1)) |
               (1ULL << (JavaParser::LONG - 1)) | (1ULL << (JavaParser::NEW - 1)) |
               (1ULL << (JavaParser::PRIVATE - 1)) | (1ULL << (JavaParser::PROTECTED - 1)) |
               (1ULL << (JavaParser::PUBLIC - 1)) | (1ULL << (JavaParser::RETURN - 1)) |
               (1ULL << (JavaParser::SHORT - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
               (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::SUPER - 1)) |
               (1ULL << (JavaParser::SWITCH - 1)) | (1ULL << (JavaParser::SYNCHRONIZED - 1)) |
               (1ULL << (JavaParser::THIS - 1)) | (1ULL << (JavaParser::THROW - 1)) | (1ULL << (JavaParser::TRY - 1)) |
               (1ULL << (JavaParser::VOID - 1)) | (1ULL << (JavaParser::WHILE - 1)) |
               (1ULL << (JavaParser::MODULE - 1)) | (1ULL << (JavaParser::OPEN - 1)) |
               (1ULL << (JavaParser::REQUIRES - 1)) | (1ULL << (JavaParser::EXPORTS - 1)) |
               (1ULL << (JavaParser::OPENS - 1)) | (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
               (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
               (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
               (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
               (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
            ((((_la - 65) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 65)) &
              ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
               (1ULL << (JavaParser::DECIMAL_LITERAL - 65)) | (1ULL << (JavaParser::HEX_LITERAL - 65)) |
               (1ULL << (JavaParser::OCT_LITERAL - 65)) | (1ULL << (JavaParser::BINARY_LITERAL - 65)) |
               (1ULL << (JavaParser::FLOAT_LITERAL - 65)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 65)) |
               (1ULL << (JavaParser::BOOL_LITERAL - 65)) | (1ULL << (JavaParser::CHAR_LITERAL - 65)) |
               (1ULL << (JavaParser::STRING_LITERAL - 65)) | (1ULL << (JavaParser::TEXT_BLOCK - 65)) |
               (1ULL << (JavaParser::NULL_LITERAL - 65)) | (1ULL << (JavaParser::LPAREN - 65)) |
               (1ULL << (JavaParser::LBRACE - 65)) | (1ULL << (JavaParser::SEMI - 65)) |
               (1ULL << (JavaParser::LT - 65)) | (1ULL << (JavaParser::BANG - 65)) |
               (1ULL << (JavaParser::TILDE - 65)) | (1ULL << (JavaParser::INC - 65)) |
               (1ULL << (JavaParser::DEC - 65)) | (1ULL << (JavaParser::ADD - 65)) | (1ULL << (JavaParser::SUB - 65)) |
               (1ULL << (JavaParser::AT - 65)) | (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0));

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- SwitchLabelContext ------------------------------------------------------------------

JavaParser::SwitchLabelContext::SwitchLabelContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::SwitchLabelContext::CASE()
{
    return getToken(JavaParser::CASE, 0);
}

tree::TerminalNode* JavaParser::SwitchLabelContext::COLON()
{
    return getToken(JavaParser::COLON, 0);
}

JavaParser::TypeTypeContext* JavaParser::SwitchLabelContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::ExpressionContext* JavaParser::SwitchLabelContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaParser::SwitchLabelContext::IDENTIFIER()
{
    return getToken(JavaParser::IDENTIFIER, 0);
}

JavaParser::IdentifierContext* JavaParser::SwitchLabelContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::SwitchLabelContext::DEFAULT()
{
    return getToken(JavaParser::DEFAULT, 0);
}

size_t JavaParser::SwitchLabelContext::getRuleIndex() const
{
    return JavaParser::RuleSwitchLabel;
}

JavaParser::SwitchLabelContext* JavaParser::switchLabel()
{
    SwitchLabelContext* _localctx = _tracker.createInstance<SwitchLabelContext>(_ctx, getState());
    enterRule(_localctx, 180, JavaParser::RuleSwitchLabel);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1259);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::CASE: {
                enterOuterAlt(_localctx, 1);
                setState(1248);
                match(JavaParser::CASE);
                setState(1254);
                _errHandler->sync(this);
                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 146, _ctx)) {
                    case 1: {
                        setState(1249);
                        antlrcpp::downCast<SwitchLabelContext*>(_localctx)->constantExpression = expression(0);
                        break;
                    }

                    case 2: {
                        setState(1250);
                        antlrcpp::downCast<SwitchLabelContext*>(_localctx)->enumConstantName =
                            match(JavaParser::IDENTIFIER);
                        break;
                    }

                    case 3: {
                        setState(1251);
                        typeType();
                        setState(1252);
                        antlrcpp::downCast<SwitchLabelContext*>(_localctx)->varName = identifier();
                        break;
                    }

                    default:
                        break;
                }
                setState(1256);
                match(JavaParser::COLON);
                break;
            }

            case JavaParser::DEFAULT: {
                enterOuterAlt(_localctx, 2);
                setState(1257);
                match(JavaParser::DEFAULT);
                setState(1258);
                match(JavaParser::COLON);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ForControlContext ------------------------------------------------------------------

JavaParser::ForControlContext::ForControlContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::EnhancedForControlContext* JavaParser::ForControlContext::enhancedForControl()
{
    return getRuleContext<JavaParser::EnhancedForControlContext>(0);
}

std::vector<tree::TerminalNode*> JavaParser::ForControlContext::SEMI()
{
    return getTokens(JavaParser::SEMI);
}

tree::TerminalNode* JavaParser::ForControlContext::SEMI(size_t i)
{
    return getToken(JavaParser::SEMI, i);
}

JavaParser::ForInitContext* JavaParser::ForControlContext::forInit()
{
    return getRuleContext<JavaParser::ForInitContext>(0);
}

JavaParser::ExpressionContext* JavaParser::ForControlContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

JavaParser::ExpressionListContext* JavaParser::ForControlContext::expressionList()
{
    return getRuleContext<JavaParser::ExpressionListContext>(0);
}

size_t JavaParser::ForControlContext::getRuleIndex() const
{
    return JavaParser::RuleForControl;
}

JavaParser::ForControlContext* JavaParser::forControl()
{
    ForControlContext* _localctx = _tracker.createInstance<ForControlContext>(_ctx, getState());
    enterRule(_localctx, 182, JavaParser::RuleForControl);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1273);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 151, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1261);
                enhancedForControl();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1263);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FINAL - 3)) | (1ULL << (JavaParser::FLOAT - 3)) |
                       (1ULL << (JavaParser::INT - 3)) | (1ULL << (JavaParser::LONG - 3)) |
                       (1ULL << (JavaParser::NEW - 3)) | (1ULL << (JavaParser::SHORT - 3)) |
                       (1ULL << (JavaParser::SUPER - 3)) | (1ULL << (JavaParser::SWITCH - 3)) |
                       (1ULL << (JavaParser::THIS - 3)) | (1ULL << (JavaParser::VOID - 3)) |
                       (1ULL << (JavaParser::MODULE - 3)) | (1ULL << (JavaParser::OPEN - 3)) |
                       (1ULL << (JavaParser::REQUIRES - 3)) | (1ULL << (JavaParser::EXPORTS - 3)) |
                       (1ULL << (JavaParser::OPENS - 3)) | (1ULL << (JavaParser::TO - 3)) |
                       (1ULL << (JavaParser::USES - 3)) | (1ULL << (JavaParser::PROVIDES - 3)) |
                       (1ULL << (JavaParser::WITH - 3)) | (1ULL << (JavaParser::TRANSITIVE - 3)) |
                       (1ULL << (JavaParser::VAR - 3)) | (1ULL << (JavaParser::YIELD - 3)) |
                       (1ULL << (JavaParser::RECORD - 3)) | (1ULL << (JavaParser::SEALED - 3)) |
                       (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    ((((_la - 67) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 67)) &
                      ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                       (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
                       (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
                       (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
                       (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
                       (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
                       (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
                       (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
                       (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
                       (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
                       (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
                    setState(1262);
                    forInit();
                }
                setState(1265);
                match(JavaParser::SEMI);
                setState(1267);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) |
                       (1ULL << (JavaParser::LONG - 3)) | (1ULL << (JavaParser::NEW - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
                       (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
                       (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    ((((_la - 67) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 67)) &
                      ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                       (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
                       (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
                       (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
                       (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
                       (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
                       (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
                       (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
                       (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
                       (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
                       (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
                    setState(1266);
                    expression(0);
                }
                setState(1269);
                match(JavaParser::SEMI);
                setState(1271);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) |
                       (1ULL << (JavaParser::LONG - 3)) | (1ULL << (JavaParser::NEW - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
                       (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
                       (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    ((((_la - 67) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 67)) &
                      ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                       (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
                       (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
                       (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
                       (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
                       (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
                       (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
                       (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
                       (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
                       (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
                       (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
                    setState(1270);
                    antlrcpp::downCast<ForControlContext*>(_localctx)->forUpdate = expressionList();
                }
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ForInitContext ------------------------------------------------------------------

JavaParser::ForInitContext::ForInitContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::LocalVariableDeclarationContext* JavaParser::ForInitContext::localVariableDeclaration()
{
    return getRuleContext<JavaParser::LocalVariableDeclarationContext>(0);
}

JavaParser::ExpressionListContext* JavaParser::ForInitContext::expressionList()
{
    return getRuleContext<JavaParser::ExpressionListContext>(0);
}

size_t JavaParser::ForInitContext::getRuleIndex() const
{
    return JavaParser::RuleForInit;
}

JavaParser::ForInitContext* JavaParser::forInit()
{
    ForInitContext* _localctx = _tracker.createInstance<ForInitContext>(_ctx, getState());
    enterRule(_localctx, 184, JavaParser::RuleForInit);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1277);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 152, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1275);
                localVariableDeclaration();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1276);
                expressionList();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- EnhancedForControlContext ------------------------------------------------------------------

JavaParser::EnhancedForControlContext::EnhancedForControlContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::VariableDeclaratorIdContext* JavaParser::EnhancedForControlContext::variableDeclaratorId()
{
    return getRuleContext<JavaParser::VariableDeclaratorIdContext>(0);
}

tree::TerminalNode* JavaParser::EnhancedForControlContext::COLON()
{
    return getToken(JavaParser::COLON, 0);
}

JavaParser::ExpressionContext* JavaParser::EnhancedForControlContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

JavaParser::TypeTypeContext* JavaParser::EnhancedForControlContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

tree::TerminalNode* JavaParser::EnhancedForControlContext::VAR()
{
    return getToken(JavaParser::VAR, 0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::EnhancedForControlContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::EnhancedForControlContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

size_t JavaParser::EnhancedForControlContext::getRuleIndex() const
{
    return JavaParser::RuleEnhancedForControl;
}

JavaParser::EnhancedForControlContext* JavaParser::enhancedForControl()
{
    EnhancedForControlContext* _localctx = _tracker.createInstance<EnhancedForControlContext>(_ctx, getState());
    enterRule(_localctx, 186, JavaParser::RuleEnhancedForControl);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1282);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 153, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1279);
                variableModifier();
            }
            setState(1284);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 153, _ctx);
        }
        setState(1287);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 154, _ctx)) {
            case 1: {
                setState(1285);
                typeType();
                break;
            }

            case 2: {
                setState(1286);
                match(JavaParser::VAR);
                break;
            }

            default:
                break;
        }
        setState(1289);
        variableDeclaratorId();
        setState(1290);
        match(JavaParser::COLON);
        setState(1291);
        expression(0);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ParExpressionContext ------------------------------------------------------------------

JavaParser::ParExpressionContext::ParExpressionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ParExpressionContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

JavaParser::ExpressionContext* JavaParser::ParExpressionContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaParser::ParExpressionContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

size_t JavaParser::ParExpressionContext::getRuleIndex() const
{
    return JavaParser::RuleParExpression;
}

JavaParser::ParExpressionContext* JavaParser::parExpression()
{
    ParExpressionContext* _localctx = _tracker.createInstance<ParExpressionContext>(_ctx, getState());
    enterRule(_localctx, 188, JavaParser::RuleParExpression);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1293);
        match(JavaParser::LPAREN);
        setState(1294);
        expression(0);
        setState(1295);
        match(JavaParser::RPAREN);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExpressionListContext ------------------------------------------------------------------

JavaParser::ExpressionListContext::ExpressionListContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::ExpressionContext*> JavaParser::ExpressionListContext::expression()
{
    return getRuleContexts<JavaParser::ExpressionContext>();
}

JavaParser::ExpressionContext* JavaParser::ExpressionListContext::expression(size_t i)
{
    return getRuleContext<JavaParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ExpressionListContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::ExpressionListContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::ExpressionListContext::getRuleIndex() const
{
    return JavaParser::RuleExpressionList;
}

JavaParser::ExpressionListContext* JavaParser::expressionList()
{
    ExpressionListContext* _localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
    enterRule(_localctx, 190, JavaParser::RuleExpressionList);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1297);
        expression(0);
        setState(1302);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(1298);
            match(JavaParser::COMMA);
            setState(1299);
            expression(0);
            setState(1304);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- MethodCallContext ------------------------------------------------------------------

JavaParser::MethodCallContext::MethodCallContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::MethodCallContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

tree::TerminalNode* JavaParser::MethodCallContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

tree::TerminalNode* JavaParser::MethodCallContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::ExpressionListContext* JavaParser::MethodCallContext::expressionList()
{
    return getRuleContext<JavaParser::ExpressionListContext>(0);
}

tree::TerminalNode* JavaParser::MethodCallContext::THIS()
{
    return getToken(JavaParser::THIS, 0);
}

tree::TerminalNode* JavaParser::MethodCallContext::SUPER()
{
    return getToken(JavaParser::SUPER, 0);
}

size_t JavaParser::MethodCallContext::getRuleIndex() const
{
    return JavaParser::RuleMethodCall;
}

JavaParser::MethodCallContext* JavaParser::methodCall()
{
    MethodCallContext* _localctx = _tracker.createInstance<MethodCallContext>(_ctx, getState());
    enterRule(_localctx, 192, JavaParser::RuleMethodCall);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1324);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(1305);
                identifier();
                setState(1306);
                match(JavaParser::LPAREN);
                setState(1308);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) |
                       (1ULL << (JavaParser::LONG - 3)) | (1ULL << (JavaParser::NEW - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
                       (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
                       (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    ((((_la - 67) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 67)) &
                      ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                       (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
                       (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
                       (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
                       (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
                       (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
                       (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
                       (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
                       (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
                       (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
                       (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
                    setState(1307);
                    expressionList();
                }
                setState(1310);
                match(JavaParser::RPAREN);
                break;
            }

            case JavaParser::THIS: {
                enterOuterAlt(_localctx, 2);
                setState(1312);
                match(JavaParser::THIS);
                setState(1313);
                match(JavaParser::LPAREN);
                setState(1315);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) |
                       (1ULL << (JavaParser::LONG - 3)) | (1ULL << (JavaParser::NEW - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
                       (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
                       (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    ((((_la - 67) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 67)) &
                      ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                       (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
                       (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
                       (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
                       (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
                       (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
                       (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
                       (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
                       (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
                       (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
                       (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
                    setState(1314);
                    expressionList();
                }
                setState(1317);
                match(JavaParser::RPAREN);
                break;
            }

            case JavaParser::SUPER: {
                enterOuterAlt(_localctx, 3);
                setState(1318);
                match(JavaParser::SUPER);
                setState(1319);
                match(JavaParser::LPAREN);
                setState(1321);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) |
                       (1ULL << (JavaParser::LONG - 3)) | (1ULL << (JavaParser::NEW - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
                       (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
                       (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    ((((_la - 67) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 67)) &
                      ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
                       (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
                       (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
                       (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
                       (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
                       (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
                       (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
                       (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
                       (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) |
                       (1ULL << (JavaParser::SUB - 67)) | (1ULL << (JavaParser::AT - 67)) |
                       (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
                    setState(1320);
                    expressionList();
                }
                setState(1323);
                match(JavaParser::RPAREN);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

JavaParser::ExpressionContext::ExpressionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::PrimaryContext* JavaParser::ExpressionContext::primary()
{
    return getRuleContext<JavaParser::PrimaryContext>(0);
}

JavaParser::MethodCallContext* JavaParser::ExpressionContext::methodCall()
{
    return getRuleContext<JavaParser::MethodCallContext>(0);
}

tree::TerminalNode* JavaParser::ExpressionContext::NEW()
{
    return getToken(JavaParser::NEW, 0);
}

JavaParser::CreatorContext* JavaParser::ExpressionContext::creator()
{
    return getRuleContext<JavaParser::CreatorContext>(0);
}

tree::TerminalNode* JavaParser::ExpressionContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

std::vector<JavaParser::TypeTypeContext*> JavaParser::ExpressionContext::typeType()
{
    return getRuleContexts<JavaParser::TypeTypeContext>();
}

JavaParser::TypeTypeContext* JavaParser::ExpressionContext::typeType(size_t i)
{
    return getRuleContext<JavaParser::TypeTypeContext>(i);
}

tree::TerminalNode* JavaParser::ExpressionContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

std::vector<JavaParser::ExpressionContext*> JavaParser::ExpressionContext::expression()
{
    return getRuleContexts<JavaParser::ExpressionContext>();
}

JavaParser::ExpressionContext* JavaParser::ExpressionContext::expression(size_t i)
{
    return getRuleContext<JavaParser::ExpressionContext>(i);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::ExpressionContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::ExpressionContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::ExpressionContext::BITAND()
{
    return getTokens(JavaParser::BITAND);
}

tree::TerminalNode* JavaParser::ExpressionContext::BITAND(size_t i)
{
    return getToken(JavaParser::BITAND, i);
}

tree::TerminalNode* JavaParser::ExpressionContext::ADD()
{
    return getToken(JavaParser::ADD, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::SUB()
{
    return getToken(JavaParser::SUB, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::INC()
{
    return getToken(JavaParser::INC, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::DEC()
{
    return getToken(JavaParser::DEC, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::TILDE()
{
    return getToken(JavaParser::TILDE, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::BANG()
{
    return getToken(JavaParser::BANG, 0);
}

JavaParser::LambdaExpressionContext* JavaParser::ExpressionContext::lambdaExpression()
{
    return getRuleContext<JavaParser::LambdaExpressionContext>(0);
}

JavaParser::SwitchExpressionContext* JavaParser::ExpressionContext::switchExpression()
{
    return getRuleContext<JavaParser::SwitchExpressionContext>(0);
}

tree::TerminalNode* JavaParser::ExpressionContext::COLONCOLON()
{
    return getToken(JavaParser::COLONCOLON, 0);
}

JavaParser::IdentifierContext* JavaParser::ExpressionContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::TypeArgumentsContext* JavaParser::ExpressionContext::typeArguments()
{
    return getRuleContext<JavaParser::TypeArgumentsContext>(0);
}

JavaParser::ClassTypeContext* JavaParser::ExpressionContext::classType()
{
    return getRuleContext<JavaParser::ClassTypeContext>(0);
}

tree::TerminalNode* JavaParser::ExpressionContext::MUL()
{
    return getToken(JavaParser::MUL, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::DIV()
{
    return getToken(JavaParser::DIV, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::MOD()
{
    return getToken(JavaParser::MOD, 0);
}

std::vector<tree::TerminalNode*> JavaParser::ExpressionContext::LT()
{
    return getTokens(JavaParser::LT);
}

tree::TerminalNode* JavaParser::ExpressionContext::LT(size_t i)
{
    return getToken(JavaParser::LT, i);
}

std::vector<tree::TerminalNode*> JavaParser::ExpressionContext::GT()
{
    return getTokens(JavaParser::GT);
}

tree::TerminalNode* JavaParser::ExpressionContext::GT(size_t i)
{
    return getToken(JavaParser::GT, i);
}

tree::TerminalNode* JavaParser::ExpressionContext::LE()
{
    return getToken(JavaParser::LE, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::GE()
{
    return getToken(JavaParser::GE, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::EQUAL()
{
    return getToken(JavaParser::EQUAL, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::NOTEQUAL()
{
    return getToken(JavaParser::NOTEQUAL, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::CARET()
{
    return getToken(JavaParser::CARET, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::BITOR()
{
    return getToken(JavaParser::BITOR, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::AND()
{
    return getToken(JavaParser::AND, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::OR()
{
    return getToken(JavaParser::OR, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::COLON()
{
    return getToken(JavaParser::COLON, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::QUESTION()
{
    return getToken(JavaParser::QUESTION, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::ASSIGN()
{
    return getToken(JavaParser::ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::ADD_ASSIGN()
{
    return getToken(JavaParser::ADD_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::SUB_ASSIGN()
{
    return getToken(JavaParser::SUB_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::MUL_ASSIGN()
{
    return getToken(JavaParser::MUL_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::DIV_ASSIGN()
{
    return getToken(JavaParser::DIV_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::AND_ASSIGN()
{
    return getToken(JavaParser::AND_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::OR_ASSIGN()
{
    return getToken(JavaParser::OR_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::XOR_ASSIGN()
{
    return getToken(JavaParser::XOR_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::RSHIFT_ASSIGN()
{
    return getToken(JavaParser::RSHIFT_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::URSHIFT_ASSIGN()
{
    return getToken(JavaParser::URSHIFT_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::LSHIFT_ASSIGN()
{
    return getToken(JavaParser::LSHIFT_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::MOD_ASSIGN()
{
    return getToken(JavaParser::MOD_ASSIGN, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::DOT()
{
    return getToken(JavaParser::DOT, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::THIS()
{
    return getToken(JavaParser::THIS, 0);
}

JavaParser::InnerCreatorContext* JavaParser::ExpressionContext::innerCreator()
{
    return getRuleContext<JavaParser::InnerCreatorContext>(0);
}

tree::TerminalNode* JavaParser::ExpressionContext::SUPER()
{
    return getToken(JavaParser::SUPER, 0);
}

JavaParser::SuperSuffixContext* JavaParser::ExpressionContext::superSuffix()
{
    return getRuleContext<JavaParser::SuperSuffixContext>(0);
}

JavaParser::ExplicitGenericInvocationContext* JavaParser::ExpressionContext::explicitGenericInvocation()
{
    return getRuleContext<JavaParser::ExplicitGenericInvocationContext>(0);
}

JavaParser::NonWildcardTypeArgumentsContext* JavaParser::ExpressionContext::nonWildcardTypeArguments()
{
    return getRuleContext<JavaParser::NonWildcardTypeArgumentsContext>(0);
}

tree::TerminalNode* JavaParser::ExpressionContext::LBRACK()
{
    return getToken(JavaParser::LBRACK, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::RBRACK()
{
    return getToken(JavaParser::RBRACK, 0);
}

tree::TerminalNode* JavaParser::ExpressionContext::INSTANCEOF()
{
    return getToken(JavaParser::INSTANCEOF, 0);
}

JavaParser::PatternContext* JavaParser::ExpressionContext::pattern()
{
    return getRuleContext<JavaParser::PatternContext>(0);
}

size_t JavaParser::ExpressionContext::getRuleIndex() const
{
    return JavaParser::RuleExpression;
}

JavaParser::ExpressionContext* JavaParser::expression()
{
    return expression(0);
}

JavaParser::ExpressionContext* JavaParser::expression(int precedence)
{
    ParserRuleContext* parentContext = _ctx;
    size_t parentState = getState();
    JavaParser::ExpressionContext* _localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
    JavaParser::ExpressionContext* previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 194;
    enterRecursionRule(_localctx, 194, JavaParser::RuleExpression, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1371);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 165, _ctx)) {
            case 1: {
                setState(1327);
                primary();
                break;
            }

            case 2: {
                setState(1328);
                methodCall();
                break;
            }

            case 3: {
                setState(1329);
                match(JavaParser::NEW);
                setState(1330);
                creator();
                break;
            }

            case 4: {
                setState(1331);
                match(JavaParser::LPAREN);
                setState(1335);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 160, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1332);
                        annotation();
                    }
                    setState(1337);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 160, _ctx);
                }
                setState(1338);
                typeType();
                setState(1343);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == JavaParser::BITAND) {
                    setState(1339);
                    match(JavaParser::BITAND);
                    setState(1340);
                    typeType();
                    setState(1345);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(1346);
                match(JavaParser::RPAREN);
                setState(1347);
                expression(22);
                break;
            }

            case 5: {
                setState(1349);
                antlrcpp::downCast<ExpressionContext*>(_localctx)->prefix = _input->LT(1);
                _la = _input->LA(1);
                if (!(((((_la - 100) & ~0x3fULL) == 0) &&
                       ((1ULL << (_la - 100)) &
                        ((1ULL << (JavaParser::INC - 100)) | (1ULL << (JavaParser::DEC - 100)) |
                         (1ULL << (JavaParser::ADD - 100)) | (1ULL << (JavaParser::SUB - 100)))) != 0))) {
                    antlrcpp::downCast<ExpressionContext*>(_localctx)->prefix = _errHandler->recoverInline(this);
                } else {
                    _errHandler->reportMatch(this);
                    consume();
                }
                setState(1350);
                expression(20);
                break;
            }

            case 6: {
                setState(1351);
                antlrcpp::downCast<ExpressionContext*>(_localctx)->prefix = _input->LT(1);
                _la = _input->LA(1);
                if (!(_la == JavaParser::BANG

                      || _la == JavaParser::TILDE)) {
                    antlrcpp::downCast<ExpressionContext*>(_localctx)->prefix = _errHandler->recoverInline(this);
                } else {
                    _errHandler->reportMatch(this);
                    consume();
                }
                setState(1352);
                expression(19);
                break;
            }

            case 7: {
                setState(1353);
                lambdaExpression();
                break;
            }

            case 8: {
                setState(1354);
                switchExpression();
                break;
            }

            case 9: {
                setState(1355);
                typeType();
                setState(1356);
                match(JavaParser::COLONCOLON);
                setState(1362);
                _errHandler->sync(this);
                switch (_input->LA(1)) {
                    case JavaParser::MODULE:
                    case JavaParser::OPEN:
                    case JavaParser::REQUIRES:
                    case JavaParser::EXPORTS:
                    case JavaParser::OPENS:
                    case JavaParser::TO:
                    case JavaParser::USES:
                    case JavaParser::PROVIDES:
                    case JavaParser::WITH:
                    case JavaParser::TRANSITIVE:
                    case JavaParser::VAR:
                    case JavaParser::YIELD:
                    case JavaParser::RECORD:
                    case JavaParser::SEALED:
                    case JavaParser::PERMITS:
                    case JavaParser::LT:
                    case JavaParser::IDENTIFIER: {
                        setState(1358);
                        _errHandler->sync(this);

                        _la = _input->LA(1);
                        if (_la == JavaParser::LT) {
                            setState(1357);
                            typeArguments();
                        }
                        setState(1360);
                        identifier();
                        break;
                    }

                    case JavaParser::NEW: {
                        setState(1361);
                        match(JavaParser::NEW);
                        break;
                    }

                    default:
                        throw NoViableAltException(this);
                }
                break;
            }

            case 10: {
                setState(1364);
                classType();
                setState(1365);
                match(JavaParser::COLONCOLON);
                setState(1367);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::LT) {
                    setState(1366);
                    typeArguments();
                }
                setState(1369);
                match(JavaParser::NEW);
                break;
            }

            default:
                break;
        }
        _ctx->stop = _input->LT(-1);
        setState(1456);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 172, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty())
                    triggerExitRuleEvent();
                previousContext = _localctx;
                setState(1454);
                _errHandler->sync(this);
                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 171, _ctx)) {
                    case 1: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1373);

                        if (!(precpred(_ctx, 18)))
                            throw FailedPredicateException(this, "precpred(_ctx, 18)");
                        setState(1374);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _input->LT(1);
                        _la = _input->LA(1);
                        if (!(((((_la - 104) & ~0x3fULL) == 0) &&
                               ((1ULL << (_la - 104)) &
                                ((1ULL << (JavaParser::MUL - 104)) | (1ULL << (JavaParser::DIV - 104)) |
                                 (1ULL << (JavaParser::MOD - 104)))) != 0))) {
                            antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _errHandler->recoverInline(this);
                        } else {
                            _errHandler->reportMatch(this);
                            consume();
                        }
                        setState(1375);
                        expression(19);
                        break;
                    }

                    case 2: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1376);

                        if (!(precpred(_ctx, 17)))
                            throw FailedPredicateException(this, "precpred(_ctx, 17)");
                        setState(1377);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _input->LT(1);
                        _la = _input->LA(1);
                        if (!(_la == JavaParser::ADD

                              || _la == JavaParser::SUB)) {
                            antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _errHandler->recoverInline(this);
                        } else {
                            _errHandler->reportMatch(this);
                            consume();
                        }
                        setState(1378);
                        expression(18);
                        break;
                    }

                    case 3: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1379);

                        if (!(precpred(_ctx, 16)))
                            throw FailedPredicateException(this, "precpred(_ctx, 16)");
                        setState(1387);
                        _errHandler->sync(this);
                        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 166, _ctx)) {
                            case 1: {
                                setState(1380);
                                match(JavaParser::LT);
                                setState(1381);
                                match(JavaParser::LT);
                                break;
                            }

                            case 2: {
                                setState(1382);
                                match(JavaParser::GT);
                                setState(1383);
                                match(JavaParser::GT);
                                setState(1384);
                                match(JavaParser::GT);
                                break;
                            }

                            case 3: {
                                setState(1385);
                                match(JavaParser::GT);
                                setState(1386);
                                match(JavaParser::GT);
                                break;
                            }

                            default:
                                break;
                        }
                        setState(1389);
                        expression(17);
                        break;
                    }

                    case 4: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1390);

                        if (!(precpred(_ctx, 15)))
                            throw FailedPredicateException(this, "precpred(_ctx, 15)");
                        setState(1391);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _input->LT(1);
                        _la = _input->LA(1);
                        if (!(((((_la - 88) & ~0x3fULL) == 0) &&
                               ((1ULL << (_la - 88)) &
                                ((1ULL << (JavaParser::GT - 88)) | (1ULL << (JavaParser::LT - 88)) |
                                 (1ULL << (JavaParser::LE - 88)) | (1ULL << (JavaParser::GE - 88)))) != 0))) {
                            antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _errHandler->recoverInline(this);
                        } else {
                            _errHandler->reportMatch(this);
                            consume();
                        }
                        setState(1392);
                        expression(16);
                        break;
                    }

                    case 5: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1393);

                        if (!(precpred(_ctx, 13)))
                            throw FailedPredicateException(this, "precpred(_ctx, 13)");
                        setState(1394);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _input->LT(1);
                        _la = _input->LA(1);
                        if (!(_la == JavaParser::EQUAL

                              || _la == JavaParser::NOTEQUAL)) {
                            antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _errHandler->recoverInline(this);
                        } else {
                            _errHandler->reportMatch(this);
                            consume();
                        }
                        setState(1395);
                        expression(14);
                        break;
                    }

                    case 6: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1396);

                        if (!(precpred(_ctx, 12)))
                            throw FailedPredicateException(this, "precpred(_ctx, 12)");
                        setState(1397);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::BITAND);
                        setState(1398);
                        expression(13);
                        break;
                    }

                    case 7: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1399);

                        if (!(precpred(_ctx, 11)))
                            throw FailedPredicateException(this, "precpred(_ctx, 11)");
                        setState(1400);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::CARET);
                        setState(1401);
                        expression(12);
                        break;
                    }

                    case 8: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1402);

                        if (!(precpred(_ctx, 10)))
                            throw FailedPredicateException(this, "precpred(_ctx, 10)");
                        setState(1403);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::BITOR);
                        setState(1404);
                        expression(11);
                        break;
                    }

                    case 9: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1405);

                        if (!(precpred(_ctx, 9)))
                            throw FailedPredicateException(this, "precpred(_ctx, 9)");
                        setState(1406);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::AND);
                        setState(1407);
                        expression(10);
                        break;
                    }

                    case 10: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1408);

                        if (!(precpred(_ctx, 8)))
                            throw FailedPredicateException(this, "precpred(_ctx, 8)");
                        setState(1409);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::OR);
                        setState(1410);
                        expression(9);
                        break;
                    }

                    case 11: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1411);

                        if (!(precpred(_ctx, 7)))
                            throw FailedPredicateException(this, "precpred(_ctx, 7)");
                        setState(1412);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::QUESTION);
                        setState(1413);
                        expression(0);
                        setState(1414);
                        match(JavaParser::COLON);
                        setState(1415);
                        expression(7);
                        break;
                    }

                    case 12: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1417);

                        if (!(precpred(_ctx, 6)))
                            throw FailedPredicateException(this, "precpred(_ctx, 6)");
                        setState(1418);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _input->LT(1);
                        _la = _input->LA(1);
                        if (!(((((_la - 87) & ~0x3fULL) == 0) &&
                               ((1ULL << (_la - 87)) &
                                ((1ULL << (JavaParser::ASSIGN - 87)) | (1ULL << (JavaParser::ADD_ASSIGN - 87)) |
                                 (1ULL << (JavaParser::SUB_ASSIGN - 87)) | (1ULL << (JavaParser::MUL_ASSIGN - 87)) |
                                 (1ULL << (JavaParser::DIV_ASSIGN - 87)) | (1ULL << (JavaParser::AND_ASSIGN - 87)) |
                                 (1ULL << (JavaParser::OR_ASSIGN - 87)) | (1ULL << (JavaParser::XOR_ASSIGN - 87)) |
                                 (1ULL << (JavaParser::MOD_ASSIGN - 87)) | (1ULL << (JavaParser::LSHIFT_ASSIGN - 87)) |
                                 (1ULL << (JavaParser::RSHIFT_ASSIGN - 87)) |
                                 (1ULL << (JavaParser::URSHIFT_ASSIGN - 87)))) != 0))) {
                            antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = _errHandler->recoverInline(this);
                        } else {
                            _errHandler->reportMatch(this);
                            consume();
                        }
                        setState(1419);
                        expression(6);
                        break;
                    }

                    case 13: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1420);

                        if (!(precpred(_ctx, 26)))
                            throw FailedPredicateException(this, "precpred(_ctx, 26)");
                        setState(1421);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::DOT);
                        setState(1433);
                        _errHandler->sync(this);
                        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 168, _ctx)) {
                            case 1: {
                                setState(1422);
                                identifier();
                                break;
                            }

                            case 2: {
                                setState(1423);
                                methodCall();
                                break;
                            }

                            case 3: {
                                setState(1424);
                                match(JavaParser::THIS);
                                break;
                            }

                            case 4: {
                                setState(1425);
                                match(JavaParser::NEW);
                                setState(1427);
                                _errHandler->sync(this);

                                _la = _input->LA(1);
                                if (_la == JavaParser::LT) {
                                    setState(1426);
                                    nonWildcardTypeArguments();
                                }
                                setState(1429);
                                innerCreator();
                                break;
                            }

                            case 5: {
                                setState(1430);
                                match(JavaParser::SUPER);
                                setState(1431);
                                superSuffix();
                                break;
                            }

                            case 6: {
                                setState(1432);
                                explicitGenericInvocation();
                                break;
                            }

                            default:
                                break;
                        }
                        break;
                    }

                    case 14: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1435);

                        if (!(precpred(_ctx, 25)))
                            throw FailedPredicateException(this, "precpred(_ctx, 25)");
                        setState(1436);
                        match(JavaParser::LBRACK);
                        setState(1437);
                        expression(0);
                        setState(1438);
                        match(JavaParser::RBRACK);
                        break;
                    }

                    case 15: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1440);

                        if (!(precpred(_ctx, 21)))
                            throw FailedPredicateException(this, "precpred(_ctx, 21)");
                        setState(1441);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->postfix = _input->LT(1);
                        _la = _input->LA(1);
                        if (!(_la == JavaParser::INC

                              || _la == JavaParser::DEC)) {
                            antlrcpp::downCast<ExpressionContext*>(_localctx)->postfix =
                                _errHandler->recoverInline(this);
                        } else {
                            _errHandler->reportMatch(this);
                            consume();
                        }
                        break;
                    }

                    case 16: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1442);

                        if (!(precpred(_ctx, 14)))
                            throw FailedPredicateException(this, "precpred(_ctx, 14)");
                        setState(1443);
                        antlrcpp::downCast<ExpressionContext*>(_localctx)->bop = match(JavaParser::INSTANCEOF);
                        setState(1446);
                        _errHandler->sync(this);
                        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 169, _ctx)) {
                            case 1: {
                                setState(1444);
                                typeType();
                                break;
                            }

                            case 2: {
                                setState(1445);
                                pattern();
                                break;
                            }

                            default:
                                break;
                        }
                        break;
                    }

                    case 17: {
                        _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
                        pushNewRecursionContext(_localctx, startState, RuleExpression);
                        setState(1448);

                        if (!(precpred(_ctx, 3)))
                            throw FailedPredicateException(this, "precpred(_ctx, 3)");
                        setState(1449);
                        match(JavaParser::COLONCOLON);
                        setState(1451);
                        _errHandler->sync(this);

                        _la = _input->LA(1);
                        if (_la == JavaParser::LT) {
                            setState(1450);
                            typeArguments();
                        }
                        setState(1453);
                        identifier();
                        break;
                    }

                    default:
                        break;
                }
            }
            setState(1458);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 172, _ctx);
        }
    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- PatternContext ------------------------------------------------------------------

JavaParser::PatternContext::PatternContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::TypeTypeContext* JavaParser::PatternContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::IdentifierContext* JavaParser::PatternContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::PatternContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::PatternContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::PatternContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::PatternContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

size_t JavaParser::PatternContext::getRuleIndex() const
{
    return JavaParser::RulePattern;
}

JavaParser::PatternContext* JavaParser::pattern()
{
    PatternContext* _localctx = _tracker.createInstance<PatternContext>(_ctx, getState());
    enterRule(_localctx, 196, JavaParser::RulePattern);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1462);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 173, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1459);
                variableModifier();
            }
            setState(1464);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 173, _ctx);
        }
        setState(1465);
        typeType();
        setState(1469);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 174, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1466);
                annotation();
            }
            setState(1471);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 174, _ctx);
        }
        setState(1472);
        identifier();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LambdaExpressionContext ------------------------------------------------------------------

JavaParser::LambdaExpressionContext::LambdaExpressionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::LambdaParametersContext* JavaParser::LambdaExpressionContext::lambdaParameters()
{
    return getRuleContext<JavaParser::LambdaParametersContext>(0);
}

tree::TerminalNode* JavaParser::LambdaExpressionContext::ARROW()
{
    return getToken(JavaParser::ARROW, 0);
}

JavaParser::LambdaBodyContext* JavaParser::LambdaExpressionContext::lambdaBody()
{
    return getRuleContext<JavaParser::LambdaBodyContext>(0);
}

size_t JavaParser::LambdaExpressionContext::getRuleIndex() const
{
    return JavaParser::RuleLambdaExpression;
}

JavaParser::LambdaExpressionContext* JavaParser::lambdaExpression()
{
    LambdaExpressionContext* _localctx = _tracker.createInstance<LambdaExpressionContext>(_ctx, getState());
    enterRule(_localctx, 198, JavaParser::RuleLambdaExpression);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1474);
        lambdaParameters();
        setState(1475);
        match(JavaParser::ARROW);
        setState(1476);
        lambdaBody();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LambdaParametersContext ------------------------------------------------------------------

JavaParser::LambdaParametersContext::LambdaParametersContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::IdentifierContext*> JavaParser::LambdaParametersContext::identifier()
{
    return getRuleContexts<JavaParser::IdentifierContext>();
}

JavaParser::IdentifierContext* JavaParser::LambdaParametersContext::identifier(size_t i)
{
    return getRuleContext<JavaParser::IdentifierContext>(i);
}

tree::TerminalNode* JavaParser::LambdaParametersContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

tree::TerminalNode* JavaParser::LambdaParametersContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::FormalParameterListContext* JavaParser::LambdaParametersContext::formalParameterList()
{
    return getRuleContext<JavaParser::FormalParameterListContext>(0);
}

std::vector<tree::TerminalNode*> JavaParser::LambdaParametersContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::LambdaParametersContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

JavaParser::LambdaLVTIListContext* JavaParser::LambdaParametersContext::lambdaLVTIList()
{
    return getRuleContext<JavaParser::LambdaLVTIListContext>(0);
}

size_t JavaParser::LambdaParametersContext::getRuleIndex() const
{
    return JavaParser::RuleLambdaParameters;
}

JavaParser::LambdaParametersContext* JavaParser::lambdaParameters()
{
    LambdaParametersContext* _localctx = _tracker.createInstance<LambdaParametersContext>(_ctx, getState());
    enterRule(_localctx, 200, JavaParser::RuleLambdaParameters);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1500);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 178, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1478);
                identifier();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1479);
                match(JavaParser::LPAREN);
                setState(1481);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 3) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 3)) &
                      ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
                       (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
                       (1ULL << (JavaParser::FINAL - 3)) | (1ULL << (JavaParser::FLOAT - 3)) |
                       (1ULL << (JavaParser::INT - 3)) | (1ULL << (JavaParser::LONG - 3)) |
                       (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
                       (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
                       (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
                       (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
                       (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
                       (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
                       (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
                       (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
                    _la == JavaParser::AT

                    || _la == JavaParser::IDENTIFIER) {
                    setState(1480);
                    formalParameterList();
                }
                setState(1483);
                match(JavaParser::RPAREN);
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(1484);
                match(JavaParser::LPAREN);
                setState(1485);
                identifier();
                setState(1490);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == JavaParser::COMMA) {
                    setState(1486);
                    match(JavaParser::COMMA);
                    setState(1487);
                    identifier();
                    setState(1492);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(1493);
                match(JavaParser::RPAREN);
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(1495);
                match(JavaParser::LPAREN);
                setState(1497);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (((((_la - 18) & ~0x3fULL) == 0) &&
                     ((1ULL << (_la - 18)) &
                      ((1ULL << (JavaParser::FINAL - 18)) | (1ULL << (JavaParser::MODULE - 18)) |
                       (1ULL << (JavaParser::OPEN - 18)) | (1ULL << (JavaParser::REQUIRES - 18)) |
                       (1ULL << (JavaParser::EXPORTS - 18)) | (1ULL << (JavaParser::OPENS - 18)) |
                       (1ULL << (JavaParser::TO - 18)) | (1ULL << (JavaParser::USES - 18)) |
                       (1ULL << (JavaParser::PROVIDES - 18)) | (1ULL << (JavaParser::WITH - 18)) |
                       (1ULL << (JavaParser::TRANSITIVE - 18)) | (1ULL << (JavaParser::VAR - 18)) |
                       (1ULL << (JavaParser::YIELD - 18)) | (1ULL << (JavaParser::RECORD - 18)) |
                       (1ULL << (JavaParser::SEALED - 18)) | (1ULL << (JavaParser::PERMITS - 18)))) != 0) ||
                    _la == JavaParser::AT

                    || _la == JavaParser::IDENTIFIER) {
                    setState(1496);
                    lambdaLVTIList();
                }
                setState(1499);
                match(JavaParser::RPAREN);
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LambdaBodyContext ------------------------------------------------------------------

JavaParser::LambdaBodyContext::LambdaBodyContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ExpressionContext* JavaParser::LambdaBodyContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

JavaParser::BlockContext* JavaParser::LambdaBodyContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

size_t JavaParser::LambdaBodyContext::getRuleIndex() const
{
    return JavaParser::RuleLambdaBody;
}

JavaParser::LambdaBodyContext* JavaParser::lambdaBody()
{
    LambdaBodyContext* _localctx = _tracker.createInstance<LambdaBodyContext>(_ctx, getState());
    enterRule(_localctx, 202, JavaParser::RuleLambdaBody);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1504);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::NEW:
            case JavaParser::SHORT:
            case JavaParser::SUPER:
            case JavaParser::SWITCH:
            case JavaParser::THIS:
            case JavaParser::VOID:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::DECIMAL_LITERAL:
            case JavaParser::HEX_LITERAL:
            case JavaParser::OCT_LITERAL:
            case JavaParser::BINARY_LITERAL:
            case JavaParser::FLOAT_LITERAL:
            case JavaParser::HEX_FLOAT_LITERAL:
            case JavaParser::BOOL_LITERAL:
            case JavaParser::CHAR_LITERAL:
            case JavaParser::STRING_LITERAL:
            case JavaParser::TEXT_BLOCK:
            case JavaParser::NULL_LITERAL:
            case JavaParser::LPAREN:
            case JavaParser::LT:
            case JavaParser::BANG:
            case JavaParser::TILDE:
            case JavaParser::INC:
            case JavaParser::DEC:
            case JavaParser::ADD:
            case JavaParser::SUB:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(1502);
                expression(0);
                break;
            }

            case JavaParser::LBRACE: {
                enterOuterAlt(_localctx, 2);
                setState(1503);
                block();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- PrimaryContext ------------------------------------------------------------------

JavaParser::PrimaryContext::PrimaryContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::PrimaryContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

JavaParser::ExpressionContext* JavaParser::PrimaryContext::expression()
{
    return getRuleContext<JavaParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaParser::PrimaryContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

tree::TerminalNode* JavaParser::PrimaryContext::THIS()
{
    return getToken(JavaParser::THIS, 0);
}

tree::TerminalNode* JavaParser::PrimaryContext::SUPER()
{
    return getToken(JavaParser::SUPER, 0);
}

JavaParser::LiteralContext* JavaParser::PrimaryContext::literal()
{
    return getRuleContext<JavaParser::LiteralContext>(0);
}

JavaParser::IdentifierContext* JavaParser::PrimaryContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::TypeTypeOrVoidContext* JavaParser::PrimaryContext::typeTypeOrVoid()
{
    return getRuleContext<JavaParser::TypeTypeOrVoidContext>(0);
}

tree::TerminalNode* JavaParser::PrimaryContext::DOT()
{
    return getToken(JavaParser::DOT, 0);
}

tree::TerminalNode* JavaParser::PrimaryContext::CLASS()
{
    return getToken(JavaParser::CLASS, 0);
}

JavaParser::NonWildcardTypeArgumentsContext* JavaParser::PrimaryContext::nonWildcardTypeArguments()
{
    return getRuleContext<JavaParser::NonWildcardTypeArgumentsContext>(0);
}

JavaParser::ExplicitGenericInvocationSuffixContext* JavaParser::PrimaryContext::explicitGenericInvocationSuffix()
{
    return getRuleContext<JavaParser::ExplicitGenericInvocationSuffixContext>(0);
}

JavaParser::ArgumentsContext* JavaParser::PrimaryContext::arguments()
{
    return getRuleContext<JavaParser::ArgumentsContext>(0);
}

size_t JavaParser::PrimaryContext::getRuleIndex() const
{
    return JavaParser::RulePrimary;
}

JavaParser::PrimaryContext* JavaParser::primary()
{
    PrimaryContext* _localctx = _tracker.createInstance<PrimaryContext>(_ctx, getState());
    enterRule(_localctx, 204, JavaParser::RulePrimary);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1524);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 181, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1506);
                match(JavaParser::LPAREN);
                setState(1507);
                expression(0);
                setState(1508);
                match(JavaParser::RPAREN);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1510);
                match(JavaParser::THIS);
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(1511);
                match(JavaParser::SUPER);
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(1512);
                literal();
                break;
            }

            case 5: {
                enterOuterAlt(_localctx, 5);
                setState(1513);
                identifier();
                break;
            }

            case 6: {
                enterOuterAlt(_localctx, 6);
                setState(1514);
                typeTypeOrVoid();
                setState(1515);
                match(JavaParser::DOT);
                setState(1516);
                match(JavaParser::CLASS);
                break;
            }

            case 7: {
                enterOuterAlt(_localctx, 7);
                setState(1518);
                nonWildcardTypeArguments();
                setState(1522);
                _errHandler->sync(this);
                switch (_input->LA(1)) {
                    case JavaParser::SUPER:
                    case JavaParser::MODULE:
                    case JavaParser::OPEN:
                    case JavaParser::REQUIRES:
                    case JavaParser::EXPORTS:
                    case JavaParser::OPENS:
                    case JavaParser::TO:
                    case JavaParser::USES:
                    case JavaParser::PROVIDES:
                    case JavaParser::WITH:
                    case JavaParser::TRANSITIVE:
                    case JavaParser::VAR:
                    case JavaParser::YIELD:
                    case JavaParser::RECORD:
                    case JavaParser::SEALED:
                    case JavaParser::PERMITS:
                    case JavaParser::IDENTIFIER: {
                        setState(1519);
                        explicitGenericInvocationSuffix();
                        break;
                    }

                    case JavaParser::THIS: {
                        setState(1520);
                        match(JavaParser::THIS);
                        setState(1521);
                        arguments();
                        break;
                    }

                    default:
                        throw NoViableAltException(this);
                }
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- SwitchExpressionContext ------------------------------------------------------------------

JavaParser::SwitchExpressionContext::SwitchExpressionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::SwitchExpressionContext::SWITCH()
{
    return getToken(JavaParser::SWITCH, 0);
}

JavaParser::ParExpressionContext* JavaParser::SwitchExpressionContext::parExpression()
{
    return getRuleContext<JavaParser::ParExpressionContext>(0);
}

tree::TerminalNode* JavaParser::SwitchExpressionContext::LBRACE()
{
    return getToken(JavaParser::LBRACE, 0);
}

tree::TerminalNode* JavaParser::SwitchExpressionContext::RBRACE()
{
    return getToken(JavaParser::RBRACE, 0);
}

std::vector<JavaParser::SwitchLabeledRuleContext*> JavaParser::SwitchExpressionContext::switchLabeledRule()
{
    return getRuleContexts<JavaParser::SwitchLabeledRuleContext>();
}

JavaParser::SwitchLabeledRuleContext* JavaParser::SwitchExpressionContext::switchLabeledRule(size_t i)
{
    return getRuleContext<JavaParser::SwitchLabeledRuleContext>(i);
}

size_t JavaParser::SwitchExpressionContext::getRuleIndex() const
{
    return JavaParser::RuleSwitchExpression;
}

JavaParser::SwitchExpressionContext* JavaParser::switchExpression()
{
    SwitchExpressionContext* _localctx = _tracker.createInstance<SwitchExpressionContext>(_ctx, getState());
    enterRule(_localctx, 206, JavaParser::RuleSwitchExpression);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1526);
        match(JavaParser::SWITCH);
        setState(1527);
        parExpression();
        setState(1528);
        match(JavaParser::LBRACE);
        setState(1532);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::CASE

               || _la == JavaParser::DEFAULT) {
            setState(1529);
            switchLabeledRule();
            setState(1534);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(1535);
        match(JavaParser::RBRACE);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- SwitchLabeledRuleContext ------------------------------------------------------------------

JavaParser::SwitchLabeledRuleContext::SwitchLabeledRuleContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::SwitchLabeledRuleContext::CASE()
{
    return getToken(JavaParser::CASE, 0);
}

JavaParser::SwitchRuleOutcomeContext* JavaParser::SwitchLabeledRuleContext::switchRuleOutcome()
{
    return getRuleContext<JavaParser::SwitchRuleOutcomeContext>(0);
}

tree::TerminalNode* JavaParser::SwitchLabeledRuleContext::ARROW()
{
    return getToken(JavaParser::ARROW, 0);
}

tree::TerminalNode* JavaParser::SwitchLabeledRuleContext::COLON()
{
    return getToken(JavaParser::COLON, 0);
}

JavaParser::ExpressionListContext* JavaParser::SwitchLabeledRuleContext::expressionList()
{
    return getRuleContext<JavaParser::ExpressionListContext>(0);
}

tree::TerminalNode* JavaParser::SwitchLabeledRuleContext::NULL_LITERAL()
{
    return getToken(JavaParser::NULL_LITERAL, 0);
}

JavaParser::GuardedPatternContext* JavaParser::SwitchLabeledRuleContext::guardedPattern()
{
    return getRuleContext<JavaParser::GuardedPatternContext>(0);
}

tree::TerminalNode* JavaParser::SwitchLabeledRuleContext::DEFAULT()
{
    return getToken(JavaParser::DEFAULT, 0);
}

size_t JavaParser::SwitchLabeledRuleContext::getRuleIndex() const
{
    return JavaParser::RuleSwitchLabeledRule;
}

JavaParser::SwitchLabeledRuleContext* JavaParser::switchLabeledRule()
{
    SwitchLabeledRuleContext* _localctx = _tracker.createInstance<SwitchLabeledRuleContext>(_ctx, getState());
    enterRule(_localctx, 208, JavaParser::RuleSwitchLabeledRule);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1548);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::CASE: {
                enterOuterAlt(_localctx, 1);
                setState(1537);
                match(JavaParser::CASE);
                setState(1541);
                _errHandler->sync(this);
                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 183, _ctx)) {
                    case 1: {
                        setState(1538);
                        expressionList();
                        break;
                    }

                    case 2: {
                        setState(1539);
                        match(JavaParser::NULL_LITERAL);
                        break;
                    }

                    case 3: {
                        setState(1540);
                        guardedPattern(0);
                        break;
                    }

                    default:
                        break;
                }
                setState(1543);
                _la = _input->LA(1);
                if (!(_la == JavaParser::COLON

                      || _la == JavaParser::ARROW)) {
                    _errHandler->recoverInline(this);
                } else {
                    _errHandler->reportMatch(this);
                    consume();
                }
                setState(1544);
                switchRuleOutcome();
                break;
            }

            case JavaParser::DEFAULT: {
                enterOuterAlt(_localctx, 2);
                setState(1545);
                match(JavaParser::DEFAULT);
                setState(1546);
                _la = _input->LA(1);
                if (!(_la == JavaParser::COLON

                      || _la == JavaParser::ARROW)) {
                    _errHandler->recoverInline(this);
                } else {
                    _errHandler->reportMatch(this);
                    consume();
                }
                setState(1547);
                switchRuleOutcome();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- GuardedPatternContext ------------------------------------------------------------------

JavaParser::GuardedPatternContext::GuardedPatternContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::GuardedPatternContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

JavaParser::GuardedPatternContext* JavaParser::GuardedPatternContext::guardedPattern()
{
    return getRuleContext<JavaParser::GuardedPatternContext>(0);
}

tree::TerminalNode* JavaParser::GuardedPatternContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::TypeTypeContext* JavaParser::GuardedPatternContext::typeType()
{
    return getRuleContext<JavaParser::TypeTypeContext>(0);
}

JavaParser::IdentifierContext* JavaParser::GuardedPatternContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

std::vector<JavaParser::VariableModifierContext*> JavaParser::GuardedPatternContext::variableModifier()
{
    return getRuleContexts<JavaParser::VariableModifierContext>();
}

JavaParser::VariableModifierContext* JavaParser::GuardedPatternContext::variableModifier(size_t i)
{
    return getRuleContext<JavaParser::VariableModifierContext>(i);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::GuardedPatternContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::GuardedPatternContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::GuardedPatternContext::AND()
{
    return getTokens(JavaParser::AND);
}

tree::TerminalNode* JavaParser::GuardedPatternContext::AND(size_t i)
{
    return getToken(JavaParser::AND, i);
}

std::vector<JavaParser::ExpressionContext*> JavaParser::GuardedPatternContext::expression()
{
    return getRuleContexts<JavaParser::ExpressionContext>();
}

JavaParser::ExpressionContext* JavaParser::GuardedPatternContext::expression(size_t i)
{
    return getRuleContext<JavaParser::ExpressionContext>(i);
}

size_t JavaParser::GuardedPatternContext::getRuleIndex() const
{
    return JavaParser::RuleGuardedPattern;
}

JavaParser::GuardedPatternContext* JavaParser::guardedPattern()
{
    return guardedPattern(0);
}

JavaParser::GuardedPatternContext* JavaParser::guardedPattern(int precedence)
{
    ParserRuleContext* parentContext = _ctx;
    size_t parentState = getState();
    JavaParser::GuardedPatternContext* _localctx = _tracker.createInstance<GuardedPatternContext>(_ctx, parentState);
    JavaParser::GuardedPatternContext* previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 210;
    enterRecursionRule(_localctx, 210, JavaParser::RuleGuardedPattern, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1576);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::LPAREN: {
                setState(1551);
                match(JavaParser::LPAREN);
                setState(1552);
                guardedPattern(0);
                setState(1553);
                match(JavaParser::RPAREN);
                break;
            }

            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FINAL:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::SHORT:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                setState(1558);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 185, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1555);
                        variableModifier();
                    }
                    setState(1560);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 185, _ctx);
                }
                setState(1561);
                typeType();
                setState(1565);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 186, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1562);
                        annotation();
                    }
                    setState(1567);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 186, _ctx);
                }
                setState(1568);
                identifier();
                setState(1573);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 187, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1569);
                        match(JavaParser::AND);
                        setState(1570);
                        expression(0);
                    }
                    setState(1575);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 187, _ctx);
                }
                break;
            }

            default:
                throw NoViableAltException(this);
        }
        _ctx->stop = _input->LT(-1);
        setState(1583);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 189, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty())
                    triggerExitRuleEvent();
                previousContext = _localctx;
                _localctx = _tracker.createInstance<GuardedPatternContext>(parentContext, parentState);
                pushNewRecursionContext(_localctx, startState, RuleGuardedPattern);
                setState(1578);

                if (!(precpred(_ctx, 1)))
                    throw FailedPredicateException(this, "precpred(_ctx, 1)");
                setState(1579);
                match(JavaParser::AND);
                setState(1580);
                expression(0);
            }
            setState(1585);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 189, _ctx);
        }
    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- SwitchRuleOutcomeContext ------------------------------------------------------------------

JavaParser::SwitchRuleOutcomeContext::SwitchRuleOutcomeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::BlockContext* JavaParser::SwitchRuleOutcomeContext::block()
{
    return getRuleContext<JavaParser::BlockContext>(0);
}

std::vector<JavaParser::BlockStatementContext*> JavaParser::SwitchRuleOutcomeContext::blockStatement()
{
    return getRuleContexts<JavaParser::BlockStatementContext>();
}

JavaParser::BlockStatementContext* JavaParser::SwitchRuleOutcomeContext::blockStatement(size_t i)
{
    return getRuleContext<JavaParser::BlockStatementContext>(i);
}

size_t JavaParser::SwitchRuleOutcomeContext::getRuleIndex() const
{
    return JavaParser::RuleSwitchRuleOutcome;
}

JavaParser::SwitchRuleOutcomeContext* JavaParser::switchRuleOutcome()
{
    SwitchRuleOutcomeContext* _localctx = _tracker.createInstance<SwitchRuleOutcomeContext>(_ctx, getState());
    enterRule(_localctx, 212, JavaParser::RuleSwitchRuleOutcome);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1593);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 191, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1586);
                block();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1590);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (((((_la - 1) & ~0x3fULL) == 0) &&
                        ((1ULL << (_la - 1)) &
                         ((1ULL << (JavaParser::ABSTRACT - 1)) | (1ULL << (JavaParser::ASSERT - 1)) |
                          (1ULL << (JavaParser::BOOLEAN - 1)) | (1ULL << (JavaParser::BREAK - 1)) |
                          (1ULL << (JavaParser::BYTE - 1)) | (1ULL << (JavaParser::CHAR - 1)) |
                          (1ULL << (JavaParser::CLASS - 1)) | (1ULL << (JavaParser::CONTINUE - 1)) |
                          (1ULL << (JavaParser::DO - 1)) | (1ULL << (JavaParser::DOUBLE - 1)) |
                          (1ULL << (JavaParser::FINAL - 1)) | (1ULL << (JavaParser::FLOAT - 1)) |
                          (1ULL << (JavaParser::FOR - 1)) | (1ULL << (JavaParser::IF - 1)) |
                          (1ULL << (JavaParser::INT - 1)) | (1ULL << (JavaParser::INTERFACE - 1)) |
                          (1ULL << (JavaParser::LONG - 1)) | (1ULL << (JavaParser::NEW - 1)) |
                          (1ULL << (JavaParser::PRIVATE - 1)) | (1ULL << (JavaParser::PROTECTED - 1)) |
                          (1ULL << (JavaParser::PUBLIC - 1)) | (1ULL << (JavaParser::RETURN - 1)) |
                          (1ULL << (JavaParser::SHORT - 1)) | (1ULL << (JavaParser::STATIC - 1)) |
                          (1ULL << (JavaParser::STRICTFP - 1)) | (1ULL << (JavaParser::SUPER - 1)) |
                          (1ULL << (JavaParser::SWITCH - 1)) | (1ULL << (JavaParser::SYNCHRONIZED - 1)) |
                          (1ULL << (JavaParser::THIS - 1)) | (1ULL << (JavaParser::THROW - 1)) |
                          (1ULL << (JavaParser::TRY - 1)) | (1ULL << (JavaParser::VOID - 1)) |
                          (1ULL << (JavaParser::WHILE - 1)) | (1ULL << (JavaParser::MODULE - 1)) |
                          (1ULL << (JavaParser::OPEN - 1)) | (1ULL << (JavaParser::REQUIRES - 1)) |
                          (1ULL << (JavaParser::EXPORTS - 1)) | (1ULL << (JavaParser::OPENS - 1)) |
                          (1ULL << (JavaParser::TO - 1)) | (1ULL << (JavaParser::USES - 1)) |
                          (1ULL << (JavaParser::PROVIDES - 1)) | (1ULL << (JavaParser::WITH - 1)) |
                          (1ULL << (JavaParser::TRANSITIVE - 1)) | (1ULL << (JavaParser::VAR - 1)) |
                          (1ULL << (JavaParser::YIELD - 1)) | (1ULL << (JavaParser::RECORD - 1)) |
                          (1ULL << (JavaParser::SEALED - 1)))) != 0) ||
                       ((((_la - 65) & ~0x3fULL) == 0) &&
                        ((1ULL << (_la - 65)) &
                         ((1ULL << (JavaParser::PERMITS - 65)) | (1ULL << (JavaParser::NON_SEALED - 65)) |
                          (1ULL << (JavaParser::DECIMAL_LITERAL - 65)) | (1ULL << (JavaParser::HEX_LITERAL - 65)) |
                          (1ULL << (JavaParser::OCT_LITERAL - 65)) | (1ULL << (JavaParser::BINARY_LITERAL - 65)) |
                          (1ULL << (JavaParser::FLOAT_LITERAL - 65)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 65)) |
                          (1ULL << (JavaParser::BOOL_LITERAL - 65)) | (1ULL << (JavaParser::CHAR_LITERAL - 65)) |
                          (1ULL << (JavaParser::STRING_LITERAL - 65)) | (1ULL << (JavaParser::TEXT_BLOCK - 65)) |
                          (1ULL << (JavaParser::NULL_LITERAL - 65)) | (1ULL << (JavaParser::LPAREN - 65)) |
                          (1ULL << (JavaParser::LBRACE - 65)) | (1ULL << (JavaParser::SEMI - 65)) |
                          (1ULL << (JavaParser::LT - 65)) | (1ULL << (JavaParser::BANG - 65)) |
                          (1ULL << (JavaParser::TILDE - 65)) | (1ULL << (JavaParser::INC - 65)) |
                          (1ULL << (JavaParser::DEC - 65)) | (1ULL << (JavaParser::ADD - 65)) |
                          (1ULL << (JavaParser::SUB - 65)) | (1ULL << (JavaParser::AT - 65)) |
                          (1ULL << (JavaParser::IDENTIFIER - 65)))) != 0)) {
                    setState(1587);
                    blockStatement();
                    setState(1592);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ClassTypeContext ------------------------------------------------------------------

JavaParser::ClassTypeContext::ClassTypeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::ClassTypeContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::ClassOrInterfaceTypeContext* JavaParser::ClassTypeContext::classOrInterfaceType()
{
    return getRuleContext<JavaParser::ClassOrInterfaceTypeContext>(0);
}

tree::TerminalNode* JavaParser::ClassTypeContext::DOT()
{
    return getToken(JavaParser::DOT, 0);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::ClassTypeContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::ClassTypeContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

JavaParser::TypeArgumentsContext* JavaParser::ClassTypeContext::typeArguments()
{
    return getRuleContext<JavaParser::TypeArgumentsContext>(0);
}

size_t JavaParser::ClassTypeContext::getRuleIndex() const
{
    return JavaParser::RuleClassType;
}

JavaParser::ClassTypeContext* JavaParser::classType()
{
    ClassTypeContext* _localctx = _tracker.createInstance<ClassTypeContext>(_ctx, getState());
    enterRule(_localctx, 214, JavaParser::RuleClassType);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1598);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 192, _ctx)) {
            case 1: {
                setState(1595);
                classOrInterfaceType();
                setState(1596);
                match(JavaParser::DOT);
                break;
            }

            default:
                break;
        }
        setState(1603);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 193, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1600);
                annotation();
            }
            setState(1605);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 193, _ctx);
        }
        setState(1606);
        identifier();
        setState(1608);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LT) {
            setState(1607);
            typeArguments();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- CreatorContext ------------------------------------------------------------------

JavaParser::CreatorContext::CreatorContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::NonWildcardTypeArgumentsContext* JavaParser::CreatorContext::nonWildcardTypeArguments()
{
    return getRuleContext<JavaParser::NonWildcardTypeArgumentsContext>(0);
}

JavaParser::CreatedNameContext* JavaParser::CreatorContext::createdName()
{
    return getRuleContext<JavaParser::CreatedNameContext>(0);
}

JavaParser::ClassCreatorRestContext* JavaParser::CreatorContext::classCreatorRest()
{
    return getRuleContext<JavaParser::ClassCreatorRestContext>(0);
}

JavaParser::ArrayCreatorRestContext* JavaParser::CreatorContext::arrayCreatorRest()
{
    return getRuleContext<JavaParser::ArrayCreatorRestContext>(0);
}

size_t JavaParser::CreatorContext::getRuleIndex() const
{
    return JavaParser::RuleCreator;
}

JavaParser::CreatorContext* JavaParser::creator()
{
    CreatorContext* _localctx = _tracker.createInstance<CreatorContext>(_ctx, getState());
    enterRule(_localctx, 216, JavaParser::RuleCreator);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1619);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::LT: {
                enterOuterAlt(_localctx, 1);
                setState(1610);
                nonWildcardTypeArguments();
                setState(1611);
                createdName();
                setState(1612);
                classCreatorRest();
                break;
            }

            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::SHORT:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 2);
                setState(1614);
                createdName();
                setState(1617);
                _errHandler->sync(this);
                switch (_input->LA(1)) {
                    case JavaParser::LBRACK: {
                        setState(1615);
                        arrayCreatorRest();
                        break;
                    }

                    case JavaParser::LPAREN: {
                        setState(1616);
                        classCreatorRest();
                        break;
                    }

                    default:
                        throw NoViableAltException(this);
                }
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- CreatedNameContext ------------------------------------------------------------------

JavaParser::CreatedNameContext::CreatedNameContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::IdentifierContext*> JavaParser::CreatedNameContext::identifier()
{
    return getRuleContexts<JavaParser::IdentifierContext>();
}

JavaParser::IdentifierContext* JavaParser::CreatedNameContext::identifier(size_t i)
{
    return getRuleContext<JavaParser::IdentifierContext>(i);
}

std::vector<JavaParser::TypeArgumentsOrDiamondContext*> JavaParser::CreatedNameContext::typeArgumentsOrDiamond()
{
    return getRuleContexts<JavaParser::TypeArgumentsOrDiamondContext>();
}

JavaParser::TypeArgumentsOrDiamondContext* JavaParser::CreatedNameContext::typeArgumentsOrDiamond(size_t i)
{
    return getRuleContext<JavaParser::TypeArgumentsOrDiamondContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::CreatedNameContext::DOT()
{
    return getTokens(JavaParser::DOT);
}

tree::TerminalNode* JavaParser::CreatedNameContext::DOT(size_t i)
{
    return getToken(JavaParser::DOT, i);
}

JavaParser::PrimitiveTypeContext* JavaParser::CreatedNameContext::primitiveType()
{
    return getRuleContext<JavaParser::PrimitiveTypeContext>(0);
}

size_t JavaParser::CreatedNameContext::getRuleIndex() const
{
    return JavaParser::RuleCreatedName;
}

JavaParser::CreatedNameContext* JavaParser::createdName()
{
    CreatedNameContext* _localctx = _tracker.createInstance<CreatedNameContext>(_ctx, getState());
    enterRule(_localctx, 218, JavaParser::RuleCreatedName);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1636);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 1);
                setState(1621);
                identifier();
                setState(1623);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::LT) {
                    setState(1622);
                    typeArgumentsOrDiamond();
                }
                setState(1632);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == JavaParser::DOT) {
                    setState(1625);
                    match(JavaParser::DOT);
                    setState(1626);
                    identifier();
                    setState(1628);
                    _errHandler->sync(this);

                    _la = _input->LA(1);
                    if (_la == JavaParser::LT) {
                        setState(1627);
                        typeArgumentsOrDiamond();
                    }
                    setState(1634);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                break;
            }

            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::SHORT: {
                enterOuterAlt(_localctx, 2);
                setState(1635);
                primitiveType();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InnerCreatorContext ------------------------------------------------------------------

JavaParser::InnerCreatorContext::InnerCreatorContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::IdentifierContext* JavaParser::InnerCreatorContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::ClassCreatorRestContext* JavaParser::InnerCreatorContext::classCreatorRest()
{
    return getRuleContext<JavaParser::ClassCreatorRestContext>(0);
}

JavaParser::NonWildcardTypeArgumentsOrDiamondContext*
JavaParser::InnerCreatorContext::nonWildcardTypeArgumentsOrDiamond()
{
    return getRuleContext<JavaParser::NonWildcardTypeArgumentsOrDiamondContext>(0);
}

size_t JavaParser::InnerCreatorContext::getRuleIndex() const
{
    return JavaParser::RuleInnerCreator;
}

JavaParser::InnerCreatorContext* JavaParser::innerCreator()
{
    InnerCreatorContext* _localctx = _tracker.createInstance<InnerCreatorContext>(_ctx, getState());
    enterRule(_localctx, 220, JavaParser::RuleInnerCreator);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1638);
        identifier();
        setState(1640);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaParser::LT) {
            setState(1639);
            nonWildcardTypeArgumentsOrDiamond();
        }
        setState(1642);
        classCreatorRest();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ArrayCreatorRestContext ------------------------------------------------------------------

JavaParser::ArrayCreatorRestContext::ArrayCreatorRestContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<tree::TerminalNode*> JavaParser::ArrayCreatorRestContext::LBRACK()
{
    return getTokens(JavaParser::LBRACK);
}

tree::TerminalNode* JavaParser::ArrayCreatorRestContext::LBRACK(size_t i)
{
    return getToken(JavaParser::LBRACK, i);
}

std::vector<tree::TerminalNode*> JavaParser::ArrayCreatorRestContext::RBRACK()
{
    return getTokens(JavaParser::RBRACK);
}

tree::TerminalNode* JavaParser::ArrayCreatorRestContext::RBRACK(size_t i)
{
    return getToken(JavaParser::RBRACK, i);
}

JavaParser::ArrayInitializerContext* JavaParser::ArrayCreatorRestContext::arrayInitializer()
{
    return getRuleContext<JavaParser::ArrayInitializerContext>(0);
}

std::vector<JavaParser::ExpressionContext*> JavaParser::ArrayCreatorRestContext::expression()
{
    return getRuleContexts<JavaParser::ExpressionContext>();
}

JavaParser::ExpressionContext* JavaParser::ArrayCreatorRestContext::expression(size_t i)
{
    return getRuleContext<JavaParser::ExpressionContext>(i);
}

size_t JavaParser::ArrayCreatorRestContext::getRuleIndex() const
{
    return JavaParser::RuleArrayCreatorRest;
}

JavaParser::ArrayCreatorRestContext* JavaParser::arrayCreatorRest()
{
    ArrayCreatorRestContext* _localctx = _tracker.createInstance<ArrayCreatorRestContext>(_ctx, getState());
    enterRule(_localctx, 222, JavaParser::RuleArrayCreatorRest);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1644);
        match(JavaParser::LBRACK);
        setState(1672);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::RBRACK: {
                setState(1645);
                match(JavaParser::RBRACK);
                setState(1650);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == JavaParser::LBRACK) {
                    setState(1646);
                    match(JavaParser::LBRACK);
                    setState(1647);
                    match(JavaParser::RBRACK);
                    setState(1652);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(1653);
                arrayInitializer();
                break;
            }

            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::NEW:
            case JavaParser::SHORT:
            case JavaParser::SUPER:
            case JavaParser::SWITCH:
            case JavaParser::THIS:
            case JavaParser::VOID:
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::DECIMAL_LITERAL:
            case JavaParser::HEX_LITERAL:
            case JavaParser::OCT_LITERAL:
            case JavaParser::BINARY_LITERAL:
            case JavaParser::FLOAT_LITERAL:
            case JavaParser::HEX_FLOAT_LITERAL:
            case JavaParser::BOOL_LITERAL:
            case JavaParser::CHAR_LITERAL:
            case JavaParser::STRING_LITERAL:
            case JavaParser::TEXT_BLOCK:
            case JavaParser::NULL_LITERAL:
            case JavaParser::LPAREN:
            case JavaParser::LT:
            case JavaParser::BANG:
            case JavaParser::TILDE:
            case JavaParser::INC:
            case JavaParser::DEC:
            case JavaParser::ADD:
            case JavaParser::SUB:
            case JavaParser::AT:
            case JavaParser::IDENTIFIER: {
                setState(1654);
                expression(0);
                setState(1655);
                match(JavaParser::RBRACK);
                setState(1662);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 203, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1656);
                        match(JavaParser::LBRACK);
                        setState(1657);
                        expression(0);
                        setState(1658);
                        match(JavaParser::RBRACK);
                    }
                    setState(1664);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 203, _ctx);
                }
                setState(1669);
                _errHandler->sync(this);
                alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx);
                while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
                    if (alt == 1) {
                        setState(1665);
                        match(JavaParser::LBRACK);
                        setState(1666);
                        match(JavaParser::RBRACK);
                    }
                    setState(1671);
                    _errHandler->sync(this);
                    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx);
                }
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ClassCreatorRestContext ------------------------------------------------------------------

JavaParser::ClassCreatorRestContext::ClassCreatorRestContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ArgumentsContext* JavaParser::ClassCreatorRestContext::arguments()
{
    return getRuleContext<JavaParser::ArgumentsContext>(0);
}

JavaParser::ClassBodyContext* JavaParser::ClassCreatorRestContext::classBody()
{
    return getRuleContext<JavaParser::ClassBodyContext>(0);
}

size_t JavaParser::ClassCreatorRestContext::getRuleIndex() const
{
    return JavaParser::RuleClassCreatorRest;
}

JavaParser::ClassCreatorRestContext* JavaParser::classCreatorRest()
{
    ClassCreatorRestContext* _localctx = _tracker.createInstance<ClassCreatorRestContext>(_ctx, getState());
    enterRule(_localctx, 224, JavaParser::RuleClassCreatorRest);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1674);
        arguments();
        setState(1676);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 206, _ctx)) {
            case 1: {
                setState(1675);
                classBody();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExplicitGenericInvocationContext ------------------------------------------------------------------

JavaParser::ExplicitGenericInvocationContext::ExplicitGenericInvocationContext(ParserRuleContext* parent,
                                                                               size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::NonWildcardTypeArgumentsContext* JavaParser::ExplicitGenericInvocationContext::nonWildcardTypeArguments()
{
    return getRuleContext<JavaParser::NonWildcardTypeArgumentsContext>(0);
}

JavaParser::ExplicitGenericInvocationSuffixContext*
JavaParser::ExplicitGenericInvocationContext::explicitGenericInvocationSuffix()
{
    return getRuleContext<JavaParser::ExplicitGenericInvocationSuffixContext>(0);
}

size_t JavaParser::ExplicitGenericInvocationContext::getRuleIndex() const
{
    return JavaParser::RuleExplicitGenericInvocation;
}

JavaParser::ExplicitGenericInvocationContext* JavaParser::explicitGenericInvocation()
{
    ExplicitGenericInvocationContext* _localctx =
        _tracker.createInstance<ExplicitGenericInvocationContext>(_ctx, getState());
    enterRule(_localctx, 226, JavaParser::RuleExplicitGenericInvocation);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1678);
        nonWildcardTypeArguments();
        setState(1679);
        explicitGenericInvocationSuffix();

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeArgumentsOrDiamondContext ------------------------------------------------------------------

JavaParser::TypeArgumentsOrDiamondContext::TypeArgumentsOrDiamondContext(ParserRuleContext* parent,
                                                                         size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::TypeArgumentsOrDiamondContext::LT()
{
    return getToken(JavaParser::LT, 0);
}

tree::TerminalNode* JavaParser::TypeArgumentsOrDiamondContext::GT()
{
    return getToken(JavaParser::GT, 0);
}

JavaParser::TypeArgumentsContext* JavaParser::TypeArgumentsOrDiamondContext::typeArguments()
{
    return getRuleContext<JavaParser::TypeArgumentsContext>(0);
}

size_t JavaParser::TypeArgumentsOrDiamondContext::getRuleIndex() const
{
    return JavaParser::RuleTypeArgumentsOrDiamond;
}

JavaParser::TypeArgumentsOrDiamondContext* JavaParser::typeArgumentsOrDiamond()
{
    TypeArgumentsOrDiamondContext* _localctx = _tracker.createInstance<TypeArgumentsOrDiamondContext>(_ctx, getState());
    enterRule(_localctx, 228, JavaParser::RuleTypeArgumentsOrDiamond);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1684);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1681);
                match(JavaParser::LT);
                setState(1682);
                match(JavaParser::GT);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1683);
                typeArguments();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- NonWildcardTypeArgumentsOrDiamondContext
//------------------------------------------------------------------

JavaParser::NonWildcardTypeArgumentsOrDiamondContext::NonWildcardTypeArgumentsOrDiamondContext(
    ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::NonWildcardTypeArgumentsOrDiamondContext::LT()
{
    return getToken(JavaParser::LT, 0);
}

tree::TerminalNode* JavaParser::NonWildcardTypeArgumentsOrDiamondContext::GT()
{
    return getToken(JavaParser::GT, 0);
}

JavaParser::NonWildcardTypeArgumentsContext*
JavaParser::NonWildcardTypeArgumentsOrDiamondContext::nonWildcardTypeArguments()
{
    return getRuleContext<JavaParser::NonWildcardTypeArgumentsContext>(0);
}

size_t JavaParser::NonWildcardTypeArgumentsOrDiamondContext::getRuleIndex() const
{
    return JavaParser::RuleNonWildcardTypeArgumentsOrDiamond;
}

JavaParser::NonWildcardTypeArgumentsOrDiamondContext* JavaParser::nonWildcardTypeArgumentsOrDiamond()
{
    NonWildcardTypeArgumentsOrDiamondContext* _localctx =
        _tracker.createInstance<NonWildcardTypeArgumentsOrDiamondContext>(_ctx, getState());
    enterRule(_localctx, 230, JavaParser::RuleNonWildcardTypeArgumentsOrDiamond);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1689);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 208, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(1686);
                match(JavaParser::LT);
                setState(1687);
                match(JavaParser::GT);
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(1688);
                nonWildcardTypeArguments();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- NonWildcardTypeArgumentsContext ------------------------------------------------------------------

JavaParser::NonWildcardTypeArgumentsContext::NonWildcardTypeArgumentsContext(ParserRuleContext* parent,
                                                                             size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::NonWildcardTypeArgumentsContext::LT()
{
    return getToken(JavaParser::LT, 0);
}

JavaParser::TypeListContext* JavaParser::NonWildcardTypeArgumentsContext::typeList()
{
    return getRuleContext<JavaParser::TypeListContext>(0);
}

tree::TerminalNode* JavaParser::NonWildcardTypeArgumentsContext::GT()
{
    return getToken(JavaParser::GT, 0);
}

size_t JavaParser::NonWildcardTypeArgumentsContext::getRuleIndex() const
{
    return JavaParser::RuleNonWildcardTypeArguments;
}

JavaParser::NonWildcardTypeArgumentsContext* JavaParser::nonWildcardTypeArguments()
{
    NonWildcardTypeArgumentsContext* _localctx =
        _tracker.createInstance<NonWildcardTypeArgumentsContext>(_ctx, getState());
    enterRule(_localctx, 232, JavaParser::RuleNonWildcardTypeArguments);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1691);
        match(JavaParser::LT);
        setState(1692);
        typeList();
        setState(1693);
        match(JavaParser::GT);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeListContext ------------------------------------------------------------------

JavaParser::TypeListContext::TypeListContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

std::vector<JavaParser::TypeTypeContext*> JavaParser::TypeListContext::typeType()
{
    return getRuleContexts<JavaParser::TypeTypeContext>();
}

JavaParser::TypeTypeContext* JavaParser::TypeListContext::typeType(size_t i)
{
    return getRuleContext<JavaParser::TypeTypeContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::TypeListContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::TypeListContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::TypeListContext::getRuleIndex() const
{
    return JavaParser::RuleTypeList;
}

JavaParser::TypeListContext* JavaParser::typeList()
{
    TypeListContext* _localctx = _tracker.createInstance<TypeListContext>(_ctx, getState());
    enterRule(_localctx, 234, JavaParser::RuleTypeList);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1695);
        typeType();
        setState(1700);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(1696);
            match(JavaParser::COMMA);
            setState(1697);
            typeType();
            setState(1702);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeTypeContext ------------------------------------------------------------------

JavaParser::TypeTypeContext::TypeTypeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ClassOrInterfaceTypeContext* JavaParser::TypeTypeContext::classOrInterfaceType()
{
    return getRuleContext<JavaParser::ClassOrInterfaceTypeContext>(0);
}

JavaParser::PrimitiveTypeContext* JavaParser::TypeTypeContext::primitiveType()
{
    return getRuleContext<JavaParser::PrimitiveTypeContext>(0);
}

std::vector<JavaParser::AnnotationContext*> JavaParser::TypeTypeContext::annotation()
{
    return getRuleContexts<JavaParser::AnnotationContext>();
}

JavaParser::AnnotationContext* JavaParser::TypeTypeContext::annotation(size_t i)
{
    return getRuleContext<JavaParser::AnnotationContext>(i);
}

std::vector<tree::TerminalNode*> JavaParser::TypeTypeContext::LBRACK()
{
    return getTokens(JavaParser::LBRACK);
}

tree::TerminalNode* JavaParser::TypeTypeContext::LBRACK(size_t i)
{
    return getToken(JavaParser::LBRACK, i);
}

std::vector<tree::TerminalNode*> JavaParser::TypeTypeContext::RBRACK()
{
    return getTokens(JavaParser::RBRACK);
}

tree::TerminalNode* JavaParser::TypeTypeContext::RBRACK(size_t i)
{
    return getToken(JavaParser::RBRACK, i);
}

size_t JavaParser::TypeTypeContext::getRuleIndex() const
{
    return JavaParser::RuleTypeType;
}

JavaParser::TypeTypeContext* JavaParser::typeType()
{
    TypeTypeContext* _localctx = _tracker.createInstance<TypeTypeContext>(_ctx, getState());
    enterRule(_localctx, 236, JavaParser::RuleTypeType);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(1706);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 210, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1703);
                annotation();
            }
            setState(1708);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 210, _ctx);
        }
        setState(1711);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::IDENTIFIER: {
                setState(1709);
                classOrInterfaceType();
                break;
            }

            case JavaParser::BOOLEAN:
            case JavaParser::BYTE:
            case JavaParser::CHAR:
            case JavaParser::DOUBLE:
            case JavaParser::FLOAT:
            case JavaParser::INT:
            case JavaParser::LONG:
            case JavaParser::SHORT: {
                setState(1710);
                primitiveType();
                break;
            }

            default:
                throw NoViableAltException(this);
        }
        setState(1723);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(1716);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (((((_la - 51) & ~0x3fULL) == 0) &&
                        ((1ULL << (_la - 51)) &
                         ((1ULL << (JavaParser::MODULE - 51)) | (1ULL << (JavaParser::OPEN - 51)) |
                          (1ULL << (JavaParser::REQUIRES - 51)) | (1ULL << (JavaParser::EXPORTS - 51)) |
                          (1ULL << (JavaParser::OPENS - 51)) | (1ULL << (JavaParser::TO - 51)) |
                          (1ULL << (JavaParser::USES - 51)) | (1ULL << (JavaParser::PROVIDES - 51)) |
                          (1ULL << (JavaParser::WITH - 51)) | (1ULL << (JavaParser::TRANSITIVE - 51)) |
                          (1ULL << (JavaParser::VAR - 51)) | (1ULL << (JavaParser::YIELD - 51)) |
                          (1ULL << (JavaParser::RECORD - 51)) | (1ULL << (JavaParser::SEALED - 51)) |
                          (1ULL << (JavaParser::PERMITS - 51)))) != 0) ||
                       _la == JavaParser::AT

                       || _la == JavaParser::IDENTIFIER) {
                    setState(1713);
                    annotation();
                    setState(1718);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(1719);
                match(JavaParser::LBRACK);
                setState(1720);
                match(JavaParser::RBRACK);
            }
            setState(1725);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- PrimitiveTypeContext ------------------------------------------------------------------

JavaParser::PrimitiveTypeContext::PrimitiveTypeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::BOOLEAN()
{
    return getToken(JavaParser::BOOLEAN, 0);
}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::CHAR()
{
    return getToken(JavaParser::CHAR, 0);
}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::BYTE()
{
    return getToken(JavaParser::BYTE, 0);
}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::SHORT()
{
    return getToken(JavaParser::SHORT, 0);
}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::INT()
{
    return getToken(JavaParser::INT, 0);
}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::LONG()
{
    return getToken(JavaParser::LONG, 0);
}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::FLOAT()
{
    return getToken(JavaParser::FLOAT, 0);
}

tree::TerminalNode* JavaParser::PrimitiveTypeContext::DOUBLE()
{
    return getToken(JavaParser::DOUBLE, 0);
}

size_t JavaParser::PrimitiveTypeContext::getRuleIndex() const
{
    return JavaParser::RulePrimitiveType;
}

JavaParser::PrimitiveTypeContext* JavaParser::primitiveType()
{
    PrimitiveTypeContext* _localctx = _tracker.createInstance<PrimitiveTypeContext>(_ctx, getState());
    enterRule(_localctx, 238, JavaParser::RulePrimitiveType);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1726);
        _la = _input->LA(1);
        if (!((((_la & ~0x3fULL) == 0) &&
               ((1ULL << _la) &
                ((1ULL << JavaParser::BOOLEAN) | (1ULL << JavaParser::BYTE) | (1ULL << JavaParser::CHAR) |
                 (1ULL << JavaParser::DOUBLE) | (1ULL << JavaParser::FLOAT) | (1ULL << JavaParser::INT) |
                 (1ULL << JavaParser::LONG) | (1ULL << JavaParser::SHORT))) != 0))) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TypeArgumentsContext ------------------------------------------------------------------

JavaParser::TypeArgumentsContext::TypeArgumentsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::TypeArgumentsContext::LT()
{
    return getToken(JavaParser::LT, 0);
}

std::vector<JavaParser::TypeArgumentContext*> JavaParser::TypeArgumentsContext::typeArgument()
{
    return getRuleContexts<JavaParser::TypeArgumentContext>();
}

JavaParser::TypeArgumentContext* JavaParser::TypeArgumentsContext::typeArgument(size_t i)
{
    return getRuleContext<JavaParser::TypeArgumentContext>(i);
}

tree::TerminalNode* JavaParser::TypeArgumentsContext::GT()
{
    return getToken(JavaParser::GT, 0);
}

std::vector<tree::TerminalNode*> JavaParser::TypeArgumentsContext::COMMA()
{
    return getTokens(JavaParser::COMMA);
}

tree::TerminalNode* JavaParser::TypeArgumentsContext::COMMA(size_t i)
{
    return getToken(JavaParser::COMMA, i);
}

size_t JavaParser::TypeArgumentsContext::getRuleIndex() const
{
    return JavaParser::RuleTypeArguments;
}

JavaParser::TypeArgumentsContext* JavaParser::typeArguments()
{
    TypeArgumentsContext* _localctx = _tracker.createInstance<TypeArgumentsContext>(_ctx, getState());
    enterRule(_localctx, 240, JavaParser::RuleTypeArguments);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1728);
        match(JavaParser::LT);
        setState(1729);
        typeArgument();
        setState(1734);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == JavaParser::COMMA) {
            setState(1730);
            match(JavaParser::COMMA);
            setState(1731);
            typeArgument();
            setState(1736);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(1737);
        match(JavaParser::GT);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- SuperSuffixContext ------------------------------------------------------------------

JavaParser::SuperSuffixContext::SuperSuffixContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

JavaParser::ArgumentsContext* JavaParser::SuperSuffixContext::arguments()
{
    return getRuleContext<JavaParser::ArgumentsContext>(0);
}

tree::TerminalNode* JavaParser::SuperSuffixContext::DOT()
{
    return getToken(JavaParser::DOT, 0);
}

JavaParser::IdentifierContext* JavaParser::SuperSuffixContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::TypeArgumentsContext* JavaParser::SuperSuffixContext::typeArguments()
{
    return getRuleContext<JavaParser::TypeArgumentsContext>(0);
}

size_t JavaParser::SuperSuffixContext::getRuleIndex() const
{
    return JavaParser::RuleSuperSuffix;
}

JavaParser::SuperSuffixContext* JavaParser::superSuffix()
{
    SuperSuffixContext* _localctx = _tracker.createInstance<SuperSuffixContext>(_ctx, getState());
    enterRule(_localctx, 242, JavaParser::RuleSuperSuffix);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1748);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::LPAREN: {
                enterOuterAlt(_localctx, 1);
                setState(1739);
                arguments();
                break;
            }

            case JavaParser::DOT: {
                enterOuterAlt(_localctx, 2);
                setState(1740);
                match(JavaParser::DOT);
                setState(1742);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if (_la == JavaParser::LT) {
                    setState(1741);
                    typeArguments();
                }
                setState(1744);
                identifier();
                setState(1746);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 216, _ctx)) {
                    case 1: {
                        setState(1745);
                        arguments();
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExplicitGenericInvocationSuffixContext
//------------------------------------------------------------------

JavaParser::ExplicitGenericInvocationSuffixContext::ExplicitGenericInvocationSuffixContext(ParserRuleContext* parent,
                                                                                           size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ExplicitGenericInvocationSuffixContext::SUPER()
{
    return getToken(JavaParser::SUPER, 0);
}

JavaParser::SuperSuffixContext* JavaParser::ExplicitGenericInvocationSuffixContext::superSuffix()
{
    return getRuleContext<JavaParser::SuperSuffixContext>(0);
}

JavaParser::IdentifierContext* JavaParser::ExplicitGenericInvocationSuffixContext::identifier()
{
    return getRuleContext<JavaParser::IdentifierContext>(0);
}

JavaParser::ArgumentsContext* JavaParser::ExplicitGenericInvocationSuffixContext::arguments()
{
    return getRuleContext<JavaParser::ArgumentsContext>(0);
}

size_t JavaParser::ExplicitGenericInvocationSuffixContext::getRuleIndex() const
{
    return JavaParser::RuleExplicitGenericInvocationSuffix;
}

JavaParser::ExplicitGenericInvocationSuffixContext* JavaParser::explicitGenericInvocationSuffix()
{
    ExplicitGenericInvocationSuffixContext* _localctx =
        _tracker.createInstance<ExplicitGenericInvocationSuffixContext>(_ctx, getState());
    enterRule(_localctx, 244, JavaParser::RuleExplicitGenericInvocationSuffix);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(1755);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case JavaParser::SUPER: {
                enterOuterAlt(_localctx, 1);
                setState(1750);
                match(JavaParser::SUPER);
                setState(1751);
                superSuffix();
                break;
            }

            case JavaParser::MODULE:
            case JavaParser::OPEN:
            case JavaParser::REQUIRES:
            case JavaParser::EXPORTS:
            case JavaParser::OPENS:
            case JavaParser::TO:
            case JavaParser::USES:
            case JavaParser::PROVIDES:
            case JavaParser::WITH:
            case JavaParser::TRANSITIVE:
            case JavaParser::VAR:
            case JavaParser::YIELD:
            case JavaParser::RECORD:
            case JavaParser::SEALED:
            case JavaParser::PERMITS:
            case JavaParser::IDENTIFIER: {
                enterOuterAlt(_localctx, 2);
                setState(1752);
                identifier();
                setState(1753);
                arguments();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ArgumentsContext ------------------------------------------------------------------

JavaParser::ArgumentsContext::ArgumentsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{}

tree::TerminalNode* JavaParser::ArgumentsContext::LPAREN()
{
    return getToken(JavaParser::LPAREN, 0);
}

tree::TerminalNode* JavaParser::ArgumentsContext::RPAREN()
{
    return getToken(JavaParser::RPAREN, 0);
}

JavaParser::ExpressionListContext* JavaParser::ArgumentsContext::expressionList()
{
    return getRuleContext<JavaParser::ExpressionListContext>(0);
}

size_t JavaParser::ArgumentsContext::getRuleIndex() const
{
    return JavaParser::RuleArguments;
}

JavaParser::ArgumentsContext* JavaParser::arguments()
{
    ArgumentsContext* _localctx = _tracker.createInstance<ArgumentsContext>(_ctx, getState());
    enterRule(_localctx, 246, JavaParser::RuleArguments);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(1757);
        match(JavaParser::LPAREN);
        setState(1759);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 3) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 3)) &
              ((1ULL << (JavaParser::BOOLEAN - 3)) | (1ULL << (JavaParser::BYTE - 3)) |
               (1ULL << (JavaParser::CHAR - 3)) | (1ULL << (JavaParser::DOUBLE - 3)) |
               (1ULL << (JavaParser::FLOAT - 3)) | (1ULL << (JavaParser::INT - 3)) | (1ULL << (JavaParser::LONG - 3)) |
               (1ULL << (JavaParser::NEW - 3)) | (1ULL << (JavaParser::SHORT - 3)) | (1ULL << (JavaParser::SUPER - 3)) |
               (1ULL << (JavaParser::SWITCH - 3)) | (1ULL << (JavaParser::THIS - 3)) |
               (1ULL << (JavaParser::VOID - 3)) | (1ULL << (JavaParser::MODULE - 3)) |
               (1ULL << (JavaParser::OPEN - 3)) | (1ULL << (JavaParser::REQUIRES - 3)) |
               (1ULL << (JavaParser::EXPORTS - 3)) | (1ULL << (JavaParser::OPENS - 3)) |
               (1ULL << (JavaParser::TO - 3)) | (1ULL << (JavaParser::USES - 3)) |
               (1ULL << (JavaParser::PROVIDES - 3)) | (1ULL << (JavaParser::WITH - 3)) |
               (1ULL << (JavaParser::TRANSITIVE - 3)) | (1ULL << (JavaParser::VAR - 3)) |
               (1ULL << (JavaParser::YIELD - 3)) | (1ULL << (JavaParser::RECORD - 3)) |
               (1ULL << (JavaParser::SEALED - 3)) | (1ULL << (JavaParser::PERMITS - 3)))) != 0) ||
            ((((_la - 67) & ~0x3fULL) == 0) &&
             ((1ULL << (_la - 67)) &
              ((1ULL << (JavaParser::DECIMAL_LITERAL - 67)) | (1ULL << (JavaParser::HEX_LITERAL - 67)) |
               (1ULL << (JavaParser::OCT_LITERAL - 67)) | (1ULL << (JavaParser::BINARY_LITERAL - 67)) |
               (1ULL << (JavaParser::FLOAT_LITERAL - 67)) | (1ULL << (JavaParser::HEX_FLOAT_LITERAL - 67)) |
               (1ULL << (JavaParser::BOOL_LITERAL - 67)) | (1ULL << (JavaParser::CHAR_LITERAL - 67)) |
               (1ULL << (JavaParser::STRING_LITERAL - 67)) | (1ULL << (JavaParser::TEXT_BLOCK - 67)) |
               (1ULL << (JavaParser::NULL_LITERAL - 67)) | (1ULL << (JavaParser::LPAREN - 67)) |
               (1ULL << (JavaParser::LT - 67)) | (1ULL << (JavaParser::BANG - 67)) |
               (1ULL << (JavaParser::TILDE - 67)) | (1ULL << (JavaParser::INC - 67)) |
               (1ULL << (JavaParser::DEC - 67)) | (1ULL << (JavaParser::ADD - 67)) | (1ULL << (JavaParser::SUB - 67)) |
               (1ULL << (JavaParser::AT - 67)) | (1ULL << (JavaParser::IDENTIFIER - 67)))) != 0)) {
            setState(1758);
            expressionList();
        }
        setState(1761);
        match(JavaParser::RPAREN);

    } catch (RecognitionException& e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

bool JavaParser::sempred(RuleContext* context, size_t ruleIndex, size_t predicateIndex)
{
    switch (ruleIndex) {
        case 97:
            return expressionSempred(antlrcpp::downCast<ExpressionContext*>(context), predicateIndex);
        case 105:
            return guardedPatternSempred(antlrcpp::downCast<GuardedPatternContext*>(context), predicateIndex);

        default:
            break;
    }
    return true;
}

bool JavaParser::expressionSempred(ExpressionContext* _localctx, size_t predicateIndex)
{
    switch (predicateIndex) {
        case 0:
            return precpred(_ctx, 18);
        case 1:
            return precpred(_ctx, 17);
        case 2:
            return precpred(_ctx, 16);
        case 3:
            return precpred(_ctx, 15);
        case 4:
            return precpred(_ctx, 13);
        case 5:
            return precpred(_ctx, 12);
        case 6:
            return precpred(_ctx, 11);
        case 7:
            return precpred(_ctx, 10);
        case 8:
            return precpred(_ctx, 9);
        case 9:
            return precpred(_ctx, 8);
        case 10:
            return precpred(_ctx, 7);
        case 11:
            return precpred(_ctx, 6);
        case 12:
            return precpred(_ctx, 26);
        case 13:
            return precpred(_ctx, 25);
        case 14:
            return precpred(_ctx, 21);
        case 15:
            return precpred(_ctx, 14);
        case 16:
            return precpred(_ctx, 3);

        default:
            break;
    }
    return true;
}

bool JavaParser::guardedPatternSempred(GuardedPatternContext* _localctx, size_t predicateIndex)
{
    switch (predicateIndex) {
        case 17:
            return precpred(_ctx, 1);

        default:
            break;
    }
    return true;
}

void JavaParser::initialize()
{
    std::call_once(javaparserParserOnceFlag, javaparserParserInitialize);
}
