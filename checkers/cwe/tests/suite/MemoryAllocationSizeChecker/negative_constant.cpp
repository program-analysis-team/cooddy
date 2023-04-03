#include <stdlib.h>

#include <cstdint>

void* bar1(uint32_t sz)
{
    return malloc(sz);
}

void bar2()
{
    int sz = -1;
    void* ptr = bar1(sz);  // -1 implicitly cast to UINT32_MAX and can lead to heap overflow
    free(ptr);
}
