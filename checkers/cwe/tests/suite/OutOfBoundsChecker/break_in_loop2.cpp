#include "break_in_loop2.h"

#include <stdint.h>

const int MAX_VAL = (int)(VAL);

void foo(uint32_t num)
{
    int arr[10], arr1[10];
    uint32_t cnt = 0;
    for (uint32_t i = 0; i < num; ++i) {
        if (cnt > MAX_VAL) {
            break;
        }
        arr[cnt] = arr1[i];
        cnt++;
    }
}
