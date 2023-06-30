#define SZ 0x42

void foo1()
{
    int arr[SZ];
    int j = 0;
    for (int i = 0; i != SZ; ++i) {
        arr[i];
    }
}

void foo2()
{
    int arr[SZ];
    for (int i = SZ - 1; i != 0; --i) {
        arr[i];
    }
}

void foo3()
{
    int arr[SZ];
    int j = 0;
    for (int i = 0; SZ != i; ++i) {
        arr[i];
    }
}

void foo4()
{
    int arr[SZ];
    for (int i = SZ - 1; 0 != i; --i) {
        arr[i];
    }
}
