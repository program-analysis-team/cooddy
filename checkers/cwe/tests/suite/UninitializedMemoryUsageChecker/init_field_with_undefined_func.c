struct A {
    int a;
    char* b;
};

void init(struct A* a, int x);

int foo(int x)
{
    struct A a;
    init(&a, x);
    return a.a;
}
