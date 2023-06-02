/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/InitListExpression.h>
#include <ast/ParamVarDecl.h>
#include <ast/StringLiteralExpression.h>

namespace HCXX {
void FunctionDecl::Print(std::string& source, int entryOffset) const
{
    auto range = GetRange();
    std::ostringstream name;
    auto splittedName = StrUtils::Split(myQualifiedName, ':');
    name << myRetTypeName << " " << splittedName.back() << "(";
    auto sep = "";
    for (auto& param : GetParams()) {
        name << sep << param->GetType().GetPrintableName() << " " << VarDecl::GetVarName(param);
        sep = ", ";
    }
    name << ")";
    SafePrint(source, entryOffset, name.str(), range);
    if (myBody != nullptr) {
        auto bodyRange = myBody->GetRange();
        SafePrint(source, entryOffset, "{", bodyRange);
        SafePrint(source, entryOffset, "}", bodyRange, bodyRange.Size() - 1);
        myBody->Print(source, entryOffset);
    }
}
std::optional<std::string> FunctionDecl::GetAttribute(std::string_view attrName) const
{
    if (attrName == "Name") {
        return GetName();
    }

    if (attrName == "FuncKind") {
        return FunctionKindToCStr(GetFuncKind());
    }

    if (attrName == "IsDefined") {
        return std::to_string(IsDefined());
    }

    if (attrName == "IsDeleted") {
        return std::to_string(IsDeleted());
    }

    if (attrName == "IsDefaulted") {
        return std::to_string(IsDefaulted());
    }

    if (attrName == "ArgsAmount") {
        return std::to_string(GetParams().size());
    }

    return Node::GetAttribute(attrName);
}

std::string FunctionDecl::GetSignature(unsigned int formatFlags) const
{
    std::ostringstream sig;

    if (formatFlags & SFF_WITH_RETURN_TYPE) {
        sig << GetRetTypeName() << " ";
    }

    sig << GetQualifiedName() << "(";

    auto sep = "";
    for (auto& param : GetParams()) {
        sig << sep << param->GetTypeName();

        auto& name = param->GetName();
        if (!name.empty() && formatFlags & SFF_WITH_PARAM_NAMES) {
            sig << " " << name;
        }

        sep = ", ";
    }
    sig << ")";

    return sig.str();
}

std::vector<const RefExpression*> FunctionDecl::FindDeclarationUsages(const Node& declNode) const
{
    std::vector<const RefExpression*> result;
    Traverse([&](const Node& node) {
        if (auto ref = Node::Cast<RefExpression>(&node); ref != nullptr && ref->GetDeclaration() == &declNode) {
            result.emplace_back(ref);
        }
    });
    return result;
}

};  // namespace HCXX
