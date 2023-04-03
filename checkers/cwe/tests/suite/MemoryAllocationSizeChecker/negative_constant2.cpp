#include <stdlib.h>

#include <cstdint>

void* bar1(uint64_t sz)
{
    return malloc(sz);
}

void bar2()
{
    int sz = -5;
    void* ptr = bar1(sz * 10);  // -50 implicitly cast to uint64_t and can lead to heap overflow
    free(ptr);
}
