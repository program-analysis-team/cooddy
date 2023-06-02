/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef HASH_IMPL_HEADER_FILE
#define HASH_IMPL_HEADER_FILE

#include <functional>
#include <utility>

namespace HCXX {

namespace Details {

//  LCOV_EXCL_START
//  It is gcov bug. It thinks that HashCombine(size_t& seed) is never called
inline size_t HashCombine(size_t seed)
{
    return seed;
}
//  LCOV_EXCL_STOP

template <class T, class... Types>
size_t HashCombine(size_t seed, T&& obj, Types&&... args)
{
    std::hash<std::remove_const_t<std::remove_reference_t<T>>> hasher;
    //  The idea is taken from boost::hash_combine
    seed ^= hasher(obj) + 0x9e3779b9 + (seed << 6u) + (seed >> 2u);

    return HashCombine(seed, std::forward<Types>(args)...);
}

}  // namespace Details

template <class... Types>
size_t CalcHash(Types&&... args)
{
    return Details::HashCombine(0, std::forward<Types>(args)...);
}

}  // namespace HCXX

#endif  //  #ifndef HASH_IMPL_HEADER_FILE
