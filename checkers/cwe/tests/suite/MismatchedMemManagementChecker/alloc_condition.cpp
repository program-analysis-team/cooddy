#include <stdlib.h>

int* foo(bool flag)
{
    if (flag) {
        return (int*)malloc(sizeof(int));
    }
    return new int(100);
}

void boo()
{
    int* ptr = foo(false);
    free(ptr);
}
