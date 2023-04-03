#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

void EXTER_ATTACK f(const char* buffer)
{
    size_t buflen = strlen(buffer);
    int g_overflow_st[10];  // 10*4 = 40 bytes

    if (buflen > 10 * sizeof(int) - 1) {
        return;
    }
    // buflen is <= 39 bytes

    // copy into (10-1)*4 = 36 bytes
    memcpy(&g_overflow_st[1], buffer, buflen);
}
