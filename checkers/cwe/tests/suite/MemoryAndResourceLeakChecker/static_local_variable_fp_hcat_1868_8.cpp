#include <stdlib.h>

struct A {
};

A* GetInstance()
{
    static A* a = 0;
    a = (A*)realloc(a, sizeof(struct A));
    return a;
}

void foo()
{
    GetInstance();  // no error
}
