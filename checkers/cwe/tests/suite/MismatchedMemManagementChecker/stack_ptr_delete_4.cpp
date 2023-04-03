struct A {
    int x;
};

void test(void* ptr)
{
    delete ptr;
}

void foo()
{
    A a;
    test(&a);
}
