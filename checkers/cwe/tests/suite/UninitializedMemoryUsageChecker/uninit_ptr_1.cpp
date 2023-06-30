void foo2(int* ptr)
{
    *ptr;
}

void foo1()
{
    int x;
    int* ptr = &x;
    foo2(ptr);
}
