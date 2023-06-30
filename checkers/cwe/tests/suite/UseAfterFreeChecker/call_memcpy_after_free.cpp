#include <memory.h>
#include <stdlib.h>

struct A {
    int x;
};

void Foo(A* a, A* b)
{
    free(a);
    memcpy(a, b, sizeof(A));
}
