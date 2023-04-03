
struct A {
    int a1;
};

typedef void (*fptr)(struct A* a);
fptr g_fillA;

void bar(struct A* a)
{
    a->a1;  // FP here
}

int foo()
{
    struct A a;
    g_fillA(&a);
    bar(&a);
    return 0;
}
