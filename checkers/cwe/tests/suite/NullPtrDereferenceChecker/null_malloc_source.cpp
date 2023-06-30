#include <stdlib.h>

int foo()
{
    int* p = (int*)malloc(sizeof(int));
    return *p;
}
