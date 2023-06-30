#include <memory>

class B {
    int field;
};

class A {
public:
    A() : ptr(new B()) {}
    B* ptr;
};

int main()
{
    A val;
    return 0;
}
