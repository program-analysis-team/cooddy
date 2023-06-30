struct A {
    int a;
};

int baz(struct A* a);

void foo(struct A* a)
{
    baz(a);
    a->a += 10;
}

int bar()
{
    struct A a;
    foo(&a);
    return 0;
}
