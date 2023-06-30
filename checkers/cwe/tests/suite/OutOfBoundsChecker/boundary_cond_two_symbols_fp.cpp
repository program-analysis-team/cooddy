#include <stdint.h>

void foo(uint32_t idx1, uint32_t idx2)
{
    int arr[10];

    if (idx1 + idx2 > 10) {
        return;
    }
    arr[idx1] = 10;
}
