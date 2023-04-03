#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

void EXTER_ATTACK f(const char* buffer)
{
    char b[8];
    memcpy(&b[1], buffer, strlen(buffer));
}
