#include <stdlib.h>

struct Foo {
    int *p;
};

void func()
{
    exit(1);
}

int main()
{
    Foo foo;
    int v;
    if (v != 1) {
        foo.p = 0;
        if (foo.p == 0)
            func();
    }
    return *foo.p;
}
