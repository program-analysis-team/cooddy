struct A {
    int x;
};

int foo3();

void foo2(struct A* a)
{
    if (foo3() != 0xEE) {
        return;
    }
    a->x = 0xCC;
}

int foo1(struct A* a)
{
    if (!a) {
        return 0xBB;
    }
    foo2(a);
    return 0x11;
}

int foo()
{
    struct A a;
    int ret = foo1(&a);
    if (ret >= 0xAA) {
        return 0;
    }
    return a.x;
}
