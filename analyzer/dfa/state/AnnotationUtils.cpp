/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <ast/CallExpression.h>
#include <ast/CxxConstructExpression.h>
#include <ast/CxxMemberCallExpression.h>
#include <ast/CxxMethodDecl.h>
#include <dfa/AnnotationUtils.h>

namespace HCXX {

int GetIndexingOffset(const CallExpression& call)
{
    switch (call.GetKind()) {
            // LCOV_EXCL_START: HCAT-2777
        case Node::Kind::CXX_CONSTRUCT_EXPRESSION:
        case Node::Kind::CXX_MEMBER_CALL_EXPRESSION:
            return 2;
            // LCOV_EXCL_STOP

        default:
            return 1;
    }
}

const Node* GetArgumentFromAnnotationPos(const CallExpression* call, uint32_t pos)
{
    if (call == nullptr) {
        return nullptr;  // LCOV_EXCL_LINE: HCAT-2777
    }

    uint32_t argOffset{};
    if (auto constructExpr = Node::Cast<CxxConstructExpression>(call); constructExpr != nullptr) {
        // C++ constructors
        switch (pos) {
                // LCOV_EXCL_START: HCAT-2777
            case 0:
                return nullptr;  // Return value (none for constructors)
            case 1:
                return call;
                //  LCOV_EXCL_STOP
            default:
                argOffset = pos - 2;
        }
    } else if (auto memberCall = Node::Cast<CxxMemberCallExpression>(call); memberCall != nullptr) {
        // Member functions
        switch (pos) {
            case 0:
                return call;  // Return value
                //  LCOV_EXCL_START
            case 1:
                return memberCall->GetObject();  // Hidden *this pointer
                //  LCOV_EXCL_STOP
            default:
                argOffset = pos - 2;
        }
    } else {
        // Free functions
        switch (pos) {
            case 0:
                return call;  // Return value
            default:
                argOffset = pos - 1;
        }
    }
    return argOffset < call->GetArguments().size() ? call->GetArguments()[argOffset] : nullptr;
}

uint32_t GetAnnotationPosFromArgument(const CallExpression* call, const Node* arg)
{
    if (call == nullptr || arg == nullptr) {
        return 0;  // LCOV_EXCL_LINE: HCAT-2777
    }

    uint32_t argOffset{};
    if (auto constructExpr = Node::Cast<CxxConstructExpression>(call); constructExpr != nullptr) {
        if (arg == call) {
            return 1;
        }
        argOffset = 2;
    } else if (auto memberCall = Node::Cast<CxxMemberCallExpression>(call); memberCall != nullptr) {
        if (call == arg) {
            return 0;
        } else if (arg == memberCall->GetObject()) {
            return 1;
        }
        argOffset = 2;
    } else {
        if (call == arg) {
            return 0;
        }
        argOffset = 1;
    }
    auto argPos = call->FindArgument(*arg);
    return argPos ? argOffset + *argPos : UINT32_MAX;
}

std::optional<uint32_t> GetParamPosInFunction(const FunctionDecl& function, const Node* param)
{
    auto paramOffset = 1;
    if (auto methodDecl = Node::Cast<CxxMethodDecl>(&function); methodDecl != nullptr && !methodDecl->IsStatic()) {
        paramOffset++;
    }
    for (uint32_t i = 0; i < function.GetParams().size(); ++i) {
        if (param == function.GetParams()[i]) {
            return i + paramOffset;
        }
    }
    // LCOV_EXCL_START
    return std::nullopt;
    // LCOV_EXCL_STOP
}

}  // namespace HCXX
