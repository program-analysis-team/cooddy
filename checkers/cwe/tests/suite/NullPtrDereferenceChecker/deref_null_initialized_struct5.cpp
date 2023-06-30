#include <string.h>
typedef struct A {
    int pointer[10];
} A;

void foo()
{
    int b = 0;
    A* a;
    memset(a, 0, sizeof(A));
    memcpy(a->pointer, &b, sizeof(int));
}
