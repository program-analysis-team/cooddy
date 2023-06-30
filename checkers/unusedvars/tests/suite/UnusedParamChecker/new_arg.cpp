#include <new>

class A {
public:
    A(int a) : myA(a){};

private:
    int myA;
};

void test(A* par, int arg)
{
    new (par) A(arg);
}
