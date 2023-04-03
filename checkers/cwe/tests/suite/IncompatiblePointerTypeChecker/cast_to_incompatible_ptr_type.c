int foo(int a, short b)
{
    unsigned short y = b;
    unsigned int* x = &y;
    *x = a;
    return 0;
}
