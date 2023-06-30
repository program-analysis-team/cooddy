#include <string.h>

void TestBadCase01()
{
    char buf[16] = {0};
    char src[10] = "test";
    memcpy_s(buf, sizeof(buf), src, sizeof(buf));
}
