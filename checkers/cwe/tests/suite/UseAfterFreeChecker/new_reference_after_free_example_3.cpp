#include <stdlib.h>

int foo(int rnd)
{
    int* a;
    a = (int*)malloc(100 * sizeof(int));
    free(a);
    int b = 12;
    if (rnd > 0) {
        a = (int*)malloc(rnd * sizeof(int));
        b = *a;
    }
    b = b + *a;
    return b;
}
