#include <stdlib.h>

struct S {
    void* p[1];
};

int my_alloc(void** p)
{
    p[0] = malloc(100);
    return 1;
}

void my_free(void** p)
{
    if (p == 0) {
        return;
    }
    if (p[0] != 0) {
        free(p[0]);
    }
}

void foo(int v)
{
    S s;
    my_alloc(s.p);
    my_free(s.p);
}
