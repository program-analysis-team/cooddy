#include <stdlib.h>

void foo()
{
    int* ptrBefore = (int*)malloc(0x123);
    int *ptr = (int*)realloc(ptrBefore,0x123);
    free(ptr);
}
