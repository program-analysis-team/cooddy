/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CallExpression.h>
#include <ast/CastExpression.h>
#include <ast/IntLiteralExpression.h>
#include <ast/StringLiteralExpression.h>
#include <ast/UnaryExpression.h>
#include <dfa/Annotation.h>
#include <dfa/AnnotationUtils.h>
#include <dfa/DfaChecker.h>
#include <dfa/DfaState.h>
#include <dfa/FormatStringKind.h>
#include <utils/Bit.h>
#include <utils/LocaleUtils.h>
#include <utils/StrUtils.h>

#include <algorithm>
#include <array>
#include <deps/span.hpp>  // Replace with std::span under C++20

#include "Checker.h"
#include "FmtSpecMismatchChecker/FmtType.h"
#include "FmtSpecMismatchChecker/PrintfValidator.h"
#include "FmtSpecMismatchChecker/ScanfValidator.h"
#include "FmtSpecMismatchChecker/ScanfsValidator.h"
#include "FmtSpecMismatchChecker/Validator.h"

using tcb::span;
using namespace std;
using namespace HCXX;

class FmtSpecMismatchChecker : public DfaChecker {
public:
    FmtSpecMismatchChecker()
    {
        myFormatStringKind = Annotation::GetKind("FormatString");
        myFormatStringArgsKind =
            Annotation::RegisterKind("FormatStringArgs", Annotation::NOT_PROPAGATED_FROM_PARAMETER);
    }

    void CheckState(DfaState& state, ProblemsHolder& holder) override
    {
        // Try parse the call expression: find format string text, arguments, etc.
        auto parseResult = ParseCallExpression(state);
        if (!parseResult.has_value()) {
            return;
        }
        auto& [kind, formatStringNode, formatArgs] = *parseResult;
        const string& formatString = formatStringNode.GetValue();

        std::unique_ptr<Validator> v = GetValidator(kind);
        if (v == nullptr) {
            return;  // LCOV_EXCL_LINE
        }

        // Check that the amount of actual arguments matches the amount of expected arguments
        vector<ParseResult> formatArgsExpected = v->ParseFormatString(formatString);
        if (formatArgs.size() < formatArgsExpected.size()) {
            holder.RegisterProblem(
                *this, formatStringNode,
                Problem{{StrLocales::GetStringLocale("FmtSpecMismatchChecker_TOO_FEW_ARGUMENTS")}, {}, "FMT.SPEC.TFA"});
        } else if (formatArgs.size() > formatArgsExpected.size()) {
            holder.RegisterProblem(
                *this, formatStringNode,
                Problem{
                    {StrLocales::GetStringLocale("FmtSpecMismatchChecker_TOO_MANY_ARGUMENTS")}, {}, "FMT.SPEC.TMA"});
        }

        // Continue checking after possible count mismatch, to give more information on missing arguments
        int argCount = min(formatArgs.size(), formatArgsExpected.size());
        for (int i = 0; i < argCount; ++i) {
            const Node* arg = formatArgs[i];
            if (arg == nullptr) {
                // Broken AST
                continue;  // LCOV_EXCL_LINE
            }

            auto& [expectedType, formatSpec, isAux] = formatArgsExpected[i];
            FmtType actualType = v->GetArgumentType(*arg);

            Detect error = v->Validate(actualType, expectedType);
            if (error) {
                holder.RegisterProblem(*this, *arg, FormatProblem(error, expectedType, formatSpec, isAux));
            }
        }
    }

private:
    static unique_ptr<Validator> GetValidator(FormatStringKind kind)
    {
        switch (kind) {
            case FormatStringKind::Printf:
                return std::make_unique<PrintfValidator>();
            case FormatStringKind::Scanf:
                return std::make_unique<ScanfValidator>();
            case FormatStringKind::Scanfs:
                // Not supported for now
                return std::make_unique<ScanfsValidator>();
                // LCOV_EXCL_START
            default:
                // This code path is a logic error. Either ParseCallExpression erroneously returned INVALID, or
                // this switch is missing a branch for a validator.
                Log(LogLevel::ERROR) << "FmtSpecMismatchChecker: Missing validator for a format string of kind "
                                     << FormatStringKindToCStr(kind) << "\n";
                return nullptr;
                // LCOV_EXCL_STOP
        }
    }

    static Problem FormatProblem(const Detect& detect, FmtType expected, std::string_view formatSpec,
                                 bool isAux = false)
    {
        string_view argument = isAux ? "auxiliary " : "";
        string_view pointer = expected.IsTypedPointer() ? "pointer to " : "";

        string message = StrUtils::ReplacePlaceholders(
            StrLocales::GetStringLocale(detect.message),
            {string{formatSpec}, to_string(expected.bitWidth / 8), string{argument}, string{pointer}});

        Replacements replacements;
        replacements.emplace_back(std::move(message));
        return Problem{replacements, {}, detect.kind};
    }

    struct ParsedCallExpression {
        FormatStringKind kind;
        const StringLiteralExpression& formatString;
        span<const Node*> formatArgs;
    };

    optional<ParsedCallExpression> ParseCallExpression(const DfaState& formatStringState)
    {
        if (!formatStringState.HasAnnotation(myFormatStringKind)) {
            return nullopt;
        }

        auto kind =
            formatStringState.GetAnnotationSources(myFormatStringKind).begin()->first.GetUserData<FormatStringKind>();

        if (kind == FormatStringKind::INVALID) {
            // No annotation indicating format string kind
            return nullopt;
        }

        auto* formatString = formatStringState.GetNodeAs<StringLiteralExpression>();
        if (formatString == nullptr) {
            // Node is not a string literal. This case is not supported
            return nullopt;
        }

        auto* call = formatString->GetParentOfType<CallExpression>();
        if (call == nullptr) {
            // FormatString not in a call expression? this shouldn't happen
            return nullopt;  // LCOV_EXCL_LINE
        }

        auto args = GetFormatArguments(formatStringState.GetFuncState().GetContext(), *call);
        if (!args.has_value()) {
            return nullopt;
        }

        return ParsedCallExpression{kind, *formatString, *args};
    }

    /**
     * @brief Find format arguments in the call expression.
     *
     * Either uses a "FormatStringArgs" annotation as a begin marker, or assumes that all variadic arguments are for the
     * format string.
     */
    optional<span<const Node*>> GetFormatArguments(const DfaFunctionContext& ctx, const CallExpression& call)
    {
        // Find the .annotations.json annotations for the function in the call expression
        DfaFunctionContext* funcCtx = ctx.GetCalleeContext(ctx.GetInstruction(call));
        if (funcCtx == nullptr) {
            return nullopt;  // LCOV_EXCL_LINE
        }

        const FunctionDecl* func = call.GetFunction();
        if (func == nullptr) {
            return nullopt;  // LCOV_EXCL_LINE
        }
        auto& params = func->GetParams();
        const vector<const Node*>& args = call.GetArguments();

        // Iterate over .annotation.json annotations in an attempt to find FormatStringArgs
        int firstFormatArg = params.size();  // Default first arg at the start of variadic arguments
        for (int i = 0; i < funcCtx->GetSizeOfAnnotations(); ++i) {
            const std::vector<Annotation>& paramAnnots = funcCtx->GetAnnotation(i);
            if (std::find_if(paramAnnots.begin(), paramAnnots.end(),
                             [&](auto& ann) { return ann.GetKind() == myFormatStringArgsKind; }) != paramAnnots.end()) {
                firstFormatArg = i - GetIndexingOffset(call);
                break;
            }
        }

        // Handle possible FormatStringArgs placed at the return type or hidden *this
        if (firstFormatArg < 0) {
            const FunctionDecl* decl = call.GetFunction();

            HCXX::Log(HCXX::LogLevel::ERROR) << "Function " << (decl ? decl->GetQualifiedName() : "<broken function>")
                                             << " contains a \"FormatStringArgs\" annotation in an invalid position\n";
            return nullopt;
        }

        auto begin = const_cast<const Node**>(args.data() + firstFormatArg);
        auto end = const_cast<const Node**>(args.data() + args.size());
        return span{begin, end};
    }

    Annotation::Kind myFormatStringKind;
    Annotation::Kind myFormatStringArgsKind;
};

unique_ptr<Checker> CreateFmtSpecMismatchChecker()
{
    return make_unique<FmtSpecMismatchChecker>();
}
