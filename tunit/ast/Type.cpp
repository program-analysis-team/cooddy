/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md

#include "ast/Type.h"

#include "ast/CxxRecordDecl.h"

namespace HCXX {
// LCOV_EXCL_START
std::string Type::Dump() const
{
    std::string res;
    if (myDeclNode != nullptr) {
        res += "Decl: " + myDeclNode->Dump();
    }
    res += " sizeInBytes: " + std::to_string(GetSizeInBits() / 8);
    if (IsPointer()) {
        res += ", pointer";
    }
    if (IsArray()) {
        res += ", array";
    }
    if (IsConstantArray()) {
        res += ", constant array";
    }
    if (IsVariableArray()) {
        res += ", variable array";
    }
    if (IsReference()) {
        res += ", reference";
    }
    if (IsIntegralType()) {
        res += ", integral";
    }
    if (IsFloatingType()) {
        res += ", floating";
    }
    if (IsConstant()) {
        res += ", constant";
    }
    if (IsUnsigned()) {
        res += ", unsigned";
    }
    return res;
}
std::string Type::GetPrintableName() const
{
    std::string name;
    auto decl = Node::Cast<RecordDecl>(GetDeclaration());
    if (decl != nullptr) {
        name = "S_" + std::to_string(decl->GetUniqueId());
    } else {
        if (IsIntegralType()) {
            name = "int";
        } else if (IsFloatingType()) {
            name = "float";
        } else {
            name = "void";
        }
        if (IsPointer()) {
            name += "*";
        }
    }
    return name;
}
// LCOV_EXCL_STOP
}  // namespace HCXX
