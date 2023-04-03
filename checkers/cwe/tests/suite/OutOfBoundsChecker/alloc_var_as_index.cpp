#include <malloc.h>
#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(const char* name)
{
    int len = strlen(name);
    if (len <= 0) {
        return;
    }
    char* mem = (char*)malloc(len);
    mem[len - 1] = 0;  // no problem here
    mem[len] = 0;      // # problem here
    free(mem);
}
