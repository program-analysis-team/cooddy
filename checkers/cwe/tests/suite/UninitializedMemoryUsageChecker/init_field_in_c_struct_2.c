struct A {
    int a;
};

int foo(int* x)
{
    *x = 42;
    return 0;
}

int main(int argc, char* argv[])
{
    struct A a;
    foo(&a.a);
    return a.a;
}
