#include <stdlib.h>

#include <cstdint>

void* bar1(int32_t sz)
{
    return malloc(sz);
}

void bar2()
{
    int sz = -1;
    void* ptr = bar1(sz);
    free(ptr);
}
