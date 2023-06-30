int bar(int x, int z)
{
    return x && z;
}

void foo1(int a, int b, int c, int* ptr)
{
    int z = bar(1 && (2 ? 3 : 0), 3 && 4);
    if (z) {
        *ptr;  // NPD
    }
}

void foo2(int a, int b, int c, int* ptr)
{
    int z = bar(1 && (2 ? 0 : 3), 3 && 4);
    if (z) {
        *ptr;  // infeasible path
    }
}
