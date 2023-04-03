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
    return ((Derived*)b)->field2;  // no Type size Mismatch
}

int main()
{
    Base* b;
    b = (Derived*)malloc(sizeof(Derived)); // TODO take a look at cast expression, and take type size from it
    //ALLOC_DERIVED(b);
    return foo(b);
}
