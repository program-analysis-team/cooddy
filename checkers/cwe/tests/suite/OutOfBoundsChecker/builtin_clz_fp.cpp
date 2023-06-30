
void foo_clz(unsigned x)
{
    auto idx = __builtin_clz(x);
    int arr[32] = {0};
    arr[idx];    // no error here

    int arr1[10] = {0};
    arr1[idx];   // error here
}

void foo_clzll(unsigned long long x)
{
    auto idx = __builtin_clzll(x);
    int arr[64] = {0};
    arr[idx];    // no error here

    int arr1[10] = {0};
    arr1[idx];   // error here
}
