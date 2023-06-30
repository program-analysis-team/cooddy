#include <stdlib.h>

struct A {
    int* ptr;
};

int main()
{
    A* a = (A*)malloc(sizeof(A));
    free(a->ptr);
    return *a->ptr;
}
