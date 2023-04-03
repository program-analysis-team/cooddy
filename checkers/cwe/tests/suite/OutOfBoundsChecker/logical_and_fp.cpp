
void foo(int hash)
{
    int arr[10];
    int idx1 = hash & ((1 << 3) - 1);
    int idx2 = hash & ((1 << 4) - 1);
    arr[idx1] = 0;  // no error here
    arr[idx2] = 0;  // error here
}
