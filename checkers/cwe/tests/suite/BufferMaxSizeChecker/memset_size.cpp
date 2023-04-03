#include <stdint.h>
#include <string.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(uint32_t size)
{
    char buff[10];
    if (size <= 10) {
        memset(buff, 0, size);  // no error here
    }
    memset(buff, 0, size);  // error here
}
