#include <memory>

std::unique_ptr<int> some_func(int x)
{
    if (x > 10) {
        return nullptr;
    }
    return std::make_unique<int>();
}

int foo(int x)
{
    auto p = some_func(x);
    if (p) {
        return *p;
    }
    return 0;
}
