int some_func1(int*& p);
int* some_func2();

int Foo(int v)
{
    int* p = 0;
    if (v) {
        int rc = some_func1(p);
        if (rc == 0)
            return 0;
    } else if (1) {
        p = some_func2();
        if (p == 0)
            return 0;
    }
    return *p;
}
