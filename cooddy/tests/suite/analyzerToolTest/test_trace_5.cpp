int bar(int x)
{
    return x;
}

void init1(int *oid, int f)
{
    if (f == 0xBBB) {
        return;
    }
    *oid = 1;
}

void init2(int *oid, int f)
{
    if (f == 0xAAA) {
        return;
    }
    *oid = 2;
}

int foo(int f1, int f2)
{
    int oid;
    init1(&oid, f1);
    init2(&oid, f2);
    return bar(oid);
}
