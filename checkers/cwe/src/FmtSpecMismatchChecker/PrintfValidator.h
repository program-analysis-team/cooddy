/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_PRINTFVALIDATOR_H_
#define COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_PRINTFVALIDATOR_H_
#include <utils/Bit.h>

#include <deps/ctre.hpp>

#include "CFormatTypeMap.h"
#include "Validator.h"

struct PrintfValidator : Validator {
    /**
     * @brief Regex pattern used to search for format specifiers in the format string.
     *
     * Note 1: If C++20 is enabled, move the string directly into the template of regex search.
     * Note 2: leading '-' inside square brackets needs to be escaped due to a bug in CTRE
     * (https://github.com/hanickadot/compile-time-regular-expressions/issues/70).
     * Note 3: Sequences like "ll" should precede alternate sequences like "l" (alternate sequence which the first one
     * starts with) to prevent matching problems
     * (https://github.com/hanickadot/compile-time-regular-expressions/issues/257).
     */
    static constexpr auto RE_PRINTF_FMT = ctll::fixed_string{
        "%([\\-+ #0]*)?(\\d+|\\*)?(\\.\\d+|\\.\\*)?(hh|h|ll|l|j|z|t|L)?(c|s|d|i|o|x|X|u|f|F|e|E|a|A|g|G|n|p|%)"};

    std::vector<ParseResult> ParseFormatString(std::string_view formatString) override
    {
        using namespace FmtTypes;

        static constexpr FmtType::Quals MP = FmtType::Quals::MUTABLE | FmtType::Quals::POINTER;
        static constexpr FmtType MP_ICHAR = {FmtType::Type::SIGNED, MP, sizeof(char) * 8};
        static constexpr FmtType MP_ISHORT = {FmtType::Type::SIGNED, MP, sizeof(short) * 8};
        static constexpr FmtType MP_INT = {FmtType::Type::SIGNED, MP, sizeof(int) * 8};
        static constexpr FmtType MP_LONG = {FmtType::Type::SIGNED, MP, sizeof(long) * 8};
        static constexpr FmtType MP_LLONG = {FmtType::Type::SIGNED, MP, sizeof(long long) * 8};
        static constexpr FmtType MP_ISIZE = {FmtType::Type::SIGNED, MP, sizeof(std::size_t) * 8};

        // clang-format off
        static constexpr CFormatTypeMap map({{
        //   hh        h          (none)  l        ll        j         z         t         L
            {INV,      INV,       INT,    WCHAR,   INV,      INV,      INV,      INV,      INV},  // c
            {INV,      INV,       P_CHAR, P_WCHAR, INV,      INV,      INV,      INV,      INV},  // s
            {ICHAR,    SHORT,     INT,    LONG,    LLONG,    ISIZE,    ISIZE,    ISIZE,    INV},  // d/i
            {UCHAR,    USHORT,    UINT,   ULONG,   ULLONG,   USIZE,    USIZE,    USIZE,    INV},  // o/x/X/u
            {INV,      INV,       F64,    F64,     INV,      INV,      INV,      INV,      FMAX}, // f/F/e/E/a/A/g/G
            {MP_ICHAR, MP_ISHORT, MP_INT, MP_LONG, MP_LLONG, MP_ISIZE, MP_ISIZE, MP_ISIZE, INV},  // n
            {INV,      INV,       P_VOID, INV,     INV,      INV,      INV,      INV,      INV},  // p
        }});
        // clang-format on

        std::vector<ParseResult> result;
        for (auto [whole, flags, width, precision, length, specifier] : ctre::range<RE_PRINTF_FMT>(formatString)) {
            if (specifier == "%") {
                continue;
            }

            // Handle special cases with additional arguments
            if (width == "*") {
                result.push_back({INT, whole.str(), true});
            }

            if (precision == ".*") {
                result.push_back({INT, whole.str(), true});
            }

            result.push_back({map(specifier, length ? length.view() : ""), whole.str()});
        }

        return result;
    }

    Detect Validate(FmtType actual, FmtType expected) override
    {
        if (expected == FmtType::Invalid()) {
            return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_INVALID_SPECIFIER"};
        }

        // Check void* conformance
        if (expected.IsVoidPointer()) {
            if (!actual || !actual.IsAnyPointer()) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_POINTER_TYPE"};  // LCOV_EXCL_LINE
            }

            return {};
        }

        // Check typed pointer conformance
        if (expected.IsTypedPointer()) {
            return ValidatePtr(actual, expected);
        } else {
            if (actual.IsAnyPointer()) {
                return Detect{"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_NON_POINTER_TYPE"};
            }
        }

        // Check float/unsigned/signed conformance
        if (expected.IsFloat()) {
            if (!actual.IsFloat()) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_FLOAT"};
            }

            if (expected.bitWidth != actual.bitWidth) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_SIZE_EXACTLY"};
            }

            return {};
        }

        if (expected.IsSigned()) {
            return ValidateSignedInt(actual, expected);
        }

        // Expected is unsigned int and actual is not
        if (expected.IsUnsigned()) {
            return ValidateUnsignedInt(actual, expected);
        }

        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "PrintfValidator::Validate: unknown expected type " << expected << "\n";
        return {};
        // LCOV_EXCL_STOP
    }

private:
    /// Sub-section of Validate that checks conformance when expected is pointer type.
    Detect ValidatePtr(FmtType actual, FmtType expected)
    {
        if (!actual || !actual.IsAnyPointer()) {
            return Detect{"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_POINTER_TYPE"};
        }

        // char* (probably) and %n values
        if (expected.IsSigned() && !actual.IsSigned()) {
            return Detect{"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_SIGNED"};
        }

        // wchar_t*, probably
        if (expected.IsUnsigned() && !actual.IsUnsigned()) {
            // This line is covered, but not on all systems due to differences in the type under wchar_t.
            return Detect{"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_UNSIGNED"};  // LCOV_EXCL_LINE
        }

        // Check mutability for %n
        if (expected.IsMutable() && !actual.IsMutable()) {
            return Detect{"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_MUTABLE_POINTER"};
        }

        // For pointers in %n and %s, size needs to match exactly
        if (expected.bitWidth != actual.bitWidth) {
            return Detect{"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_SIZE_EXACTLY"};
        }

        return {};
    }

    /// Sub-section of Validate that checks conformance when expected is signed int.
    Detect ValidateSignedInt(FmtType actual, FmtType expected)
    {
        if (actual.IsSigned()) {
            if ((expected.bitWidth <= 32 && actual.bitWidth <= 32) || actual.occupiedBitWidth < actual.bitWidth) {
                // Casting to bigger width for signed integers is allowed in 2 cases:
                // 1. Sign bit is not set, meaning reinterpret_cast-like conversion will not affect the value;
                // 2. Both expected and actual are not bigger than int (4 bytes), meaning integer promotion will promote
                //    actual to int and special logic will convert that int to any value less than 4 bytes.
                if (expected.bitWidth < actual.occupiedBitWidth) {
                    return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_SIZE_AT_MOST"};
                }

                return {};
            }

            // Sign bit is set, only exact width allowed
            if (expected.bitWidth != actual.bitWidth) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_SIZE_EXACTLY"};
            }

            return {};
        }

        if (!actual.IsUnsigned()) {
            // Actual is not an integer at all
            return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_SIGNED"};
        }

        if (actual.occupiedBitWidth >= expected.bitWidth) {
            // Actual occupies as many bits as expected or more (will overflow)
            return Detect{"FMT.SPEC.SIGN", "FmtSpecMismatchChecker_EXPECTED_SIGNED"};
        }

        return {};
    }
};

#endif  // COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_PRINTFVALIDATOR_H_
