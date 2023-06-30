struct A {
    int x;
};

void foo3(struct A *a)
{
    a->x;
}

void foo2(struct A *a)
{
    foo3(a);
}

int foo1()
{
    struct A a;
    foo2(&a);
    return 0;
}
