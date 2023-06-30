#include <stdlib.h>

int foo2(void** p, int v)
{
    void* p1 = 0;
    p1 = malloc(100);
    if (v == 100) {
        free(p1);
        return 0;
    }
    *p = p1;
    return 1;
}

void foo(int v)
{
    void* p = 0;
    if (foo2(&p, v) == 0) {
        return;
    }
    free(p);
}
