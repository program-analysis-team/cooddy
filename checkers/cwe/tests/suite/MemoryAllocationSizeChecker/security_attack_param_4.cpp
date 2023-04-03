#include <stdint.h>
#include <stdlib.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK foo(size_t size)
{
    void* ptr = nullptr;
    if (size < INT32_MAX) {
        ptr = malloc(size);
    }
    free(ptr);
}
