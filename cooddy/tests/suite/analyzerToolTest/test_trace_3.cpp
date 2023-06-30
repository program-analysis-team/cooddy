int baz(int x, int y)
{
    if (x < y) {
        return -1;
    }
    if (x > y) {
        return 0;
    }
    if (x < y) {
        return -1;
    }
    return -2;
}

int foo(int f1, int x, int y)
{
    if (baz(x, y) < 0) {
        return 0;
    }

    int* ptr = nullptr;
    return *ptr;
}
