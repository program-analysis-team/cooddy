#include "string.h"

struct A {
    int a;
    char* b;
};

void init(struct A* a)
{
    memset(a, 0, sizeof(struct A));
}

int main(int argc, char* argv[])
{
    struct A a;
    init(&a);
    return a.a;
}
