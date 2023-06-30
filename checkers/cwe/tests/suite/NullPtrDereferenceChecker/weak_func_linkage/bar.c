__attribute__((weak)) int* foo()
{
    return (int*)0xDEADBEEF;
}

__attribute__((weak)) int* baz()
{
    return 0;
}

int test1()
{
    int* a = foo();
    return *a;  // found a definition in foo.c, NPD is here
}

int test2()
{
    int* a = baz();
    return *a;  // baz() is undefined, no report here
}
