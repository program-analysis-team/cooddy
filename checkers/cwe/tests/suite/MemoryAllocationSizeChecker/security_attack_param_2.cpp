#include <stdint.h>
#include <stdlib.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(size_t size)
{
    int i = 0;
    while (i < size) {
        void* ptr = malloc(1024);  // error here
        i++;
        free(ptr);
    }
}
