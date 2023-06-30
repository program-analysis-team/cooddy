#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char* a;
};

int test(A* arg)
{
    A* a = (struct A*)malloc(sizeof(struct A));
    memcpy(a, arg, sizeof(A));
    auto val = a->a;

    return 0;
}
