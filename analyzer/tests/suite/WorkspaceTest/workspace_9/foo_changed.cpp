
extern void boo1(int idx);
extern void boo2(int idx);

void foo1(int idx)
{
    int arr[10];
    arr[idx] = idx;
}

void foo2(int idx)
{
    int arr[10];
    arr[idx] = idx;
}

void foo3()
{
    boo2(10);
}

int some();

void foo4()
{
    int idx = some();
    boo1(idx); // new problem
}
