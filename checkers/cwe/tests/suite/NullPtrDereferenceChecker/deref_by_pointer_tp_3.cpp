void foo2(int** ptr)
{
    **ptr;
}

void foo1()
{
    int* x = nullptr;
    int** ptr = &x;
    foo2(ptr);
}
