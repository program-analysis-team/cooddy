#include <string.h>

typedef void (*tfunc)(void *p, int *rc);
tfunc g_func;

typedef struct _A {
    int **pptr;
} A;

typedef struct _B {
    int *ptr;
} B;

void get_ptr(int v, int **ptr, int *rc)
{
    if (v == 10) {
        *rc = 1;
        return;
    }
    A a;
    a.pptr = ptr;
    g_func(&a, rc);
}

int foo(int v)
{
    B b = {0};
    int rc;
    get_ptr(v, &b.ptr, &rc);
    if (rc == 0)
        return *b.ptr;  // no error here
    return 0;
}
