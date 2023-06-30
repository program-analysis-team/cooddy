#include <stdlib.h>

void foo()
{
    int ptr[5];
    free(ptr);
}
