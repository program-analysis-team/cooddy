#include <stdint.h>

uint32_t some_func();

void foo()
{
    int64_t val = 0;
    uint32_t idx = some_func();
    while (idx--) {
        val += idx;
    }
}
