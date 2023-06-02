/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CurrentNodeMatcherCompiler.h"

#include <sstream>

namespace {
std::string MakeRegExString()
{
    std::ostringstream oss;
    //  Start of the line + spaces + optional node type
    oss << "^\\s*([a-zA-Z_][a-zA-Z0-9_]*)?";

    //  Spaces and '[' which starts the attributes
    oss << "\\s*(\\[";
    {
        //  The first attribute
        oss << "\\s*@([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*'([^']*)'";
        //  Other attributes
        oss << "((\\s+AND\\s+@[a-zA-Z_][a-zA-Z0-9_]*\\s*=\\s*'[^']*')*)?";
    }
    //  Spaces and ']' which ends the attributes
    oss << "\\s*\\])?";

    //   End of line or start of another term
    oss << "\\s*($|/)";

    return oss.str();
}

std::string MakeRegExStringForNonFirstAttributes()
{
    std::ostringstream oss;
    //  The first(at the moment) attribute in the sequence
    oss << "\\s+AND\\s+@([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*'([^']*)'";
    //  Other attributes
    oss << "((\\s+AND\\s+@[a-zA-Z_][a-zA-Z0-9_]*\\s*=\\s*'[^']*')*)?";
    return oss.str();
}

template <typename SubMatch>
std::string_view SubMatchToStringView(const SubMatch& subMatch)
{
    return std::string_view(subMatch.first, subMatch.length());
};

}  // namespace

namespace HCXX::Hast::Details {

CurrentNodeMatcherCompiler::CurrentNodeMatcherCompiler()
    : myRegex(MakeRegExString()), myNonFirstAttributesRegex(MakeRegExStringForNonFirstAttributes())
{}

std::optional<CurrentNodeMatcherCompiler::CompileResult> CurrentNodeMatcherCompiler::Compile(
    std::string_view expression) const
{
    std::cmatch match;  // TODO:  HCAT-677: replace regex parsing with formal grammar parsing
    if (!std::regex_search(&*expression.begin(), &*expression.end(), match, myRegex,
                           std::regex_constants::match_continuous)) {
        return std::nullopt;
    }

    //  If we processing something like "CxxCtorDecl[@IsCopy='0' AND @IsMove='0' AND @IsExplicit='0']"
    //  the match result should contain following elements(index->value):
    //  0 - whole expression: CxxCtorDecl[@IsCopy='0' AND @IsMove='0' AND @IsExplicit='0']
    //  1 - node type: CxxCtorDecl
    //  2 - all attributes with square braces(the element isn't used):
    //      [@IsCopy='0' AND @IsMove='0' AND @IsExplicit='0']
    //  3 - name of the first atttribute: IsCopy
    //  4 - value of the first atttribute: 0
    //  5 - all attributes but the first one: AND @IsMove='0' AND @IsExplicit='0'
    //  6 - the last attribute(the element isn't used): AND @IsExplicit='0'
    //  7 - end of line or slash which starts the next term:

    if (match[5].matched) {
        if (!match[3].matched || !match[4].matched) {
            //  LCOV_EXCL_START
            //  Attributes starts from the 'AND', for example,
            //  MyNodeType[AND @MyAttr='MyValue']
            return std::nullopt;
            //  LCOV_EXCL_STOP
        }
    }

    if (!match[1].matched && !match[3].matched && !match[4].matched) {
        //  Neither node type nor attribute are provided
        return std::nullopt;
    }

    std::vector<std::pair<std::string, std::string>> attributes;
    if (match[3].matched && match[4].matched) {
        attributes.emplace_back(SubMatchToStringView(match[3]), SubMatchToStringView(match[4]));
    }

    if (match[5].matched) {
        if (!ProcessNonFirstAttributes(SubMatchToStringView(match[5]), attributes)) {
            //  LCOV_EXCL_START
            return std::nullopt;
            //  LCOV_EXCL_STOP
        }
    }

    std::string_view nodeType;
    if (match[1].matched) {
        nodeType = SubMatchToStringView(match[1]);
    }

    size_t usedSymbolsAmount = match[7].first - &*expression.begin();
    CompileResult res{std::string(nodeType), std::move(attributes), usedSymbolsAmount};
    return res;
}
bool CurrentNodeMatcherCompiler::ProcessNonFirstAttributes(
    std::string_view str, std::vector<std::pair<std::string, std::string>>& attributes) const
{
    while (!str.empty()) {
        std::cmatch match;
        if (!std::regex_search(&*str.begin(), &*str.end(), match, myNonFirstAttributesRegex,
                               std::regex_constants::match_continuous)) {
            //  LCOV_EXCL_START
            //   It looks rather strange, we have already checked the format of the string
            return false;
            //  LCOV_EXCL_STOP
        }

        //  If we processing something like "AND @IsMove='0' AND @IsExplicit='0' AND @IsDefaulted='0'"
        //  the match result should contain following elements(index->value):
        //  -   0   -   the string itself: AND @IsMove='0' AND @IsExplicit='0' AND @IsDefaulted='0'
        //  -   1   -   first attribute name: IsMove
        //  -   2   -   first attribute value: 0
        //  -   3   -   all attributes but the first one: AND @IsExplicit='0' AND @IsDefaulted='0'
        //  -   4   -   the last attribute(we don't use it): AND @IsDefaulted='0'

        attributes.emplace_back(SubMatchToStringView(match[1]), SubMatchToStringView(match[2]));
        str = SubMatchToStringView(match[3]);
    }
    return true;
}

}  // namespace HCXX::Hast::Details
