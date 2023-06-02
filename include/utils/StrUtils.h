/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef STR_UTILS_HEADER_FILE
#define STR_UTILS_HEADER_FILE

#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "deps/span.hpp"

namespace HCXX {

namespace StrUtils {

std::string BigSnakeCaseToCamelCase(std::string_view param);

std::string CamelCaseToBigSnakeCase(std::string_view param);

// Replace both functions with member functions of str in C++20
bool StartsWith(std::string_view str, std::string_view query);
bool EndsWith(std::string_view str, std::string_view ending);

std::string Replace(std::string_view str, std::string_view from, std::string_view to);

std::string ToLower(std::string_view str);
std::string ToUpper(std::string_view str);

std::string_view TrimWhitespace(std::string_view str);
template <typename T, typename = std::enable_if_t<std::is_same_v<T, std::string>>>
// requires std::same_as<T, std::string>  // Replace enable_if with this in C++20
std::string_view TrimWhitespace(T&& s) = delete;

std::string ReplacePlaceholders(std::string_view str, const std::vector<std::string>& replacements);

template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string Join(std::initializer_list<T> parts, std::string_view separator);
template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string Join(std::initializer_list<T> parts, char separator);
template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string Join(tcb::span<const T> parts, std::string_view separator);
template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string Join(tcb::span<const T> parts, char separator);

std::vector<std::string_view> Split(std::string_view s, char separator, bool skipEmpty = true);

// Calling Split on a temporary string is explicitly disallowed, to prevent construction of dangling string_views.
// enable_if is required to prevent char* from picking this overload via a converting constructor.
template <typename T, typename = std::enable_if_t<std::is_same_v<T, std::string>>>
// requires std::same_as<T, std::string>  // Replace enable_if with this in C++20
std::vector<std::string_view> Split(T&& s, char separator, bool skipEmpty = true) = delete;

uint32_t GetStringId(std::string_view s);

const std::string& GetStringById(uint32_t stringId);

std::string GetLastToken(const std::string& s, const char* separators);

/**
 * @brief Convert a string of characters to an integral value.
 *
 * Designed to be a faster alternative to "stoi" family of functions and to support string_view.
 */
template <typename IntType>
// requires std::integral<IntType>  // C++20
IntType ToInteger(std::string_view str);

/**
 * @brief Find last matching brace in the string, starting from pos. Ignore nested braces.
 *
 * Example:
 * ```
 * FindMatchingBrace("std::string<char, std::char_traits<char>>", 0, '<', '>')
 *                               ^                            ^
 * ```
 *
 * @return Position of the last matching closing brace, or npos if the first opening brace is not matched.
 */
size_t FindMatchingBrace(std::string_view str, size_t pos, char lbrace, char rbrace);

/**
 * @brief Determine if a name, qualified or otherwise, potentially missing some template arguments, matches a
 * fully-qualified name with template arguments.
 *
 * This function assumes that names are valid and formatted as to not include any extra characters like spaces.
 *
 * Examples of matching names:
 * pattern: std::basic_string<char, std::char_traits<char>, std::allocator<char>>::assign
 * query:   std::basic_string::assign
 *
 * pattern: std::get<1>
 * query:   std::get
 *
 * If "allowPartialMatch" is true, partially-qualified or unqualified names like "begin" will match names like
 * "std::begin".
 */
bool MatchQualifiedName(std::string_view pattern, std::string_view query);
bool MatchPartiallyQualifiedName(std::string_view pattern, std::string_view query);

}  //  namespace StrUtils
}  //  namespace HCXX

template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string HCXX::StrUtils::Join(std::initializer_list<T> parts, std::string_view separator)
{
    return Join(tcb::span(parts.begin(), parts.size()), separator);
}

template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string HCXX::StrUtils::Join(std::initializer_list<T> parts, char separator)
{
    return Join(tcb::span(parts.begin(), parts.size()), separator);
}

template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string HCXX::StrUtils::Join(tcb::span<const T> parts, std::string_view separator)
{
    // COODDY_SUPPRESS ContainerSpecificationChecker
    std::string result;

    std::string_view sep = "";
    for (auto&& part : parts) {
        result.append(sep);
        result.append(part);

        sep = separator;
    }

    return result;
}

template <typename T>
// requires std::convertible_to<T, std::string_view>  // C++20
std::string HCXX::StrUtils::Join(tcb::span<const T> parts, char separator)
{
    std::string sep(1, separator);  // SSO prevents any memory allocation
    return Join(parts, sep);
}

template <typename IntType>
// requires std::integral<IntType>  // C++20
IntType HCXX::StrUtils::ToInteger(std::string_view str)
{
    IntType result = 0;
    const char* first = str.data();
    const char* last = first + str.size();

    auto [next, error] = std::from_chars(first, last, result);

    if (error == std::errc::result_out_of_range) {
        throw std::out_of_range("ToInteger: parsed value is not in range of this integer type");
    } else if (error == std::errc::invalid_argument) {
        throw std::invalid_argument("ToInteger: could not perform conversion");
    }

    if (next != last) {
        // from_chars did not consume the entire string
        throw std::invalid_argument("ToInteger: could not perform conversion: bad char at the end of string");
    }

    return result;
}

#endif  //   #ifndef STR_UTILS_HEADER_FILE
