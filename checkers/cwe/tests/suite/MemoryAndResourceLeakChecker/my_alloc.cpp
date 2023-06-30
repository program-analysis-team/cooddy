#include <stdlib.h>

void* my_malloc_void(unsigned size)
{
    return malloc(size);
}

int* my_malloc_int(unsigned size)
{
    return (int*)malloc(size);
}

int* my_new_int(int x)
{
    return new int(x);
}
