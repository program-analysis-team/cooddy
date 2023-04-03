#include <stdlib.h>

void* initFunc(bool flag)
{
    return flag != 0x987 ? malloc(0x123) : (void*)0x1234;
}
