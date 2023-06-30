#include <stdlib.h>

void foo()
{
    int *ptr[5] = {0};

    free(ptr[1]);
}
