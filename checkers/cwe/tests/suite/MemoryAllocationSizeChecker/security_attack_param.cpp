#include <stdint.h>
#include <stdlib.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(size_t size)
{
    void* ptr = malloc(size);  // error here
    free(ptr);
}
