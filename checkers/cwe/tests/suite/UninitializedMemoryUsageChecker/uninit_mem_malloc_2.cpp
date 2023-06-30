#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char a;
};

char test()
{
    A* a = (struct A*)malloc(sizeof(struct A));
    if (!a) {  // NO UNINIT here
        return 0;
    }
    a->b = 234;

    return a->a;// uninit here
}
