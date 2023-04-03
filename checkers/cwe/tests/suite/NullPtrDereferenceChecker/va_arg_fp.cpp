#include <stdarg.h>

int func(bool flag, ...)
{
    int* p = 0;
    va_list vl;
    va_start(vl, flag);
    if (flag) {
        p = (int*)va_arg(vl, int*);
    }
    return !flag ? 0 : *p;
}

int main()
{
    bool flag;
    return func(flag, "");
}
