#include <new>

struct A {
    int* p = 0;

    void func()
    {
        p = new int;
    }
    ~A()
    {
        delete p;
    }
};

void boo()
{
    A a;
    a.func();
    // no error here
}
