int* foo()
{
    static int x = 42;
    return &x;
}
