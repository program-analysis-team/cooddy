/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// These methods implement Uri encoding and decoding
//
#ifndef COODDY_LSP_SERVER_URIENCODING_H_
#define COODDY_LSP_SERVER_URIENCODING_H_

#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

constexpr std::string_view PATH_RESERVED = "?#[]";

inline std::string UrlEncode(std::string_view str, std::string_view reserved)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (auto character : str) {
        auto byte = static_cast<unsigned char>(character);

        if (byte & 0b10000000 || reserved.find(character) != reserved.npos) {
            // This is a UTF-8 character or a reserved character
            escaped << '%' << std::setw(2) << static_cast<int>(byte);
        } else {
            escaped << character;
        }
    }

    return escaped.str();
}

inline char FromHex(char ch)
{
    return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

inline std::string UrlDecode(const std::string& text)
{
    std::ostringstream escaped;
    escaped.fill('0');

    for (auto i = text.begin(), n = text.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        if (c == '%') {
            if (i[1] != 0 && i[2] != 0) {
                escaped << static_cast<char>(static_cast<unsigned int>(FromHex(i[1])) << 4u |
                                             static_cast<unsigned int>(FromHex(i[2])));
                i += 2;
            }
        } else if (c == '+') {
            escaped << ' ';
        } else {
            escaped << c;
        }
    }
    std::string result = escaped.str();
    if (!result.compare(0, 8, "file:///")) {
        result.erase(result.begin(), result.begin() + 8);
        result[0] = std::toupper(result[0]);
    }
    return result;
}

#endif  // COODDY_LSP_SERVER_URIENCODING_H_
