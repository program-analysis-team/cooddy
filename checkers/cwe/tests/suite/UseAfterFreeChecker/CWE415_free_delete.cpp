
#include "stdlib.h"

void foo()
{
    int* a = (int*)malloc(sizeof(int));
    free(a);
    delete(a);
}
