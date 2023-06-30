void init2(int* x, int f1, int f2, int f3)
{
    if (f2 == 0xCC) {
        *x = 2;
        return;
    }
    if (f3 == 0xDD) {
        return;
    }
    *x = 3;
    return;
}

void init1(int* x, int f1, int f2, int f3)
{
    if (f1 == 0xBB) {
        *x = 1;
    } else {
        init2(x, f1, f2, f3);
    }
}

int foo(int f1, int f2, int f3, int f4)
{
    if (f1 == 0xAA) {
        return 0;
    }

    int x;
    init1(&x, f2, f3, f4);
    return x;
}
