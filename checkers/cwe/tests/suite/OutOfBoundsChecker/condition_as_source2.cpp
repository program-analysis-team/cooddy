

void boo(int* arr, int p)
{
    for (int i = p; i < 21; ++i) {
        arr[i] = 0;
    }
}

void foo(int p)
{
    int arr[20];
    if (p >= 100) {
        return;
    }
    boo(arr, p);
}
