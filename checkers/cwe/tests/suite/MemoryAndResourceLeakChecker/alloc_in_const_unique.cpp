#include <memory>

class B {
public:
    int field;
};

class A {
public:
    A() : ptr(new B()) {};
    B* ptr;
};

int main()
{
    A val;
    return 0;
}
