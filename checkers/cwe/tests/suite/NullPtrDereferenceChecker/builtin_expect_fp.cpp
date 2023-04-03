
int* bar(int v)
{
    static int gv = 9;
    if (v == 10)
        return 0;
    return &gv;
}

int foo(int v)
{
    int* p = bar(v);
    if (__builtin_expect(__builtin_expect(!p, 0) || __builtin_expect(v == 100, 0), 0))
        return 0;
    return *p;  // no NPD
}
