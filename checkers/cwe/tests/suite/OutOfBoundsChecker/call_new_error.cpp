#include <cstddef>
#include <new>

struct Foo {
    void* operator new(std::size_t, int, int p)
    {
        return (int*)0 + p;
    }
};

void func()
{
    auto p = new (10, 0) Foo;
}
