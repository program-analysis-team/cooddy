void foo(int* val)
{
    *val = 123;
}

int main()
{
    int a;
    foo(&(a));
    return a;
}
