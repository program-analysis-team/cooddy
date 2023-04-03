#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

int printf(const char* const _Format, ...);
int fprintf(FILE* const _Stream, const char* const _Format, ...);
int sprintf(char* _Buffer, const char* _Format, ...);
int snprintf(char* const _Buffer, const size_t _BufferCount, const char* const _Format, ...);

int printf_s(const char* const _Format, ...);
int fprintf_s(FILE* const _Stream, const char* const _Format, ...);
int sprintf_s(char* const _Buffer, const size_t _BufferCount, const char* const _Format, ...);
int snprintf_s(char* const _Buffer, const size_t _BufferCount, const size_t _MaxCount, const char* const _Format, ...);

int wprintf(const wchar_t* const _Format, ...);
int fwprintf(FILE* const _Stream, const wchar_t* const _Format, ...);
int swprintf(wchar_t* const _Buffer, const size_t _BufferCount, const wchar_t* const _Format, ...);

int wprintf_s(const wchar_t* const _Format, ...);
int fwprintf_s(FILE* const _Stream, const wchar_t* const _Format, ...);
int swprintf_s(wchar_t* const _Buffer, const size_t _BufferCount, const wchar_t* const _Format, ...);

int vprintf(const char* const _Format, va_list _ArgList);
int vfprintf(FILE* const _Stream, const char* const _Format, va_list _ArgList);
int vsprintf(char* _Buffer, const char* _Format, va_list _ArgList);
int vsnprintf(char* const _Buffer, const size_t _BufferCount, const char* const _Format, va_list _ArgList);

int vprintf_s(const char* const _Format, va_list _ArgList);
int vfprintf_s(FILE* const _Stream, const char* const _Format, va_list _ArgList);
int vsprintf_s(char* const _Buffer, const size_t _BufferCount, const char* const _Format, va_list _ArgList);
int vsnprintf_s(char* const _Buffer, const size_t _BufferCount, const size_t _MaxCount, const char* const _Format,
                va_list _ArgList);

int vwprintf(const wchar_t* const _Format, va_list _ArgList);
int vfwprintf(FILE* const _Stream, const wchar_t* const _Format, va_list _ArgList);
int vswprintf(wchar_t* const _Buffer, const size_t _BufferCount, const wchar_t* const _Format, va_list _ArgList);

int vwprintf_s(const wchar_t* const _Format, va_list _ArgList);
int vfwprintf_s(FILE* const _Stream, const wchar_t* const _Format, va_list _ArgList);
int vswprintf_s(wchar_t* const _Buffer, const size_t _BufferCount, const wchar_t* const _Format, va_list _ArgList);

int f1(FILE* f, char* s, wchar_t* ws)
{
    printf("%d", 1);
    fprintf(f, "%d", 1);
    sprintf(s, "%d", 1);
    snprintf(s, 10, "%d", 1);

    printf_s("%d", 1);
    fprintf_s(f, "%d", 1);
    sprintf_s(s, 10, "%d", 1);
    snprintf_s(s, 10, 10 - 1, "%d", 1);  // As defined in custom annotations file

    wprintf(L"%d", 1);
    fwprintf(f, L"%d", 1);
    swprintf(ws, 10, L"%d", 1);

    wprintf_s(L"%d", 1);
    fwprintf_s(f, L"%d", 1);
    swprintf_s(ws, 10, L"%d", 1);
}

int f2(FILE* f, char* s, wchar_t* ws, ...)
{
    va_list args;
    va_start(args, ws);

    vprintf("%d", args);
    vfprintf(f, "%d", args);
    vsprintf(s, "%d", args);
    vsnprintf(s, 10, "%d", args);

    vprintf_s("%d", args);
    vfprintf_s(f, "%d", args);
    vsprintf_s(s, 10 - 1, "%d", args);
    vsnprintf_s(s, 10, 10 - 1, "%d", args);

    vwprintf(L"%d", args);
    vfwprintf(f, L"%d", args);
    vswprintf(ws, 10, L"%d", args);

    vwprintf_s(L"%d", args);
    vfwprintf_s(f, L"%d", args);
    vswprintf_s(ws, 10, L"%d", args);
}
