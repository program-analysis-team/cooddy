#include <stdint.h>

#define MAX_VALUE 1000

void foo(uint32_t v, uint32_t c)
{
    int arr[256];
    if (v <= c || v >= MAX_VALUE) {
        return;
    }
    arr[MAX_VALUE - v] = 0;
}
