#include <stdlib.h>

struct Base {
    int field1;
    int a;
    int b;
};

struct Derived : public Base {
    int field2;
};

int foo(void* b)
{
    ((Base*)b)->field1 = 5;        // No Out of Bounds here
    return ((Derived*)b)->field2;  // Out of bounds here
}

int main()
{
    int* b = new int[3];
    return foo(b);
}
