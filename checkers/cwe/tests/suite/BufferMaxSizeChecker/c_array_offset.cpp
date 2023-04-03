#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

void EXTER_ATTACK f(const char* buffer)
{
    size_t buflen = strlen(buffer);
    char g_overflow_st[8];

    if (buflen > 8) {
        return;
    }

    memcpy(&g_overflow_st[1], buffer, buflen);
}
