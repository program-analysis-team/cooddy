void foo(int *&x)
{
    int z = 42;
    x = &z;
}

int main()
{
    int *x = nullptr;
    foo(x);
    return *x;
}
