#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char* a;
};

A* func()
{
    auto a = (struct A*)malloc(sizeof(struct A));
    a->b = 123;
    return a;
}

int main(int argc, char* argv[])
{
    A* a = func();
    auto val = a->b;
    return 0;
}
