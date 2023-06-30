#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char* a;
};

int main()
{
    A* a;
    a = (struct A*)malloc(sizeof(struct A));
    a->b;
    return 0;
}
