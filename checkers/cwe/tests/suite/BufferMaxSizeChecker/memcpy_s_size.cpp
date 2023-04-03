#include <stdint.h>

int memcpy_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(uint32_t size)
{
    char buff[10];
    if (size <= 10 && size >= 3) {
        memcpy_s(buff, size, "123", 3);  // no error here
    }
    if (size >= 3) {
        memcpy_s(buff, size, "123", 3);  // error here
    }
}
