void boo(int* x, int f)
{
    if (f) {
        *x = 42;
    }
}
void foo1(int* x, int f)
{
    boo(x, f);
}
void foo2(int* x, int f)
{
    foo1(x, f);
}
void foo3(int* x, int f)
{
    foo2(x, f);
}
void foo4(int* x, int f)
{
    foo3(x, f);
}
void foo5(int* x, int f)
{
    foo4(x, f);
}
void foo6(int* x, int f)
{
    foo5(x, f);
}
int foo(int f)
{
    int x;
    foo5(&x, f);
    return x;
}
