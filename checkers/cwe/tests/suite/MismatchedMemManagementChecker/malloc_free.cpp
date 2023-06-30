#include <stdlib.h>

void foo()
{
    int *ptr = (int*)malloc(0x123);
    free(ptr);
}
