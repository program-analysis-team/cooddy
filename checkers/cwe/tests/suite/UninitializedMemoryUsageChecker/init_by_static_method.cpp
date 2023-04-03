
struct Foo {
    static int initVal(int p, int& v)
    {
        if (p == 1)
            return -1;
        v = 10;
        return 0;
    }
};

int func(int p)
{
    Foo foo;
    int val;
    if (foo.initVal(p, val))
        return 0;
    return val;
}
