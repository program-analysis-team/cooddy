#include <stdint.h>

void foo()
{
    int arr[4] = {0, 0, 0, 0};
    uint32_t num = 1;
    uint32_t i = 0;

    for(i = num; i > 0; --i) {
        arr[i - 1];
    }
}
