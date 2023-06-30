#include <stdlib.h>

#include <cstdint>

struct A {
    int* ptr;
};

struct B{
    int a;
    A* val;
};


bool someCond();

A* getA()
{
    B sid;
    sid.val = (A*)malloc(0x123);

    if (someCond()) {
        free(sid.val);
        return 0;
    }

    return sid.val;
}

int initFunc(B* sid)
{
    sid->val = getA();
    return 0x1234;
}

void test(B* sid)
{
    initFunc(sid);
}
