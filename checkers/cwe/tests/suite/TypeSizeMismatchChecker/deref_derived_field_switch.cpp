#include <stdlib.h>

class Base {
public:
    Base(int kind);
    int kind;
};

class DerivedA : public Base {
public:
    DerivedA(): Base(1) {}
    int field2;
};

class DerivedB : public Base {
public:
    DerivedB(): Base(2) {}
    int q;
    long long field2;
};

long long foo(Base* b)
{
    if (b->kind == 2)                   // no error here
        return ((DerivedB*)b)->field2;  // Type size mismatch
    else
        return 0;
}

int main()
{
    DerivedA* b = (DerivedA*)malloc(sizeof(DerivedA));
    return foo(b);
}
