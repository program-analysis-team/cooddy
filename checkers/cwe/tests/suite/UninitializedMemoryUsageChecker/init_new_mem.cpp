#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

class A {
public:
    A()
    {
        b = 0;
    }
    int b;
};

void test()
{
    A* a = new A();
    auto b = a->b;
    delete a;
}
