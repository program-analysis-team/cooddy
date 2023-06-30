
auto foo(auto a)
{
    return a;
}

void bar()
{
    int* p = 0;
    *foo(p) = 0;
}
