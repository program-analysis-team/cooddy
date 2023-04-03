
#include "stdlib.h"

void foo()
{
    int* a = new int;
    delete a;
    free(a);
}
