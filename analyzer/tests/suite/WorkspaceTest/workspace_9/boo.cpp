
extern void foo1(int idx);
extern void foo2(int idx);

void boo1(int idx)
{
    int arr[10];
    arr[idx] = idx;
}

void boo2(int idx)
{
    int arr[10];
    arr[idx] = idx;
}

void boo3()
{
    foo1(10);
}

void boo4()
{
    foo2(20);
}
