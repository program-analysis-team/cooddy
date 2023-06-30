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
    if (!a) {
        return -1;
    }
    *a = *arg;
    auto val = a->b;

    return 0;
}
