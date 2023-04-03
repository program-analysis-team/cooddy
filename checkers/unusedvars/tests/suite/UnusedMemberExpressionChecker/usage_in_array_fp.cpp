#include <vector>

struct Foo {
    int x = 0;  // no error here
};

int func(int p)
{
    std::vector<std::vector<Foo>> foo(10, std::vector<Foo>(10));
    auto& arr = foo.back();
    return arr[p].x;
}
