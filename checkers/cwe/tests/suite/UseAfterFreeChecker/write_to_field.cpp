#include <stdlib.h>

struct A {
    int x;
};

void Foo(A* a)
{
    free(a);
    a->x = 42;
}
