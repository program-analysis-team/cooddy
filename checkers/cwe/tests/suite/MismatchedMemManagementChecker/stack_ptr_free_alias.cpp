#include <stdlib.h>

void foo()
{
    int ptr[5];
    int* ptr2 = ptr;
    free(ptr2);
}
