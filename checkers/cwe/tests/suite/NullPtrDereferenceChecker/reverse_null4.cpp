void bar(int *q) {
    *q;
}

int foo(int v, int* p)
{
    bar(p);
    int *x = p;
    if (v == 10 && x == nullptr) {
        return 1;
    }
    return 0;
}
