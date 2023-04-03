#include <stdlib.h>

#define ALLOC_DERIVED(ptr) \
        ptr = (Derived*)malloc(sizeof(Derived))

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
    ALLOC_DERIVED(b);
    return foo(b);
}
