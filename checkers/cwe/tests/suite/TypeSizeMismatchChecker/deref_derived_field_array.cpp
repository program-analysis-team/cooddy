#include <stdlib.h>

struct Base {
    int field1;
    int lol;
};

struct Derived : public Base {
    int field2;
};

int foo(void* b)
{
    int v = ((Base*)b)->field1;    // no error here
    return ((Derived*)b)->field2;  // TypeSize error here, but no OOB
}

int main()
{
    Base* b = new Base[2];
    return foo(b);
}
