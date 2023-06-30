
void foo(long long idx)
{
    int arr[65] = {0};
    arr[__builtin_ffs(idx)];    // no error here
    arr[__builtin_ffsll(idx)];  // no error here

    int arr1[10] = {0};
    arr1[__builtin_ffsll(idx)];  // error here
}
