void foo(int *&x)
{
    int z = 42;
    x = &z;
}

void boo(int *&x)
{
    foo(x);
}

int main()
{
    int *x;
    boo(x);
}
