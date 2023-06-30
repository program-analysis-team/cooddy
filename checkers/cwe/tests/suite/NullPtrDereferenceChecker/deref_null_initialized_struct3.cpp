#include <string.h>
typedef struct A {
    int pointer;
} A;

int* foo()
{
    A* a;
    memset(a, 0, sizeof(A));
    return &a->pointer;
}
