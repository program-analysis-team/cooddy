/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CxxRecordDecl.h>
#include <ast/InitListExpression.h>
#include <ast/StringLiteralExpression.h>
#include <ast/VarDecl.h>

#include "ast/EnumConstantDecl.h"

namespace HCXX {

void VarDecl::Print(std::string& source, int entryOffset) const
{
    auto t = TypedNode::GetType();
    auto name = VarDecl::GetVarName(this);
    SafePrint(source, entryOffset, t.GetPrintableName() + " " + name, GetRange());
    if (myArraySizeExpr != nullptr) {
        auto sizeRange = myArraySizeExpr->GetRange();
        SafePrint(source, entryOffset, "[", sizeRange, -1);
        SafePrint(source, entryOffset, "]", sizeRange, sizeRange.Size());
        myArraySizeExpr->Print(source, entryOffset);
    }
    if (myInitializer != nullptr) {
        auto initRange = myInitializer->GetRange();
        SafePrint(source, entryOffset, "=", initRange, -2);
        myInitializer->Print(source, entryOffset);
    }
}
// LCOV_EXCL_START
std::string VarDecl::Dump() const
{
    auto t = TypedNode::GetType();
    std::string res = Node::Dump() + ", sizeOfType: " + std::to_string(GetSizeOfType()) +
                      (IsMatrixArray() ? ", matrix" : "") +
                      (t.IsArray() ? ", arrayNum: " + std::to_string(GetNumOfElementsInArray()) : "") + ", " + t.Dump();
    return res;
}
// LCOV_EXCL_STOP

std::string VarDecl::GetVarName(const Node* decl)
{
    auto tu = const_cast<TranslationUnit*>(decl->GetTranslationUnit());
    // LCOV_EXCL_START
    if (tu == nullptr) {
        return "_";
    }
    // LCOV_EXCL_STOP
    auto enumDecl = Node::Cast<EnumConstantDecl>(decl);
    if (enumDecl != nullptr) {
        return std::to_string(enumDecl->GetValue());
    }
    // LCOV_EXCL_START
    auto fieldDecl = Node::Cast<FieldDecl>(decl);
    if (fieldDecl != nullptr) {
        std::string name;
        name += static_cast<char>(fieldDecl->GetRecordUniqueId() % 26 + 97);
        return name;
    }
    // LCOV_EXCL_STOP
    auto paramVarDecl = Node::Cast<ParamVarDecl>(decl);
    if (paramVarDecl != nullptr && paramVarDecl->GetName().empty()) {
        const_cast<ParamVarDecl*>(paramVarDecl)->SetName(tu->GetNextName());
        return paramVarDecl->GetName();
    }
    auto varDecl = Node::Cast<VarDecl>(decl);
    if (varDecl != nullptr && varDecl->GetName().empty()) {
        int id = varDecl->GetUniqueId();
        if (id > 0 && id < UINT32_MAX) {
            const_cast<VarDecl*>(varDecl)->SetName("g_" + std::to_string(varDecl->GetUniqueId()));
        } else {
            const_cast<VarDecl*>(varDecl)->SetName(tu->GetNextName(varDecl->IsLocalVariableDeclaration()));
        }
        return varDecl->GetName();
    }
    auto name = decl->GetName();
    return name.empty() ? "_" : std::move(name);
}

uint32_t VarDecl::GetNumOfElementsInArray() const
{
    auto typedNode = Node::Cast<TypedNode>(myArraySizeExpr);
    if (typedNode != nullptr) {
        return typedNode->GetSLimitedValue();
    }
    auto initNode = Node::Cast<InitListExpression>(myInitializer);
    if (initNode != nullptr) {
        return initNode->GetInitList().size();
    }
    return 0;
}

void VarDecl::Init()
{
    if (GetType().IsConstant()) {
        auto typedInitNode = Node::Cast<TypedNode>(myInitializer);
        if (typedInitNode != nullptr && typedInitNode->IsConstExpr()) {
            myConstValue = typedInitNode->GetSLimitedValue();
        }
    }
}

bool VarDecl::IsConstExpr() const
{
    return myConstValue != UNDEF_VALUE;
}

int64_t VarDecl::GetSLimitedValue() const
{
    return myConstValue;
}

bool VarDecl::IsMemoryBuffer(uint32_t& sizeofBuffer) const
{
    auto type = GetType();
    if (!type.IsPointer() && (type.IsConstantArray() || Node::Cast<RecordDecl>(type.GetDeclaration()) != nullptr)) {
        sizeofBuffer = GetSizeOfType();
        return true;
    }
    if (myInitializer != nullptr) {
        if (auto literal = Node::Cast<StringLiteralExpression>(myInitializer->GetInnerNode()); literal) {
            sizeofBuffer = literal->GetValue().size();
            return true;
        }
    }
    sizeofBuffer = 0;
    return type.IsArray();
}

};  // namespace HCXX
