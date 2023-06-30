struct A {
    int a;
    char* b;
};

void foo(struct A* a, int x)
{
    switch (x) {
        case 0:
            a->a = 0;
            break;
        default:
            a->a = 0;
            break;
    }
}

int bar(int x)
{
    struct A a;
    foo(&a, x);
    return a.a;
}
