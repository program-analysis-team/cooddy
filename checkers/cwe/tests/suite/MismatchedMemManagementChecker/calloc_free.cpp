#include <stdlib.h>

void foo()
{
    int *ptr = (int*)calloc(1,0x123);
    free(ptr);
}
