#include <stdint.h>
#include <stdlib.h>

void foo(size_t size)
{
    void* ptr = malloc(size);  // no error here
    free(ptr);
}
