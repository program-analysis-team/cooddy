#include <memory>

struct Foo {
    Foo(int& v) : x(v) {}
    void func()
    {
        x = 10;
    }
    int& x;  // no error here
};

int main()
{
    int ret = 0;
    Foo foo(ret);
    foo.func();
    return foo.x;
}
