#include <stdint.h>
#include <stdlib.h>

class A {
public:
    ~A() {}
};

template <class T>
void foo()
{
    A a, a2, a3;
    void* ptr = malloc(42);
}

void bar()
{
    foo<int>();
    foo<char>();
}
