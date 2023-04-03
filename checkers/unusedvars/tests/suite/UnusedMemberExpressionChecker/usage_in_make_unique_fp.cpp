#include <memory>

struct Bar {
    Bar(int v) {}
};

struct Foo {
    void func()
    {
        std::make_unique<Bar>(x);
    }
    int x;  // no error here
};

int main()
{
    Foo foo;
    foo.func();
    return 0;
}
