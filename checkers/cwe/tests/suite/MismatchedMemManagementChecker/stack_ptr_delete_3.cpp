void foo3(int** ptr)
{
    delete *ptr;
}

void foo2(int* ptr)
{
    int** ptr2 = &ptr;
    foo3(ptr2);
}

void foo1()
{
    int x;
    int* ptr = &x;
    foo2(ptr);
}
