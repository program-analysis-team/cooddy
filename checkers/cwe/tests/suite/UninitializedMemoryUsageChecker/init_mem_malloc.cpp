#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char* a;
};

int main(int argc, char* argv[])
{
    A* a = (struct A*)malloc(sizeof(struct A));
    if (!a) {
        return -1;
    }
    a->b = 234;
    auto val = a->b;

    return 0;
}
