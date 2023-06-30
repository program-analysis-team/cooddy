#include <string.h>
#define XXX_memcpy_s memcpy_s

void TestBadCase01()
{
    char buf[16] = {0};
    char src[10] = "test";
    XXX_memcpy_s(buf, sizeof(buf), src, sizeof(buf));
}
