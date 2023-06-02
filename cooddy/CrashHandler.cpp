/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "CrashHandler.h"

#include <cxxabi.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string_view>
#include <thread>
#include <unordered_set>

#if !defined(_WIN32) && __has_include(<unistd.h>)
/// strsignal and related functions are only available on POSIX.
#include <cstring>
#define POSIX_SYSTEM
#endif

#ifdef USE_STACKTRACE_LIB
#include <backtrace.h>
#include <execinfo.h>
#endif

#ifdef _WIN32
// clang-format off
#include "Windows.h"
#include "DbgHelp.h"
// clang-format on
#endif  // _WIN32

namespace {

std::string_view SignalToStr(int signal)
{
#ifdef POSIX_SYSTEM
    const char* name = strsignal(signal);
    if (name) {
        return name;
    }
    return "Unknown signal";
#else
    // All signals known by standard C++.
    switch (signal) {
        case SIGTERM:
            return "Terminated";
        case SIGSEGV:
            return "Segmentation fault";
        case SIGINT:
            return "Interrupt";
        case SIGILL:
            return "Illegal instruction";
        case SIGABRT:
            return "Aborted";
        case SIGFPE:
            return "Floating point exception";
        default:
            return "Unknown signal";
    }
#endif  // POSIX_SYSTEM
}

#ifdef USE_STACKTRACE_LIB

void PrintStacktrace()
{
    std::cerr << "Stacktrace:\n";
    backtrace_state* state = backtrace_create_state("", 1, 0, 0);
    if (state != nullptr) {
        std::cerr << "Thread ID: " << std::this_thread::get_id() << std::endl;
        backtrace_print(state, 0, stderr);
    } else {
        std::cerr << "Error: failed to print stacktrace: libbacktrace error\n";
    }
}

#elif defined(_WIN32)

/**
 * @brief Get the initial frame for walking the stack, along with the machine type.
 *
 * @return std::pair<machine-type, initial-frame>
 */
std::pair<DWORD, STACKFRAME64> GetInitialFrame(const CONTEXT& context)
{
    std::pair<DWORD, STACKFRAME64> result;
    DWORD& machine_type = result.first;
    STACKFRAME64& frame = result.second;

#ifdef _M_IX86  // x86
    machine_type = IMAGE_FILE_MACHINE_I386;
    frame.AddrPC.Offset = context.Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.Esp;
    frame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64    // x86-64
    machine_type = IMAGE_FILE_MACHINE_AMD64;
    frame.AddrPC.Offset = context.Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.Rsp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.Rsp;
    frame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64   // Intel Itanium
    machine_type = IMAGE_FILE_MACHINE_IA64;
    frame.AddrPC.Offset = context.StIIP;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.IntSp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrBStore.Offset = context.RsBSP;
    frame.AddrBStore.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.IntSp;
    frame.AddrStack.Mode = AddrModeFlat;
#elif _M_ARM64  // ARM64
    machine_type = IMAGE_FILE_MACHINE_ARM64;
    frame.AddrPC.Offset = context.Pc;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.Fp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.Sp;
    frame.AddrStack.Mode = AddrModeFlat;
#else
    static_assert(false, "could not determine machine type");
#endif

    return result;
}

void PrintStacktrace()
{
    HANDLE this_process = GetCurrentProcess();
    HANDLE this_thread = GetCurrentThread();

    CONTEXT context{};
    RtlCaptureContext(&context);
    auto [machine_type, frame] = GetInitialFrame(context);

    SymInitialize(this_process, nullptr, true);

    while (true) {
        bool ok = StackWalk64(machine_type, this_process, this_thread, &frame, &context, nullptr,
                              SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
        if (!ok)
            break;

        // IMAGEHLP_SYMBOL64 has a flexible array member at the end
        struct {
            IMAGEHLP_SYMBOL64 data = {};
            char _[1024] = {};
        } symbol_holder{};
        auto& symbol = symbol_holder.data;

        symbol.SizeOfStruct = sizeof(symbol_holder);
        symbol.MaxNameLength = 1024;
        DWORD64 displacement;
        if (SymGetSymFromAddr64(this_process, frame.AddrPC.Offset, &displacement, &symbol)) {
            std::cerr << symbol.Name << "\n";
        } else {
            std::cerr << "<unknown>\n";
        }

        IMAGEHLP_LINE64 line = {sizeof(IMAGEHLP_LINE64)};
        DWORD offset_from_symbol = 0;
        if (SymGetLineFromAddr64(this_process, frame.AddrPC.Offset, &offset_from_symbol, &line)) {
            std::cerr << "\t" << line.FileName << ":" << line.LineNumber;
        } else {
            std::cerr << "\t???:0\n";
        }
    }
}

#else

void PrintStacktrace() {}

#endif  // USE_STACKTRACE_LIB

}  // namespace

void SignalHandler(int sig)
{
    std::cerr << "Error: signal " << sig << " (" << SignalToStr(sig) << ")\n";
    PrintStacktrace();
    std::exit(128 + sig);
}

void TerminateHandler()
{
    std::exception_ptr eptr = std::current_exception();
    if (eptr) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception& e) {
            std::cerr << "Error: Unhandled " << abi::__cxa_current_exception_type()->name() << ": " << e.what() << "\n";
        } catch (...) {
            std::cerr << "Error: Unknown unhandled exception\n";
        }
    } else {
        std::cerr << "Terminate called without an active exception\n";
    }

    PrintStacktrace();
    std::exit(1);
}

void SetCrashHandler()
{
    std::set_terminate(TerminateHandler);
    std::unordered_set<int32_t> signals{SIGINT, SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGTERM};  // ISO C99 signals
#ifdef __MINGW32__
    signals.insert({SIGABRT_COMPAT, SIGBREAK, SIGABRT2});  // mingw signals
#endif
#ifdef POSIX_SYSTEM
    signals.insert({SIGHUP, SIGQUIT, SIGTRAP, SIGBUS, SIGSYS, SIGPIPE, SIGALRM});  // POSIX signals
#endif
    for (auto signal : signals) {
        std::signal(signal, SignalHandler);
    }
}
