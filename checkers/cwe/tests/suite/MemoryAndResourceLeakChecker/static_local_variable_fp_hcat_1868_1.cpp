#include <stdlib.h>

struct A {
};

A* GetInstance()
{
    static A* a = (A*)malloc(sizeof(struct A));
    return a;
}

void foo()
{
    GetInstance();  // no error
}
