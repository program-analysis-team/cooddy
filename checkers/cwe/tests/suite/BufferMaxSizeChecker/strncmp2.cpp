#include <stdint.h>

int strncmp(char* dstBuff, char* srcBuff, int srcSize);

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK
struct A {
    char buff[20];
    char buff2[10];
};
void a(uint32_t size) {
    A buffer;
    if(size<20)
        strncmp(buffer.buff, buffer.buff2, size);  // error here
}
void EXTER_ATTACK b(uint32_t size) {
    a(size);
}
