/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_DFA_ANNOTATIONUTILS_H_
#define COODDY_ANALYZER_SOURCE_DFA_ANNOTATIONUTILS_H_
#include <ast/CallExpression.h>
#include <ast/Node.h>

namespace HCXX {

/**
 * @brief Get an offset 'k' such that argumentIndex + k = annotationIndex.
 *
 * @return 1 for free functions, 2 for member functions and constructors.
 */
int GetIndexingOffset(const CallExpression& call);

/**
 * @brief Get an AST node corresponding to an n-th argument of any call expression.
 *
 * For the purposes of this function, arguments include the return value and the hidden *this argument.
 *
 * @param call Call expression.
 * @param pos Argument position. Return value is pos 0, normal arguments, including the hidden *this argument, start
 * from 1.
 * @return A pointer to an AST node for this argument. If the number of arguments is less than pos, returns nullptr.
 * The return value (pos == 0) node is the CallExpression node itself. For C++ constructors, return value node is
 * nullptr, and the *this pointer is the CallExpression node itself.
 */
const Node* GetArgumentFromAnnotationPos(const CallExpression* call, uint32_t pos);

/**
 * @brief Get the index of an argument or return value in a FunctionAnnotation from a CallExpression node.
 *
 * See the reverse function GetArgumentFromAnnotationPos for more information.
 *
 * @return argument index or UINT32_MAX if the argument was not found.
 */
uint32_t GetAnnotationPosFromArgument(const CallExpression* call, const Node* arg);

/**
 * @brief Get the index of a parameter or return value in a FunctionAnnotation from a FunctionDecl node.
 */
std::optional<uint32_t> GetParamPosInFunction(const FunctionDecl& function, const Node* param);

}  // namespace HCXX

#endif  // COODDY_ANALYZER_SOURCE_DFA_ANNOTATIONUTILS_H_
