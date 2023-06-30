void foo(unsigned int* x)
{
    *x = 60;
}

void func1()
{
    unsigned int y;
    foo(&y);
}

void func2()
{
    unsigned short y;
    // unsigned short* is forcibly converted to int* and then implicitly converted to unsigned int*
    foo((int*)&y);  // out-of-bounds memory access
}

void func3()
{
    unsigned short y;
    // unsigned short* is implicitly converted to int*
    foo(&y);
}
