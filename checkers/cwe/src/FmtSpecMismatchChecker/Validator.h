/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_VALIDATOR_H_
#define COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_VALIDATOR_H_
#include <ast/Node.h>

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "FmtType.h"

struct Detect {
    std::string_view kind;
    std::string_view message;

    explicit operator bool() const
    {
        return !kind.empty();
    }
};

struct ParseResult {
    FmtType type;
    std::string match;  // A bug in CTRE prevents me from using string_view here
    bool isAuxiliary = false;
};

struct Validator {
    virtual std::vector<ParseResult> ParseFormatString(std::string_view formatString) = 0;

    /**
     * @brief Determine the FmtType corresponding to this argument.
     *
     * It's possible to return a detect straight from this function, if at this stage it's obvious that a problem
     * occured. In this case validation against the expected argument is not performed.
     *
     * @return A FmtType for this node and an empty detect, or FmtType::Invalid and either a detect to report
     * to report something, or an empty detect to silently continue.
     */
    virtual FmtType GetArgumentType(const HCXX::Node& node)
    {
        // Remove integer promotion, if any
        const HCXX::Node* unpromotedNode = GetUnpromotedNode(node);
        if (unpromotedNode == nullptr) {
            // Broken AST
            return FmtType::Invalid();  // LCOV_EXCL_LINE
        }

        // Get a more precise type if node is a literal integer
        FmtType result = GetIntLiteralType(*unpromotedNode);
        if (result != FmtType::Invalid()) {
            // HCXX::Node was an int literal
            return result;
        }

        return FmtType::FromGenericType(unpromotedNode->GetType());
    }

    /**
     * @brief Determine if actual argument type is allowed for this format specifier.
     *
     * @return A detect (or an empty, falsy detect) indicating a problem, if any.
     */
    virtual Detect Validate(FmtType actual, FmtType expected) = 0;

    virtual ~Validator() = default;

protected:
    /**
     * @brief Get node of argument for format string, before integer promotion affects it.
     *
     * Special handling of integer promotion is required because it can change the signed-ness of type, as well as for
     * relaxed handling of integer literals.
     * See https://en.cppreference.com/w/c/language/conversion#Default_argument_promotions
     *
     * @return HCXX::Node contained within integral cast, original node if there was no cast or nullptr if AST was
     * corrupted.
     */
    const HCXX::Node* GetUnpromotedNode(const HCXX::Node& node)
    {
        auto* cast = HCXX::Node::Cast<HCXX::CastExpression>(&node);
        if (cast != nullptr && cast->IsImplicit() && cast->GetCastKind() == HCXX::CastExpression::Kind::INTEGRAL_CAST) {
            return cast->GetCastedExpression();
        }

        return &node;
    }

    /// If the node is an integer literal, get a more precise type, i.e. count occupied bits.
    FmtType GetIntLiteralType(const HCXX::Node& node)
    {
        const HCXX::Node* n = &node;

        // Handle possible negation with unary '-'
        bool negated = false;
        auto* unaryExpr = HCXX::Node::Cast<HCXX::UnaryExpression>(n);
        if (unaryExpr != nullptr) {
            n = unaryExpr->GetOperand();

            if (unaryExpr->GetOperation() == HCXX::UnaryExpression::Operation::MINUS) {
                negated = true;
            }
        }

        auto* literalExpr = HCXX::Node::Cast<HCXX::IntLiteralExpression>(n);
        if (literalExpr == nullptr) {
            return FmtType::Invalid();
        }

        HCXX::Type type = literalExpr->GetType();
        FmtType::Type fmtType = type.IsUnsigned() ? FmtType::Type::UNSIGNED : FmtType::Type::SIGNED;

        // Determine the amount of bits this int takes up
        if (negated) {
            // If negated, the highest bit is set, int takes up all of the space...
            return FmtType{fmtType, static_cast<uint8_t>(type.GetSizeInBits())};
        }

        // ...otherwise it takes up only some bits on the lower side.
        uint64_t value = literalExpr->GetValue();
        return FmtType{fmtType, static_cast<uint8_t>(type.GetSizeInBits()), static_cast<uint8_t>(BitWidth(value))};
    }

    /// Validate if expected is an unsigned integer.
    Detect ValidateUnsignedInt(FmtType actual, FmtType expected)
    {
        if (actual.IsUnsigned()) {
            if (expected.bitWidth < actual.occupiedBitWidth) {
                return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_SIZE_AT_MOST"};
            }

            return {};
        }

        // Actual is not an integer at all
        if (!actual.IsSigned()) {
            return Detect{"FMT.SPEC.MISMATCH", "FmtSpecMismatchChecker_EXPECTED_UNSIGNED"};
        }

        // Actual occupies all its bits (meaning sign bit is set) or
        // actual occupies as many bits as expected or more (either sign bit is set or will overflow)
        if (actual.bitWidth == actual.occupiedBitWidth || actual.occupiedBitWidth >= expected.bitWidth) {
            return Detect{"FMT.SPEC.SIGN", "FmtSpecMismatchChecker_EXPECTED_UNSIGNED"};
        }

        return {};
    }
};

#endif  // COODDY_ANALYZER_SOURCE_CHECKERS_CWE_FMTSPECMISMATCHCHECKER_VALIDATOR_H_
