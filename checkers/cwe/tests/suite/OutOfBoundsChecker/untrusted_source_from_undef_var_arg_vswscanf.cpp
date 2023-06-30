#include <iostream>
#include <stdarg.h>

int bar(wchar_t* src, wchar_t * format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    int ret = vswscanf(src, format, arglist);
    va_end(arglist);
    return ret;
}

void foo()
{
    wchar_t buf[2456];
    int arr[123];
    int a = fread(buf, 2456, 2456, fopen("file", "r"));
    arr[a] = 123;    // error here
    char str[123];
    int b = bar(buf, L"%s", str);
    arr[b];          // error here
}
