#include <stdlib.h>

int my_alloc(void** p)
{
    void* p1 = malloc(100);
    if (p1 == 0) {
        return 0;
    }
    p[0] = p1;
    return 1;
}

void my_free(void** p)
{
    if (p == 0) {
        return;
    }
    if (p[0] != 0) {
        free(p[0]);
        p[0] = 0;
    }
}

int foo(void** p, int v)
{
    if (!my_alloc(p)) {
        return 0;
    }
    if (v == 100) {
        my_free(p);
        return 0;
    }
    return 1;
}

void boo(int v)
{
    void* arr[2];
    if (!foo(arr, v)) {
        return;
    }
    my_free(arr);
}
