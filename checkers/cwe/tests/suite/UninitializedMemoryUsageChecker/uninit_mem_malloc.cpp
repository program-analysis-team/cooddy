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
    A* a = (A*)malloc(sizeof(struct A));
    if (a == nullptr) {  // NO UNINIT here
        return -1;
    }
    auto val = a->b;  // uninit here

    return 0;
}
