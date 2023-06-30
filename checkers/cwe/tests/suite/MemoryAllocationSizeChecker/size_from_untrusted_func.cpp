#include <stdint.h>
#include <stdlib.h>

size_t bar();

void foo()
{
    auto size = bar();
    void* ptr = malloc(size);  // error here
    free(ptr);
}
