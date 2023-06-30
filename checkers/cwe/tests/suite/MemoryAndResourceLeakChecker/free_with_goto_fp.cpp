#include <stdlib.h>

void my_free(char** p)
{
    if (*p == 0) {
        return;
    }
    free(*p);
    *p = 0;
}

void foo(int v)
{
    char* p = 0;
    if (v == 10) {
        goto err;
    }
    p = (char*)malloc(100);
    if (p == 0) {
        goto err;
    }
err:
    my_free(&p);
}
