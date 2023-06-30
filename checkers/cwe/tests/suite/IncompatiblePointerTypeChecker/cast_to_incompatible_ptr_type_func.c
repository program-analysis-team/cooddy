void bar(int* p)
{
    *p = 10;
}

void foo()
{
    unsigned short a;
    bar(&a);
}
