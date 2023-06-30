#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char* a;
};

A* func();

int main(int argc, char* argv[])
{
    A* a = (struct A*)malloc(sizeof(struct A));
    a = func();
    auto val = a->a;
    return 0;
}
