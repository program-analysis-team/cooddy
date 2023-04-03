#include <memory>

struct A {
    int func()
    {
        return 0;
    }
};

std::unique_ptr<A> some_func(int x)
{
    if (x > 10) {
        return nullptr;
    }
    return std::make_unique<A>();
}

int foo(int x)
{
    auto p = some_func(x);
    return p->func();
}
