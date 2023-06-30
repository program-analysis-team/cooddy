#include <stdlib.h>

#include <cstdint>
struct A {
    int* ptr;
};

bool someCond();

A* getA()
{
    A* sid;
    sid = (A*)malloc(0x123);

    if (someCond()) {
        free(sid);
        sid = 0;
        return sid;
    }

    return sid;
}

int initFunc(A** sid)
{
    *sid = getA();
    return 0;
}

void test(A** sid)
{
    initFunc(sid);
}
