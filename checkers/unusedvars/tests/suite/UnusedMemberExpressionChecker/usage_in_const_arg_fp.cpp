
struct Bar {
};

struct Foo {
    Bar x;  // no error here
};

void func(bool flag, const Bar& v)
{
    Bar tmp;
    tmp = v;
}

int test(bool flag)
{
    Foo foo;
    func(flag, foo.x);
    return 0;
}
