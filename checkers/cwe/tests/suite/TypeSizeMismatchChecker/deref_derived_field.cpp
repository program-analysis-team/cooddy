#include <stdlib.h>

struct Base {
    int field1;
};

struct Derived : public Base {
    int field2;
};

int foo(Base* b)
{
    int v = b->field1;             // no error here
    return ((Derived*)b)->field2;  // Type size Mismatch
}

int main()
{
    Base* b;
    b = (Base*)malloc(sizeof(Base));
    return foo(b);
}
