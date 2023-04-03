#include <stdlib.h>

#include <cstdint>

void bar1(uint32_t idx)
{
    int arr[0x10];
    arr[idx] = 0; // OOB here
}

void bar2()
{
    int idx = -1;
    bar1(idx);
}
