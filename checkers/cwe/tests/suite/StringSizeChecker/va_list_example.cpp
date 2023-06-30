#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <cstdarg>

int vscanf_s(const char* format, ...);

int call_vscanf_s(const char *format, ...)
{
    int result;
    va_list arglist;
    va_start(arglist, format);
    result = vscanf_s(format, arglist);
    va_end(arglist);
    return result;
}

int main( void )
{
    char  s[81];
    int result = call_vscanf_s("%s ", s, 10);
    return result;
}
