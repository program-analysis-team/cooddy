void bar(int *x);

void foo(int x)
{
    int y = x;
    bar(&y);
}
