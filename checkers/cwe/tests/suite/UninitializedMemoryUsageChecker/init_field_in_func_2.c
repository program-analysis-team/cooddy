struct A {
    int a;
};

void foo(int* x)
{
    if (x) {
        *x = 42;
    }
}

int main(int argc, char* argv[])
{
    struct A a;
    foo(&a.a);
    return a.a;
}
