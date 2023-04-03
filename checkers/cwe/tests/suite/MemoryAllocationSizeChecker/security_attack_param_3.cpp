#include <stdint.h>
#include <stdlib.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(size_t size)
{
    auto newsize = size + 42;
    auto p = malloc(newsize);
    free(p);
}
