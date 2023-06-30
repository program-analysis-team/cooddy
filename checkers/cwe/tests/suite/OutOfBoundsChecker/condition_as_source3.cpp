#include <stdint.h>

#define MAX_VALUE 1000

void foo(uint32_t v)
{
    int arr[MAX_VALUE];
    if (v > MAX_VALUE) {
        return;
    }
    arr[v] = 0;
}
