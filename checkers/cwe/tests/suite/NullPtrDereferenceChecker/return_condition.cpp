
struct Foo {
    int *p;
};

int func(Foo *foo)
{
    int val;
    if (val == 0) {
        foo->p = 0;
        return 1;
    }
    foo->p = (int *)0xbeaf;
    return 0;
}

int main()
{
    Foo foo;
    int ret = func(&foo);
    if (!ret)
        return *foo.p;  // no problem here
    if (func(&foo))
        return *foo.p;  // problem is here
    return 0;
}
