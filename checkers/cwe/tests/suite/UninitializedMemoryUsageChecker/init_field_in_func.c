struct A {
    int a;
    char* b;
};

int foo(int* x, int y)
{
    if (y) {
        *x = 42;
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    struct A a;
    if (foo(&a.a, argc)) {
        return a.a;
    }
    return 0;
}
