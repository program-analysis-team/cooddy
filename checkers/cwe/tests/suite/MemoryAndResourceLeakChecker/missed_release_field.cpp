#include <cstdlib>

struct A {
    int *p1;
};

void baz(A *a) {}

void foo()
{
    A a;
    a.p1 = (int *)malloc(42);
    baz(&a);
}
