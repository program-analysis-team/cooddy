/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// DO NOT EDIT !!! This file was generated with a script.
//
// JSON for C++
// https://github.com/ascheglov/json-cpp
// Version 0.1 alpha, rev. 170121e2dc099895064305e38bfb25d90a807ce3
// Generated 2014-03-27 17:16:47.104492 UTC
//
// Belongs to the public domain

// LCOV_EXCL_START
#pragma once

//----------------------------------------------------------------------
// json-cpp.h begin

//----------------------------------------------------------------------
// json-cpp/parse.hpp begin

#include <istream>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>

//----------------------------------------------------------------------
// json-cpp/ParserError.hpp begin

#include <cassert>
#include <cstddef>
#include <exception>
#include <map>
#include <string>
#include <vector>

#if defined _MSC_VER
#define JSONCPP_INTERNAL_NOEXCEPT_ throw()
#else
#define JSONCPP_INTERNAL_NOEXCEPT_ noexcept
#endif

#define DECLARE_FIELDS(...)                            \
    template <class X>                                 \
    void serialize(jsoncpp::Stream<X>& stream)         \
    {                                                  \
        jsoncpp::fields(*this, stream, ##__VA_ARGS__); \
    }

namespace jsoncpp {
class ParserError : public std::exception {
public:
    enum Type {
        NoError,
        Eof,
        UnexpectedCharacter,
        InvalidEscapeSequence,
        NoTrailSurrogate,
        UnexpectedType,
        UnknownField,
        NumberIsOutOfRange,
    };

    ParserError(Type type, std::size_t line, std::size_t column) : m_type{type}, m_line{line}, m_column{column}
    {
        assert(type != NoError);
    }

    virtual const char* what() const JSONCPP_INTERNAL_NOEXCEPT_ override
    {
        if (m_what.empty()) {
            m_what = "JSON parser error at line ";
            m_what += std::to_string(m_line);
            m_what += ", column ";
            m_what += std::to_string(m_column);
            switch (m_type) {
                case Eof:
                    m_what += ": unexpected end of file";
                    break;
                case UnexpectedCharacter:
                    m_what += ": unexpected character";
                    break;
                case InvalidEscapeSequence:
                    m_what += ": invalid escape sequence";
                    break;
                case NoTrailSurrogate:
                    m_what += ": no UTF-16 trail surrogate";
                    break;
                case UnexpectedType:
                    m_what += ": unexpected value type";
                    break;
                case UnknownField:
                    m_what += ": unknown field name";
                    break;
                case NumberIsOutOfRange:
                    m_what += ": number is out of range";
                    break;
                case NoError:
                default:
                    m_what += ": INTERNAL ERROR";
                    break;
            }
        }

        return m_what.c_str();
    }

    Type type() const
    {
        return m_type;
    }
    std::size_t line() const
    {
        return m_line;
    }
    std::size_t column() const
    {
        return m_column;
    }

private:
    Type m_type;
    std::size_t m_line;
    std::size_t m_column;

    mutable std::string m_what;
};
}  // namespace jsoncpp

#undef JSONCPP_INTERNAL_NOEXCEPT_

// json-cpp/ParserError.hpp end
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// json-cpp/Stream.hpp begin

namespace jsoncpp {
template <class Traits>
class Stream;

namespace details {
template <typename CharT, class X>
struct Traits2 {};

template <class Traits>
struct ParserTraits {};

template <class Traits>
struct GeneratorTraits {};
}  // namespace details

template <class X>
using Parser = Stream<details::ParserTraits<X>>;

template <class X>
using Generator = Stream<details::GeneratorTraits<X>>;

template <typename X, typename T>
inline auto serialize(Stream<X>& stream, T& value) -> decltype(value.serialize(stream), void())
{
    value.serialize(stream);
}
}  // namespace jsoncpp
// json-cpp/Stream.hpp end
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// json-cpp/value_types.hpp begin

namespace jsoncpp {
// Helper masks
const auto TypeIsNotFundamental = 0x40;
const auto TypeIsCollection = 0x80;

enum class Type {
    Undefined = 0,  // Helper type for debugging variant-like types
    Null = 0x01,
    Boolean = 0x02,
    Number = 0x04,
    String = 0x08 | TypeIsNotFundamental,
    Array = 0x10 | TypeIsNotFundamental | TypeIsCollection,
    Object = 0x20 | TypeIsNotFundamental | TypeIsCollection,
};
}  // namespace jsoncpp
// json-cpp/value_types.hpp end
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// json-cpp/details/parser_utility.hpp begin

#include <cassert>
#include <cstddef>
#include <utility>

namespace jsoncpp {
namespace details {
template <typename CharT>
struct CStrIterator {
    using this_type = CStrIterator<CharT>;

    CStrIterator()
    {
        static CharT null{0};
        m_ptr = &null;
    }

    CStrIterator(const CharT* ptr) : m_ptr{ptr} {}

    const CharT& operator*()
    {
        return *m_ptr;
    }
    const CharT* operator->()
    {
        return m_ptr;
    }

    this_type& operator++()
    {
        assert(!isEnd());
        ++m_ptr;
        return *this;
    }

    this_type operator++(int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    bool operator==(const this_type& rhs) const
    {
        return isEnd() == rhs.isEnd();
    }
    bool operator!=(const this_type& rhs) const
    {
        return !this->operator==(rhs);
    }

private:
    const CharT* m_ptr;

    bool isEnd() const
    {
        return *m_ptr == 0;
    }
};

class Diagnostics {
public:
    void nextColumn()
    {
        ++m_column;
    }
    void newLine()
    {
        ++m_line;
        m_column = 0;
    }

    ParserError makeError(ParserError::Type type) const
    {
        return {type, m_line, m_column};
    }

private:
    std::size_t m_column{0};
    std::size_t m_line{1};
};

template <typename InputIterator>
struct Reader {
    using this_type = Reader<InputIterator>;

    Reader(InputIterator first, InputIterator last) : m_iter(first), m_end(last)
    {
        checkEnd();
    }

    char operator*()
    {
        return *m_iter;
    }
    this_type& operator++()
    {
        checkEnd();
        ++m_iter;
        m_diag.nextColumn();
        return *this;
    }

    void checkEnd()
    {
        if (m_iter == m_end)
            throw m_diag.makeError(ParserError::Eof);
    }

    char getNextChar()
    {
        auto prev = *m_iter;
        ++*this;
        return prev;
    }

    Diagnostics m_diag;
    InputIterator m_iter, m_end;
};
}  // namespace details
}  // namespace jsoncpp

// json-cpp/details/parser_utility.hpp end
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// json-cpp/details/number_parser.hpp begin

#include <cmath>

namespace jsoncpp {
namespace details {
inline bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

template <typename Iterator>
inline unsigned parseIntNumber(Iterator& iter)
{
    auto intPart = 0U;  // TBD: 0ULL ?

    do {
        intPart = intPart * 10 + (*iter - '0');

        ++iter;
    } while (isDigit(*iter));

    return intPart;
}

template <typename Iterator>
inline double parseRealNumber(Iterator& iter)
{
    double number = 0;

    if (*iter == '0') {
        ++iter;
    } else {
        number = parseIntNumber(iter);
    }

    // here `ch` is a peeked character, need to call eat()

    if (*iter == '.') {
        ++iter;

        auto mul = 0.1;
        while (isDigit(*iter)) {
            number += (*iter - '0') * mul;
            mul /= 10;
            ++iter;
        }
    }

    // here `ch` is a peeked character, need to call eat()

    if (*iter == 'e' || *iter == 'E') {
        ++iter;

        auto negate = *iter == '-';
        if (negate || *iter == '+')
            ++iter;
        // FIXME: check `ch` for non-digit

        auto e = parseIntNumber(iter);

        if (negate)
            number /= std::pow(10, e);
        else
            number *= std::pow(10, e);
    }

    return number;
}
}  // namespace details
}  // namespace jsoncpp
// json-cpp/details/number_parser.hpp end
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// json-cpp/details/string_parser.hpp begin

#include <string>

namespace jsoncpp {
namespace details {
inline char32_t utf16SurrogatePairToUtf32(char32_t lead, char32_t trail)
{
    return 0x10000 | (lead - 0xD800) << 10 | (trail - 0xDC00);
}

inline void utf32ToUtf8(char32_t c, std::string& str)
{
    auto add = [&str](char32_t c) { str.push_back(static_cast<char>(c)); };

    if (c < 0x80) {
        add(c);
    } else if (c < 0x800) {
        add(0xC0 | c >> 6);
        add(0x80 | (c & 0x3f));
    } else if (c < 0x10000) {
        add(0xE0 | c >> 12);
        add(0x80 | ((c >> 6) & 0x3f));
        add(0x80 | (c & 0x3f));
    } else if (c < 0x200000) {
        add(0xF0 | c >> 18);
        add(0x80 | ((c >> 12) & 0x3f));
        add(0x80 | ((c >> 6) & 0x3f));
        add(0x80 | (c & 0x3f));
    } else if (c < 0x4000000) {
        add(0xF8 | c >> 24);
        add(0x80 | ((c >> 18) & 0x3f));
        add(0x80 | ((c >> 12) & 0x3f));
        add(0x80 | ((c >> 6) & 0x3f));
        add(0x80 | (c & 0x3f));
    } else {
        add(0xFC | c >> 30);
        add(0x80 | ((c >> 24) & 0x3f));
        add(0x80 | ((c >> 18) & 0x3f));
        add(0x80 | ((c >> 12) & 0x3f));
        add(0x80 | ((c >> 6) & 0x3f));
        add(0x80 | (c & 0x3f));
    }
}

enum class CharType { Raw, CodePoint, UTF16Pair };

template <typename CharT, std::size_t CharSize>
inline void addToStr(std::basic_string<CharT>& str, CharType type, char32_t c1, char32_t c2);

template <>
inline void addToStr<char, 1>(std::basic_string<char>& str, CharType type, char32_t c1, char32_t c2)
{
    if (type == CharType::Raw) {
        str.push_back(static_cast<char>(c1));
    } else if (type == CharType::CodePoint) {
        utf32ToUtf8(c1, str);
    } else {
        auto c32 = utf16SurrogatePairToUtf32(c1, c2);
        utf32ToUtf8(c32, str);
    }
}

template <>
inline void addToStr<wchar_t, 2>(std::basic_string<wchar_t>& str, CharType type, char32_t c1, char32_t c2)
{
    str.push_back(static_cast<wchar_t>(c1));
    if (type == CharType::UTF16Pair)
        str.push_back(static_cast<wchar_t>(c2));
}

template <>
inline void addToStr<wchar_t, 4>(std::basic_string<wchar_t>& str, CharType type, char32_t c1, char32_t c2)
{
    auto c = (type == CharType::UTF16Pair) ? utf16SurrogatePairToUtf32(c1, c2) : c1;
    str.push_back(static_cast<wchar_t>(c));
}

template <typename Iterator>
inline int parseHexDigit(Iterator& iter, ParserError::Type& err)
{
    auto ch = *iter;
    ++iter;
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    err = ParserError::InvalidEscapeSequence;
    return 0;
}

template <typename Iterator>
inline char32_t parseUTF16CodeUnit(Iterator& iter, ParserError::Type& err)
{
    auto n = parseHexDigit(iter, err) << 12;
    n |= parseHexDigit(iter, err) << 8;
    n |= parseHexDigit(iter, err) << 4;
    n |= parseHexDigit(iter, err);
    return static_cast<char32_t>(n);
}

template <typename Iterator, typename CharT>
inline ParserError::Type parseStringImpl(Iterator& iter, std::basic_string<CharT>& str)
{
    str.clear();
    auto add = [&str](CharType type, char32_t c1, char32_t c2) { addToStr<CharT, sizeof(CharT)>(str, type, c1, c2); };

    for (;;) {
        auto ch = static_cast<char32_t>(*iter);
        ++iter;
        if (ch == '"')
            return ParserError::NoError;

        if (ch == '\\') {
            ch = static_cast<char32_t>(*iter);
            ++iter;
            switch (ch) {
                case '\\':
                case '"':
                case '/':
                    break;

                case 'b':
                    ch = '\b';
                    break;
                case 'f':
                    ch = '\f';
                    break;
                case 'n':
                    ch = '\n';
                    break;
                case 'r':
                    ch = '\r';
                    break;
                case 't':
                    ch = '\t';
                    break;

                case 'u': {
                    ParserError::Type err{ParserError::NoError};
                    auto codeUnit = parseUTF16CodeUnit(iter, err);
                    if (err != ParserError::NoError)
                        return err;

                    if (codeUnit >= 0xD800 && codeUnit < 0xDC00) {
                        if (*iter != '\\')
                            return ParserError::NoTrailSurrogate;
                        ++iter;
                        if (*iter != 'u')
                            return ParserError::NoTrailSurrogate;
                        ++iter;

                        auto trailSurrogate = parseUTF16CodeUnit(iter, err);
                        if (err != ParserError::NoError)
                            return err;

                        add(CharType::UTF16Pair, codeUnit, trailSurrogate);
                    } else {
                        add(CharType::CodePoint, codeUnit, 0);
                    }
                }
                    continue;

                default:
                    return ParserError::InvalidEscapeSequence;
            }
        }

        add(CharType::Raw, ch, 0);
    }
}
}  // namespace details
}  // namespace jsoncpp

// json-cpp/details/string_parser.hpp end
//----------------------------------------------------------------------

namespace jsoncpp {
template <typename CharT, typename InputIterator>
class Stream<details::ParserTraits<details::Traits2<CharT, InputIterator>>> {
public:
    using this_type = Parser<details::Traits2<CharT, InputIterator>>;

    explicit Stream(InputIterator first, InputIterator last, bool ignoreUnknownFields = false)
        : m_reader{first, last}, m_ignoreUnknownFields{ignoreUnknownFields}
    {
        nextValue();
    }

    Type getType() const
    {
        return m_type;
    }
    bool getBoolean() const
    {
        return m_boolean;
    }
    double getNumber() const
    {
        return m_number;
    }
    const std::string& getFieldName() const
    {
        return m_fieldName;
    }

    void checkType(Type type) const
    {
        if (getType() != type)
            throw makeError(ParserError::UnexpectedType);
    }

    bool isListEnd(char terminator)
    {
        eatWhitespace();
        if (*m_reader != terminator)
            return false;

        ++m_reader;
        return true;
    }

    void eatListSeparator()
    {
        eatWhitespace();
        check(',');
        eatWhitespace();
    }

    void nextNameValuePair()
    {
        eatWhitespace();
        check('"');
        parseString(m_fieldName);
        eatWhitespace();
        check(':');
        nextValue();
    }

    void nextValue()
    {
        eatWhitespace();
        m_type = nextValueImpl();
    }

    template <typename DstCharT>
    void parseString(std::basic_string<DstCharT>& str)
    {
        auto err = parseStringImpl(m_reader, str);
        if (err != ParserError::NoError)
            throw m_reader.m_diag.makeError(err);
    }

    void parseUnknownField()
    {
        if (!m_ignoreUnknownFields) {
            throw makeError(ParserError::UnknownField);
        }
        serialize(*this, nullptr);
    }

    ParserError makeError(ParserError::Type type) const
    {
        return m_reader.m_diag.makeError(type);
    }

    InputIterator getCurIter() const
    {
        return m_reader.m_iter;
    }

private:
    Type nextValueImpl()
    {
        switch (*m_reader) {
            case '{':
                ++m_reader;
                return Type::Object;
            case '[':
                ++m_reader;
                return Type::Array;
            case 't':
                ++m_reader;
                checkLiteral("true");
                m_boolean = true;
                return Type::Boolean;
            case 'f':
                ++m_reader;
                checkLiteral("false");
                m_boolean = false;
                return Type::Boolean;
            case 'n':
                ++m_reader;
                checkLiteral("null");
                return Type::Null;
            case '"':
                ++m_reader;
                return Type::String;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                m_number = parseRealNumber(m_reader);
                return Type::Number;

            case '-':
                ++m_reader;
                m_number = -parseRealNumber(m_reader);
                return Type::Number;
            default:
                throw unexpectedCharacter();
        }
    }

    ParserError unexpectedCharacter() const
    {
        return makeError(ParserError::UnexpectedCharacter);
    }

    void check(char expectedChar)
    {
        if (*m_reader != expectedChar)
            throw unexpectedCharacter();

        ++m_reader;
    }

    template <std::size_t N>
    void checkLiteral(const char (&literal)[N])
    {
        static_assert(N > 2, "");
        for (auto i = 1; i != N - 1; ++i, ++m_reader)
            if (*m_reader != literal[i])
                throw unexpectedCharacter();
    }

    void eatWhitespace()
    {
        for (;; ++m_reader) {
            switch (*m_reader) {
                case '/':
                    ++m_reader;
                    check('/');
                    while (*m_reader != '\n')
                        ++m_reader;

                    // no break here
                case '\n':
                    m_reader.m_diag.newLine();
                    break;

                case ' ':
                case '\t':
                case '\r':
                    break;

                default:
                    return;
            }
        }
    }

    details::Reader<InputIterator> m_reader;

    Type m_type;
    double m_number;
    bool m_boolean;
    std::string m_fieldName;
    bool m_ignoreUnknownFields;
};

template <class X>
inline void serialize(Parser<X>& parser, bool& value)
{
    parser.checkType(Type::Boolean);
    value = parser.getBoolean();
}

template <class X, typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value>::type serialize(Parser<X>& parser, T& value)
{
    parser.checkType(Type::Number);
    auto number = parser.getNumber();
    value = static_cast<T>(number);
    if (value != number)
        throw parser.makeError(ParserError::NumberIsOutOfRange);
}

template <class X, typename T>
inline typename std::enable_if<std::is_enum<T>::value>::type serialize(Parser<X>& parser, T& value)
{
    typename std::underlying_type<T>::type temp = 0;
    serialize(parser, temp);
    value = static_cast<T>(temp);
}

template <class X, typename DstCharT>
inline void serialize(Parser<X>& parser, std::basic_string<DstCharT>& value)
{
    if (parser.getType() == Type::Number) {
        value = std::to_string((int)parser.getNumber());
        return;
    }
    parser.checkType(Type::String);
    parser.parseString(value);
}

template <class X>
inline void serialize(Parser<X>& parser, std::nullptr_t)
{
    if (parser.getType() == Type::String) {
        std::string s;
        serialize(parser, s);
    } else if (parser.getType() == Type::Array) {
        std::vector<std::nullptr_t> v;
        serialize(parser, v);
    } else if (parser.getType() == Type::Object) {
        std::map<std::string, std::nullptr_t> o;
        serialize(parser, o);
    }
}

namespace details {
template <class X, typename Callback>
inline void parseList(Parser<X>& parser, Type type, char terminator, Callback&& callback)
{
    parser.checkType(type);

    while (!parser.isListEnd(terminator)) {
        callback();

        if (parser.isListEnd(terminator))
            return;

        parser.eatListSeparator();
    }
}
}  // namespace details

template <class X, typename Callback>
inline void parseObject(Parser<X>& parser, Callback&& callback)
{
    details::parseList(parser, Type::Object, '}', [&] {
        parser.nextNameValuePair();
        callback(parser.getFieldName());
    });
}

template <class X, typename Callback>
void parseArray(Parser<X>& parser, Callback&& callback)
{
    details::parseList(parser, Type::Array, ']', [&] {
        parser.nextValue();
        callback();
    });
}

template <typename CharT, class T, typename InputIterator>
inline void parse(T& object, InputIterator first, InputIterator last, bool ignoreUnknownFields = false)
{
    Parser<details::Traits2<CharT, InputIterator>> stream{first, last, ignoreUnknownFields};
    serialize(stream, object);
}

template <typename T, typename CharT>
inline void parse(T& object, const CharT* str, bool ignoreUnknownFields = false)
{
    details::CStrIterator<CharT> first{str}, last;
    parse<CharT>(object, first, last, ignoreUnknownFields);
}

template <typename T, typename CharT>
inline void parse(T& object, std::basic_string<CharT>& str, bool ignoreUnknownFields = false)
{
    parse<CharT>(object, std::begin(str), std::end(str), ignoreUnknownFields);
}

template <typename T, typename CharT>
inline void parse(T& object, std::basic_istream<CharT>& stream, bool ignoreUnknownFields = false)
{
    std::istreambuf_iterator<CharT> first{stream}, last;
    parse<CharT>(object, first, last, ignoreUnknownFields);
}
}  // namespace jsoncpp

// json-cpp/parse.hpp end
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// json-cpp/std_types.hpp begin

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//----------------------------------------------------------------------
// json-cpp/generate.hpp begin

#include <sstream>
#include <string>

//----------------------------------------------------------------------
// json-cpp/details/string_writer.hpp begin

#include <string>

namespace jsoncpp {
namespace details {
template <typename SrcCharT, typename Sink>
inline void writeString(const std::basic_string<SrcCharT>& str, Sink&& sink)
{
    sink('"');
    for (auto iter = std::begin(str), last = std::end(str); iter != last; ++iter) {
        switch (char32_t ch = static_cast<unsigned char>(*iter)) {
            case '"':
                sink('\\');
                sink('"');
                break;
            case '\\':
                sink('\\');
                sink('\\');
                break;
            case '\b':
                sink('\\');
                sink('b');
                break;
            case '\f':
                sink('\\');
                sink('f');
                break;
            case '\n':
                sink('\\');
                sink('n');
                break;
            case '\r':
                sink('\\');
                sink('r');
                break;
            case '\t':
                sink('\\');
                sink('t');
                break;
            default:
                if (ch < '\x20') {
                    const auto table = "0123456789ABCDEF";
                    unsigned n = static_cast<unsigned char>(ch);
                    sink('\\');
                    sink('u');
                    sink('0');
                    sink('0');
                    sink(table[n >> 4]);
                    sink(table[n & 15]);
                } else {
                    sink(static_cast<char>(ch));
                }
        }
    }
    sink('"');
}
}  // namespace details
}  // namespace jsoncpp

// json-cpp/details/string_writer.hpp end
//----------------------------------------------------------------------

namespace jsoncpp {
template <typename CharT, typename Sink>
class Stream<details::GeneratorTraits<details::Traits2<CharT, Sink>>> {
public:
    using this_type = Generator<details::Traits2<CharT, Sink>>;

    explicit Stream(Sink& sink, const char* separator = "") : m_sink(&sink), m_separator(separator) {}

    void objectBegin()
    {
        (*m_sink) << "{" << m_separator;
    }

    void fieldName(const char* name)
    {
        (*m_sink) << '"' << name << "\": ";
        // TODO: use writeString (?)
    }

    template <typename StrCharT>
    void fieldName(const std::basic_string<StrCharT>& name)
    {
        (*m_sink) << '"' << name << "\": ";
        // TODO: use writeString (?)
    }

    void separator()
    {
        (*m_sink) << ", " << m_separator;
    }

    void objectEnd()
    {
        (*m_sink) << '}' << m_separator;
    }

    void arrayBegin()
    {
        (*m_sink) << '[' << m_separator;
    }

    void arrayEnd()
    {
        (*m_sink) << ']' << m_separator;
    }

    friend void serialize(this_type& stream, std::nullptr_t)
    {
        (*stream.m_sink) << "null";
    }

    friend void serialize(this_type& stream, bool value)
    {
        (*stream.m_sink) << (value ? "true" : "false");
    }

    friend void serialize(this_type& stream, const char* value)
    {
        serialize(stream, std::string(value));
    }

    template <typename T>
    friend typename std::enable_if<std::is_arithmetic<T>::value>::type serialize(this_type& stream, T& value)
    {
        (*stream.m_sink) << value;
    }

    template <typename T>
    friend typename std::enable_if<std::is_enum<T>::value>::type serialize(this_type& stream, T& value)
    {
        auto temp = static_cast<typename std::underlying_type<T>::type>(value);
        (*stream.m_sink) << temp;
    }

    template <typename SrcCharT>
    friend void serialize(this_type& stream, const std::basic_string<SrcCharT>& value)
    {
        details::writeString(value, [&stream](char c) { stream.m_sink->put(c); });
    }

private:
    Sink* m_sink;
    const char* m_separator;
};

template <class X, typename Pointer>
inline void writePointer(Generator<X>& generator, Pointer& ptr)
{
    if (ptr) {
        serialize(generator, *ptr);
    } else {
        serialize(generator, nullptr);
    }
}

template <class X, typename Range>
inline void writeRange(Generator<X>& generator, Range& range)
{
    generator.arrayBegin();

    auto iter = std::begin(range);
    const auto& last = std::end(range);
    if (iter != last) {
        for (;;) {
            serialize(generator, *iter);

            ++iter;
            if (iter == last)
                break;

            generator.separator();
        }
    }

    generator.arrayEnd();
}

template <class T>
inline std::string to_string(const T& object, const char* separator = "")
{
    std::ostringstream rawStream;
    Generator<details::Traits2<char, std::ostream>> stream{rawStream, separator};
    serialize(stream, const_cast<T&>(object));
    return rawStream.str();
}
}  // namespace jsoncpp

// json-cpp/generate.hpp end
//----------------------------------------------------------------------

namespace jsoncpp {
template <class X, typename T>
inline void serialize(Parser<X>& parser, std::shared_ptr<T>& obj)
{
    if (parser.getType() != jsoncpp::Type::Null) {
        obj = std::make_shared<T>();
        serialize(parser, *obj);
    } else {
        obj.reset();
    }
}

template <class X, typename T>
inline void serialize(Generator<X>& generator, std::shared_ptr<T>& obj)
{
    writePointer(generator, obj);
}

template <class X, typename T>
inline void serialize(Parser<X>& parser, std::unique_ptr<T>& obj)
{
    if (parser.getType() != jsoncpp::Type::Null) {
        obj->reset(new T());
        serialize(parser, *obj);
    } else {
        obj.reset();
    }
}

template <class X, typename T>
inline void serialize(Generator<X>& generator, std::unique_ptr<T>& obj)
{
    writePointer(generator, obj);
}

namespace details {
template <class X, typename C>
inline void serializeContainer(Parser<X>& parser, C& c)
{
    c.clear();

    parseArray(parser, [&] {
        c.emplace_back();
        serialize(parser, c.back());
    });
}

template <class X, typename C>
inline void serializeContainer(Generator<X>& generator, C& c)
{
    writeRange(generator, c);
}

template <class X, typename C>
inline void serializeSet(Parser<X>& parser, C& c)
{
    c.clear();

    parseArray(parser, [&] {
        typename C::value_type value;
        serialize(parser, value);
        c.insert(value);
    });
}

template <class X, typename C>
inline void serializeSet(Generator<X>& generator, C& c)
{
    writeRange(generator, c);
}

template <class X, typename C>
inline void serializeStrMap(Parser<X>& parser, C& c)
{
    c.clear();

    parseObject(parser, [&](const std::string& name) { serialize(parser, c[name]); });
}

template <class X, typename C>
inline void serializeStrMap(Generator<X>& generator, C& c)
{
    generator.objectBegin();

    auto iter = std::begin(c);
    const auto& last = std::end(c);
    if (iter != last) {
        for (;;) {
            generator.fieldName(iter->first);
            serialize(generator, iter->second);

            ++iter;
            if (iter == last)
                break;

            generator.separator();
        }
    }

    generator.objectEnd();
}
}  // namespace details

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::vector<T>& arr)
{
    details::serializeContainer(stream, arr);
}

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::list<T>& arr)
{
    details::serializeContainer(stream, arr);
}

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::forward_list<T>& arr)
{
    details::serializeContainer(stream, arr);
}

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::deque<T>& arr)
{
    details::serializeContainer(stream, arr);
}

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::set<T>& arr)
{
    details::serializeSet(stream, arr);
}

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::unordered_set<T>& arr)
{
    details::serializeSet(stream, arr);
}

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::map<std::string, T>& t)
{
    details::serializeStrMap(stream, t);
}

template <class X, typename T>
inline void serialize(Stream<X>& stream, std::unordered_map<std::string, T>& t)
{
    details::serializeStrMap(stream, t);
}
}  // namespace jsoncpp
// json-cpp/std_types.hpp end
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// json-cpp/serialization_helpers.hpp begin

#include <array>
#include <unordered_map>

namespace jsoncpp {
namespace details {

template <class X, typename T>
inline void writeField(Generator<X>& generator, const char* name, const T& value)
{
    generator.fieldName(name);
    serialize(generator, const_cast<T&>(value));
}

template <class X, typename T, typename... F>
inline void writeField(Generator<X>& generator, const char* name, const T& value, F&&... fieldsDef)
{
    writeField(generator, name, value);
    generator.separator();
    writeField(generator, fieldsDef...);
}

template <typename ParserT>
class FieldsTable {
public:
    template <typename... F>
    FieldsTable(F&&... fieldsDef)
    {
        m_map.reserve(sizeof...(fieldsDef) / 2);
        add(1, fieldsDef...);
    }

    struct FieldInfo {
        template <typename T>
        FieldInfo(T&, std::size_t idx)
        {
            m_fieldIdx = idx;
            m_parseFn = [](ParserT& parser, void* fieldPtr) {
                serialize(parser, static_cast<T&>(*reinterpret_cast<T*>(fieldPtr)));
            };
        }

        std::size_t m_fieldIdx;
        void (*m_parseFn)(ParserT& parser, void* fieldPtr);
    };

    const FieldInfo* find(const std::string& name) const
    {
        auto it = m_map.find(name);
        return it == m_map.end() ? nullptr : &it->second;
    }

private:
    template <typename T, typename... F>
    void add(std::size_t idx, const char* name, T& value, F&&... otherFields)
    {
        m_map.emplace(name, FieldInfo(value, idx));
        add(idx + 2, otherFields...);
    }

    void add(std::size_t /*idx*/) {}

    std::unordered_map<std::string, FieldInfo> m_map;
};

inline void* makePtrs(const char*)
{
    return nullptr;
}

template <typename T>
inline void* makePtrs(T& obj)
{
    return &obj;
}

}  // namespace details

template <class Cls, class X, typename... F>
inline void fields(Cls&, Parser<X>& parser, F&&... fieldsDef)
{
    std::array<void*, sizeof...(fieldsDef)> ptrs{details::makePtrs(fieldsDef)...};

    static const details::FieldsTable<Parser<X>> table{fieldsDef...};

    auto&& handler = [&](const std::string& fieldName) {
        auto fieldInfo = table.find(fieldName);
        if (fieldInfo == nullptr) {
            parser.parseUnknownField();
        } else {
            auto fieldPtr = ptrs[fieldInfo->m_fieldIdx];
            fieldInfo->m_parseFn(parser, fieldPtr);
        }
    };

    parseObject(parser, handler);
}

template <class Cls, class X, typename... F>
inline void fields(Cls&, Generator<X>& generator, F&&... fieldsDef)
{
    generator.objectBegin();
    details::writeField(generator, fieldsDef...);
    generator.objectEnd();
}
}  // namespace jsoncpp

// json-cpp/serialization_helpers.hpp end
//----------------------------------------------------------------------

// json-cpp.h end
//----------------------------------------------------------------------

// LCOV_EXCL_STOP
