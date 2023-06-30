#include <stdint.h>

void foo(uint32_t num)
{
    int arr[10], arr1[10];
    uint32_t cnt = 0;
    for (uint32_t i = 0; i < num; ++i) {
        if (cnt > 10) {
            break;
        }
        arr[cnt] = arr1[i];  // OOB here
        cnt++;
    }
}
