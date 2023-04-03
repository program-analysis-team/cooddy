void foo()
{
    int arr[64];
    arr[63 ^ 63] = 0;  // FP
    arr[128 ^ 128] = 0;  // FP
    arr[63 | 63] = 0;  // FP
    arr[63 & 63] = 0;  // FP
}