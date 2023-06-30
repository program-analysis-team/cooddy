int foo3();

int foo2(int *x, int sz)
{
    do {
        if (foo3()) {
            *x = 42;
            sz--;
        }
    } while (sz > 0);
    return 0;
}

void foo()
{
    int x;
    int ret = foo2(&x, sizeof(int));
    if (ret != 0) {
        return;
    }
    int y = x;
}
