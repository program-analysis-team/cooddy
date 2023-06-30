#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SZ 10

struct Foo {
    int a;
    int b[10];
    int c[];
};

int foo1()
{
    unsigned sz = sizeof(struct Foo) + sizeof(int) * SZ;
    struct Foo* p = (struct Foo*)malloc(sz);
    if (p) {
        memset(p, 0, sz);
        p->a = 10;     // ok
        p->b[0] = 10;  // ok
        p->c[0] = 42;  // ok
        free(p);
    }
    return 0;
}

void foo2()
{
    unsigned sz = sizeof(struct Foo) + sizeof(int) * SZ;
    struct Foo* p = (struct Foo*)malloc(sz);
    if (p) {
        memset(p, 0, sz);
        memset(p->c, 0, sizeof(int) * SZ);  // ok
        free(p);
    }
}
