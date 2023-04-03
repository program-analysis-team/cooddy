#include <iostream>

struct Foo {
    int x;
};

void bar(Foo *f)
{
    new (f) Foo{42};
}
