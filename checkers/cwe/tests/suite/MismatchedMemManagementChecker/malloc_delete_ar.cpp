#include <stdlib.h>

void foo()
{
    int *ptr;
    ptr = (int *)malloc(sizeof(int));
    delete[] ptr;
}
