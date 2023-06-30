#include <stdlib.h>

void foo2(void** p)
{
    void* p1 = 0;
    p1 = malloc(100);
    *p = p1;
}

void foo(int v)
{
    void* p = 0;
    foo2(&p);

    if (v == 10) {
        return;  // error here
    }
    free(p);
}
