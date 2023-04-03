#include <string.h>

#include <cstdlib>

void bar1(const char* str)
{
    int size = atoi(str);
    malloc(size);
}

EXTER_ATTACK void foo(const char* p)
{
    char str[100];
    strncpy(str, p, sizeof(str) - 1);
    bar1(str);
}
