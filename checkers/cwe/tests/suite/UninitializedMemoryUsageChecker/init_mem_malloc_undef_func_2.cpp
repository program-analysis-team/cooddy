#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char* a;
};

void func(A**);

int main(int argc, char* argv[])
{
    A* a = (struct A*)malloc(sizeof(struct A));
    func(&a);
    auto val = a->a;
    return 0;
}
