

void func(int* v, int p)
{
    if (p == 0) {
        *v = 10;
    } else {
        v = nullptr;
    }
}

int foo(int p)
{
    int v;
    func(&v, p);
    return v;
}
