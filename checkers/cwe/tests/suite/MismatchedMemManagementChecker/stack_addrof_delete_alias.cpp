#include <stdlib.h>

void foo()
{
    int ptr;
    int* ptr2 = &ptr;
    delete (int*)ptr2;
}
