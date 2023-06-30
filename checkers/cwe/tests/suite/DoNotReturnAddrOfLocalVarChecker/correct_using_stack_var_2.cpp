void foo(int*& a)
{
    int x = 0;
    a = &x;
    a = nullptr;
}

int main()
{
    int* a;
    foo(a);
    return 0;
}
