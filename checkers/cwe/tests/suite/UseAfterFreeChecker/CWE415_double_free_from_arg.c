
#include "stdlib.h"

void foo(int* a)
{
    free(a);
    free(a);
}
