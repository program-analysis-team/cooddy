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
    return ((Derived*)b)->field2;  // No out of bounds here because there are 2 allocated items.
                                   // anyway it is still a weird code
}

int main()
{
    Base* b = new Base[2];
    return foo(b);
}
