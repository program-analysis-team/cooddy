#include <stdlib.h>

struct A {
    int a;
};

int main()
{
    A* val = (A*)malloc(sizeof(A));
    return val->a;  // error here
}
