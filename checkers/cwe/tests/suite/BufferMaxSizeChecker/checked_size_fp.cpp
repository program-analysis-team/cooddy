#include <stdint.h>
#include <string.h>

int strncpy_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

static void foo(char* dstBuff, uint32_t dstMax, char* srcBuff, uint32_t srcSize)
{
    if (dstMax < srcSize) {
        return;
    }
    strncpy_s(dstBuff, dstMax, srcBuff, srcSize);
}

void bar(char* name)
{
    foo(name, strlen(name), "Some long string", 16);
}
