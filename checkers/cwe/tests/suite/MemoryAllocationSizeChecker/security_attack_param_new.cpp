#include <stdint.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

void EXTER_ATTACK foo(uint32_t size)
{
    int* ptr = new int[size];  // error here
    delete[] ptr;
}
