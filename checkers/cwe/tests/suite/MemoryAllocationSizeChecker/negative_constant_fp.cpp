#include <stdlib.h>

#include <cstdint>

void* bar1(uint16_t sz)
{
    return malloc(sz); // no error here (64536 is not so big)
}

void bar2()
{
    int sz = -1;
    void* ptr = bar1(sz);
    free(ptr);
}
