/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_SCANFVALIDATOR_H_
#define COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_SCANFVALIDATOR_H_
#include <utils/StrUtils.h>

#include <deps/ctre.hpp>

#include "Validator.h"

struct ScanfValidator : Validator {
    static constexpr auto RE_SCANF_FMT = ctll::fixed_string{
        "%(\\*)?(\\d+)?(hh|h|ll|l|j|z|t|L)?(c|s|d|i|o|x|X|u|f|F|e|E|a|A|g|G|n|p|%|\\[\\^?\\]?[^\\]]*\\])"};

    std::vector<ParseResult> ParseFormatString(std::string_view formatString) override
    {
        using namespace FmtTypes;

        // Types with unspecified signedness
        static constexpr FmtType XCHAR = {FmtType::Type::SIGNED | FmtType::Type::UNSIGNED, sizeof(char) * 8};
        static constexpr FmtType XSHORT = {FmtType::Type::SIGNED | FmtType::Type::UNSIGNED, sizeof(short) * 8};
        static constexpr FmtType XINT = {FmtType::Type::SIGNED | FmtType::Type::UNSIGNED, sizeof(int) * 8};
        static constexpr FmtType XLONG = {FmtType::Type::SIGNED | FmtType::Type::UNSIGNED, sizeof(long) * 8};
        static constexpr FmtType XLLONG = {FmtType::Type::SIGNED | FmtType::Type::UNSIGNED, sizeof(long long) * 8};
        static constexpr FmtType XSIZE = {FmtType::Type::SIGNED | FmtType::Type::UNSIGNED, sizeof(std::size_t) * 8};

        // make a mutable pointer out of every argument
        auto p = [](FmtType type) constexpr {
            type.quals = FmtType::Quals::MUTABLE | FmtType::Quals::POINTER;
            return type;
        };

        // clang-format off
        static constexpr CFormatTypeMap map({{
        //   hh        h          (none)   l         ll         j         z         t         L
            {INV,      INV,       p(CHAR), p(WCHAR), INV,       INV,      INV,      INV,      INV},  // c
            {INV,      INV,       p(CHAR), p(WCHAR), INV,       INV,      INV,      INV,      INV},  // s/[
            {p(XCHAR), p(XSHORT), p(XINT), p(XLONG), p(XLLONG), p(XSIZE), p(USIZE), p(ISIZE), INV},  // d/i
            {p(XCHAR), p(XSHORT), p(XINT), p(XLONG), p(XLLONG), p(XSIZE), p(USIZE), p(ISIZE), INV},  // o/x/X/u
            {INV,      INV,       p(F32),  p(F64),   INV,       INV,      INV,      INV,      p(FMAX)}, // f/F/e/E...
            {p(XCHAR), p(XSHORT), p(XINT), p(XLONG), p(XLLONG), p(XSIZE), p(USIZE), p(ISIZE), INV},  // n
            {INV,      INV,       PP_VOID, INV,      INV,       INV,      INV,      INV,      INV},  // p
        }});
        // clang-format on

        std::vector<ParseResult> result;
        for (auto [whole, assign_suppress, width, length, specifier] : ctre::range<RE_SCANF_FMT>(formatString)) {
            if (assign_suppress || specifier == "%") {
                continue;
            }

            FmtType type = map(specifier.view(), length.view());
            result.push_back({type, whole.str()});
        }

        return result;
    }

    Detect Validate(FmtType actual, FmtType expected) override
    {
        if (expected == FmtType::Invalid()) {
            return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_INVALID_SPECIFIER"};
        }

        // Check for void** conformance
        if (expected == FmtTypes::PP_VOID) {
            if (actual != FmtTypes::PP_VOID) {
                return {"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_VOID_PP"};
            }

            return {};
        }

        // Check for a bugged expected value - must be a mutable pointer.
        // LCOV_EXCL_START
        if (!expected.IsTypedPointer() || !expected.IsMutable()) {
            HCXX::Log(HCXX::LogLevel::ERROR) << "ScanfValidator::Validate: unknown expected type " << expected << "\n";
            return {};
        }
        // LCOV_EXCL_STOP

        // Common checks for all types except void**
        if (!actual.IsTypedPointer()) {
            return {"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_POINTER_TYPE"};
        }

        if (!actual.IsMutable()) {
            return {"FMT.SPEC.PTR", "FmtSpecMismatchChecker_EXPECTED_MUTABLE_POINTER"};
        }

        // Check float conformance
        if (expected.IsFloat()) {
            if (!actual.IsFloat()) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_FLOAT"};
            }

            if (expected.bitWidth != actual.bitWidth) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_SIZE_EXACTLY"};
            }

            return {};
        }

        // Check signed/unsigned conformance
        if (expected.IsSigned() || expected.IsUnsigned()) {
            auto mismatchId = expected.IsSigned() ? "FmtSpecMismatchChecker_EXPECTED_SIGNED"
                                                  : "FmtSpecMismatchChecker_EXPECTED_UNSIGNED";

            // Complete type mismatch
            if (!(actual.IsSigned() || actual.IsUnsigned())) {
                return Detect{"FMT.SPEC.MISMATCH", mismatchId};
            }

            // Signedness mismatch
            if (!(expected.IsSigned() && expected.IsUnsigned()) && (expected.type != actual.type)) {
                return Detect{"FMT.SPEC.SIGN", mismatchId};
            }

            if (expected.bitWidth != actual.bitWidth) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_SIZE_EXACTLY"};
            }

            return {};
        }

        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::ERROR) << "ScanfValidator::Validate: unknown expected type " << expected << "\n";
        return {};
        // LCOV_EXCL_STOP
    }
};

#endif  // COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_SCANFVALIDATOR_H_
