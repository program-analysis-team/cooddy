#include <stdint.h>

int foo(uint32_t v)
{
    int* p = 0;
    if (v == 0)
        return 0;
    if (v > 0)
        return 1;
    return *p;  // no error here
}

int bar(int32_t v)
{
    int* p = 0;
    if (v == 0)
        return 0;
    if (v > 0)
        return 1;
    return *p;  // error here
}
