/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <utils/FileEntry.h>
#include <utils/StrUtils.h>
#include <utils/UniqueId.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string_view>

#include "StringsMapping.h"

namespace HCXX {

namespace StrUtils {

StringsMapping g_stringsMapping;

std::string CamelCaseToBigSnakeCase(std::string_view param)
{
    std::string res;
    bool isPrevLetterSmall = false;
    for (const char c : param) {
        if ('a' <= c && c <= 'z') {
            isPrevLetterSmall = true;
            res.append(1, c - 'a' + 'A');
            continue;
        }

        if ('A' <= c && c <= 'Z') {
            if (isPrevLetterSmall) {
                res.append(1, '_');
            }
            res.append(1, c);
            isPrevLetterSmall = false;
            continue;
        }

        //  It is possible that current char is '1' and etc.
        //  In my opinion, we have to add underscore after it if
        //  next symbol is capital letter
        isPrevLetterSmall = true;
        res.append(1, c);
    }

    return res;
}

std::string BigSnakeCaseToCamelCase(std::string_view param)
{
    std::string res;
    res.reserve(param.size());

    bool keepUpper = true;
    for (const char c : param) {
        if (c == '_') {
            keepUpper = true;
            continue;
        }

        if ('A' <= c && c <= 'Z') {
            if (!keepUpper) {
                res.append(1, c - 'A' + 'a');
                continue;
            }

            keepUpper = false;
        }

        res.append(1, c);
    }

    return res;
}

bool StartsWith(std::string_view str, std::string_view query)
{
    return str.rfind(query, 0) == 0;
}

bool EndsWith(std::string_view str, std::string_view query)
{
    return str.size() >= query.size() && str.substr(str.size() - query.size()) == query;
}

std::string_view TrimWhitespace(std::string_view str)
{
    auto isspace = [](char c) { return std::isspace(c); };
    std::size_t first = std::find_if_not(str.begin(), str.end(), isspace) - str.begin();
    std::size_t last = std::find_if_not(str.rbegin(), str.rend(), isspace).base() - str.begin();

    return str.substr(first, last - first);
}

std::string Replace(std::string_view str, std::string_view from, std::string_view to)
{
    std::string result{str};

    if (from.empty()) {
        return result;
    }

    std::size_t startPos = 0;
    while ((startPos = result.find(from, startPos)) != std::string::npos) {
        result.replace(startPos, from.length(), to);
        startPos += to.length();
    }

    return result;
}

std::string ToLower(std::string_view str)
{
    std::string result;
    result.reserve(str.size());
    std::transform(str.begin(), str.end(), std::back_inserter(result), [](auto c) { return std::tolower(c); });

    return result;
}

std::string ToUpper(std::string_view str)
{
    std::string result;
    result.reserve(str.size());
    std::transform(str.begin(), str.end(), std::back_inserter(result), [](auto c) { return std::toupper(c); });

    return result;
}

// replace all occurrences like '%<uint>%' by replacements[<uint> - 1]
// if <uint> == 0 or > replacements.size() placeholder will be removed in result string
std::string ReplacePlaceholders(std::string_view str, const std::vector<std::string>& replacements)
{
    static const std::regex placeholderRegex("%\\d+%");
    std::string result(str);

    struct Placeholder {
        int64_t start;
        int64_t size;
        std::size_t value;
    };
    std::vector<Placeholder> placeholders;

    auto placeholdersBegin = std::sregex_iterator(result.begin(), result.end(), placeholderRegex);

    for (std::sregex_iterator i = placeholdersBegin; i != std::sregex_iterator(); ++i) {
        Placeholder placeholder{i->position(), i->length()};
        placeholder.value = std::strtoul(i->str().data() + 1, nullptr,
                                         10);  //   There isn't any need to perform additional checks for strtoul, regex
                                               //   has confirmed that it is integer
        placeholders.emplace_back(placeholder);
    }

    if (placeholders.empty()) {
        return result;
    }

    int64_t shift = 0;
    for (auto& placeholder : placeholders) {
        placeholder.start += shift;

        if (placeholder.value != 0 && placeholder.value <= replacements.size()) {
            result.replace(placeholder.start, placeholder.size, replacements[placeholder.value - 1]);
            shift += replacements[placeholder.value - 1].size() - placeholder.size;
        } else {
            // LCOV_EXCL_START
            result.erase(placeholder.start, placeholder.size);
            shift -= placeholder.size;
            // LCOV_EXCL_STOP
        }
    }

    return result;
}

std::vector<std::string_view> Split(std::string_view s, char separator, bool skipEmpty)
{
    using namespace std;
    vector<string_view> output;

    size_t prevPos = 0;
    size_t pos = 0;

    while (true) {
        pos = s.find(separator, pos);
        string_view substring = s.substr(prevPos, pos - prevPos);

        if (!skipEmpty || !substring.empty()) {
            output.push_back(substring);
        }

        if (pos == string_view::npos) {
            break;
        }

        prevPos = ++pos;
    }

    return output;
}

uint32_t GetStringId(std::string_view s)
{
    return g_stringsMapping.GetStringId(s);
}

const std::string& GetStringById(uint32_t stringId)
{
    return g_stringsMapping.GetStringById(stringId);
}

std::string GetLastToken(const std::string& s, const char* separators)
{
    auto p = s.find_last_of(separators);
    return p != std::string::npos ? s.substr(p + 1) : "";
}

size_t FindMatchingBrace(std::string_view str, size_t pos, char lbrace, char rbrace)
{
    int unmatched = 0;

    for (size_t i = pos; i < str.size(); ++i) {
        if (str[i] == lbrace) {
            ++unmatched;
        } else if (str[i] == rbrace) {
            --unmatched;
            if (unmatched == 0) {
                return i;
            }
        }
    }

    return str.npos;
}

// MatchQualifiedName & supporting functions ===========================================================================
namespace {

/// Return a character in the string, prevent possible out-of-bounds
char CharAt(std::string_view str, size_t pos)
{
    return pos >= str.size() ? '\0' : str[pos];
}

/// Check if str[pos] is a legal character for an identifier name, in the loosest possible sense.
bool IsLegal(std::string_view str, size_t pos)
{
    return pos >= str.size() ? false : (str[pos] != '<' && str[pos] != ':');
}

/// Read an identifier name, starting from pos. Return the end+1 position.
size_t ReadName(std::string_view str, size_t pos)
{
    size_t first = pos;

    while (IsLegal(str, pos)) {
        ++pos;
    }

    // Special parsing for functions "operator<" and "operator<<"
    if (str.substr(first, pos - first) == "operator" && CharAt(str, pos) == '<') {
        //  Advance until current char is no longer '<'
        do {
            ++pos;
        } while (CharAt(str, pos) == '<');

        // After "operator<<" can be either a template-arg-list or end of string. If template, walk back one '<'.
        if (!CharAt(str, pos) == '\0') {
            --pos;
        }
    }

    return pos;
}

/// A combination of a scope name and any template arguments it might have.
struct ScopeToken {
    std::string_view name;
    std::string_view args;
};

/**
 * @brief Read the whole scope identifer, with the name and the template args, until :: or end of string.
 * @returns A ScopeToken and the position of the next identifier, skipping over :: (or end of string).
 */
std::pair<ScopeToken, size_t> ReadScope(std::string_view str, size_t pos)
{
    size_t last = ReadName(str, pos);
    size_t templateIdx = last;
    if (CharAt(str, last) == '<') {
        // If template args were specified, read them
        last = FindMatchingBrace(str, last, '<', '>') + 1;  // +1 because last is next after end
    }
    size_t nextToken = last;

    if (!CharAt(str, nextToken) == '\0') {
        nextToken += 2;  // Skip ::
    }

    return {
        ScopeToken{str.substr(pos, templateIdx - pos), str.substr(templateIdx, last - templateIdx)},
        nextToken,
    };
}

template <bool allowPartialMatch>
bool MatchQualifiedNameImpl(std::string_view pattern, std::string_view query)
{
    if (pattern.empty()) {
        return query.empty();
    } else if (query.empty()) {
        return false;
    }

    auto [pScope, pNext] = ReadScope(pattern, 0);
    auto [qScope, qNext] = ReadScope(query, 0);

    // Scope names must be the same, template args allowed to be missing in the query
    if ((pScope.name != qScope.name) || (!qScope.args.empty() && pScope.args != qScope.args)) {
        if constexpr (allowPartialMatch) {
            return MatchQualifiedNameImpl<true>(pattern.substr(pNext), query);
        } else {
            return false;
        }
    }

    if constexpr (allowPartialMatch) {
        // Match tail (not partially). If fails, match partially, from the beginning
        bool result = MatchQualifiedNameImpl<false>(pattern.substr(pNext), query.substr(qNext));
        return result ? result : MatchQualifiedNameImpl<true>(pattern.substr(pNext), query);
    } else {
        return MatchQualifiedNameImpl<false>(pattern.substr(pNext), query.substr(qNext));
    }
}

}  // namespace

bool MatchQualifiedName(std::string_view pattern, std::string_view query)
{
    return MatchQualifiedNameImpl<false>(pattern, query);
}

bool MatchPartiallyQualifiedName(std::string_view pattern, std::string_view query)
{
    return MatchQualifiedNameImpl<true>(pattern, query);
}

// =====================================================================================================================

}  //  namespace StrUtils

std::string UniqueId::ToString() const
{
    std::stringstream ss;
    auto data = Data();
    for (uint32_t i = 0; i < 16; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << uint32_t(data[i]);
    }
    return ss.str();
}

}  //  namespace HCXX
