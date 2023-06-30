void init(int *y, int x1)
{
    if (y) {
        *y = 0xBB;
    }
}

int foo(int x1)
{
    int y;
    init(&y, x1);
    return y;
}
