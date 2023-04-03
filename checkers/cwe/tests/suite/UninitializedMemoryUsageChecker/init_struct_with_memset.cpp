#include "string.h"

struct A {
    int a;
    char* b;
};

int main(int argc, char* argv[])
{
    struct A a;
    memset(&a, 0, sizeof(struct A));
    return a.a;
}
