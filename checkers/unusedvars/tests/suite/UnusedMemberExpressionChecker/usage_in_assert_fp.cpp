#define NDEBUG
#include <cassert>

struct Foo {
    int x;  // no error here
    int y;
};

int main()
{
    Foo foo = {0, 0};
    assert(foo.x == 10);
    return foo.y;
}
