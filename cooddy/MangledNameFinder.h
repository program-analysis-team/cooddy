/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_TOOL_MANGLEDNAMEFINDER_H_
#define COODDY_ANALYZER_TOOL_MANGLEDNAMEFINDER_H_

#include <Analyzer.h>
#include <Parser.h>
#include <ast/FunctionDecl.h>
#include <utils/Log.h>
#include <utils/StrUtils.h>

#include <unordered_set>
#include <utility>

using namespace HCXX;

struct MangledNameFinder : Analyzer::Consumer {
public:
    using ResultType = std::vector<std::pair<std::string, std::string>>;

    MangledNameFinder(std::string_view funcName, const Parser& parser) : myParser(parser)
    {
        if (StrUtils::StartsWith(funcName, "::")) {
            myFuncName = funcName.substr(2);
            myFullyQualifiedNameOnly = true;
        } else {
            myFuncName = funcName;
            myFullyQualifiedNameOnly = false;
        }
    }

    bool ShouldAnalyzeUnit(TranslationUnit& unit) override
    {
        auto match = [&](std::string_view pattern, std::string_view query) {
            return myFullyQualifiedNameOnly ? StrUtils::MatchQualifiedName(pattern, query)
                                            : StrUtils::MatchPartiallyQualifiedName(pattern, query);
        };

        for (auto* node : unit.GetNodes()) {
            auto decl = Node::Cast<FunctionDecl>(node);
            auto addToSignatures = [&]() {
                mySignatures.emplace(decl->GetSignature(FunctionDecl::SFF_WITH_RETURN_TYPE),
                                     myParser.GetMangledNameWithoutAbi(decl->GetMangledName()));
            };

            if (decl == nullptr) {
                continue;
            }

            std::string qualifiedName = decl->GetQualifiedName();

            if (match(qualifiedName, myFuncName)) {
                addToSignatures();
            }
        }
        return false;
    }

    ResultType GetResult() const
    {
        std::vector result(mySignatures.begin(), mySignatures.end());
        std::sort(result.begin(), result.end(), [](auto& lhs, auto& rhs) { return lhs.first < rhs.first; });

        return result;
    }

    static std::string FormatResult(std::string_view funcName, const ResultType& items)
    {
        std::ostringstream ss;

        // Print helpful message when no signatures were found
        if (items.size() == 0) {
            ss << "Found no signatures matching \"" << funcName << "\".\n";
            return ss.str();
        }

        // Columns in output are automatically resized. For two colums we need to calculate size of the left column.
        constexpr static std::string_view signatureColName = "SIGNATURE";
        auto size = [](auto& val) { return val.first.size(); };
        size_t signatureColWidth = size(
            *std::max_element(items.begin(), items.end(), [&](auto& lhs, auto& rhs) { return size(lhs) < size(rhs); }));
        signatureColWidth = std::max(signatureColWidth, signatureColName.size());

        // Padding on the right for strings in the left column
        auto padRight = [](std::string_view str, size_t targetSize, char padding = ' ') {
            std::string result(targetSize, padding);
            result.replace(0, str.size(), str);
            return result;
        };

        // Printing result
        ss << "Mangled names for \"" << funcName << "\":\n"
           << padRight(signatureColName, signatureColWidth) << "  MANGLED NAME\n";

        for (auto& [signature, mangledName] : items) {
            ss << padRight(signature, signatureColWidth) << "  "
               << (mangledName.empty() ? "<template function>" : mangledName) << "\n";
        }

        return ss.str();
    }

    std::string FormatResult() const
    {
        return FormatResult(myFuncName, GetResult());
    }

private:
    struct StringPairHash {
        size_t operator()(const std::pair<std::string, std::string>& val) const
        {
            std::hash<std::string> h;
            return h(val.first) ^ h(val.second);
        }
    };

    std::string myFuncName;
    const Parser& myParser;
    bool myFullyQualifiedNameOnly = false;
    std::unordered_set<std::pair<std::string, std::string>, StringPairHash> mySignatures;
};

#endif  // COODDY_ANALYZER_TOOL_MANGLEDNAMEFINDER_H_
