/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_UTILS_MEMORY_H_
#define COODDY_ANALYZER_INCLUDE_UTILS_MEMORY_H_
#include <memory>

/**
 * @brief Make a shared pointer from an object that is NOT supposed to be deleted.
 *
 * Shared pointers created by this functions DO NOT delete the pointed-to object when the last instance goes out of
 * scope. This is intended as a workaround when you need to pass stack variables to functions taking shared_ptr.
 *
 * Warning: it is the caller's responsibility to manage the lifetime of obj. All memory management of shared_ptr is
 * disabled. Use-after-free and memory leaks can occur if the object is managed improperly.
 *
 * @param obj Object whose lifetime is managed by someone else.
 * @return std::shared_ptr pointing to obj.
 */
template <typename T>
inline std::shared_ptr<T> MakeSharedNoDelete(T& obj) noexcept
{
    // Use shared_ptr's aliasing constructor to store obj without allocating a control block.
    return std::shared_ptr<T>{std::shared_ptr<T>{}, &obj};
}

#endif  // COODDY_ANALYZER_INCLUDE_UTILS_MEMORY_H_
