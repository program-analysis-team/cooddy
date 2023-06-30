extern void _assert() __attribute__((__noreturn__));

int bar()
{
    int x;
    _assert();
    return x;
}
