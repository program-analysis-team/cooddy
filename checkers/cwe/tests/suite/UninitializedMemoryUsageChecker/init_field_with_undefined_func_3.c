struct A {
    int a;
    char* b;
};

void init(int* a, int x);

int foo(int x)
{
    struct A a;
    init(&a.a, x);
    return a.a;
}
