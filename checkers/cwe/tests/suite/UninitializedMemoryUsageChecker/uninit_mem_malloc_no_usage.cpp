//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#include <cstdint>

struct A {
    int b;
    char* a;
};

A* malloc(int);

int main(int argc, char* argv[])
{
    A* a = malloc(sizeof(struct A));
    if (a == nullptr) {
        return -1;
    }

    return 0;
}
