#include <iostream>

struct A {
    int* ptr;
};

int main()
{
    A* a = new A();
    delete a;
    return *a->ptr;
}
