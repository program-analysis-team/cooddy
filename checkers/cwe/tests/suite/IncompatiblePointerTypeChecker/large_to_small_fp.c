#include <stdio.h>

void foo()
{
    int c;
    int b = 0x41424344;
    memcpy(&c, &b, sizeof(b));
}
