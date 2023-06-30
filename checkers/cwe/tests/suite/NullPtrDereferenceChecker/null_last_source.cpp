
int* foo()
{
    return 0;
}

int* boo()
{
    return foo();
}

int bar()
{
    int* p = boo();
    return *p;
}
