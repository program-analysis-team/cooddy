#include <stdint.h>

void foo(bool flag)
{
    uint8_t buff[22] = {0};
    uint16_t *p16 = ((uint16_t *)buff) + 1;
    uint32_t *p32 = ((uint32_t *)p16) + 1;
    uint64_t *p64 = ((uint64_t *)p32) + 1;

    uint64_t v = *p64;                   // no error here
    uint8_t v1 = *(uint8_t *)(p64 + 1);  // error here (2 + 4 + 8 + 8 + 1 > 22)
}
