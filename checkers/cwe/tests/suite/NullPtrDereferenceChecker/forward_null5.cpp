void bar(int* q) {
    *q;
}

int foo(int* p, int v)
{
    if (v == 10 || p == nullptr) {
        bar(p);
    }
    return 0;
}
