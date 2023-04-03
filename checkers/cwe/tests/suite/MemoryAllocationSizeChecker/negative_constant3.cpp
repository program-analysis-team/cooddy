#include <stdlib.h>

#include <cstdint>

void* bar1(int64_t sz)
{
    return malloc(sz);
}

void bar2()
{
    uint64_t sz = 0x8000000000000000ULL;
    void* ptr = bar1(sz);
    free(ptr);
}
