struct A {
    int x;
    int* bar()
    {
        return &x;
    }
};

int* baz()
{
    A a;
    return a.bar();
}
