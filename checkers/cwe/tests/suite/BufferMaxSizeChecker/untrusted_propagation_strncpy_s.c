#include <stdlib.h>
#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

void EXTER_ATTACK Foo(char *src) {
    char name[10];
    strncpy_s(name, sizeof(name), src, sizeof(name));
    int idx = atoi(name);
    char dest[10] = {0};
    strncpy(dest, name, idx);
}
