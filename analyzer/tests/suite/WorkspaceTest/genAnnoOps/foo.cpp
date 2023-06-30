#include "stdlib.h"

int boo();
int someAnnotatedFunc();

void system_func_call()
{
    malloc(0x1234);
}

int foo()
{
    int p[23];
    return p[boo()] + 123;
}
