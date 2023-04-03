#include <stdint.h>

uint32_t g_arr[64] = {100, 200, 300};

void foo(uint32_t val1, uint32_t val2, uint32_t val3)
{
    auto ulShift = (val1 << 3) & 0x18;
    auto ulCrcResidue = (g_arr[val2] >> ulShift) & 0xff;
    ulCrcResidue = ulCrcResidue ^ val3;

    g_arr[ulCrcResidue >> 2] = 10;
}
