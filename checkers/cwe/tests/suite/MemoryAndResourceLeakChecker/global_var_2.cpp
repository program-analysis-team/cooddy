#include <cstdlib>

struct Foo {
    int* x;
};

Foo* f;

void baz()
{
    Foo f2;
    f2.x = (int*)malloc(42);
    *f = f2;
}
