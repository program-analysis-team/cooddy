extern void _assert() __attribute__((__noreturn__));

void baz(int *x)
{
    _assert();
    *x = 42;
}

int bar()
{
    int x;
    baz(&x);
    return x;
}
