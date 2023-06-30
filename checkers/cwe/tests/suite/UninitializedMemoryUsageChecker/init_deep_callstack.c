int foo3();

void foo2(int* a)
{
    if (foo3() != 0xEE) {
        return;
    }
    *a = 0xCC;
}

int foo1(int* a)
{
    if (!a) {
        return 0xBB;
    }
    *a = 0xDD;
    foo2(a);
    return 0x11;
}

int foo()
{
    int a;
    int ret = foo1(&a);
    if (ret >= 0xAA) {
        return 0;
    }
    return a;
}
