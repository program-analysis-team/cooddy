/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_CFORMATTYPEMAP_H_
#define COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_CFORMATTYPEMAP_H_
#include <utils/Log.h>

#include <string_view>

#include "FmtType.h"

/**
 * @brief Class for mapping C-style format strings (printf and scanf) to an expected type.
 *
 * Printf and scanf maps are different. An instance of this map is constructed with an appropriate map in
 * PrintfValidator and ScanfValidator for internal usage when parsing the format string.
 *
 * CFormatTypeMap is costructed from a two-dimensional std::array.
 * Inner array is 9 length modifiers: hh, h, (none), l, ll, j, z, t, L
 * Outer array is 7 type specifiers: c, s/[, d/i, o/x/X/u, f/F/e/E/a/A/g/G, n, p
 */
class CFormatTypeMap {
public:
    static constexpr int N_LENGTH_MODIFIERS = 9;
    static constexpr int N_SPECIFIERS = 7;
    using MapType = std::array<std::array<FmtType, N_LENGTH_MODIFIERS>, N_SPECIFIERS>;

    constexpr explicit CFormatTypeMap(const MapType& map) : myMap(map) {}

    /**
     * @brief Find the correct type for a format specifier.
     *
     * Assumes that the specifier and length modifier strings are valid.
     */
    FmtType operator()(std::string_view specifier, std::string_view lengthModifier = "") const
    {
        size_t lengthIndex = LengthIndex(lengthModifier);
        size_t specifierIndex = SpecifierIndex(specifier);

        return myMap[specifierIndex][lengthIndex];
    }

private:
    MapType myMap;

    /// Get column index for this length specifier.
    static int LengthIndex(std::string_view length)
    {
        static constexpr std::array<std::string_view, N_LENGTH_MODIFIERS> lengths = {"hh", "h", "",  "l", "ll",
                                                                                     "j",  "z", "t", "L"};
        int result = std::find(lengths.begin(), lengths.end(), length) - lengths.begin();

        if (result == N_LENGTH_MODIFIERS) {
            // LCOV_EXCL_START
            HCXX::Log(HCXX::LogLevel::ERROR)
                << "CFormatTypeMap::LengthIndex: no known length modifier \"" << length << "\"\n";
            return 0;
            // LCOV_EXCL_STOP
        }
        return result;
    }

    /// Get row index for this format specifier.
    static int SpecifierIndex(std::string_view specifier)
    {
        static constexpr std::array<std::string_view, N_SPECIFIERS> specifiers = {"c", "s", "d", "o", "f", "n", "p"};
        std::string_view normalizedSpecifier;

        if (specifier[0] == '[') {
            normalizedSpecifier = "s";
        } else if (specifier == "i") {
            normalizedSpecifier = "d";
        } else if (specifier == "x" || specifier == "X" || specifier == "u") {
            normalizedSpecifier = "o";
        } else if (specifier == "F" || specifier == "e" || specifier == "E" || specifier == "a" || specifier == "A" ||
                   specifier == "g" || specifier == "G") {
            normalizedSpecifier = "f";
        } else {
            normalizedSpecifier = specifier;
        }
        int result = find(specifiers.begin(), specifiers.end(), normalizedSpecifier) - specifiers.begin();

        if (result == N_SPECIFIERS) {
            // LCOV_EXCL_START
            HCXX::Log(HCXX::LogLevel::ERROR)
                << "CFormatTypeMap::SpecifierIndex: no known specifier \"" << specifier << "\"\n";
            return 0;
            // LCOV_EXCL_STOP
        }
        return result;
    }
};

#endif  // COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_CFORMATTYPEMAP_H_
