#include <memory.h>
#define SIZE_BUF 8

void Foo(char *buf, char *src)
{
    memcpy_s(buf, 4, src, SIZE_BUF);
}

int main()
{
    char dest[SIZE_BUF] = {};
    char source[SIZE_BUF] = "TESTSTG";
    Foo(dest, source);
}
