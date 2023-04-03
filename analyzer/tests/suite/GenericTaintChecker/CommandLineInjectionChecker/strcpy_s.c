#include <stdlib.h>
#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

int EXTER_ATTACK f(char* str)
{
    char buf[10];
    strcpy_s(buf, 10, str);

    system(buf);
}
