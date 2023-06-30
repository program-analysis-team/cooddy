#include <iostream>
#include <stdarg.h>

int bar(char* src, char* format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    int ret = vsscanf(src, format, arglist);
    va_end(arglist);
    return ret;
}

void foo()
{
    char buf[2456];
    int arr[123];
    int a = fread(buf, 2456, 2456, fopen("file", "r"));
    arr[a] = 123;    // error here
    char str[123];
    int b = bar(buf, "%s", str);
    arr[b];          // error here
}
