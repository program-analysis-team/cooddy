#include <stdlib.h>

struct A {
    int x;
};

void Foo(A *a)
{
    a->x = 42;
}

void Bar(A *a)
{
    free(a);
    Foo(a);
}
