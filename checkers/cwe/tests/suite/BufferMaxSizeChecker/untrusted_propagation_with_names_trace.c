#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

#define pool_strncpy(dest, destMax, src, count) strncpy((dest), (src), (count))

void EXTER_ATTACK foo(char *src) {
    char name[10];
    pool_strncpy(name, sizeof(name), src, strlen(src));
}
