struct A {
    int *x;
};

void bar(A *a)
{
    *a->x = 42;
}

int foo(A *a)
{
    int x = 0;
    a->x = &x;
    bar(a);
    a->x = nullptr;
    return x;
}

int main()
{
    A a;
    return foo(&a);
}
