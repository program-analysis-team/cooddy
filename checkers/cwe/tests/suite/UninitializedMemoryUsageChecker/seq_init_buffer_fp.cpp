#include <string.h>

typedef struct _S {
    int a;
    int b;
} S;

int foo(int v, char *buff, int size)
{
    char *curBuff = buff;
    int curSize = size;
    if (v == 0) {
        memset(curBuff, 0, 4);
        curSize -= 4;
        curBuff += 4;
    }
    while (curSize != 0) {
        memset(curBuff, 0, 4);
        curSize -= 4;
        curBuff += 4;
    }
    return 0;
}

int bar(int v)
{
    S s;
    foo(v, &s, sizeof(s));
    return s.a;
}
