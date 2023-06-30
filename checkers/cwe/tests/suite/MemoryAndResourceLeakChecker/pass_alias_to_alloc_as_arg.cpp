#include <stdlib.h>

void foo(int *x) {free(x);}

void baz()
{
    int *x = (int*)malloc(0x1234);
    foo(x);
}
