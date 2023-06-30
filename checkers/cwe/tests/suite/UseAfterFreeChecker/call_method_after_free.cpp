#include <stdlib.h>

struct A {
    int x;
    void Bar() {}
};

void Foo(A* a)
{
    free(a);
    a->Bar();
}
