#include <string.h>
#define XXX_memcpy_s memcpy_s

void TestBadCase01(char *buf, size_t bufSize, char *msg)
{
    XXX_memcpy_s(buf, bufSize, msg, bufSize);
}

int main(int argc, char **argv)
{
    char buf[16] = {0};
    char src[10] = "test";
    if (argc > 1) {
        TestBadCase01(buf, sizeof(buf), src);
    }
    return 0;
}
