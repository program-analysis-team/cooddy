#include <stdint.h>

int some_func();

int foo()
{
    int arr[10] = {0};
    uint16_t idx = some_func();
    if (idx >= 10)
        return 0;
    return arr[idx];
}
