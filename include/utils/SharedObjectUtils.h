/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_SHAREDOBJECTSUTILS_H
#define COODDY_SHAREDOBJECTSUTILS_H

#ifdef _WIN32
struct IUnknown;
#include "windows.h"
#undef ERROR
#else
#include <dlfcn.h>
#endif

namespace SharedObjectUtils {

#ifdef _WIN32
using LibHandle = HINSTANCE;
using LibSymbol = FARPROC;
LibHandle LoadSharedObject(const std::string& sharedObjectPath)
{
    if (!sharedObjectPath.empty()) {
        return LoadLibraryA((sharedObjectPath + ".dll").c_str());
    }
    return nullptr;
}
template <class TSymbol>
TSymbol GetSymbol(LibHandle lib, const char* symbolName)
{
    if (lib == nullptr || symbolName == nullptr) {
        return nullptr;
    }
    // COODDY_SUPPRESS
    return (TSymbol)GetProcAddress(lib, symbolName);
}
// LCOV_EXCL_START
bool FreeSharedObject(LibHandle lib)
{
    if (lib != nullptr) {
        return FreeLibrary(lib);
    }
    return true;
}
// LCOV_EXCL_STOP
#else

using LibHandle = void*;
using LibSymbol = void*;
LibHandle LoadSharedObject(const std::string& sharedObjectPath)
{
    return dlopen((sharedObjectPath + ".so").c_str(), RTLD_NOW);
}
template <class TSymbol>
TSymbol GetSymbol(LibHandle lib, const char* symbolName)
{
    if (lib == nullptr || symbolName == nullptr) {
        return nullptr;  // LCOV_EXCL_LINE: HCAT-2777
    }
    // COODDY_SUPPRESS
    return (TSymbol)dlsym(lib, symbolName);
}
// LCOV_EXCL_START
bool FreeSharedObject(LibHandle lib)
{
    if (lib != nullptr) {
        return !dlclose(lib);
    }
    return true;
}
// LCOV_EXCL_STOP
#endif

}  // namespace SharedObjectUtils

#endif  // COODDY_SHAREDOBJECTSUTILS_H
