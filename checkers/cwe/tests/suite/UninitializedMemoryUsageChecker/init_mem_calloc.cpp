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
    A* a = (struct A*)calloc(sizeof(struct A), 0);
    if (!a) {
        return -1;
    }
    auto val = a->a;

    return 0;
}
